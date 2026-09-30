#include "number_guessing_game.h"

#include <cassert>
#include <sstream>
#include <string>

namespace {

void expect_in_order(const std::string& text, const std::string& first,
                     const std::string& second, const std::string& third,
                     const std::string& fourth) {
  const std::size_t first_position = text.find(first);
  const std::size_t second_position = text.find(second);
  const std::size_t third_position = text.find(third);
  const std::size_t fourth_position = text.find(fourth);

  assert(first_position != std::string::npos);
  assert(second_position != std::string::npos);
  assert(third_position != std::string::npos);
  assert(fourth_position != std::string::npos);
  assert(first_position < second_position);
  assert(second_position < third_position);
  assert(third_position < fourth_position);
}

std::size_t count_occurrences(const std::string& text,
                              const std::string& value) {
  std::size_t count = 0;
  std::size_t position = 0;

  while ((position = text.find(value, position)) != std::string::npos) {
    ++count;
    position += value.size();
  }

  return count;
}

void test_guess_range() {
  assert(is_valid_guess(minimum_guess));
  assert(is_valid_guess(maximum_guess));
  assert(!is_valid_guess(minimum_guess - 1));
  assert(!is_valid_guess(maximum_guess + 1));
}

void test_guess_comparison() {
  assert(compare_guess(4, 5) == guess_result::too_low);
  assert(compare_guess(6, 5) == guess_result::too_high);
  assert(compare_guess(5, 5) == guess_result::correct);
}

void test_scripted_game_session() {
  std::istringstream input("\nabc\n4.5\n0\n4\n6\n5\n");
  std::ostringstream output;

  const game_status status = run_game(input, output, 5);

  assert(status == game_status::guessed_correctly);
  assert(count_occurrences(output.str(), "Invalid guess.") == 4);
  expect_in_order(output.str(), "Invalid guess.", "Too low. Try again.",
                  "Too high. Try again.", "Correct! You guessed the number.");
}

void test_input_end() {
  std::istringstream input;
  std::ostringstream output;

  const game_status status = run_game(input, output, 5);

  assert(status == game_status::input_ended);
  assert(output.str().find("Input ended. Goodbye.") != std::string::npos);
}

}  // namespace

int main() {
  test_guess_range();
  test_guess_comparison();
  test_scripted_game_session();
  test_input_end();
}
