# Spec: Number Guessing Game

## Objective

Build a small interactive C++ command-line game. On each launch, the program chooses one random integer in the inclusive range 1–10. The player enters whole-number guesses until the number is found. After every valid incorrect guess, the program says whether it was too low or too high; after a correct guess, it confirms success and exits with status `0`.

The game must handle untrusted terminal input safely: blank, non-numeric, decimal-looking, and out-of-range entries are rejected with a message and do not end the game. If input closes before a correct guess, the program ends cleanly with a defined non-zero status rather than looping indefinitely.

Success means a player can complete the game interactively, while automated tests deterministically verify all input and feedback paths without relying on a random target.

## Tech Stack

- Language: C++17.
- Standard library only; do not add a package manager or third-party dependency.
- Randomness: `std::random_device`, `std::mt19937`, and `std::uniform_int_distribution<int>`.
- Input handling: `std::getline` plus strict whole-line integer parsing (for example, `std::from_chars`).
- Tests: a standalone C++ assertion-based executable.
- Shell: POSIX-compatible `bash` for `test_runner.sh`.

## Commands

These commands apply after the feature is implemented:

```bash
# Build an interactive binary outside the repository and play it.
g++ -std=c++17 -Wall -Wextra -Werror main.cpp number_guessing_game.cpp -o /tmp/number_guessing_game
/tmp/number_guessing_game

# Build and run the automated test executable.
g++ -std=c++17 -Wall -Wextra -Werror -I. number_guessing_game.cpp tests/number_guessing_game_test.cpp -o /tmp/number_guessing_game_test && /tmp/number_guessing_game_test

# Run the repository's non-interactive validation script.
./test_runner.sh
```

## Project Structure

```text
main.cpp                            application entry point; chooses the random target
number_guessing_game.h              game constants, result type, and stream-based API
number_guessing_game.cpp            parsing, validation, comparisons, and game loop
tests/number_guessing_game_test.cpp deterministic rule and scripted-session tests
test_runner.sh                      compiles and runs automated tests using temporary output
README.md                           player and developer instructions
specs/20260930-number-guessing-game-plan.md  reviewed implementation plan
specs/20260930-number-guessing-game-spec.md  this specification
```

`main.cpp` must not own input-validation rules or feedback branches. It generates one target per process and delegates the game session to the module. The module accepts `std::istream`, `std::ostream`, and a supplied target so tests can drive a known session.

## Code Style

- Use descriptive `snake_case` names for functions and variables, and a scoped enum or equivalent explicit type for guess outcomes.
- Keep public declarations in the header and implementation details in the `.cpp` file.
- Keep the entry point limited to random-target creation and orchestration.
- Use braces and straightforward early exits; avoid global mutable state and test-only runtime flags.
- Make the inclusive bounds a single source of truth rather than repeating numeric literals throughout the game logic.

```cpp
enum class guess_result { too_low, too_high, correct };

guess_result compare_guess(int guess, int target) {
  if (guess < target) {
    return guess_result::too_low;
  }
  if (guess > target) {
    return guess_result::too_high;
  }
  return guess_result::correct;
}
```

## Testing Strategy

Use `tests/number_guessing_game_test.cpp` to compile a separate test executable. Tests must not depend on the result of a real random draw or on a human terminal.

- Test range validation at both inclusive boundaries (`1`, `10`) and just outside them (`0`, `11`).
- Test guess comparison for lower, higher, and equal values.
- Provide `std::istringstream` input and a fixed target to a stream-based game loop; capture output with `std::ostringstream`.
- Assert the scripted output reports invalid input, too-low feedback, too-high feedback, and success in that order.
- Include blank, non-numeric (`abc`), decimal-looking (`4.5`), and out-of-range input cases; numeric prefixes with trailing content must be rejected, not partially accepted.
- Include an empty-input/EOF session and assert it terminates cleanly.
- Use `test_runner.sh` as the normal all-tests command. It must create its binary in a `mktemp` directory and remove it with a shell trap.

No numeric coverage threshold is required for this small repository. The test suite must cover every branch in the game rules and all user-visible terminal outcomes.

## Boundaries

### Always

- Compile with `-std=c++17 -Wall -Wextra -Werror`.
- Validate the entire player input line before using a value as a guess.
- Use one target for the duration of a game session.
- Run the automated test executable and `./test_runner.sh` before considering the work complete.
- Keep generated binaries outside the repository working tree.

### Ask First

- Add or upgrade external dependencies, build systems, or testing frameworks.
- Change the game range, introduce an attempt limit, scoring, replay behavior, or persistent data.
- Change container configuration, CI, or repository-wide compiler settings.

### Never

- Commit secrets or generated binaries.
- Accept partial numeric input such as `4.5` as a legal guess of `4`.
- Use modulo arithmetic for target selection.
- Make automated tests rely on a particular random sequence.
- Remove or weaken a failing test to make validation pass.

## Success Criteria

- Each program launch selects exactly one target through an inclusive uniform distribution from 1 to 10.
- The program keeps prompting after every invalid or incorrect guess until success or EOF.
- Valid guesses below the target receive too-low feedback; valid guesses above it receive too-high feedback.
- A valid exact match receives success feedback and exits with code `0`.
- Invalid input never crashes the program, ends a normal game, or becomes a different valid guess through partial parsing.
- EOF prints a closing message and returns a documented non-zero exit code.
- The automated test suite exercises validation, comparison, a full scripted game, and EOF without interactive input.
- All commands in the Commands section complete without errors after implementation.

## Open Questions

- This specification assumes unlimited guesses and no replay prompt. Confirm this is the intended experience before implementation; otherwise update this specification and the reviewed plan first.
