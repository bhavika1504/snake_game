#ifndef SNAKEMAP_H
#define SNAKEMAP_H

#include <vector>
#include <string>
#include <utility>
#include <chrono>
#include "Snake.h"

class SnakeMap {
private:
    int width, height;
    Snake* snake;

    std::pair<int, int> food;
    std::pair<int, int> powerFruit;

    bool powerFruitActive;
    std::chrono::steady_clock::time_point powerStartTime;

    std::string foodEmoji, emptyEmoji, snakeEmoji, powerEmoji, powerSnakeEmoji, borderEmoji;

public:
    SnakeMap(int width, int height, Snake* snake);

    void clearScreen();
    void draw();

    void spawnFood();
    void spawnPowerFruit();
    bool checkCollision();
    bool eatFood();
    bool eatPowerFruit();
    void resize(int newW, int newH);
    void setEmojiSize(const std::string& mode);
};

#endif
