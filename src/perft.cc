#include <iostream>

#include "absl/time/clock.h"
#include "board.h"

int main() {
  Board board = Board();
  std::cout << "┌─────────────────────────────────┐\n";
  std::cout << "│         Start position.         │\n";
  std::cout << "└─────────────────────────────────┘\n";
  std::cout << board.to_pretty_str();

  absl::Time start = absl::Now();
  std::cout << "Depth: 0.\n\tComputed result: " << number_of_moves(board, 0)
            << ".\n\tCorrect result:  " << 1 << ".\n";
  absl::Time end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 1.\n\tComputed result: " << number_of_moves(board, 1)
            << ".\n\tCorrect result:  " << 20 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 2.\n\tComputed result: " << number_of_moves(board, 2)
            << ".\n\tCorrect result:  " << 400 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 3.\n\tComputed result: " << number_of_moves(board, 3)
            << ".\n\tCorrect result:  " << 8902 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 4.\n\tComputed result: " << number_of_moves(board, 4)
            << ".\n\tCorrect result:  " << 197281 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  board = Board(
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
  std::cout << "┌─────────────────────────────────┐\n";
  std::cout << "│             Kiwipete.           │\n";
  std::cout << "└─────────────────────────────────┘\n";
  std::cout << board.to_pretty_str();

  start = absl::Now();
  std::cout << "Depth: 0.\n\tComputed result: " << number_of_moves(board, 0)
            << ".\n\tCorrect result:  " << 1 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 1.\n\tComputed result: " << number_of_moves(board, 1)
            << ".\n\tCorrect result:  " << 48 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 2.\n\tComputed result: " << number_of_moves(board, 2)
            << ".\n\tCorrect result:  " << 2039 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 3.\n\tComputed result: " << number_of_moves(board, 3)
            << ".\n\tCorrect result:  " << 97862 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 4.\n\tComputed result: " << number_of_moves(board, 4)
            << ".\n\tCorrect result:  " << 4085603 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  board = Board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
  std::cout << "┌─────────────────────────────────┐\n";
  std::cout << "│           Position 3.           │\n";
  std::cout << "└─────────────────────────────────┘\n";
  std::cout << board.to_pretty_str();

  start = absl::Now();
  std::cout << "Depth: 0.\n\tComputed result: " << number_of_moves(board, 0)
            << ".\n\tCorrect result:  " << 1 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 1.\n\tComputed result: " << number_of_moves(board, 1)
            << ".\n\tCorrect result:  " << 14 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 2.\n\tComputed result: " << number_of_moves(board, 2)
            << ".\n\tCorrect result:  " << 191 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 3.\n\tComputed result: " << number_of_moves(board, 3)
            << ".\n\tCorrect result:  " << 2812 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 4.\n\tComputed result: " << number_of_moves(board, 4)
            << ".\n\tCorrect result:  " << 43238 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  board =
      Board("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
  std::cout << "┌─────────────────────────────────┐\n";
  std::cout << "│           Position 4.           │\n";
  std::cout << "└─────────────────────────────────┘\n";
  std::cout << board.to_pretty_str();

  start = absl::Now();
  std::cout << "Depth: 0.\n\tComputed result: " << number_of_moves(board, 0)
            << ".\n\tCorrect result:  " << 1 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 1.\n\tComputed result: " << number_of_moves(board, 1)
            << ".\n\tCorrect result:  " << 6 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 2.\n\tComputed result: " << number_of_moves(board, 2)
            << ".\n\tCorrect result:  " << 264 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 3.\n\tComputed result: " << number_of_moves(board, 3)
            << ".\n\tCorrect result:  " << 9467 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 4.\n\tComputed result: " << number_of_moves(board, 4)
            << ".\n\tCorrect result:  " << 422333 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  board = Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  std::cout << "┌─────────────────────────────────┐\n";
  std::cout << "│           Position 5.           │\n";
  std::cout << "└─────────────────────────────────┘\n";
  std::cout << board.to_pretty_str();

  start = absl::Now();
  std::cout << "Depth: 0.\n\tComputed result: " << number_of_moves(board, 0)
            << ".\n\tCorrect result:  " << 1 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 1.\n\tComputed result: " << number_of_moves(board, 1)
            << ".\n\tCorrect result:  " << 44 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 2.\n\tComputed result: " << number_of_moves(board, 2)
            << ".\n\tCorrect result:  " << 1486 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 3.\n\tComputed result: " << number_of_moves(board, 3)
            << ".\n\tCorrect result:  " << 62379 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 4.\n\tComputed result: " << number_of_moves(board, 4)
            << ".\n\tCorrect result:  " << 2103487 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  board = Board(
      "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 "
      "10");
  std::cout << "┌─────────────────────────────────┐\n";
  std::cout << "│           Position 6.           │\n";
  std::cout << "└─────────────────────────────────┘\n";
  std::cout << board.to_pretty_str();

  start = absl::Now();
  std::cout << "Depth: 0.\n\tComputed result: " << number_of_moves(board, 0)
            << ".\n\tCorrect result:  " << 1 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 1.\n\tComputed result: " << number_of_moves(board, 1)
            << ".\n\tCorrect result:  " << 46 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 2.\n\tComputed result: " << number_of_moves(board, 2)
            << ".\n\tCorrect result:  " << 2079 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 3.\n\tComputed result: " << number_of_moves(board, 3)
            << ".\n\tCorrect result:  " << 89890 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";

  start = absl::Now();
  std::cout << "Depth: 4.\n\tComputed result: " << number_of_moves(board, 4)
            << ".\n\tCorrect result:  " << 3894594 << ".\n";
  end = absl::Now();
  std::cout << "\tTime taken: " << end - start << ".\n";
}
