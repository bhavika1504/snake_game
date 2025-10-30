#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include <utility>
#include "Direction.h"

class Snake {
private:
    std::deque<std::pair<int, int>> body;
    Direction dir;
    bool grow;
    bool isPowered;
    int powerTimer;

public:
    Snake(int x, int y);
    void move();
    void changeDirection(Direction newDir);
    bool collision(int width, int height);
    bool eatsItself();
    std::pair<int, int> getHead() const;
    const std::deque<std::pair<int, int>>& getBody() const;
    int getSize() const;
    void setGrow();
    void activatePower();
    void updatePower();
    bool isPowerActive() const;
    int getPowerTimeLeft() const;
};

#endif