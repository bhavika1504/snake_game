#ifndef INPUT_H
#define INPUT_H

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#endif

class Input {
private:
#ifndef _WIN32
    struct termios oldt, newt;
#endif

public:
    void init();
    void reset();
    char getInput();
private:
    bool kbhit();
};

#endif