#include <iostream>
#include "Game.h"

using namespace std;

int main() {
    cout << "=== SNAKE GAME ===\n";
    cout << "Controls:\n";
    cout << "  WASD or Arrow Keys - Move\n";
    cout << "  Q - Quit\n";
    cout << "  +/- - Increase/Decrease board size\n";
    cout << "  E - ASCII mode (larger display)\n";
    cout << "  N - Emoji mode (normal)\n\n";
    cout << "Special: Collect ⭐ power fruit for invincibility!\n";
    cout << "Press any key to start...\n";
    cin.get();
    
    int width = 30;
    int height = 20;
    
    Game game(width, height);
    game.run();
    
    return 0;
}