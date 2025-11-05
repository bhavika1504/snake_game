#include "SnakeMap.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

SnakeMap::SnakeMap(int w, int h, Snake* s)
    : width(w), height(h), snake(s),
      powerFruitActive(false), powerFruitTimer(0),
      emojiMode(false), messageTimer(0) {

    srand(static_cast<unsigned>(time(0)));

    // Default visuals
    snakeEmoji = "🟩";
    foodEmoji = "🍎";
    powerEmoji = "💎";
    emptyEmoji = "  ";
    powerSnakeEmoji = "🟨";
    borderEmoji = "⬜";

    // Spawn initial food
    spawnFood();
}

void SnakeMap::spawnFood() {
    // avoid placing on snake body
    int x, y;
    bool ok;
    do {
        x = rand() % height;
        y = rand() % width;
        ok = true;
        for (auto &seg : snake->getBody()) {
            if (seg.first == x && seg.second == y) { ok = false; break; }
        }
    } while (!ok);
    food = {x, y};

    string foodOptions[] = {"🍎", "🍉", "🍌", "🍇", "🍒", "🍊","🥑"};
    foodEmoji = foodOptions[rand() % 7];
}

void SnakeMap::spawnPowerFruit() {
    int x, y;
    bool ok;
    do {
        x = rand() % height;
        y = rand() % width;
        ok = true;
        for (auto &seg : snake->getBody()) {
            if (seg.first == x && seg.second == y) { ok = false; break; }
        }
        if (x == food.first && y == food.second) ok = false;
    } while (!ok);
    powerFruit = {x, y};
    powerFruitActive = true;
    powerFruitTimer = 100;
}

bool SnakeMap::checkFood() {
    auto head = snake->getHead();
    return (head.first == food.first && head.second == food.second);
}

bool SnakeMap::checkPowerFruit() {
    if (!powerFruitActive) return false;
    auto head = snake->getHead();
    if (head.first == powerFruit.first && head.second == powerFruit.second) {
        // activate power on snake, map will hide fruit
        snake->activatePower();
        powerFruitActive = false;

        // in-game message for a short while
        modeMessage = "⚡ Invincible Mode Activated for 10 seconds! ⚡";
        messageTimer = 15;
#ifdef _WIN32
        Sleep(300);
#else
        usleep(300000);
#endif
        return true;
    }
    return false;
}

void SnakeMap::updatePowerFruit() {
    if (!powerFruitActive && (rand() % 100) == 0) {
        spawnPowerFruit();
    }
    if (powerFruitActive && --powerFruitTimer <= 0) {
        powerFruitActive = false;
    }
}

// We keep clearScreen for possible use, but **do not call it every frame**.
// It is intentionally not used in draw() anymore.
void SnakeMap::clearScreen() {
#ifdef _WIN32
    // prefer not to use system("cls") per-frame; leave empty or do cursor reset.
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD topLeft = {0, 0};
    SetConsoleCursorPosition(hOut, topLeft);
#else
    cout << "\033[2J\033[H";
#endif
}

void SnakeMap::draw() {
    // draw into a buffer first (double-buffer)
    std::ostringstream frame;

    // Top border
    for (int i = 0; i < width + 2; i++) frame << borderEmoji;
    frame << "\n";

    for (int i = 0; i < height; i++) {
        frame << borderEmoji;
        for (int j = 0; j < width; j++) {
            auto head = snake->getHead();

            if (head.first == i && head.second == j) {
                if (snake->isPowerActive()) {
                    frame << ((snake->getPowerTimeLeft() % 2 == 0) ? "🟦" : "🟨");
                } else {
                    frame << snakeEmoji;
                }
            } else if (food.first == i && food.second == j) {
                frame << foodEmoji;
            } else if (powerFruitActive && powerFruit.first == i && powerFruit.second == j) {
                frame << ((rand() % 2) ? "💎" : "💥");
            } else {
                bool printed = false;
                for (auto& seg : snake->getBody()) {
                    if (seg.first == i && seg.second == j) {
                        frame << (snake->isPowerActive() ? powerSnakeEmoji : snakeEmoji);
                        printed = true;
                        break;
                    }
                }
                if (!printed) frame << emptyEmoji;
            }
        }
        frame << borderEmoji << "\n";
    }

    // Bottom border
    for (int i = 0; i < width + 2; i++) frame << borderEmoji;
    frame << "\n";

    // Game info
    frame << "Movement: WASD or Arrow Keys | Press 'E' for Emoji Mode\n";
    frame << "Score: " << (int)snake->getBody().size() - 1;
    if (snake->isPowerActive())
        frame << " | ⚡ Invincible (" << snake->getPowerTimeLeft() << "s) ⚡";
    frame << "\n";

    // Show temporary message (like emoji or invincible activation)
    if (messageTimer > 0) {
        frame << "\n" << modeMessage << "\n";
        messageTimer--;
    }

    // Print frame in one shot
    cout << frame.str();
    cout.flush();
}

void SnakeMap::resize(int newWidth, int newHeight) {
    width = max(10, newWidth);
    height = max(10, newHeight);
    spawnFood();
}

void SnakeMap::setEmojiSize(const string& size) {
    if (size == "small") {
        snakeEmoji = "▒▒";
        borderEmoji = "##";
    } else if (size == "medium") {
        snakeEmoji = "▓▓";
        borderEmoji = "██";
    } else if (size == "large") {
        snakeEmoji = "🟩";
        borderEmoji = "⬜";
    } else if (size == "xlarge") {
        snakeEmoji = "🟩";
        borderEmoji = "⬛";
    } else if (size == "huge") {
        snakeEmoji = "🟩";
        borderEmoji = "🧱";
    } else if (size == "emoji") {
        snakeEmoji = "🐍";
        borderEmoji = "⬜";
    }
}

void SnakeMap::toggleEmojiMode() {
    emojiMode = !emojiMode;
    if (emojiMode) {
        snakeEmoji = "🐍";
        borderEmoji = "⬜";
        modeMessage = "✨ Emoji Mode Activated! ✨";
    } else {
        snakeEmoji = "🟩";
        borderEmoji = "⬜";
        modeMessage = "💤 Emoji Mode Off";
    }
    messageTimer = 10;
}
