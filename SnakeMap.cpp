#include "SnakeMap.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

SnakeMap::SnakeMap(int w, int h, Snake* s) 
    : width(w), height(h), snake(s), powerFruitActive(false), powerFruitTimer(0) {
    srand(time(0));
    
#ifdef _WIN32
    // Check if running in Windows Terminal (supports emojis better)
    const char* wt_session = getenv("WT_SESSION");
    if (wt_session != nullptr) {
        // Windows Terminal detected - use emojis!
        snakeEmoji = "🟩";
        foodEmoji = "🍎";
        powerEmoji = "⭐";
        emptyEmoji = "⬛";
        powerSnakeEmoji = "🟨";
    } else {
        // Regular CMD - use ASCII
        snakeEmoji = "[]";
        foodEmoji = "@@";
        powerEmoji = "**";
        emptyEmoji = "  ";
        powerSnakeEmoji = "##";
    }
#else
    // Use emojis on Linux/Mac
    snakeEmoji = "🟩";
    foodEmoji = "🍎";
    powerEmoji = "⭐";
    emptyEmoji = "⬛";
    powerSnakeEmoji = "🟨";
#endif
    
    spawnFood();
}

void SnakeMap::spawnFood() {
    int x, y;
    bool conflict;
    do {
        x = rand() % height;
        y = rand() % width;
        conflict = false;
        for (auto& seg : snake->getBody()) {
            if (seg.first == x && seg.second == y) conflict = true;
        }
        if (powerFruitActive && powerFruit.first == x && powerFruit.second == y) {
            conflict = true;
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
            if (seg.first == x && seg.second == y) conflict = true;
        }
        if (food.first == x && food.second == y) conflict = true;
    } while (conflict);
    
    powerFruit = {x, y};
    powerFruitActive = true;
    powerFruitTimer = 200; // Power fruit stays for ~30 seconds
}

bool SnakeMap::checkFood() {
    if (snake->getHead() == food) {
        snake->setGrow();
        spawnFood();
        return true;
    }
    return false;
}

bool SnakeMap::checkPowerFruit() {
    if (powerFruitActive && snake->getHead() == powerFruit) {
        snake->activatePower();
        powerFruitActive = false;
        powerFruitTimer = 0;
        return true;
    }
    return false;
}

void SnakeMap::updatePowerFruit() {
    // Spawn power fruit randomly
    if (!powerFruitActive && rand() % 100 < 2) { // 2% chance each frame
        spawnPowerFruit();
    }
    
    // Remove power fruit after timer expires
    if (powerFruitActive && powerFruitTimer > 0) {
        powerFruitTimer--;
        if (powerFruitTimer == 0) {
            powerFruitActive = false;
        }
    }
}

void SnakeMap::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void SnakeMap::draw() {
    clearScreen();
    
    // Display power status
    if (snake->isPowerActive()) {
        cout << "⚡ POWER MODE ACTIVE! ⚡ Time left: " << snake->getPowerTimeLeft() << "s\n";
    }
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bool printed = false;
            
            // Draw food
            if (food.first == i && food.second == j) {
                cout << foodEmoji;
                printed = true;
            }
            
            // Draw power fruit
            if (!printed && powerFruitActive && powerFruit.first == i && powerFruit.second == j) {
                cout << powerEmoji;
                printed = true;
            }
            
            // Draw snake
            if (!printed) {
                for (auto& seg : snake->getBody()) {
                    if (seg.first == i && seg.second == j) {
                        if (snake->isPowerActive()) {
                            cout << powerSnakeEmoji;
                        } else {
                            cout << snakeEmoji;
                        }
                        printed = true;
                        break;
                    }
                }
            }
            
            // Draw empty space
            if (!printed) cout << emptyEmoji;
        }
        cout << "\n";
    }
    
    cout << "Score: " << snake->getSize() - 1 << "\n";
    cout << "Controls: WASD/Arrow Keys | Q: Quit | +/-: Resize Board\n";
}

void SnakeMap::resize(int newWidth, int newHeight) {
    if (newWidth < 10) newWidth = 10;
    if (newHeight < 10) newHeight = 10;
    if (newWidth > 50) newWidth = 50;
    if (newHeight > 40) newHeight = 40;
    
    width = newWidth;
    height = newHeight;
    
    // Respawn food and power fruit if they're out of bounds
    if (food.first >= height || food.second >= width) {
        spawnFood();
    }
    if (powerFruitActive && (powerFruit.first >= height || powerFruit.second >= width)) {
        powerFruitActive = false;
    }
}

void SnakeMap::setEmojiSize(const string& size) {
    if (size == "emoji") {
        // Switch to emoji mode
        snakeEmoji = "🟩";
        foodEmoji = "🍎";
        powerEmoji = "⭐";
        emptyEmoji = "⬛";
        powerSnakeEmoji = "🟨";
    } else {
        // ASCII mode (default for Windows)
        snakeEmoji = "[]";
        foodEmoji = "@@";
        powerEmoji = "**";
        emptyEmoji = "  ";
        powerSnakeEmoji = "##";
    }
}