#include "Snake.h"

Snake::Snake(int x, int y) : dir(RIGHT), grow(false), isPowered(false), powerTimer(0) {
    body.push_back({x, y});
}

void Snake::move() {
    std::pair<int, int> head = body.front();
    switch (dir) {
        case UP: head.first--; break;
        case DOWN: head.first++; break;
        case LEFT: head.second--; break;
        case RIGHT: head.second++; break;
    }
    body.push_front(head);
    if (!grow) {
        body.pop_back();
    } else {
        grow = false;
    }
}

void Snake::changeDirection(Direction newDir) {
    if ((dir == UP && newDir != DOWN) || (dir == DOWN && newDir != UP) ||
        (dir == LEFT && newDir != RIGHT) || (dir == RIGHT && newDir != LEFT)) {
        dir = newDir;
    }
}

bool Snake::collision(int width, int height) {
    if (isPowered) return false; // Invincible when powered
    
    auto head = body.front();
    return (head.first < 0 || head.first >= height || 
            head.second < 0 || head.second >= width);
}

bool Snake::eatsItself() {
    auto head = body.front();
    for (std::size_t i = 1; i < body.size(); i++) {
        if (body[i] == head) return true;
    }
    return false;
}

std::pair<int, int> Snake::getHead() const {
    return body.front();
}

std::deque<std::pair<int, int>>& Snake::getBody() {
    return body;
}

const std::deque<std::pair<int, int>>& Snake::getBody() const {
    return body;
}

int Snake::getSize() const {
    return body.size();
}

void Snake::setGrow() {
    grow = true;
}

void Snake::activatePower() {
    isPowered = true;
    powerTimer = 67; // 10 seconds at 150ms per frame
}

void Snake::updatePower() {
    if (isPowered && powerTimer > 0) {
        powerTimer--;
        if (powerTimer == 0) {
            isPowered = false;
        }
    }
}

bool Snake::isPowerActive() const {
    return isPowered;
}

int Snake::getPowerTimeLeft() const {
    return (powerTimer * 150) / 1000; // Convert to seconds
}