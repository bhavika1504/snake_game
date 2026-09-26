#include "Game.h"
#include <iostream>
#include "Direction.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

Game::Game(int width, int height, IInput* customInput) 
    : loser(0), gameOver(false), gameSpeed(250) {
    snake = new Snake(height / 2, width / 2);
    snake2 = new Snake(height / 4, width / 4);
    map = new SnakeMap(width, height, snake, snake2);
    if (customInput) {
        input = customInput;
        ownsInput = false;
    } else {
        input = new Input();
        ownsInput = true;
    }
    input->init();
    paused = false;

    cout << "\033[2J\033[H";
}

Game::~Game() {
    input->reset();
    delete snake;
    delete snake2;
    delete map;
    if (ownsInput) {
        delete input;
    }
}

void Game::run() {
    while (!gameOver) {
        processInput();

        if (!paused) {
            update();
            render();

            // Control frame speed dynamically
#ifdef _WIN32
            Sleep(gameSpeed);
#else
            usleep(gameSpeed * 1000); 
#endif
        } else {
            // If paused, just wait a little so CPU isn’t overloaded
#ifdef _WIN32
            Sleep(200);
#else
            usleep(200000);
#endif
        }
    }

    // After loop ends (gameOver == true)
    if (loser == 1)
        cout << "\n💀 Game Over! Player 1 lost! P1 Score: " << snake->getSize() - 1 << " | P2 Score: " << snake2->getSize() - 1 << " 💀\n";
    else if (loser == 2)
        cout << "\n💀 Game Over! Player 2 lost! P1 Score: " << snake->getSize() - 1 << " | P2 Score: " << snake2->getSize() - 1 << " 💀\n";
    else
        cout << "\n💀 Game Over! P1 Score: " << snake->getSize() - 1 << " | P2 Score: " << snake2->getSize() - 1 << " 💀\n";
#ifdef _WIN32
    Sleep(2000); // brief pause before exit
#else
    usleep(2000000);
#endif
}


void Game::processInput() {
    char c = input->getInput();
    switch (c) {
        // Player 1: arrow keys only
        case 'U':
            snake->changeDirection(UP);
            break;
        case 'D':
            snake->changeDirection(DOWN);
            break;
        case 'L':
            snake->changeDirection(LEFT);
            break;
        case 'R':
            snake->changeDirection(RIGHT);
            break;

        // Player 2: WASD
        case 'w': case 'W':
            snake2->changeDirection(UP);
            break;
        case 's': case 'S':
            snake2->changeDirection(DOWN);
            break;
        case 'a': case 'A':
            snake2->changeDirection(LEFT);
            break;
        case 'd':
            snake2->changeDirection(RIGHT);
            break;

        // Quit
        case 'q': case 'Q':
            gameOver = true;
            break;
        case 'p': case 'P':
            paused = !paused;
            if (paused)
                std::cout << "\n⏸ Game Paused — Press 'P' to Resume ⏯\n";
            else
                std::cout << "\n▶ Game Resumed!\n";
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
    snake2->move();

    auto head1 = snake->getHead();
    int row1 = head1.first;
    int col1 = head1.second;

    auto head2 = snake2->getHead();
    int row2 = head2.first;
    int col2 = head2.second;

    // Wall collision / wrapping for Player 1
    if (row1 < 0 || row1 >= map->getHeight() ||
        col1 < 0 || col1 >= map->getWidth()) {
        if (snake->isPowerActive()) {
            if (row1 < 0) row1 = map->getHeight() - 1;
            if (row1 >= map->getHeight()) row1 = 0;
            if (col1 < 0) col1 = map->getWidth() - 1;
            if (col1 >= map->getWidth()) col1 = 0;
            snake->getBody().front() = {row1, col1};
        } else {
            loser = 1;
            gameOver = true;
            return;
        }
    }

    // Wall collision / wrapping for Player 2
    if (row2 < 0 || row2 >= map->getHeight() ||
        col2 < 0 || col2 >= map->getWidth()) {
        if (snake2->isPowerActive()) {
            if (row2 < 0) row2 = map->getHeight() - 1;
            if (row2 >= map->getHeight()) row2 = 0;
            if (col2 < 0) col2 = map->getWidth() - 1;
            if (col2 >= map->getWidth()) col2 = 0;
            snake2->getBody().front() = {row2, col2};
        } else {
            loser = 2;
            gameOver = true;
            return;
        }
    }

    // Self-collision
    if (snake->eatsItself()) {
        loser = 1;
        gameOver = true;
        return;
    }
    if (snake2->eatsItself()) {
        loser = 2;
        gameOver = true;
        return;
    }

    // Cross-collision: P1 head hits P2 body
    for (auto& seg : snake2->getBody()) {
        if (snake->getHead() == seg) {
            loser = 1;
            gameOver = true;
            return;
        }
    }
    // Cross-collision: P2 head hits P1 body
    for (auto& seg : snake->getBody()) {
        if (snake2->getHead() == seg) {
            loser = 2;
            gameOver = true;
            return;
        }
    }

    // Eat food — grow whichever snake's head is on the food
    if (map->checkFood()) {
        auto food = map->getFood();
        auto h1 = snake->getHead();
        auto h2 = snake2->getHead();
        if (h1.first == food.first && h1.second == food.second) snake->setGrow();
        if (h2.first == food.first && h2.second == food.second) snake2->setGrow();
        map->spawnFood();

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
    snake2->updatePower();
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
