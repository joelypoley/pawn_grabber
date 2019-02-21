#include <iostream>
#include <string>
#include <unordered_map>
#include "board.h"

int main() {
  // Board board = Board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
  // number_of_moves(board, 2);
  Board board = Board(
      "r3k2r/p1ppPpb1/bn1q1np1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 2");
  // Board board = Board("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1");
  std::unordered_map<std::string, int> um;
  number_of_moves(board, 1, Move(0, 0, Piece::pawn, MoveType::simple, board),
                  true, &um);
  int total = 0;
  for (auto [key, val] : um) {
    std::cout << key << ": " << val << '\n';
    total += val;
  }
  std::cout << '\n';
  std::cout << "Nodes searched: " << total << '\n';
  ;
  //   std::cout << number_of_moves(
  //                    board, 4,
  //                    Move(str_to_square("g2"), str_to_square("h1"),
  //                    Piece::pawn,
  //                         MoveType::promotion_to_queen, board))
  //                    .to_pretty_str()
  //             << std::endl;
}