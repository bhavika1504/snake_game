#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <utility>
#include <chrono>
#include "Direction.h"   // ✅ add this

class Snake {
private:
    std::vector<std::pair<int, int>> body;
    bool powerActive;
    std::chrono::steady_clock::time_point powerStartTime;

public:
    Snake(int startX, int startY);

    void move(Direction dir, int width, int height);
    void grow();
    bool checkCollision(int width, int height);
    bool isPowerActive();
    void activatePower();

    const std::vector<std::pair<int, int>>& getBody() const;
};

#endif
