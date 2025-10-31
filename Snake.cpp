#include "Snake.h"
#include <algorithm>
using namespace std;

Snake::Snake(int startX, int startY)
    : powerActive(false) {
    body.push_back({startX, startY});
}

void Snake::move(Direction dir, int width, int height) {
    // Move head
    pair<int, int> head = body.front();
    switch (dir) {
        case Direction::UP: head.second--; break;
        case Direction::DOWN: head.second++; break;
        case Direction::LEFT: head.first--; break;
        case Direction::RIGHT: head.first++; break;
    }

    // Add new head
    body.insert(body.begin(), head);
    // Remove tail
    body.pop_back();

    // Handle power fruit duration (5s)
    if (powerActive) {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::seconds>(now - powerStartTime).count();
        if (elapsed > 5)
            powerActive = false;
    }
}

void Snake::grow() {
    // Add one more segment at tail (duplicate last)
    body.push_back(body.back());
}

bool Snake::checkCollision(int width, int height) {
    auto head = body.front();

    // Wall collision
    if (head.first <= 0 || head.second <= 0 || head.first >= width - 1 || head.second >= height - 1)
        return true;

    // Self collision (ignore head)
    for (size_t i = 1; i < body.size(); ++i)
        if (body[i] == head)
            return true;

    return false;
}

bool Snake::isPowerActive() {
    return powerActive;
}

void Snake::activatePower() {
    powerActive = true;
    powerStartTime = chrono::steady_clock::now();
}

const vector<pair<int, int>>& Snake::getBody() const {
    return body;
}
