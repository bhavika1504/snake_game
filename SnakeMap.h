#ifndef SNAKEMAP_H
#define SNAKEMAP_H

#include <utility>
#include <string>
#include "Snake.h"

class SnakeMap {
private:
    int width, height;
    std::pair<int, int> food;
    std::pair<int, int> powerFruit;
    bool powerFruitActive;
    int powerFruitTimer;
    Snake* snake;
    
    std::string snakeEmoji;
    std::string foodEmoji;
    std::string powerEmoji;
    std::string emptyEmoji;
    std::string powerSnakeEmoji;
    std::string borderEmoji;

    bool emojiMode;      // ✅ Track emoji mode (on/off)

    // ✅ NEW: For smooth on-screen messages (like “Invincible Mode Activated!”)
    std::string modeMessage;
    int messageTimer;

public:
    SnakeMap(int w, int h, Snake* s);
    void spawnFood();
    void spawnPowerFruit();
    bool checkFood();
    bool checkPowerFruit();
    void updatePowerFruit();
    void draw();
    void clearScreen();
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    // Dynamic board size
    void resize(int newWidth, int newHeight);
    
    // Emoji size control (uses Unicode variations)
    void setEmojiSize(const std::string& size);

    // ✅ Added: toggle emoji mode (E key)
    void toggleEmojiMode();
};

#endif
