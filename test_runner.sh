#!/usr/bin/env bash

set -euo pipefail

build_directory=$(mktemp -d "${TMPDIR:-/tmp}/number_guessing_game.XXXXXX")
trap 'rm -rf "$build_directory"' EXIT

g++ -std=c++17 -Wall -Wextra -Werror -I. number_guessing_game.cpp \
  tests/number_guessing_game_test.cpp -o "$build_directory/number_guessing_game_test"
"$build_directory/number_guessing_game_test"
