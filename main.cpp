#include <iostream>
#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif
#include "Game.h"

using namespace std;

int main() {
#ifdef _WIN32
   
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif

    cout << "\033[1;32m"; // Bright green color
    cout << "====================================================================\n";
    cout << R"(
   ____   _   _    _    _  __ _____     ____      _    __  __ _____ 
  / ___| | \ | |  / \  | |/ /| ____|   / ___|    / \  |  \/  | ____|
  \___ \ |  \| | / _ \ | ' / |  _|    | |  _    / _ \ | |\/| |  _|  
   ___) || |\  |/ ___ \| . \ | |___   | |_| |  / ___ \| |  | | |___ 
  |____/ |_| \_/_/   \_\_|\_\|_____|   \____| /_/   \_\_|  |_|_____|
    )" << "\n";
    cout << "====================================================================\n";
    cout << "                    🐍  S N A K E   G A M E  🐍\n";
    cout << "====================================================================\n\n";

    cout << "\033[1;36m"; // Cyan
    cout << "🎮  CONTROLS:\n";
    cout << "--------------------------------------------------------------------\n";
    cout << "  ▶  Movement :  WASD  or  Arrow Keys\n";
    cout << "  ⏸  Pause    :  P\n";
    cout << "  ❌  Quit     :  Q\n";
    cout << "  ➕  Increase Board Size :  +\n";
    cout << "  ➖  Decrease Board Size :  -\n\n";

    cout << "\033[1;35m"; // Magenta
    cout << "📏  SIZE CONTROLS (Press during game):\n";
    cout << "--------------------------------------------------------------------\n";
    cout << "  1 - Small     (██)\n";
    cout << "  2 - Medium    (███)\n";
    cout << "  3 - Large     (████)   [DEFAULT]\n";
    cout << "  4 - X-Large   (██████)\n";
    cout << "  5 - HUGE      (████████)\n";
    cout << "  E - Emoji Mode (🟩)\n\n";

    cout << "\033[1;33m"; // Yellow
    cout << "⭐  SPECIAL POWER FRUIT MODE:\n";
    cout << "--------------------------------------------------------------------\n";
    cout << "  🌟  Collect ★★★★ Power Fruit for 10 sec invincibility!\n";
    cout << "  💥  You can pass through walls while powered!\n\n";

    cout << "\033[1;37m"; // White
    cout << "Press \033[1;32mENTER\033[1;37m to start your adventure... 🚀\n";

    cin.get();
    
    int width = 20;
    int height = 20;
    
    Game game(width, height);
    game.run();
    
    return 0;
}
