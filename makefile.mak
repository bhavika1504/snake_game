# ---------------------------------------------------------
# Makefile for Snake Game (Cross-Platform)
# Author: Bhavika Mulani
# ---------------------------------------------------------

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Detect OS and set platform-specific settings
ifeq ($(OS),Windows_NT)
    TARGET  = snake_game.exe
    RM      = del /Q
    LDFLAGS =
else
    TARGET  = snake_game
    RM      = rm -f
    LDFLAGS = -lpthread
endif

# Source and header files
SRCS    = main.cpp Snake.cpp SnakeMap.cpp Input.cpp Game.cpp
OBJS    = $(SRCS:.cpp=.o)
HEADERS = Direction.h Snake.h SnakeMap.h Input.h Game.h

# ---------------------------------------------------------
# Build Targets
# ---------------------------------------------------------

# Default target: build the executable
all: $(TARGET)

# Link object files into the final binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile each .cpp file to .o
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Remove all compiled objects and executable
clean:
	$(RM) $(OBJS) $(TARGET)

# Build and run
run: all
	./$(TARGET)

.PHONY: all clean run
