# Lab 4 — Group A

| | |
|---|---|
| Repository | bhavika1504/snake_game |
| Base tag | `lab4-base` at commit `0b22a801463efb37392ec82f47497ad86f4a63bb` |
| Pull request | Lab 4: Keyboard arrow keys and pause input update snake direction and game state |

---

## 1. Five rules — [5]

Written before opening the source. Behaviour, with an observable outcome.

| # | Rule |
|---|---|
| 1 | When the snake moves in a given direction, it cannot immediately reverse into its exact opposite direction in a single turn (e.g. attempting to move LEFT while moving RIGHT is ignored). |
| 2 | The snake dies (triggers self-collision) when its head coordinates enter any coordinate occupied by its own body segments. |
| 3 | When food is spawned on the board, it must always appear on an empty cell that is not occupied by any segment of either snake's body. |
| 4 | When a snake's head moves onto the coordinate containing the food, the snake eats the food, causing it to grow by one segment on its next movement and accelerating game speed. |
| 5 | When the player presses an arrow key ('U', 'D', 'L', 'R') or pause key ('p') during the game loop, the game updates the snake's direction or toggles pause state accordingly. |

If you could not state one of your own game's rules without going to look, say which and
why. It costs no marks.

All five rules describe core observable game mechanics and were stated upfront from gameplay requirements.

---

## 2. What you could test, and what stopped you — [10]

No source changes in this part. Every `file:line` below is a line in `lab4-base`.

| # | Rule | Test written? | Blocking dependency (`file:line` + what it is) |
|---|---|---|---|
| 1 | Direction reversal prevention | Yes | None (pure logic inside `Snake::changeDirection`) |
| 2 | Self-collision detection | Yes | None (pure logic inside `Snake::eatsItself`) |
| 3 | Food spawns on unoccupied cell | No | `SnakeMap.cpp:38-39` — `rand() % height` and `rand() % width` called in loop; coupled with `srand(time(0))` in `SnakeMap.cpp:19` prevents deterministic coordinate control |
| 4 | Snake grows upon eating food | No | `Game.cpp:16` — `new SnakeMap(...)` concrete instantiation in constructor, and `Game.cpp:215-221` has no public API to inject food position or isolate `map->checkFood()` |
| 5 | Keyboard input updates snake direction | No | `Input.cpp:33` — `_getch()` / `_kbhit()` reads directly from hardware console, and `Game.cpp:17` hardcodes `new Input()` with no injection point |

> **Rules testable without modifying the source: 2 / 5**

"It needs user input" is not a blocking dependency. `main.cpp:214 — getch() called inside
the game loop` is.

---

## 3. Coverage, and what it missed — [6]

| | |
|---|---|
| Line coverage | 58.18 % (Snake.cpp) / 8.21 % (Overall project) |
| Branch coverage | 51.06 % (Snake.cpp) / 4.20 % (Overall project) |
| Command used | `g++ --coverage -O0 -g Snake.cpp SnakeMap.cpp Input.cpp Game.cpp tests.cpp -o snake_test.exe && ./snake_test.exe && gcov -b snake_test-Snake.gcno snake_test-Game.gcno snake_test-SnakeMap.gcno snake_test-Input.gcno` |

**One rule that is executed by the suite but not verified by it:**

| | |
|---|---|
| Rule | Rule 1 (Normal movement preserves length / pops tail when grow is false) |
| Line that runs | `Snake.cpp:17` — `body.pop_back();` |
| The assertion that is missing | `assert(snake.getSize() == 1);` (The test verified head position `(10, 11)` but never asserted that the tail popped or that body length remained unchanged). |

---

## 4. The seam — [10]

| | |
|---|---|
| Rule made testable | Rule 5: Keyboard arrow keys and pause input update snake direction and game state |
| Commit 1 (seam) | `60584bb` |
| Commit 2 (test) | `15b5f78` |
| Seam kind | object |
| Enabling point | `Game::Game(int width, int height, IInput* customInput = nullptr)` constructor parameter in `Game.h` / `Game.cpp:16-23` |
| What production code gave up | Production code gave up its concrete ownership of `Input` instantiation. Instead of hardcoding `input = new Input()`, `Game` now accepts any `IInput` implementation, delegating the choice of input source (hardware console vs. test stub) to the caller while preserving identical default behavior when `customInput` is null. |

The last row is graded. If the honest answer is "nothing", write that and say why the
seam cost nothing here.

---

## 5. The double — [4]

| | |
|---|---|
| What you passed through the seam | stub |
| The method under test | `Game::processInput()` |

Two sentences: was the collaborator asked a question or told to do something, and why
does that decide the answer above?

During `Game::processInput()`, the collaborator is asked a question (`input->getInput()`) and returns a character value rather than performing a side-effect command. Because the method under test queries state through a return value, a Stub providing pre-programmed answers is passed through the seam to enable state-based verification on the game.

---

## 6. Two smells in your own tests — [5]

| | Smell | `file:line` | One-line fix |
|---|---|---|---|
| 1 | Assertion roulette | `tests.cpp:52-55` | Add descriptive failure messages to each sequential growth assertion or split them into distinct helper checks. |
| 2 | Eager test | `tests.cpp:34-49` | Split `test_rule1_reverse_direction` into two separate tests: `test_valid_direction_change()` and `test_reverse_direction_ignored()`. |

Fixing them is optional. Finding them is not.
