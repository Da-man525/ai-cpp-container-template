#include "number_guessing_game.h"

#include <iostream>
#include <random>

int main() {
  std::mt19937 engine(std::random_device{}());
  std::uniform_int_distribution<int> distribution(minimum_guess, maximum_guess);

  const game_status status = run_game(std::cin, std::cout, distribution(engine));
  return status == game_status::guessed_correctly ? 0 : 1;
}
