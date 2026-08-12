#ifndef GAME_H
#define GAME_H

#include "Snake.h"
#include "SnakeMap.h"
#include "Input.h"

class Game {
private:
    Snake* snake;
    Snake* snake2;
    SnakeMap* map;
    int loser;
    Input* input;
    bool gameOver;
    int gameSpeed;
    bool paused;



public:
    Game(int width, int height);
    ~Game();
    void run();
    void processInput();
    void update();
    void render();
    bool isGameOver() const;
    void setGameSpeed(int speed);
};

#endif