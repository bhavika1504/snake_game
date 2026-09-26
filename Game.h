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
    IInput* input;
    bool ownsInput;
    bool gameOver;
    int gameSpeed;
    bool paused;

public:
    Game(int width, int height, IInput* customInput = nullptr);
    ~Game();
    void run();
    void processInput();
    void update();
    void render();
    bool isGameOver() const;
    void setGameSpeed(int speed);
    Snake* getSnake() const { return snake; }
    Snake* getSnake2() const { return snake2; }
    SnakeMap* getMap() const { return map; }
    bool isPaused() const { return paused; }
};

#endif