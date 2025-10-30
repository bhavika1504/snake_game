#include "Input.h"

void Input::init() {
#ifndef _WIN32
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
#endif
}

void Input::reset() {
#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}

bool Input::kbhit() {
#ifdef _WIN32
    return _kbhit();
#else
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
#endif
}

char Input::getInput() {
#ifdef _WIN32
    if (kbhit()) {
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            switch (ch) {
                case 72: return 'U'; // UP
                case 80: return 'D'; // DOWN
                case 75: return 'L'; // LEFT
                case 77: return 'R'; // RIGHT
            }
        } else {
            return ch;
        }
    }
    return 0;
#else
    if (kbhit()) {
        char seq[3] = {0};
        read(STDIN_FILENO, &seq, 3);
        if (seq[0] == '\033') {
            switch (seq[2]) {
                case 'A': return 'U'; // UP
                case 'B': return 'D'; // DOWN
                case 'C': return 'R'; // RIGHT
                case 'D': return 'L'; // LEFT
            }
        } else {
            return seq[0];
        }
    }
    return 0;
#endif
}