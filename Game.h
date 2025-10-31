#ifndef GAME_H
#define GAME_H

#include "Snake.h"
#include "SnakeMap.h"
#include "Direction.h"

class Game {
private:
    Snake* snake;
    SnakeMap* map;
    int width, height;
    bool gameOver;
    Direction direction;

public:
    Game(int width, int height);
    ~Game();

    void run();
    void input();
    void logic();
};

#endif
