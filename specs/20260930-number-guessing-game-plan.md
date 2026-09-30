# Feature: Number Guessing Game

## Feature Description

Add a command-line number guessing game. Each new game selects a random whole number from 1 through 10, asks the player to enter guesses, and gives feedback until the player finds the generated number.

## User Story

As a command-line player
I want to guess a randomly selected number from 1 through 10
So that I can play a short interactive guessing game and know whether each guess is too high, too low, or correct.

## Problem Statement

`main.cpp` currently exits immediately, so the template has no runnable application behavior. The project needs a small, self-contained example that demonstrates interactive input, random-number generation, and automated verification.

## Solution Statement

Keep the game standard-library-only. Put the complete testable game loop behind a small module that takes input and output streams plus a supplied target; let `main.cpp` only generate the random target and invoke that loop using `std::cin` and `std::cout`. Parse one complete input line at a time so entries such as `4.5` cannot be silently accepted as `4`. Generate the target with a `std::mt19937` engine seeded from `std::random_device` and a uniform distribution whose inclusive range is 1–10. Continue prompting after high or low guesses and terminate successfully after a correct guess.

## Relevant Files

- `main.cpp` — Replace the empty program with the interactive console loop.
- `test_runner.sh` — Update the existing validation entry point so automated tests can run without starting the interactive game.
- `README.md` — Document how to compile, play, and run tests once the game exists.

### New Files

- `number_guessing_game.h` — Declare the game bounds, guess-result model, and testable stream-based game-loop API.
- `number_guessing_game.cpp` — Implement whole-line parsing, range validation, guess comparison, and the game loop.
- `tests/number_guessing_game_test.cpp` — Add a standalone assertion-based test executable for rules and scripted player sessions; no third-party test framework is needed for this small project.

## Implementation Plan

### Phase 1: Foundation

Create a minimal game module that owns the fixed inclusive range (`1` through `10`) and classifies a valid guess as low, high, or correct. Its public entry point accepts `std::istream`, `std::ostream`, and a supplied valid target, which makes the full player flow deterministic in tests without adding a test-only command-line option.

### Phase 2: Core Implementation

Use the module from `main.cpp`. Seed one `std::mt19937` engine from `std::random_device`, draw a target using `std::uniform_int_distribution<int>(1, 10)`, and pass it to the game loop. The loop prints a concise introduction, repeatedly reads one complete player line, rejects malformed or out-of-range values with a clear message, reports low/high feedback for valid incorrect guesses, and prints a success message before returning exit code `0` for a correct guess.

### Phase 3: Integration

Add focused tests and update the project script and README so both developers and players have clear commands. Change the existing script from an interactive launcher into a non-interactive test runner, and compile temporary binaries outside the repository so validation does not create an unignored `app` artifact.

## Step by Step Tasks

### 1. Define the game rules module

- Add `number_guessing_game.h` and `number_guessing_game.cpp`.
- Define a single source of truth for the inclusive bounds 1 and 10 and an explicit low/high/correct result type.
- Implement a game-loop function that receives `std::istream`, `std::ostream`, and a valid target. Keep random generation out of this function.
- Read a complete line with `std::getline`; parse it as exactly one integer (for example, with `std::from_chars` after whitespace handling) and reject trailing non-whitespace content.
- Implement helpers to determine whether a parsed guess is in range and whether it is too low, too high, or correct.

### 2. Add unit tests before wiring the console

- Add `tests/number_guessing_game_test.cpp` using standard `assert` checks and a non-zero return on failure.
- Verify both boundaries are accepted, values below 1 and above 10 are rejected, and each comparison outcome is correct.
- Feed scripted `std::istringstream` input into the game loop with a fixed target and assert its captured `std::ostringstream` output contains the invalid-input, too-low, too-high, and success messages in order.
- Add a scripted EOF session and verify the loop returns cleanly rather than prompting forever.

### 3. Implement the interactive game flow

- Replace the empty body in `main.cpp` with minimal orchestration: create one seeded `std::mt19937`, use `std::uniform_int_distribution<int>(1, 10)` to select one target, and call the game loop with standard streams.
- Prompt until the player enters an integer in the allowed range or standard input reaches EOF.
- For malformed, decimal, or out-of-range entries, display the validation message and reprompt; do not partially accept a numeric prefix.
- For valid incorrect guesses, state whether the guess is too low or too high; for a correct guess, congratulate the player and exit normally.
- Treat EOF as a normal cancelled session: display a short closing message and return a defined non-zero status from `main`.

### 4. Make validation repeatable and document the commands

- Update `test_runner.sh` to compile and run the test executable separately from the interactive application, using a `mktemp` directory plus a cleanup trap rather than writing `app` into the repository root.
- Update `README.md` with a build/play command and the test command.
- Run every validation command below as the final implementation step.

## Testing Strategy

### Unit Tests

- Range validation accepts 1 and 10 and rejects 0 and 11.
- Guess comparison returns low, high, and correct for representative values.
- Scripted game sessions verify the exact feedback sequence for invalid, low, high, and correct guesses against a known target.
- Empty input / EOF ends cleanly rather than spinning indefinitely.

### Edge Cases

- An empty or whitespace-only entry is rejected and the game prompts again.
- A non-numeric entry such as `abc` is rejected and the game prompts again.
- A decimal-looking entry such as `4.5` is not accepted as a valid whole-number guess.
- An out-of-range integer such as `0` or `11` is rejected and the game continues.
- The first valid guess can be correct.
- Multiple incorrect guesses retain the same randomly generated target.

## Acceptance Criteria

- The game generates exactly one random integer inclusively between 1 and 10 per run.
- The program repeatedly accepts guesses until the generated number is entered.
- Incorrect valid guesses receive accurate “too low” or “too high” feedback.
- Invalid or out-of-range input does not crash or terminate the game.
- A correct guess displays a success message and returns exit code 0.
- Automated tests cover rule behavior and pass without requiring interactive input.
- EOF ends a game session predictably and does not create an infinite prompt loop.

## Validation Commands

Execute every command to validate the feature works correctly with zero regressions.

```bash
g++ -std=c++17 -Wall -Wextra -Werror -I. number_guessing_game.cpp tests/number_guessing_game_test.cpp -o /tmp/number_guessing_game_test && /tmp/number_guessing_game_test
g++ -std=c++17 -Wall -Wextra -Werror main.cpp number_guessing_game.cpp -o /tmp/number_guessing_game
printf 'abc\n4.5\n0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n' | /tmp/number_guessing_game
./test_runner.sh
```

The piped smoke check deliberately supplies malformed input and all legal guesses, so it completes regardless of the generated target. The test executable is the authoritative check for each feedback path because it supplies a fixed target.

## Notes

- This plan assumes a standard retry-until-correct game rather than a single-attempt game, since the request does not specify an attempt limit.
- No external random-number or testing library is necessary.
- An explicit stream-based loop is the smallest useful abstraction here: it prevents nondeterministic interactive tests without exposing test-only behavior to players.
- A future enhancement could track the number of guesses or offer replay, but neither is in scope for this request.
