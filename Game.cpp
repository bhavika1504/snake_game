#include "Game.h"
#include <iostream>
#include "Direction.h"
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
    
    cout << "\n💀 Game Over! Final Score: " << snake->getSize() - 1 << " 💀\n";
}

void Game::processInput() {
    char c = input->getInput();
    switch (c) {
        // Up: w / W / 'U' (your Input may map arrow up to 'U')
        case 'w': case 'W': case 'U':
            snake->changeDirection(UP);
            break;

        // Down: s / S / 'X' (avoid using 'D' here because it's used for right)
        case 's': case 'S': case 'X':
            snake->changeDirection(DOWN);
            break;

        // Left: a / A / 'L'
        case 'a': case 'A': case 'L':
            snake->changeDirection(LEFT);
            break;

        // Right: d / D / 'R'
        case 'd': case 'D': case 'R':
            snake->changeDirection(RIGHT);
            break;

        case 'q': case 'Q':
            gameOver = true;
            break;

        // Resize board
        case '+': case '=':
            map->resize(map->getWidth() + 2, map->getHeight() + 2);
            break;
        case '-': case '_':
            map->resize(map->getWidth() - 2, map->getHeight() - 2);
            break;

        // Toggle emoji mode (press E)
        case 'e': case 'E':
            map->toggleEmojiMode();
            break;

        // Block size presets
        case '1':
            map->setEmojiSize("small");
            break;
        case '2':
            map->setEmojiSize("medium");
            break;
        case '3':
            map->setEmojiSize("large");
            break;
        case '4':
            map->setEmojiSize("xlarge");
            break;
        case '5':
            map->setEmojiSize("huge");
            break;

        default:
            // no-op for other keys
            break;
    }
}

void Game::update() {
    snake->move();

    auto head = snake->getHead();
    int newRow = head.first;
    int newCol = head.second;

    // Wall collision / wrap when powered
    if (newRow < 0 || newRow >= map->getHeight() || 
        newCol < 0 || newCol >= map->getWidth()) {

        if (snake->isPowerActive()) {
            if (newRow < 0) newRow = map->getHeight() - 1;
            if (newRow >= map->getHeight()) newRow = 0;
            if (newCol < 0) newCol = map->getWidth() - 1;
            if (newCol >= map->getWidth()) newCol = 0;
            snake->getBody().front() = {newRow, newCol};
        } else {
            gameOver = true;
            return;
        }
    }

    // Self-collision
    if (snake->eatsItself()) {
        gameOver = true;
        return;
    }

    // Food
    if (map->checkFood()) {
        snake->setGrow();
        map->spawnFood(); // new food with (random) emoji
        if (gameSpeed > 50) gameSpeed -= 2;
    }

    // Power fruit
    if (map->checkPowerFruit()) {
        // Snake activation handled inside checkPowerFruit (and Game/Map coordinated)
        // keep logic simple here; don't force immediate respawn
    }

    map->updatePowerFruit();
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
