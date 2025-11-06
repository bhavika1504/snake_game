# 🐍Snake Game

A feature-rich, cross-platform C++ Snake Game with dynamic board resizing, smooth gameplay, and a power-up system for extra challenge and fun.

# ✨ Features
Cross-Platform: Works seamlessly on Windows, Linux, and macOS
Dynamic Board Sizing: Resize the game board during play
Power Fruit System: Collect ⭐ to become invincible for 10 seconds
Visual Modes: Toggle between Emoji and ASCII display
Smooth Gameplay: Responsive controls and progressive speed increase
Object-Oriented Design: Cleanly modularized codebase for clarity and scalability

# 🎮 Controls
Key	Action
W / ↑	Move Up
S / ↓	Move Down
A / ←	Move Left
D / →	Move Right
Q	Quit Game
P/p Pause Game
+ / =	Increase Board Size
- / _	Decrease Board Size
1	Small Blocks
2	Medium Blocks
3	Large Blocks (Default)
4	X-Large Blocks
5	Huge Blocks
E	Toggle Emoji Mode
🚀 Building and Running
Linux / macOS
# Build
make

# Run
./snake_game

# Or build and run together
make run

# Clean build files
make clean

Windows (MinGW)
mingw32-make
snake_game.exe

# Clean build
mingw32-make clean

Manual Compilation
# Linux/macOS
g++ -std=c++17 -o snake_game main.cpp Snake.cpp SnakeMap.cpp Input.cpp Game.cpp -lpthread

# Windows
g++ -std=c++17 -o snake_game.exe main.cpp Snake.cpp SnakeMap.cpp Input.cpp Game.cpp

# 📁 Project Structure
snake-game/
├── Direction.h         # Direction enumeration
├── Input.h / Input.cpp # Cross-platform input handling
├── Snake.h / Snake.cpp # Snake entity logic
├── SnakeMap.h / SnakeMap.cpp # Game board and rendering
├── Game.h / Game.cpp   # Main game controller
├── main.cpp            # Entry point
├── Makefile            # Build configuration
└── README.md           # Documentation

# 🎯 Gameplay Mechanics
🍎 Regular Food
Increases snake length by 1
Appears randomly
Slightly increases game speed

# ⭐ Power Fruit
Appears randomly (~2% chance per frame)
Remains visible for ~30 seconds
Grants 10 seconds of invincibility
During invincibility:
Snake can pass through walls
Still dies on self-collision
Power timer shown at the top
Board and Display
Default size: 30×20
Range: 10×10 → 50×40
Modes: Emoji or ASCII, switchable anytime

# 🔧 Technical Details
Language: C++17
Dependencies: Standard Library only
Threading: std::this_thread::sleep_for() for timing
Cross-Platform I/O: Uses conio.h (Windows) / termios (Unix)
Screen Control: ANSI escape sequences for clean refresh

# 🐛 Known Limitations
Terminal emoji appearance depends on terminal font and scaling
Rapid key presses can occasionally queue extra moves
Small terminals may truncate board display

# 💡 Future Enhancements
Add difficulty levels
Implement high-score tracking
Introduce obstacles in between
Add local multiplayer mode
Optional sound effects (via SDL or SFML)

# Requirements
C++17 or later
Compatible Compilers: g++, clang, or MinGW

# Author
Bhavika Mulani
Developed for Intro to Programming coursework.
Demonstrates strong application of object-oriented design, modular architecture, and interactive console UI in C++.

# Optional Requirement
Set PowerShell to use UTF-8
Run this once per session before running your game:

chcp 65001
$OutputEncoding = [Console]::OutputEncoding = [System.Text.Encoding]::UTF8