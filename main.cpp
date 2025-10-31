#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Game.h"

using namespace std;

int main() {
#ifdef _WIN32
    // Enable ANSI escape processing on Windows consoles (PowerShell / conhost / Windows Terminal)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif

    cout << "=== 🐍 SUPER SNAKE GAME 🐍 ===\n\n";
    cout << "Controls:\n";
    cout << "  Movement: WASD or Arrow Keys\n";
    cout << "  Q - Quit\n";
    cout << "  +/- - Increase/Decrease board size\n\n";
    cout << "SIZE CONTROLS (Press during game):\n";
    cout << "  1 - Small   (██)\n";
    cout << "  2 - Medium  (███)\n";
    cout << "  3 - Large   (████) [DEFAULT]\n";
    cout << "  4 - XLarge  (██████)\n";
    cout << "  5 - HUGE    (████████)\n";
    cout << "  E - Emoji   (🟩)\n\n";
    cout << "⭐ SPECIAL: Collect ★★★★ power fruit for 10 seconds of invincibility!\n";
    cout << "   While powered, you can pass through walls!\n\n";
    cout << "Press ENTER to start...\n";
    cin.get();
    
    int width = 20;
    int height = 20;
    
    Game game(width, height);
    game.run();
    
    return 0;
}
