#include <array>
#include <iostream>

#include "absl/time/clock.h"
#include "board.h"

int main() {
  constexpr int num_boards = 6;
  const std::array<Board, num_boards> boards{
      Board(),
      Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - "
            "0 1"),
      Board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"),
      Board("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"),
      Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"),
      Board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - "
            "- 0 "
            "10")};

  constexpr int max_depth = 6;

  const std::array<std::array<int, max_depth>, num_boards> num_nodes{
      {{1, 20, 400, 8902, 197281, 4865609},
       {1, 48, 2039, 97862, 4085603, 193690690},
       {1, 14, 191, 2812, 43238, 674624},
       {1, 6, 264, 9467, 422333, 15833292},
       {1, 44, 1486, 62379, 2103487, 89941194},
       {1, 46, 2079, 89890, 3894594, 164075551}}};

  for (int depth = 0; depth < max_depth; ++depth) {
    std::cout << "Depth " << depth << ".\n";
    for (int board_idx = 0; board_idx < boards.size(); ++board_idx) {
      absl::Time start = absl::Now();
      const int computed_res = number_of_moves(boards[board_idx], depth);
      absl::Time end = absl::Now();
      const int correct_res = num_nodes[board_idx][depth];

      std::cout << "\tPos " << board_idx << ":\n";
      std::cout << "\t\tComputed result: " << computed_res << ".\n";
      std::cout << "\t\tCorrect result:  " << correct_res << ".\n";
      std::cout << "\t\tTime taken: " << end - start << ".\n";

      if (computed_res != correct_res) {
        std::cout << "\033[0;31mIncorrect results!\n";
        return 0;
      }
    }
  }
}
