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

class IInput {
public:
    virtual ~IInput() = default;
    virtual void init() = 0;
    virtual void reset() = 0;
    virtual char getInput() = 0;
};

class Input : public IInput {
private:
#ifndef _WIN32
    struct termios oldt, newt;
#endif

public:
    void init() override;
    void reset() override;
    char getInput() override;
private:
    bool kbhit();
};

#endif