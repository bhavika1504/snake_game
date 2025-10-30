#include <iostream>
#include <deque>
#include <utility>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

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

using namespace std;

enum Direction { UP, DOWN, LEFT, RIGHT };

// ---------- Cross-platform input ----------
#ifdef _WIN32
char getInput() {
    if (_kbhit()) {
        int ch = _getch();
        if(ch == 0 || ch == 224){ // arrow keys
            ch = _getch();
            switch(ch){
                case 72: return 'U'; // UP
                case 80: return 'D'; // DOWN
                case 75: return 'L'; // LEFT
                case 77: return 'R'; // RIGHT
            }
        } else return ch;
    }
    return 0;
}
#else
struct termios oldt, newt;
void initInput() {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void resetInput() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

bool kbhit() {
    struct timeval tv = {0,0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO+1, &fds, NULL, NULL, &tv) > 0;
}

char getInput() {
    char c = 0;
    if(kbhit()){
        char seq[3];
        read(STDIN_FILENO, &seq, 3);
        if(seq[0]=='\033'){ // arrow key
            switch(seq[2]){
                case 'A': return 'U'; // UP
                case 'B': return 'D'; // DOWN
                case 'C': return 'R'; // RIGHT
                case 'D': return 'L'; // LEFT
            }
        } else return seq[0];
    }
    return 0;
}
#endif

// ---------- Snake Class ----------
class Snake {
public:
    deque<pair<int,int>> body;
    Direction dir;
    bool grow;

    Snake(int x, int y) {
        body.push_back({x, y});
        dir = RIGHT;
        grow = false;
    }

    void move() {
        pair<int,int> head = body.front();
        switch(dir) {
            case UP: head.first--; break;
            case DOWN: head.first++; break;
            case LEFT: head.second--; break;
            case RIGHT: head.second++; break;
        }
        body.push_front(head);
        if(!grow) body.pop_back();
        else grow=false;
    }

    void changeDirection(Direction newDir) {
        if((dir==UP && newDir!=DOWN) || (dir==DOWN && newDir!=UP) ||
           (dir==LEFT && newDir!=RIGHT) || (dir==RIGHT && newDir!=LEFT))
            dir=newDir;
    }

    bool collision(int width, int height) {
        auto head = body.front();
        return (head.first<0 || head.first>=height || head.second<0 || head.second>=width);
    }

    bool eatsItself() {
        auto head = body.front();
        for(size_t i=1;i<body.size();i++)
            if(body[i]==head) return true;
        return false;
    }

    pair<int,int> getHead() { return body.front(); }
};

// ---------- Map and Food ----------
class SnakeMap {
public:
    int width, height;
    pair<int,int> food;
    Snake *snake;

    SnakeMap(int w, int h, Snake *s) {
        width=w; height=h; snake=s;
        srand(time(0));
        spawnFood();
    }

    void spawnFood() {
        int x,y;
        bool conflict;
        do {
            x=rand()%height;
            y=rand()%width;
            conflict=false;
            for(auto &seg : snake->body) if(seg.first==x && seg.second==y) conflict=true;
        } while(conflict);
        food={x,y};
    }

    bool checkFood() {
        if(snake->getHead()==food) {
            snake->grow=true;
            spawnFood();
            return true;
        }
        return false;
    }

    void draw() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                bool printed=false;
                if(food.first==i && food.second==j){ cout<<"🍎"; printed=true; }
                for(auto &seg : snake->body){
                    if(seg.first==i && seg.second==j){ cout<<"🟩"; printed=true; break; }
                }
                if(!printed) cout<<"⬛";
            }
            cout<<"\n";
        }
        cout<<"Score: "<<snake->body.size()-1<<"\n";
    }
};

// ---------- Main ----------
int main() {
    int width=30, height=20; // bigger board
    Snake snake(height/2, width/2);
    SnakeMap map(width, height, &snake);

#ifndef _WIN32
    initInput();
#endif

    bool gameOver=false;
    while(!gameOver){
        char c = getInput();
        switch(c){
            case 'w': case 'U': snake.changeDirection(UP); break;
            case 's': case 'D': snake.changeDirection(DOWN); break;
            case 'a': case 'L': snake.changeDirection(LEFT); break;
            case 'd': case 'R': snake.changeDirection(RIGHT); break;
            case 'q': gameOver=true; break;
        }

        snake.move();
        if(snake.collision(width,height) || snake.eatsItself()){ gameOver=true; break; }
        map.checkFood();
        map.draw();
        this_thread::sleep_for(chrono::milliseconds(150)); // faster speed
    }

#ifndef _WIN32
    resetInput();
#endif
    cout<<"Game Over! Final Score: "<<snake.body.size()-1<<"\n";
    return 0;
}
