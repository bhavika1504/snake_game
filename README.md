# 🐍 Enhanced Snake Game

A feature-rich, cross-platform Snake game written in C++ with dynamic board sizing and power-up mechanics!

## ✨ Features

- **Cross-Platform**: Works on Windows, Linux, and macOS
- **Dynamic Board Sizing**: Increase or decrease board size during gameplay
- **Power Fruit System**: Collect ⭐ to become invincible for 10 seconds!
- **Visual Modes**: Switch between emoji and ASCII display modes
- **Smooth Gameplay**: Responsive controls with progressive difficulty
- **Clean Architecture**: Object-oriented design with separate files

## 🎮 Controls

| Key | Action |
|-----|--------|
| W / ↑ | Move Up |
| S / ↓ | Move Down |
| A / ← | Move Left |
| D / → | Move Right |
| Q | Quit Game |
| + / = | Increase Board Size |
| - / _ | Decrease Board Size |
| **1** | **Small Blocks (██)** |
| **2** | **Medium Blocks (███)** |
| **3** | **Large Blocks (████) - DEFAULT** |
| **4** | **XLarge Blocks (██████)** |
| **5** | **HUGE Blocks (████████)** |
| E | Emoji Mode (🟩🍎⭐) |

## 🚀 Building and Running

### Linux/macOS

```bash
# Compile
make

# Run
./snake

# Or compile and run
make run

# Clean build files
make clean
```

### Windows (MinGW)

```bash
# Compile
mingw32-make

# Run
snake.exe

# Clean
mingw32-make clean
```

### Manual Compilation

```bash
# Linux/macOS
g++ -std=c++11 -o snake main.cpp Snake.cpp SnakeMap.cpp Input.cpp Game.cpp -lpthread

# Windows
g++ -std=c++11 -o snake.exe main.cpp Snake.cpp SnakeMap.cpp Input.cpp Game.cpp
```

## 📁 Project Structure

```
snake-game/
├── Direction.h          # Direction enum
├── Input.h/cpp         # Cross-platform input handling
├── Snake.h/cpp         # Snake entity logic
├── SnakeMap.h/cpp      # Game map and rendering
├── Game.h/cpp          # Game loop and controller
├── main.cpp            # Entry point
├── Makefile            # Build configuration
└── README.md           # This file
```

## 🎯 Gameplay Mechanics

### Regular Food (🍎)
- Increases snake length by 1
- Spawns randomly on the board
- Slightly increases game speed

### Power Fruit (⭐)
- Spawns randomly with 2% chance per frame
- Stays on board for ~30 seconds
- Grants **10 seconds of invincibility**
- During power mode:
  - Snake turns golden (🟨)
  - Can pass through walls without dying
  - Still dies from eating itself
  - Timer displayed at top of screen

### Dynamic Board
- Start size: 30x20
- Min size: 10x10
- Max size: 50x40
- Resize during gameplay with +/- keys

### Display Modes
- **Emoji Mode**: Colorful Unicode emojis (default)
- **ASCII Mode**: Text characters for larger terminal display

## 🔧 Technical Details

- **Language**: C++11
- **Dependencies**: Standard library only
- **Threading**: Uses `<thread>` for timing
- **Platform Detection**: Conditional compilation for OS-specific code
- **Terminal Control**: Native APIs (conio.h for Windows, termios for Unix)

## 📝 Notes

- Terminal emoji size depends on your terminal settings
- ASCII mode provides a "larger" visual experience
- Power fruit appearance is random but balanced
- Game speed increases gradually with score
- All collisions respect the power-up status

## 🐛 Known Limitations

- Terminal emoji sizing is controlled by terminal emulator, not the program
- Fast typing may queue multiple direction changes
- Very small terminal windows may not display properly

## 🤝 Contributing

Feel free to fork, modify, and enhance! Some ideas:
- Add difficulty levels
- Implement high score system
- Add more power-up types
- Create multiplayer mode
- Add sound effects (with external library)

## 📜 License

Free to use and modify. Have fun! 🎮

---

**Enjoy the game!** 🐍✨