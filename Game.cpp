#include "Game.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

Game::Game(int width, int height) : gameOver(false), gameSpeed(150) {
    snake = new Snake(height / 2, width / 2);
    map = new SnakeMap(width, height, snake);
    input = new Input();
    input->init();
}

Game::~Game() {
    input->reset();
    delete snake;
    delete map;
    delete input;
}

void Game::run() {
    while (!gameOver) {
        processInput();
        update();
        render();
        this_thread::sleep_for(chrono::milliseconds(gameSpeed));
    }
    
    cout << "Game Over! Final Score: " << snake->getSize() - 1 << "\n";
}

void Game::processInput() {
    char c = input->getInput();
    switch (c) {
        case 'w': case 'W': case 'U':
            snake->changeDirection(UP);
            break;
        case 's': case 'S':
            snake->changeDirection(DOWN);
            break;
        case 'D': // Arrow down
            snake->changeDirection(DOWN);
            break;
        case 'a': case 'A': case 'L':
            snake->changeDirection(LEFT);
            break;
        case 'd':
            snake->changeDirection(RIGHT);
            break;
        case 'R': // Arrow right
            snake->changeDirection(RIGHT);
            break;
        case 'q': case 'Q':
            gameOver = true;
            break;
        case '+': case '=':
            map->resize(map->getWidth() + 2, map->getHeight() + 2);
            break;
        case '-': case '_':
            map->resize(map->getWidth() - 2, map->getHeight() - 2);
            break;
        case 'e': case 'E':
            // Toggle emoji size (ASCII mode)
            map->setEmojiSize("large");
            break;
        case 'n': case 'N':
            // Normal emoji mode
            map->setEmojiSize("normal");
            break;
    }
}

void Game::update() {
    snake->move();
    
    // Check wall collision (only matters if not powered)
    if (snake->collision(map->getWidth(), map->getHeight())) {
        if (!snake->isPowerActive()) {
            gameOver = true;
            return;
        }
        // Wrap around when powered
        auto head = snake->getHead();
        if (head.first < 0) head.first = map->getHeight() - 1;
        if (head.first >= map->getHeight()) head.first = 0;
        if (head.second < 0) head.second = map->getWidth() - 1;
        if (head.second >= map->getWidth()) head.second = 0;
    }
    
    // Check self-collision
    if (snake->eatsItself()) {
        gameOver = true;
        return;
    }
    
    // Check food
    if (map->checkFood()) {
        // Increase speed slightly as score increases
        if (gameSpeed > 50) {
            gameSpeed -= 2;
        }
    }
    
    // Check power fruit
    map->checkPowerFruit();
    
    // Update power fruit spawn/despawn
    map->updatePowerFruit();
    
    // Update snake power timer
    snake->updatePower();
}

void Game::render() {
    map->draw();
}

bool Game::isGameOver() const {
    return gameOver;
}

void Game::setGameSpeed(int speed) {
    gameSpeed = speed;
}