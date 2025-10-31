#include "SnakeMap.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

SnakeMap::SnakeMap(int width, int height, Snake* snake)
    : width(width), height(height), snake(snake) {
    foodEmoji = "🍎 ";
    emptyEmoji = "  ";
    snakeEmoji = "🟩 ";
    powerEmoji = "💥 ";
    powerSnakeEmoji = "⚡ ";
    borderEmoji = "⬛ ";
    powerFruitActive = false;
    srand((unsigned)time(nullptr));
    spawnFood();
}

void SnakeMap::clearScreen() {
    cout << "\033[H"; // move cursor to top-left (no flicker)
}

void SnakeMap::spawnFood() {
    int x, y;
    bool conflict;
    do {
        x = rand() % height;
        y = rand() % width;
        conflict = false;
        for (auto& seg : snake->getBody()) {
            if (seg.first == x && seg.second == y) { conflict = true; break; }
        }
    } while (conflict);
    food = {x, y};
}

void SnakeMap::spawnPowerFruit() {
    if (powerFruitActive) return;
    int x, y;
    bool conflict;
    do {
        x = rand() % height;
        y = rand() % width;
        conflict = false;
        for (auto& seg : snake->getBody()) {
            if (seg.first == x && seg.second == y) { conflict = true; break; }
        }
        if (!conflict && food.first == x && food.second == y) conflict = true;
    } while (conflict);
    powerFruit = {x, y};
    powerFruitActive = true;
    powerStartTime = chrono::steady_clock::now();
}

void SnakeMap::draw() {
    clearScreen();

    if (snake->isPowerActive())
        cout << "⚡ INVINCIBLE MODE! ⚡ " << snake->getPowerTimeLeft() << "s left ⚡\n";
    else
        cout << "                                      \n";

    // top border
    cout << borderEmoji;
    for (int j = 0; j < width; j++) cout << borderEmoji;
    cout << borderEmoji << "\n";

    for (int i = 0; i < height; i++) {
        cout << borderEmoji;
        for (int j = 0; j < width; j++) {
            bool printed = false;

            if (food.first == i && food.second == j) {
                cout << foodEmoji; printed = true;
            } else if (powerFruitActive && powerFruit.first == i && powerFruit.second == j) {
                cout << powerEmoji; printed = true;
            } else {
                for (auto& seg : snake->getBody()) {
                    if (seg.first == i && seg.second == j) {
                        cout << (snake->isPowerActive() ? powerSnakeEmoji : snakeEmoji);
                        printed = true;
                        break;
                    }
                }
            }

            if (!printed) cout << emptyEmoji;
        }
        cout << borderEmoji << "\n";
    }

    // bottom border
    cout << borderEmoji;
    for (int j = 0; j < width; j++) cout << borderEmoji;
    cout << borderEmoji << "\n";

    cout << "Score: " << snake->getSize() - 1;
    if (snake->isPowerActive()) cout << " | ⚡ Invincible ⚡";
    cout << "\nControls: WASD / Arrows | Q=Quit | +/- Resize | 1–5 Size | E Emoji\n";
    cout.flush();
}

bool SnakeMap::checkCollision() {
    auto head = snake->getBody().front();
    if (head.first < 0 || head.first >= height || head.second < 0 || head.second >= width) {
        if (snake->isPowerActive()) return false;
        return true;
    }
    for (size_t i = 1; i < snake->getBody().size(); i++)
        if (snake->getBody()[i] == head) return true;
    return false;
}

bool SnakeMap::eatFood() {
    auto head = snake->getBody().front();
    if (head == food) {
        snake->grow();
        spawnFood();
        if (rand() % 6 == 0) spawnPowerFruit();
        return true;
    }
    return false;
}

bool SnakeMap::eatPowerFruit() {
    auto head = snake->getBody().front();
    if (powerFruitActive && head == powerFruit) {
        snake->activatePower();
        powerFruitActive = false;
        return true;
    }
    return false;
}

void SnakeMap::resize(int newW, int newH) {
    if (newW < 10 || newH < 10) return;
    width = newW;
    height = newH;
    spawnFood();
}

void SnakeMap::setEmojiSize(const std::string& mode) {
    if (mode == "emoji") snakeEmoji = "🟩 ";
    else if (mode == "small") snakeEmoji = "░░";
    else if (mode == "medium") snakeEmoji = "▒▒";
    else if (mode == "large") snakeEmoji = "▓▓";
    else if (mode == "xlarge") snakeEmoji = "██";
    else if (mode == "huge") snakeEmoji = "███";
}
