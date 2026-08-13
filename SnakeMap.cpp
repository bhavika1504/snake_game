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

SnakeMap::SnakeMap(int w, int h, Snake* s, Snake* s2)
    : width(w), height(h),
      powerFruitActive(false), powerFruitTimer(0),
      snake(s), snake2(s2),
      emojiMode(false), messageTimer(0) {

    srand(static_cast<unsigned>(time(0)));

    // Default visuals
    snakeEmoji = "🟩";
    foodEmoji = "🍎";
    powerEmoji = "🌟";
    emptyEmoji = "  ";
    powerSnakeEmoji = "🟨";
    borderEmoji = "⬜";

    // Spawn initial food
    spawnFood();
}

void SnakeMap::spawnFood() {
    // avoid placing on either snake body
    int x, y;
    bool ok;
    do {
        x = rand() % height;
        y = rand() % width;
        ok = true;
        for (auto &seg : snake->getBody()) {
            if (seg.first == x && seg.second == y) { ok = false; break; }
        }
        if (ok) {
            for (auto &seg : snake2->getBody()) {
                if (seg.first == x && seg.second == y) { ok = false; break; }
            }
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
        if (ok) {
            for (auto &seg : snake2->getBody()) {
                if (seg.first == x && seg.second == y) { ok = false; break; }
            }
        }
        if (x == food.first && y == food.second) ok = false;
    } while (!ok);
    powerFruit = {x, y};
    powerFruitActive = true;
    powerFruitTimer = 100;
}

bool SnakeMap::checkFood() {
    auto head1 = snake->getHead();
    auto head2 = snake2->getHead();
    return ((head1.first == food.first && head1.second == food.second) ||
            (head2.first == food.first && head2.second == food.second));
}

bool SnakeMap::checkPowerFruit() {
    if (!powerFruitActive) return false;
    auto head1 = snake->getHead();
    auto head2 = snake2->getHead();
    bool hit1 = (head1.first == powerFruit.first && head1.second == powerFruit.second);
    bool hit2 = (head2.first == powerFruit.first && head2.second == powerFruit.second);
    if (hit1 || hit2) {
        if (hit1) snake->activatePower();
        if (hit2) snake2->activatePower();
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
            auto head1 = snake->getHead();
            auto head2 = snake2->getHead();

            if (head1.first == i && head1.second == j) {
                if (snake->isPowerActive()) {
                    frame << ((snake->getPowerTimeLeft() % 2 == 0) ? "🟦" : "🟨");
                } else {
                    frame << snakeEmoji;
                }
            } else if (head2.first == i && head2.second == j) {
                if (snake2->isPowerActive()) {
                    frame << ((snake2->getPowerTimeLeft() % 2 == 0) ? "🟦" : "🟨");
                } else {
                    frame << "🟥";
                }
            } else if (food.first == i && food.second == j) {
                frame << foodEmoji;
            } else if (powerFruitActive && powerFruit.first == i && powerFruit.second == j) {
                frame << ((rand() % 2) ? "🌟" : "💥");
            } else {
                bool printed = false;
                for (auto& seg : snake->getBody()) {
                    if (seg.first == i && seg.second == j) {
                        frame << (snake->isPowerActive() ? powerSnakeEmoji : snakeEmoji);
                        printed = true;
                        break;
                    }
                }
                if (!printed) {
                    for (auto& seg : snake2->getBody()) {
                        if (seg.first == i && seg.second == j) {
                            frame << (snake2->isPowerActive() ? powerSnakeEmoji : "🟥");
                            printed = true;
                            break;
                        }
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
    frame << "P1 (Arrow Keys): Score " << (int)snake->getBody().size() - 1;
    if (snake->isPowerActive())
        frame << " | ⚡ Invincible (" << snake->getPowerTimeLeft() << "s) ⚡";
    frame << "  |  P2 (WASD): Score " << (int)snake2->getBody().size() - 1;
    if (snake2->isPowerActive())
        frame << " | ⚡ Invincible (" << snake2->getPowerTimeLeft() << "s) ⚡";
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
        snakeEmoji = "░";     // small tile
        borderEmoji = "▒"; 
    } 
    else if (size == "medium") {
        snakeEmoji = "▒";     // thicker
        borderEmoji = "▓";
    } 
    else if (size == "large") {
        snakeEmoji = "█";     // BIG one-block tile ✅
        borderEmoji = "▓";
    } 
    else if (size == "xlarge") {
        snakeEmoji = "██";    // twice wide block
        borderEmoji = "▓▓";
    } 
    else if (size == "huge") {
        snakeEmoji = "████";  // super big
        borderEmoji = "▓▓▓▓";
    } 
    else if (size == "emoji") {
        snakeEmoji = "🟢";     // fun emoji mode
        borderEmoji = "⬛";
    }
}

void SnakeMap::toggleEmojiMode() {
    emojiMode = !emojiMode;
    if (emojiMode) {
        snakeEmoji = "🟢";
        borderEmoji = "⬜";
        modeMessage = "✨ Emoji Mode Activated! ✨";
    } else {
        snakeEmoji = "🟩";
        borderEmoji = "⬜";
        modeMessage = "💤 Emoji Mode Off";
    }
    messageTimer = 10;
}
