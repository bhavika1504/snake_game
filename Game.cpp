#include "Game.h"
#include <iostream>
#include "Direction.h"
#include <thread>
#include <chrono>

using namespace std;

Game::Game(int width, int height) 
    : gameOver(false), gameSpeed(250) {
    snake = new Snake(height / 2, width / 2);
    map = new SnakeMap(width, height, snake);
    input = new Input();
    input->init();

    cout << "\033[2J\033[H";
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
    this_thread::sleep_for(chrono::seconds(2)); // Small pause before exit
}

void Game::processInput() {
    char c = input->getInput();
    switch (c) {
        // Up: w / W / arrow-up (mapped to 'U')
        case 'w': case 'W': case 'U':
            snake->changeDirection(UP);
            break;

        // Down: s / S / arrow-down (mapped to 'D')
        case 's': case 'S': case 'D':
            snake->changeDirection(DOWN);
            break;

        // Left: a / A / arrow-left (mapped to 'L')
        case 'a': case 'A': case 'L':
            snake->changeDirection(LEFT);
            break;

        // Right: d / arrow-right (mapped to 'R')
        case 'd': case 'R':
            snake->changeDirection(RIGHT);
            break;

        // Quit
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

        // Toggle emoji mode
        case 'e': case 'E':
            map->toggleEmojiMode();
            break;

        // Emoji size presets
        case '1': map->setEmojiSize("small"); break;
        case '2': map->setEmojiSize("medium"); break;
        case '3': map->setEmojiSize("large"); break;
        case '4': map->setEmojiSize("xlarge"); break;
        case '5': map->setEmojiSize("huge"); break;

        default:
            // no action
            break;
    }
}

void Game::update() {
    snake->move();

    auto head = snake->getHead();
    int newRow = head.first;
    int newCol = head.second;

    // Wall collision / wrapping
    if (newRow < 0 || newRow >= map->getHeight() ||
        newCol < 0 || newCol >= map->getWidth()) {
        if (snake->isPowerActive()) {
            // Wrap around when powered
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

    // Self-collision (if head overlaps any other body part)
    if (snake->eatsItself()) {
        gameOver = true;
        return;
    }

    // Eat food
    if (map->checkFood()) {
        snake->setGrow();
        map->spawnFood();

        //  Gradually increase speed, but stay playable
        if (gameSpeed > 120) {
            gameSpeed -= 10;
        }
    }

    // Power fruit
    if (map->checkPowerFruit()) {
        // Power handling inside SnakeMap/Snake
    }

    map->updatePowerFruit();
    snake->updatePower();
}

void Game::render() {
    // Move cursor to top-left — no full clear, smoother motion
    cout << "\033[H";
    map->draw();
    cout.flush(); // Prevents flicker in Windows Terminal
}

bool Game::isGameOver() const {
    return gameOver;
}

void Game::setGameSpeed(int speed) {
    gameSpeed = speed;
}
