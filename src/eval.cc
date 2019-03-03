#include "board.h"

#include <cstdlib>
#include <iostream>

#include "absl/time/clock.h"

int main(int argc, char** argv) {
  Board board(argv[1]);
  int depth = std::atoi(argv[2]);
  std::cout << "Depth: " << depth << '\n';
  std::vector<Move> pv;
  absl::Time start = absl::Now();
  std::cout << "Starting evaluation at time: " << start << '\n';
  int eval = evaluation(board, depth, &pv);
  absl::Time end = absl::Now();
  std::cout << "Evaluation: " << eval << '\n';
  std::cout << "Principal variation: " << '\n';
  for (Move move : pv) {
    std::cout << '\t' << move.to_pretty_str() << '\n';
  }
  std::cout << "Time taken: " << end - start << '\n';
}