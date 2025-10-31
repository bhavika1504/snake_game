#include "Game.h"
#include <iostream>
#include <thread>
#include <conio.h>
using namespace std;

Game::Game(int width, int height)
    : running(true), width(width), height(height), direction(Direction::RIGHT) {
    snake = std::make_unique<Snake>(width / 2, height / 2);
    map = std::make_unique<SnakeMap>(width, height, snake.get());
}

void Game::run() {
    while (running) {
        draw();
        input();
        logic();
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }
}

void Game::input() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'w': case 'W': direction = Direction::UP; break;
            case 's': case 'S': direction = Direction::DOWN; break;
            case 'a': case 'A': direction = Direction::LEFT; break;
            case 'd': case 'D': direction = Direction::RIGHT; break;
            case 'q': case 'Q': running = false; break;

            // ✅ Resize map
            case '+': map->resize(width + 2, height + 1); width += 2; height += 1; break;
            case '-': map->resize(width - 2, height - 1); width -= 2; height -= 1; break;

            // ✅ Change snake block size / emoji
            case '1': map->setEmojiSize("small"); break;
            case '2': map->setEmojiSize("medium"); break;
            case '3': map->setEmojiSize("large"); break;
            case '4': map->setEmojiSize("xlarge"); break;
            case '5': map->setEmojiSize("huge"); break;
            case 'e': case 'E': map->setEmojiSize("emoji"); break;
        }
    }
}

void Game::logic() {
    snake->move(direction, width, height);

    if (map->checkCollision()) {
        running = false;
        std::cout << "\n💀 GAME OVER! 💀\n";
        return;
    }

    map->eatFood();
    map->eatPowerFruit();
}

void Game::draw() {
    map->draw();
}
