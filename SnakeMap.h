#ifndef SNAKEMAP_H
#define SNAKEMAP_H

#include <utility>
#include <chrono>
#include <string>
#include "Snake.h"

class SnakeMap {
private:
    int width, height;
    Snake* snake;
    std::pair<int, int> food;
    std::pair<int, int> powerFruit;
    bool powerFruitActive;
    std::chrono::steady_clock::time_point powerStartTime;

    std::string foodEmoji, emptyEmoji, snakeEmoji, powerEmoji;
    std::string powerSnakeEmoji, borderEmoji;
    
public:
    SnakeMap(int width, int height, Snake* snake);

    void draw();
    void clearScreen();
    void spawnFood();
    void spawnPowerFruit();

    bool checkCollision();
    bool eatFood();
    bool eatPowerFruit();

    void resize(int newW, int newH);
    void setEmojiSize(const std::string& mode);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif
