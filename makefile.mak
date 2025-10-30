# Makefile for Snake Game (Cross-platform)

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

# Detect OS
ifeq ($(OS),Windows_NT)
    TARGET = snake.exe
    RM = del /Q
    LDFLAGS =
else
    TARGET = snake
    RM = rm -f
    LDFLAGS = -lpthread
endif

# Source files
SRCS = main.cpp Snake.cpp SnakeMap.cpp Input.cpp Game.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = Direction.h Snake.h SnakeMap.h Input.h Game.h

# Default target
all: $(TARGET)

# Link object files
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	$(RM) $(OBJS) $(TARGET)

# Run the game
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run