#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Game.h"

using namespace std;

int main() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif

    cout << "=== 🐍 SUPER SNAKE GAME 🐍 ===\n\n";
    cout << "Controls: WASD / Arrows | Q Quit | +/- Resize | 1–5 Size | E Emoji\n";
    cout << "⭐ Collect 💥 for 10s INVINCIBLE MODE ⭐\n\n";
    cout << "Press ENTER to start...";
    cin.get();

    Game game(20, 20);
    game.run();

    return 0;
}
