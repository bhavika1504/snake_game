#ifndef GAME_H
#define GAME_H

#include "Snake.h"
#include "SnakeMap.h"
#include <memory>

class Game {
private:
    bool running;
    int width, height;
    Direction direction;
    std::unique_ptr<Snake> snake;
    std::unique_ptr<SnakeMap> map;

public:
    Game(int width, int height);
    void run();
    void input();
    void logic();
    void draw();
};

#endif
