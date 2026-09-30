#pragma once

#include <iosfwd>

constexpr int minimum_guess = 1;
constexpr int maximum_guess = 10;

enum class guess_result { too_low, too_high, correct };
enum class game_status { guessed_correctly, input_ended };

bool is_valid_guess(int guess);
guess_result compare_guess(int guess, int target);
game_status run_game(std::istream& input, std::ostream& output, int target);
