#include "number_guessing_game.h"

#include <charconv>
#include <cctype>
#include <istream>
#include <ostream>
#include <string>

namespace {

bool parse_whole_number(const std::string& line, int& value) {
  std::size_t first = 0;
  while (first < line.size() &&
         std::isspace(static_cast<unsigned char>(line[first]))) {
    ++first;
  }

  if (first == line.size()) {
    return false;
  }

  const char* begin = line.data() + first;
  const char* end = line.data() + line.size();
  const std::from_chars_result parsed = std::from_chars(begin, end, value);

  if (parsed.ec != std::errc()) {
    return false;
  }

  const char* remaining = parsed.ptr;
  while (remaining != end &&
         std::isspace(static_cast<unsigned char>(*remaining))) {
    ++remaining;
  }

  return remaining == end;
}

}  // namespace

bool is_valid_guess(int guess) {
  return guess >= minimum_guess && guess <= maximum_guess;
}

guess_result compare_guess(int guess, int target) {
  if (guess < target) {
    return guess_result::too_low;
  }
  if (guess > target) {
    return guess_result::too_high;
  }
  return guess_result::correct;
}

game_status run_game(std::istream& input, std::ostream& output, int target) {
  output << "Guess the number between " << minimum_guess << " and "
         << maximum_guess << ".\n";

  std::string line;
  while (true) {
    output << "Enter your guess: ";
    if (!std::getline(input, line)) {
      output << "\nInput ended. Goodbye.\n";
      return game_status::input_ended;
    }

    int guess = 0;
    if (!parse_whole_number(line, guess) || !is_valid_guess(guess)) {
      output << "Invalid guess. Enter a whole number from " << minimum_guess
             << " to " << maximum_guess << ".\n";
      continue;
    }

    switch (compare_guess(guess, target)) {
      case guess_result::too_low:
        output << "Too low. Try again.\n";
        break;
      case guess_result::too_high:
        output << "Too high. Try again.\n";
        break;
      case guess_result::correct:
        output << "Correct! You guessed the number.\n";
        return game_status::guessed_correctly;
    }
  }
}
