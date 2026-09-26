#include <iostream>
#include <cassert>
#include <vector>
#include <utility>
#include "Snake.h"
#include "Direction.h"
#include "Game.h"
#include "Input.h"

// Test tracking counters
static int passed_count = 0;
static int failed_count = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { \
            std::cout << "  [PASS] " << msg << "\n"; \
            passed_count++; \
        } else { \
            std::cerr << "  [FAIL] " << msg << "\n"; \
            failed_count++; \
        } \
    } while (0)

// Test Double: Stub for IInput interface (Object Seam)
class StubInput : public IInput {
private:
    std::vector<char> keys;
    size_t index;
public:
    StubInput(const std::vector<char>& key_seq) : keys(key_seq), index(0) {}
    void init() override {}
    void reset() override {}
    char getInput() override {
        if (index < keys.size()) {
            return keys[index++];
        }
        return 0;
    }
};

// -----------------------------------------------------------------------------
// Rule 1: A snake cannot reverse directly into the opposite direction
// -----------------------------------------------------------------------------
void test_rule1_reverse_direction() {
    std::cout << "--- Testing Rule 1: Direction Reversal Prevention ---\n";
    Snake snake(10, 10); // Initial direction is RIGHT

    // Reversing RIGHT -> LEFT should be rejected
    snake.changeDirection(LEFT);
    snake.move(); // Moves RIGHT to (10, 11)
    TEST_ASSERT(snake.getHead() == std::make_pair(10, 11), 
                "Snake ignores immediate 180-degree turn (LEFT while moving RIGHT)");

    // Turning UP is valid
    snake.changeDirection(UP);
    snake.move(); // Moves UP to (9, 11)
    TEST_ASSERT(snake.getHead() == std::make_pair(9, 11), 
                "Snake successfully changes direction to UP");

    // Reversing UP -> DOWN should be rejected
    snake.changeDirection(DOWN);
    snake.move(); // Continues UP to (8, 11)
    TEST_ASSERT(snake.getHead() == std::make_pair(8, 11), 
                "Snake ignores immediate 180-degree turn (DOWN while moving UP)");
}

// -----------------------------------------------------------------------------
// Rule 2: Snake self-collision detection
// -----------------------------------------------------------------------------
void test_rule2_self_collision() {
    std::cout << "\n--- Testing Rule 2: Self-Collision Detection ---\n";
    Snake snake(5, 5);

    // Grow snake to 5 segments
    snake.setGrow(); snake.move(); // (5, 6)
    snake.setGrow(); snake.move(); // (5, 7)
    snake.setGrow(); snake.move(); // (5, 8)
    snake.setGrow(); snake.move(); // (5, 9)

    TEST_ASSERT(snake.getSize() == 5, "Snake successfully grew to length 5");
    TEST_ASSERT(!snake.eatsItself(), "Snake does not collide with itself while moving in a line");

    // Loop snake back into its own body: UP -> LEFT -> DOWN
    snake.changeDirection(UP);
    snake.move(); // (4, 9)
    snake.changeDirection(LEFT);
    snake.move(); // (4, 8)
    snake.changeDirection(DOWN);
    snake.move(); // (5, 8) -> Collides with body segment at (5, 8)

    TEST_ASSERT(snake.eatsItself() == true, 
                "Snake detects self-collision when head enters occupied body tile");
}

// -----------------------------------------------------------------------------
// Rule 5 (via Seam): Keyboard arrow keys update Snake direction in Game::processInput
// -----------------------------------------------------------------------------
void test_rule5_input_processing_via_seam() {
    std::cout << "\n--- Testing Rule 5: Input Processing via Seam (StubInput) ---\n";

    // Scenario A: P1 'U' key turns Snake 1 UP
    {
        StubInput stub({'U'});
        Game game(20, 20, &stub);
        game.processInput();
        game.update();
        TEST_ASSERT(game.getSnake()->getHead() == std::make_pair(9, 10),
                    "Game::processInput correctly directed Snake 1 UP upon 'U' key");
    }

    // Scenario B: P1 'L' key turns Snake 1 LEFT (after valid intermediate turn)
    {
        StubInput stub({'L'});
        Game game(20, 20, &stub);
        game.getSnake()->changeDirection(UP);
        game.processInput();
        game.update();
        TEST_ASSERT(game.getSnake()->getHead() == std::make_pair(10, 9),
                    "Game::processInput correctly directed Snake 1 LEFT upon 'L' key");
    }

    // Scenario C: 'P' key toggles game pause state
    {
        StubInput stub({'p'});
        Game game(20, 20, &stub);
        TEST_ASSERT(!game.isPaused(), "Game starts in unpaused state");
        game.processInput();
        TEST_ASSERT(game.isPaused(), "Game paused state toggled ON after 'p' keypress");
    }
}

int main() {
    std::cout << "========================================\n";
    std::cout << "     Lab 4 Snake Game Test Suite        \n";
    std::cout << "========================================\n\n";

    test_rule1_reverse_direction();
    test_rule2_self_collision();
    test_rule5_input_processing_via_seam();

    std::cout << "\n========================================\n";
    std::cout << "Results: " << passed_count << " passed, " << failed_count << " failed.\n";
    std::cout << "========================================\n";

    return (failed_count == 0) ? 0 : 1;
}
