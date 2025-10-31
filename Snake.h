#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include <utility>
#include "Direction.h"

class Snake {
private:
    std::deque<std::pair<int, int>> body;
    Direction dir;
    bool growNext;
    bool isPowered;
    int powerTimer;

public:
    Snake(int x, int y);

    void move();
    void changeDirection(Direction newDir);
    bool collision(int width, int height);
    bool eatsItself();

    void grow();
    void activatePower();
    void updatePower();
    bool isPowerActive() const;
    int getPowerTimeLeft() const;

    std::pair<int, int> getHead() const;
    std::deque<std::pair<int, int>>& getBody();
    const std::deque<std::pair<int, int>>& getBody() const;
    int getSize() const;
};

#endif
