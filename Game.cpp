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
            // Switch to original emoji mode (smallest)
            map->setEmojiSize("emoji");
            break;
        case '1':
            // Small blocks (2-wide)
            map->setEmojiSize("small");
            break;
        case '2':
            // Medium blocks (3-wide)
            map->setEmojiSize("medium");
            break;
        case '3':
            // Large blocks (4-wide) - DEFAULT
            map->setEmojiSize("large");
            break;
        case '4':
            // Extra large blocks (6-wide)
            map->setEmojiSize("xlarge");
            break;
        case '5':
            // HUGE blocks (8-wide)
            map->setEmojiSize("huge");
            break;
    }
}

void Game::update() {
    snake->move();
    
    // Get current head position
    auto head = snake->getHead();
    int newRow = head.first;
    int newCol = head.second;
    bool hitWall = false;
    
    // Check wall collision
    if (newRow < 0 || newRow >= map->getHeight() || 
        newCol < 0 || newCol >= map->getWidth()) {
        hitWall = true;
        
        if (snake->isPowerActive()) {
            // WRAP AROUND when powered - teleport to other side!
            if (newRow < 0) newRow = map->getHeight() - 1;
            if (newRow >= map->getHeight()) newRow = 0;
            if (newCol < 0) newCol = map->getWidth() - 1;
            if (newCol >= map->getWidth()) newCol = 0;
            
            // Update the snake's head position to wrapped location
            snake->getBody().front() = {newRow, newCol};
        } else {
            // Not powered - game over
            gameOver = true;
            return;
        }
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