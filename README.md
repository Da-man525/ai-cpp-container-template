# cpp-container-template

## Getting Started

This repository is compatible with [cpp-container](https://github.com/ChicoState/cpp-container). If not already built on your machine, clone and build it.

Run the container:

```bash
docker run -v "$(pwd)":/usr/src -it cpp-container
```

Run the application interactively in a shell:

```bash
docker run -v "$(pwd)":/usr/src -it cpp-container sh
```

## Number Guessing Game

Build and play the game:

```bash
g++ -std=c++17 -Wall -Wextra -Werror main.cpp number_guessing_game.cpp -o /tmp/number_guessing_game
/tmp/number_guessing_game
```

The game chooses one number from 1 through 10 for each run. Enter whole-number
guesses until you find it; the program tells you whether a valid guess is too
low or too high. Invalid input is rejected and does not end the game.

Run the automated tests:

```bash
./test_runner.sh
```

## Structure

- `.agents` - AI agent configurations and skills (in `/skills` subdirectory) for this project
- `.` - The root directory contains the C++ code for the application as well as necessary scripts
- `specs` - Specification documentation
- `tests` - Test code
