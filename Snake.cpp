#include "Snake.h"

Snake::Snake(int x, int y)
    : dir(Direction::RIGHT), growNext(false), isPowered(false), powerTimer(0) {
    body.push_back({x, y});
}

void Snake::move() {
    std::pair<int, int> head = body.front();

    switch (dir) {
        case Direction::UP: head.first--; break;
        case Direction::DOWN: head.first++; break;
        case Direction::LEFT: head.second--; break;
        case Direction::RIGHT: head.second++; break;
        default: break;
    }

    body.push_front(head);

    if (!growNext)
        body.pop_back();
    else
        growNext = false;
}

void Snake::changeDirection(Direction newDir) {
    if ((dir == Direction::UP && newDir == Direction::DOWN) ||
        (dir == Direction::DOWN && newDir == Direction::UP) ||
        (dir == Direction::LEFT && newDir == Direction::RIGHT) ||
        (dir == Direction::RIGHT && newDir == Direction::LEFT))
        return;

    if (newDir != Direction::STOP)
        dir = newDir;
}

bool Snake::collision(int width, int height) {
    if (isPowered) return false;
    auto head = body.front();
    return (head.first < 0 || head.first >= height ||
            head.second < 0 || head.second >= width);
}

bool Snake::eatsItself() {
    auto head = body.front();
    for (size_t i = 1; i < body.size(); ++i)
        if (body[i] == head) return true;
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

void Snake::grow() {
    growNext = true;
}

void Snake::activatePower() {
    isPowered = true;
    powerTimer = 67; // ≈10s if each frame ~150ms
}

void Snake::updatePower() {
    if (isPowered && powerTimer > 0) {
        powerTimer--;
        if (powerTimer == 0)
            isPowered = false;
    }
}

bool Snake::isPowerActive() const {
    return isPowered;
}

int Snake::getPowerTimeLeft() const {
    return (powerTimer * 150) / 1000; // in seconds
}
