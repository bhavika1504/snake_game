#include "Game.h"
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>

using namespace std;

Game::Game(int width, int height) {
    this->width = width;
    this->height = height;
    gameOver = false;
    direction = Direction::STOP;
    snake = new Snake(width / 2, height / 2);
    map = new SnakeMap(width, height, snake);
}

Game::~Game() {
    delete snake;
    delete map;
}

void Game::run() {
    cout << "\033[2J\033[H\033[?25l"; // clear + hide cursor

    while (!gameOver) {
        input();
        logic();
        map->draw();
        this_thread::sleep_for(chrono::milliseconds(snake->isPowerActive() ? 60 : 100));
    }

    cout << "\033[?25h"; // show cursor
    cout << "\n\n💀 GAME OVER 💀\nScore: " << snake->getSize() - 1 << "\n";
}

void Game::input() {
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            int ch2 = _getch();
            switch (ch2) {
                case 72: direction = Direction::UP; break;
                case 80: direction = Direction::DOWN; break;
                case 75: direction = Direction::LEFT; break;
                case 77: direction = Direction::RIGHT; break;
            }
        } else {
            switch (ch) {
                case 'w': case 'W': direction = Direction::UP; break;
                case 's': case 'S': direction = Direction::DOWN; break;
                case 'a': case 'A': direction = Direction::LEFT; break;
                case 'd': case 'D': direction = Direction::RIGHT; break;
                case 'q': case 'Q': gameOver = true; break;
                case '+': case '=': map->resize(map->getWidth() + 2, map->getHeight() + 2); break;
                case '-': case '_': map->resize(map->getWidth() - 2, map->getHeight() - 2); break;
                case 'e': case 'E': map->setEmojiSize("emoji"); break;
                case '1': map->setEmojiSize("small"); break;
                case '2': map->setEmojiSize("medium"); break;
                case '3': map->setEmojiSize("large"); break;
                case '4': map->setEmojiSize("xlarge"); break;
                case '5': map->setEmojiSize("huge"); break;
            }
        }
    }
}

void Game::logic() {
    snake->changeDirection(direction);
    snake->move();

    if (map->checkCollision() && !snake->isPowerActive()) {
        gameOver = true;
        return;
    }

    map->eatPowerFruit();
    map->eatFood();
    snake->updatePower();
}
