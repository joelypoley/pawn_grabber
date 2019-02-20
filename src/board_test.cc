#include "src/board.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>

#include "absl/strings/str_cat.h"
#include "gtest/gtest.h"

namespace {
const int board_size = 8;
}  // namespace.

TEST(SquareUtils, IsSquare) {
  EXPECT_FALSE(is_square(0));
  EXPECT_TRUE(is_square(str_to_square("a1")));
  EXPECT_FALSE(is_square(str_to_square("a1") | is_square(str_to_square("a2"))));
}

TEST(SquareUtils, SquareIndex) {
  EXPECT_EQ(square_idx(str_to_square("h1")), 0);
  EXPECT_EQ(square_idx(str_to_square("a8")), 63);
  EXPECT_EQ(square_idx(str_to_square("a1")), 7);
}

TEST(SquareUtils, RankIndex) {
  EXPECT_EQ(rank_idx(str_to_square("h1")), 0);
  EXPECT_EQ(rank_idx(str_to_square("h2")), 1);
  EXPECT_EQ(rank_idx(str_to_square("a8")), 7);
  EXPECT_EQ(rank_idx(str_to_square("e4")), 3);
}

TEST(SquareUtils, FileIndex) {
  EXPECT_EQ(file_idx(str_to_square("h1")), 7);
  EXPECT_EQ(file_idx(str_to_square("a8")), 0);
  EXPECT_EQ(file_idx(str_to_square("e4")), 4);
}

TEST(SquareUtils, OnAFile) {
  EXPECT_TRUE(on_a_file(str_to_square("a1")));
  EXPECT_TRUE(on_a_file(str_to_square("a8")));
  EXPECT_FALSE(on_a_file(str_to_square("b4")));
  EXPECT_FALSE(on_a_file(str_to_square("e4")));
  EXPECT_FALSE(on_a_file(str_to_square("h4")));
}

TEST(SquareUtils, OnHFile) {
  EXPECT_TRUE(on_h_file(str_to_square("h1")));
  EXPECT_TRUE(on_h_file(str_to_square("h8")));
  EXPECT_FALSE(on_h_file(str_to_square("e4")));
  EXPECT_FALSE(on_h_file(str_to_square("g4")));
  EXPECT_FALSE(on_h_file(str_to_square("g8")));
}

TEST(SquareDirections, E4) {
  Bitboard e4 = str_to_square("e4");
  EXPECT_EQ(str_to_square("e5"), north_of(e4));
  EXPECT_EQ(str_to_square("f5"), northeast_of(e4));
  EXPECT_EQ(str_to_square("d5"), northwest_of(e4));
  EXPECT_EQ(str_to_square("d4"), west_of(e4));
  EXPECT_EQ(str_to_square("f4"), east_of(e4));
  EXPECT_EQ(str_to_square("e3"), south_of(e4));
  EXPECT_EQ(str_to_square("d3"), southwest_of(e4));
  EXPECT_EQ(str_to_square("f3"), southeast_of(e4));
}

TEST(SquareDirections, A8) {
  Bitboard a8 = str_to_square("a8");
  EXPECT_EQ(0, north_of(a8));
  EXPECT_EQ(0, northeast_of(a8));
  EXPECT_EQ(0, northwest_of(a8));
  EXPECT_EQ(0, west_of(a8));
  EXPECT_EQ(str_to_square("b8"), east_of(a8));
  EXPECT_EQ(str_to_square("a7"), south_of(a8));
  EXPECT_EQ(0, southwest_of(a8));
  EXPECT_EQ(str_to_square("b7"), southeast_of(a8));
}

TEST(SquareDirections, A1) {
  Bitboard a1 = str_to_square("a1");
  EXPECT_EQ(str_to_square("a2"), north_of(a1));
  EXPECT_EQ(str_to_square("b2"), northeast_of(a1));
  EXPECT_EQ(0, northwest_of(a1));
  EXPECT_EQ(0, west_of(a1));
  EXPECT_EQ(str_to_square("b1"), east_of(a1));
  EXPECT_EQ(0, south_of(a1));
  EXPECT_EQ(0, southwest_of(a1));
  EXPECT_EQ(0, southeast_of(a1));
}

TEST(SquareDirections, H1) {
  Bitboard h1 = str_to_square("h1");
  EXPECT_EQ(str_to_square("h2"), north_of(h1));
  EXPECT_EQ(0, northeast_of(h1));
  EXPECT_EQ(str_to_square("g2"), northwest_of(h1));
  EXPECT_EQ(str_to_square("g1"), west_of(h1));
  EXPECT_EQ(0, east_of(h1));
  EXPECT_EQ(0, south_of(h1));
  EXPECT_EQ(0, southwest_of(h1));
  EXPECT_EQ(0, southeast_of(h1));
}

TEST(SquareDirections, H8) {
  Bitboard h8 = str_to_square("h8");
  EXPECT_EQ(0, north_of(h8));
  EXPECT_EQ(0, northeast_of(h8));
  EXPECT_EQ(0, northwest_of(h8));
  EXPECT_EQ(str_to_square("g8"), west_of(h8));
  EXPECT_EQ(0, east_of(h8));
  EXPECT_EQ(str_to_square("h7"), south_of(h8));
  EXPECT_EQ(str_to_square("g7"), southwest_of(h8));
  EXPECT_EQ(0, southeast_of(h8));
}

TEST(BoardConstructor, StartPosition) {
  Board start_board;
  EXPECT_EQ(start_board.white_pawns_, 0xFF00);
  EXPECT_EQ(start_board.white_rooks_, 0x81);
  EXPECT_EQ(start_board.white_knights_, 0x42);
  EXPECT_EQ(start_board.white_bishops_, 0x24);
  EXPECT_EQ(start_board.white_queens_, 0x10);
  EXPECT_EQ(start_board.white_king_, 0x8);
  EXPECT_EQ(start_board.black_pawns_, 0xFF000000000000);
  EXPECT_EQ(start_board.black_rooks_, 0x8100000000000000);
  EXPECT_EQ(start_board.black_knights_, 0x4200000000000000);
  EXPECT_EQ(start_board.black_bishops_, 0x2400000000000000);
  EXPECT_EQ(start_board.black_queens_, 0x1000000000000000);
  EXPECT_EQ(start_board.black_king_, 0x800000000000000);
  EXPECT_TRUE(start_board.is_whites_move_);
  EXPECT_EQ(start_board.en_passant_square_, std::nullopt);
  EXPECT_TRUE(start_board.white_has_right_to_castle_kingside_);
  EXPECT_TRUE(start_board.white_has_right_to_castle_queenside_);
  EXPECT_TRUE(start_board.black_has_right_to_castle_kingside_);
  EXPECT_TRUE(start_board.black_has_right_to_castle_queenside_);
  EXPECT_EQ(start_board.fifty_move_clock_, 0);
  EXPECT_EQ(start_board.num_moves_, 1);
}

TEST(PPrint, StartPosition) {
  Board start_board;
  EXPECT_EQ(start_board.to_pretty_str(),
            absl::StrCat("  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n",
                         "8 │ ♜ │ ♞ │ ♝ │ ♛ │ ♚ │ ♝ │ ♞ │ ♜ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "7 │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "6 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "5 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "4 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "3 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "2 │ ♙ │ ♙ │ ♙ │ ♙ │ ♙ │ ♙ │ ♙ │ ♙ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "1 │ ♖ │ ♘ │ ♗ │ ♕ │ ♔ │ ♗ │ ♘ │ ♖ │\n",
                         "  └───┴───┴───┴───┴───┴───┴───┴───┘\n",
                         "    a   b   c   d   e   f   g   h\n"));
}

TEST(BoardConstructor, KingsPawn) {
  Board board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
  EXPECT_EQ(board.white_pawns_, 0x800F700);
  EXPECT_EQ(board.white_rooks_, 0x81);
  EXPECT_EQ(board.white_knights_, 0x42);
  EXPECT_EQ(board.white_bishops_, 0x24);
  EXPECT_EQ(board.white_queens_, 0x10);
  EXPECT_EQ(board.white_king_, 0x8);
  EXPECT_EQ(board.black_pawns_, 0xFF000000000000);
  EXPECT_EQ(board.black_rooks_, 0x8100000000000000);
  EXPECT_EQ(board.black_knights_, 0x4200000000000000);
  EXPECT_EQ(board.black_bishops_, 0x2400000000000000);
  EXPECT_EQ(board.black_queens_, 0x1000000000000000);
  EXPECT_EQ(board.black_king_, 0x800000000000000);
  EXPECT_FALSE(board.is_whites_move_);
  const Bitboard e3 = (1ULL << (2 * board_size + 3));
  EXPECT_EQ(board.en_passant_square_, e3);
  EXPECT_TRUE(board.white_has_right_to_castle_kingside_);
  EXPECT_TRUE(board.white_has_right_to_castle_queenside_);
  EXPECT_TRUE(board.black_has_right_to_castle_kingside_);
  EXPECT_TRUE(board.black_has_right_to_castle_queenside_);
  EXPECT_EQ(board.fifty_move_clock_, 0);
  EXPECT_EQ(board.num_moves_, 1);
}

TEST(PPrint, KingsPawn) {
  Board board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
  EXPECT_EQ(board.to_pretty_str(),
            absl::StrCat("  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n",
                         "8 │ ♜ │ ♞ │ ♝ │ ♛ │ ♚ │ ♝ │ ♞ │ ♜ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "7 │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "6 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "5 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "4 │   │   │   │   │ ♙ │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "3 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "2 │ ♙ │ ♙ │ ♙ │ ♙ │   │ ♙ │ ♙ │ ♙ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "1 │ ♖ │ ♘ │ ♗ │ ♕ │ ♔ │ ♗ │ ♘ │ ♖ │\n",
                         "  └───┴───┴───┴───┴───┴───┴───┴───┘\n",
                         "    a   b   c   d   e   f   g   h\n"));
}

TEST(BoardConstructor, Sicilian) {
  Board board("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");
  EXPECT_EQ(board.white_pawns_, 0x800F700);
  EXPECT_EQ(board.white_rooks_, 0x81);
  EXPECT_EQ(board.white_knights_, 0x42);
  EXPECT_EQ(board.white_bishops_, 0x24);
  EXPECT_EQ(board.white_queens_, 0x10);
  EXPECT_EQ(board.white_king_, 0x8);
  EXPECT_EQ(board.black_pawns_, 0xDF002000000000);
  EXPECT_EQ(board.black_rooks_, 0x8100000000000000);
  EXPECT_EQ(board.black_knights_, 0x4200000000000000);
  EXPECT_EQ(board.black_bishops_, 0x2400000000000000);
  EXPECT_EQ(board.black_queens_, 0x1000000000000000);
  EXPECT_EQ(board.black_king_, 0x800000000000000);
  EXPECT_TRUE(board.is_whites_move_);
  const Bitboard c6 = (1ULL << (5 * board_size + 5));
  EXPECT_EQ(board.en_passant_square_, c6);
  EXPECT_TRUE(board.white_has_right_to_castle_kingside_);
  EXPECT_TRUE(board.white_has_right_to_castle_queenside_);
  EXPECT_TRUE(board.black_has_right_to_castle_kingside_);
  EXPECT_TRUE(board.black_has_right_to_castle_queenside_);
  EXPECT_EQ(board.fifty_move_clock_, 0);
  EXPECT_EQ(board.num_moves_, 2);
}

TEST(PPrint, Sicilian) {
  Board board("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");
  EXPECT_EQ(board.to_pretty_str(),
            absl::StrCat("  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n",
                         "8 │ ♜ │ ♞ │ ♝ │ ♛ │ ♚ │ ♝ │ ♞ │ ♜ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "7 │ ♟ │ ♟ │   │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "6 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "5 │   │   │ ♟ │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "4 │   │   │   │   │ ♙ │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "3 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "2 │ ♙ │ ♙ │ ♙ │ ♙ │   │ ♙ │ ♙ │ ♙ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "1 │ ♖ │ ♘ │ ♗ │ ♕ │ ♔ │ ♗ │ ♘ │ ♖ │\n",
                         "  └───┴───┴───┴───┴───┴───┴───┴───┘\n",
                         "    a   b   c   d   e   f   g   h\n"));
}

TEST(BoardConstructor, Nf3Sicilian) {
  Board board("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
  EXPECT_EQ(board.white_pawns_, 0x800F700);
  EXPECT_EQ(board.white_rooks_, 0x81);
  EXPECT_EQ(board.white_knights_, 0x40040);
  EXPECT_EQ(board.white_bishops_, 0x24);
  EXPECT_EQ(board.white_queens_, 0x10);
  EXPECT_EQ(board.white_king_, 0x8);
  EXPECT_EQ(board.black_pawns_, 0xDF002000000000);
  EXPECT_EQ(board.black_rooks_, 0x8100000000000000);
  EXPECT_EQ(board.black_knights_, 0x4200000000000000);
  EXPECT_EQ(board.black_bishops_, 0x2400000000000000);
  EXPECT_EQ(board.black_queens_, 0x1000000000000000);
  EXPECT_EQ(board.black_king_, 0x800000000000000);
  EXPECT_FALSE(board.is_whites_move_);
  EXPECT_EQ(board.en_passant_square_, std::nullopt);
  EXPECT_TRUE(board.white_has_right_to_castle_kingside_);
  EXPECT_TRUE(board.white_has_right_to_castle_queenside_);
  EXPECT_TRUE(board.black_has_right_to_castle_kingside_);
  EXPECT_TRUE(board.black_has_right_to_castle_queenside_);
  EXPECT_EQ(board.fifty_move_clock_, 1);
  EXPECT_EQ(board.num_moves_, 2);
}

TEST(PPrint, Nf3Sicilian) {
  Board board("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
  EXPECT_EQ(board.to_pretty_str(),
            absl::StrCat("  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n",
                         "8 │ ♜ │ ♞ │ ♝ │ ♛ │ ♚ │ ♝ │ ♞ │ ♜ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "7 │ ♟ │ ♟ │   │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "6 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "5 │   │   │ ♟ │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "4 │   │   │   │   │ ♙ │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "3 │   │   │   │   │   │ ♘ │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "2 │ ♙ │ ♙ │ ♙ │ ♙ │   │ ♙ │ ♙ │ ♙ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "1 │ ♖ │ ♘ │ ♗ │ ♕ │ ♔ │ ♗ │   │ ♖ │\n",
                         "  └───┴───┴───┴───┴───┴───┴───┴───┘\n",
                         "    a   b   c   d   e   f   g   h\n"));
}

TEST(BoardConstructor, SicilianWith2Ke2) {
  Board board("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPPKPPP/RNBQ1BNR b kq - 1 2");
  EXPECT_EQ(board.white_pawns_, 0x800F700);
  EXPECT_EQ(board.white_rooks_, 0x81);
  EXPECT_EQ(board.white_knights_, 0x42);
  EXPECT_EQ(board.white_bishops_, 0x24);
  EXPECT_EQ(board.white_queens_, 0x10);
  EXPECT_EQ(board.white_king_, 0x800);
  EXPECT_EQ(board.black_pawns_, 0xDF002000000000);
  EXPECT_EQ(board.black_rooks_, 0x8100000000000000);
  EXPECT_EQ(board.black_knights_, 0x4200000000000000);
  EXPECT_EQ(board.black_bishops_, 0x2400000000000000);
  EXPECT_EQ(board.black_queens_, 0x1000000000000000);
  EXPECT_EQ(board.black_king_, 0x800000000000000);
  EXPECT_FALSE(board.is_whites_move_);
  EXPECT_EQ(board.en_passant_square_, std::nullopt);
  EXPECT_FALSE(board.white_has_right_to_castle_kingside_);
  EXPECT_FALSE(board.white_has_right_to_castle_queenside_);
  EXPECT_TRUE(board.black_has_right_to_castle_kingside_);
  EXPECT_TRUE(board.black_has_right_to_castle_queenside_);
  EXPECT_EQ(board.fifty_move_clock_, 1);
  EXPECT_EQ(board.num_moves_, 2);
}

TEST(PPrint, SicilianWith2Ke2) {
  Board board("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPPKPPP/RNBQ1BNR b kq - 1 2");
  EXPECT_EQ(board.to_pretty_str(),
            absl::StrCat("  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n",
                         "8 │ ♜ │ ♞ │ ♝ │ ♛ │ ♚ │ ♝ │ ♞ │ ♜ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "7 │ ♟ │ ♟ │   │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "6 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "5 │   │   │ ♟ │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "4 │   │   │   │   │ ♙ │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "3 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "2 │ ♙ │ ♙ │ ♙ │ ♙ │ ♔ │ ♙ │ ♙ │ ♙ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "1 │ ♖ │ ♘ │ ♗ │ ♕ │   │ ♗ │ ♘ │ ♖ │\n",
                         "  └───┴───┴───┴───┴───┴───┴───┴───┘\n",
                         "    a   b   c   d   e   f   g   h\n"));
}

TEST(BoardConstructor, KingVsKing) {
  Board board("4k3/8/8/8/8/8/8/4K3 w - - 0 55");
  EXPECT_EQ(board.white_pawns_, 0x0);
  EXPECT_EQ(board.white_rooks_, 0x0);
  EXPECT_EQ(board.white_knights_, 0x0);
  EXPECT_EQ(board.white_bishops_, 0x0);
  EXPECT_EQ(board.white_queens_, 0x0);
  EXPECT_EQ(board.white_king_, 0x8);
  EXPECT_EQ(board.black_pawns_, 0x0);
  EXPECT_EQ(board.black_rooks_, 0x0);
  EXPECT_EQ(board.black_knights_, 0x0);
  EXPECT_EQ(board.black_bishops_, 0x0);
  EXPECT_EQ(board.black_queens_, 0x0);
  EXPECT_EQ(board.black_king_, 0x800000000000000);
  EXPECT_TRUE(board.is_whites_move_);
  EXPECT_EQ(board.en_passant_square_, std::nullopt);
  EXPECT_FALSE(board.white_has_right_to_castle_kingside_);
  EXPECT_FALSE(board.white_has_right_to_castle_queenside_);
  EXPECT_FALSE(board.black_has_right_to_castle_kingside_);
  EXPECT_FALSE(board.black_has_right_to_castle_queenside_);
  EXPECT_EQ(board.fifty_move_clock_, 0);
  EXPECT_EQ(board.num_moves_, 55);
}

TEST(PPrint, KingVsKing) {
  Board board("4k3/8/8/8/8/8/8/4K3 w - - 0 55");
  EXPECT_EQ(board.to_pretty_str(),
            absl::StrCat("  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n",
                         "8 │   │   │   │   │ ♚ │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "7 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "6 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "5 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "4 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "3 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "2 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "1 │   │   │   │   │ ♔ │   │   │   │\n",
                         "  └───┴───┴───┴───┴───┴───┴───┴───┘\n",
                         "    a   b   c   d   e   f   g   h\n"));
}

TEST(BoardConstructor, LucenaPosition) {
  Board board("1K1k4/1P6/8/8/8/8/r7/2R5 w - - 0 60");
  EXPECT_EQ(board.white_pawns_, 0x40000000000000);
  EXPECT_EQ(board.white_rooks_, 0x20);
  EXPECT_EQ(board.white_knights_, 0x0);
  EXPECT_EQ(board.white_bishops_, 0x0);
  EXPECT_EQ(board.white_queens_, 0x0);
  EXPECT_EQ(board.white_king_, 0x4000000000000000);
  EXPECT_EQ(board.black_pawns_, 0x0);
  EXPECT_EQ(board.black_rooks_, 0x8000);
  EXPECT_EQ(board.black_knights_, 0x0);
  EXPECT_EQ(board.black_bishops_, 0x0);
  EXPECT_EQ(board.black_queens_, 0x0);
  EXPECT_EQ(board.black_king_, 0x1000000000000000);
  EXPECT_TRUE(board.is_whites_move_);
  EXPECT_EQ(board.en_passant_square_, std::nullopt);
  EXPECT_FALSE(board.white_has_right_to_castle_kingside_);
  EXPECT_FALSE(board.white_has_right_to_castle_queenside_);
  EXPECT_FALSE(board.black_has_right_to_castle_kingside_);
  EXPECT_FALSE(board.black_has_right_to_castle_queenside_);
  EXPECT_EQ(board.fifty_move_clock_, 0);
  EXPECT_EQ(board.num_moves_, 60);
}

TEST(PPrint, LucenaPosition) {
  Board board("1K1k4/1P6/8/8/8/8/r7/2R5 w - - 0 60");
  EXPECT_EQ(board.to_pretty_str(),
            absl::StrCat("  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n",
                         "8 │   │ ♔ │   │ ♚ │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "7 │   │ ♙ │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "6 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "5 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "4 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "3 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "2 │ ♜ │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "1 │   │   │ ♖ │   │   │   │   │   │\n",
                         "  └───┴───┴───┴───┴───┴───┴───┴───┘\n",
                         "    a   b   c   d   e   f   g   h\n"));
}

TEST(BoardConstructor, MiddleGame) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");
  EXPECT_EQ(board.white_pawns_, str_to_square("g2") | str_to_square("f2") |
                                    str_to_square("b2") | str_to_square("a2") |
                                    str_to_square("h3") | str_to_square("e3") |
                                    str_to_square("d4"));
  EXPECT_EQ(board.white_rooks_, str_to_square("a1") | str_to_square("f1"));
  EXPECT_EQ(board.white_knights_, str_to_square("c3") | str_to_square("d2"));
  EXPECT_EQ(board.white_bishops_, 0);
  EXPECT_EQ(board.white_queens_, str_to_square("b3"));
  EXPECT_EQ(board.white_king_, str_to_square("h1"));
  EXPECT_EQ(board.black_pawns_, str_to_square("a7") | str_to_square("b7") |
                                    str_to_square("c6") | str_to_square("e4") |
                                    str_to_square("f7") | str_to_square("g7") |
                                    str_to_square("h6"));
  EXPECT_EQ(board.black_rooks_, str_to_square("a8") | str_to_square("f8"));
  EXPECT_EQ(board.black_knights_, str_to_square("g4"));
  EXPECT_EQ(board.black_bishops_, str_to_square("g6"));
  EXPECT_EQ(board.black_queens_, str_to_square("h4"));
  EXPECT_EQ(board.black_king_, str_to_square("g8"));
  EXPECT_TRUE(board.is_whites_move_);
  EXPECT_EQ(board.en_passant_square_, std::nullopt);
  EXPECT_FALSE(board.white_has_right_to_castle_kingside_);
  EXPECT_FALSE(board.white_has_right_to_castle_queenside_);
  EXPECT_FALSE(board.black_has_right_to_castle_kingside_);
  EXPECT_FALSE(board.black_has_right_to_castle_queenside_);
  EXPECT_EQ(board.fifty_move_clock_, 1);
  EXPECT_EQ(board.num_moves_, 18);
}

TEST(PPrint, MiddleGame) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");
  EXPECT_EQ(board.to_pretty_str(),
            absl::StrCat("  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n",
                         "8 │ ♜ │   │   │   │   │ ♜ │ ♚ │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "7 │ ♟ │ ♟ │   │   │   │ ♟ │ ♟ │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "6 │   │   │ ♟ │   │   │   │ ♝ │ ♟ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "5 │   │   │   │   │   │   │   │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "4 │   │   │   │ ♙ │ ♟ │   │ ♞ │ ♛ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "3 │   │ ♕ │ ♘ │   │ ♙ │   │   │ ♙ │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "2 │ ♙ │ ♙ │   │ ♘ │   │ ♙ │ ♙ │   │\n",
                         "  ├───┼───┼───┼───┼───┼───┼───┼───┤\n",
                         "1 │ ♖ │   │   │   │   │ ♖ │   │ ♔ │\n",
                         "  └───┴───┴───┴───┴───┴───┴───┴───┘\n",
                         "    a   b   c   d   e   f   g   h\n"));
}

TEST(AllPieces, White) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");
  const std::vector<std::string> white_squares = {"a1", "f1", "h1", "a2", "b2",
                                                  "d2", "f2", "g2", "b3", "c3",
                                                  "e3", "h3", "d4"};
  std::vector<Bitboard> white_squares_bb(white_squares.size());
  std::transform(white_squares.begin(), white_squares.end(),
                 white_squares_bb.begin(), str_to_square);
  Bitboard bb =
      std::reduce(white_squares_bb.begin(), white_squares_bb.end(), 0,
                  [](Bitboard bb1, Bitboard bb2) { return bb1 | bb2; });
  EXPECT_EQ(board.white_pieces(), bb);
}

TEST(PseudoLegalMoves, A1Rook) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");
  const Bitboard a1 = str_to_square("a1");

  std::vector<Move> test_north_moves;
  board.pseudolegal_sliding_moves(Direction::north, Color::white, a1,
                                  &test_north_moves);
  EXPECT_TRUE(test_north_moves.empty());

  std::vector<Move> test_west_moves;
  board.pseudolegal_sliding_moves(Direction::west, Color::white, a1,
                                  &test_west_moves);
  EXPECT_TRUE(test_west_moves.empty());

  std::vector<Move> test_south_moves;
  board.pseudolegal_sliding_moves(Direction::south, Color::white, a1,
                                  &test_south_moves);
  EXPECT_TRUE(test_south_moves.empty());

  // TODO: Change to computed_east_moves and east_moves.
  std::vector<Move> east_moves = {
      Move(a1, str_to_square("b1")), Move(a1, str_to_square("c1")),
      Move(a1, str_to_square("d1")), Move(a1, str_to_square("e1"))};
  std::vector<Move> test_east_moves;
  board.pseudolegal_sliding_moves(Direction::east, Color::white, a1,
                                  &test_east_moves);
  EXPECT_EQ(test_east_moves, east_moves);
}

TEST(PseudoLegalMoves, F1Rook) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");
  const Bitboard f1 = str_to_square("f1");

  std::vector<Move> test_north_moves;
  board.pseudolegal_sliding_moves(Direction::north, Color::white, f1,
                                  &test_north_moves);
  EXPECT_TRUE(test_north_moves.empty());

  std::vector<Move> test_south_moves;
  board.pseudolegal_sliding_moves(Direction::south, Color::white, f1,
                                  &test_south_moves);
  EXPECT_TRUE(test_south_moves.empty());

  std::vector<Move> east_moves = {{f1, str_to_square("g1")}};
  std::vector<Move> test_east_moves;
  board.pseudolegal_sliding_moves(Direction::east, Color::white, f1,
                                  &test_east_moves);
  EXPECT_EQ(test_east_moves, east_moves);

  std::vector<Move> west_moves = {
      Move(f1, str_to_square("e1")), Move(f1, str_to_square("d1")),
      Move(f1, str_to_square("c1")), Move(f1, str_to_square("b1"))};
  std::vector<Move> test_west_moves;
  board.pseudolegal_sliding_moves(Direction::west, Color::white, f1,
                                  &test_west_moves);
  EXPECT_EQ(test_west_moves, west_moves);
}

TEST(PseudoLegalMoves, B3Queen) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");
  const Bitboard b3 = str_to_square("b3");

  std::vector<Move> test_east_moves;
  board.pseudolegal_sliding_moves(Direction::east, Color::white, b3,
                                  &test_east_moves);
  EXPECT_TRUE(test_east_moves.empty());

  std::vector<Move> test_southwest_moves;
  board.pseudolegal_sliding_moves(Direction::southwest, Color::white, b3,
                                  &test_southwest_moves);
  EXPECT_TRUE(test_southwest_moves.empty());

  std::vector<Move> test_south_moves;
  board.pseudolegal_sliding_moves(Direction::south, Color::white, b3,
                                  &test_south_moves);
  EXPECT_TRUE(test_south_moves.empty());

  std::vector<Move> north_moves = {
      Move(b3, str_to_square("b4")), Move(b3, str_to_square("b5")),
      Move(b3, str_to_square("b6")), Move(b3, str_to_square("b7"))};
  std::vector<Move> test_north_moves;
  board.pseudolegal_sliding_moves(Direction::north, Color::white, b3,
                                  &test_north_moves);
  EXPECT_EQ(test_north_moves, north_moves);

  std::vector<Move> northwest_moves = {Move(b3, str_to_square("a4"))};
  std::vector<Move> test_northwest_moves;
  board.pseudolegal_sliding_moves(Direction::northwest, Color::white, b3,
                                  &test_northwest_moves);
  EXPECT_EQ(test_northwest_moves, northwest_moves);

  std::vector<Move> northeast_moves = {
      Move(b3, str_to_square("c4")), Move(b3, str_to_square("d5")),
      Move(b3, str_to_square("e6")), Move(b3, str_to_square("f7"))};
  std::vector<Move> test_northeast_moves;
  board.pseudolegal_sliding_moves(Direction::northeast, Color::white, b3,
                                  &test_northeast_moves);
  EXPECT_EQ(test_northeast_moves, northeast_moves);

  std::vector<Move> west_moves = {Move(b3, str_to_square("a3"))};
  std::vector<Move> test_west_moves;
  board.pseudolegal_sliding_moves(Direction::west, Color::white, b3,
                                  &test_west_moves);
  EXPECT_EQ(test_west_moves, west_moves);

  std::vector<Move> southeast_moves = {
      Move(b3, str_to_square("c2")),
      Move(b3, str_to_square("d1")),
  };
  std::vector<Move> test_southeast_moves;
  board.pseudolegal_sliding_moves(Direction::southeast, Color::white, b3,
                                  &test_southeast_moves);
  EXPECT_EQ(test_southeast_moves, southeast_moves);
}

TEST(PseudoLegalMoves, A8Rook) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");
  const Bitboard a8 = str_to_square("a8");

  std::vector<Move> test_north_moves;
  board.pseudolegal_sliding_moves(Direction::north, Color::black, a8,
                                  &test_north_moves);
  EXPECT_TRUE(test_north_moves.empty());

  std::vector<Move> test_west_moves;
  board.pseudolegal_sliding_moves(Direction::west, Color::black, a8,
                                  &test_west_moves);
  EXPECT_TRUE(test_west_moves.empty());

  std::vector<Move> test_south_moves;
  board.pseudolegal_sliding_moves(Direction::south, Color::black, a8,
                                  &test_south_moves);
  EXPECT_TRUE(test_south_moves.empty());

  std::vector<Move> east_moves = {
      Move(a8, str_to_square("b8")), Move(a8, str_to_square("c8")),
      Move(a8, str_to_square("d8")), Move(a8, str_to_square("e8"))};
  std::vector<Move> test_east_moves;
  board.pseudolegal_sliding_moves(Direction::east, Color::black, a8,
                                  &test_east_moves);
  EXPECT_EQ(test_east_moves, east_moves);
}

TEST(PseudoLegalMoves, F8Rook) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");
  const Bitboard f8 = str_to_square("f8");

  std::vector<Move> test_north_moves;
  board.pseudolegal_sliding_moves(Direction::north, Color::black, f8,
                                  &test_north_moves);
  EXPECT_TRUE(test_north_moves.empty());

  std::vector<Move> test_east_moves;
  board.pseudolegal_sliding_moves(Direction::east, Color::black, f8,
                                  &test_east_moves);
  EXPECT_TRUE(test_east_moves.empty());

  std::vector<Move> test_south_moves;
  board.pseudolegal_sliding_moves(Direction::south, Color::black, f8,
                                  &test_south_moves);
  EXPECT_TRUE(test_south_moves.empty());

  std::vector<Move> west_moves = {
      Move(f8, str_to_square("e8")), Move(f8, str_to_square("d8")),
      Move(f8, str_to_square("c8")), Move(f8, str_to_square("b8"))};
  std::vector<Move> test_west_moves;
  board.pseudolegal_sliding_moves(Direction::west, Color::black, f8,
                                  &test_west_moves);
  EXPECT_EQ(test_west_moves, west_moves);
}

TEST(PseudoLegalMoves, G6Bishop) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");
  const Bitboard g6 = str_to_square("g6");

  std::vector<Move> test_northwest_moves;
  board.pseudolegal_sliding_moves(Direction::northwest, Color::black, g6,
                                  &test_northwest_moves);
  EXPECT_TRUE(test_northwest_moves.empty());

  std::vector<Move> northeast_moves = {Move(g6, str_to_square("h7"))};
  std::vector<Move> test_northeast_moves;
  board.pseudolegal_sliding_moves(Direction::northeast, Color::black, g6,
                                  &test_northeast_moves);
  EXPECT_EQ(test_northeast_moves, northeast_moves);

  std::vector<Move> southwest_moves = {Move(g6, str_to_square("f5"))};
  std::vector<Move> test_southwest_moves;
  board.pseudolegal_sliding_moves(Direction::southwest, Color::black, g6,
                                  &test_southwest_moves);
  EXPECT_EQ(test_southwest_moves, southwest_moves);

  std::vector<Move> southeast_moves = {Move(g6, str_to_square("h5"))};
  std::vector<Move> test_southeast_moves;
  board.pseudolegal_sliding_moves(Direction::southeast, Color::black, g6,
                                  &test_southeast_moves);
  EXPECT_EQ(test_southeast_moves, southeast_moves);
}

TEST(PseudoLegalMoves, H4Queen) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");
  const Bitboard h4 = str_to_square("h4");

  std::vector<Move> north_moves = {Move(h4, str_to_square("h5"))};
  std::vector<Move> test_north_moves;
  board.pseudolegal_sliding_moves(Direction::north, Color::black, h4,
                                  &test_north_moves);
  EXPECT_EQ(test_north_moves, north_moves);

  std::vector<Move> south_moves = {Move(h4, str_to_square("h3"))};
  std::vector<Move> test_south_moves;
  board.pseudolegal_sliding_moves(Direction::south, Color::black, h4,
                                  &test_south_moves);
  EXPECT_EQ(test_south_moves, south_moves);

  std::vector<Move> test_west_moves;
  board.pseudolegal_sliding_moves(Direction::west, Color::black, h4,
                                  &test_west_moves);
  EXPECT_TRUE(test_west_moves.empty());

  std::vector<Move> test_east_moves;
  board.pseudolegal_sliding_moves(Direction::east, Color::black, h4,
                                  &test_east_moves);
  EXPECT_TRUE(test_east_moves.empty());

  std::vector<Move> test_northeast_moves;
  board.pseudolegal_sliding_moves(Direction::northeast, Color::black, h4,
                                  &test_northeast_moves);
  EXPECT_TRUE(test_northeast_moves.empty());

  std::vector<Move> test_southeast_moves;
  board.pseudolegal_sliding_moves(Direction::southeast, Color::black, h4,
                                  &test_southeast_moves);
  EXPECT_TRUE(test_southeast_moves.empty());

  std::vector<Move> northwest_moves = {
      Move(h4, str_to_square("g5")),
      Move(h4, str_to_square("f6")),
      Move(h4, str_to_square("e7")),
      Move(h4, str_to_square("d8")),
  };
  std::vector<Move> test_northwest_moves;
  board.pseudolegal_sliding_moves(Direction::northwest, Color::black, h4,
                                  &test_northwest_moves);
  EXPECT_EQ(test_northwest_moves, northwest_moves);

  std::vector<Move> southwest_moves = {
      Move(h4, str_to_square("g3")),
      Move(h4, str_to_square("f2")),
  };
  std::vector<Move> test_southwest_moves;
  board.pseudolegal_sliding_moves(Direction::southwest, Color::black, h4,
                                  &test_southwest_moves);
  EXPECT_EQ(test_southwest_moves, southwest_moves);
}

TEST(PseudoLegalMoves, BishopMovesWhite) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 w - - 4 36");

  std::vector<Move> moves = {Move(str_to_square("d2"), str_to_square("c1")),
                             Move(str_to_square("d2"), str_to_square("e3")),
                             Move(str_to_square("d2"), str_to_square("f4")),
                             Move(str_to_square("d2"), str_to_square("g5")),
                             Move(str_to_square("d2"), str_to_square("e1")),
                             Move(str_to_square("d2"), str_to_square("h6"))

  };
  std::vector<Move> test_moves;
  board.pseudolegal_bishop_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, BishopMovesBlack) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 b - - 4 36");

  std::vector<Move> moves = {Move(str_to_square("f8"), str_to_square("e7")),
                             Move(str_to_square("f8"), str_to_square("d6")),
                             Move(str_to_square("f8"), str_to_square("c5")),
                             Move(str_to_square("f8"), str_to_square("b4")),
                             Move(str_to_square("f8"), str_to_square("a3"))};
  std::vector<Move> test_moves;
  board.pseudolegal_bishop_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, RookMovesWhite) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 w - - 4 36");

  std::vector<Move> moves = {Move(str_to_square("a1"), str_to_square("a2")),
                             Move(str_to_square("a1"), str_to_square("a3")),
                             Move(str_to_square("a1"), str_to_square("b1")),
                             Move(str_to_square("a1"), str_to_square("c1")),
                             Move(str_to_square("a1"), str_to_square("a4")),
                             Move(str_to_square("d1"), str_to_square("c1")),
                             Move(str_to_square("d1"), str_to_square("b1")),
                             Move(str_to_square("d1"), str_to_square("e1")),
                             Move(str_to_square("d1"), str_to_square("f1")),
                             Move(str_to_square("d1"), str_to_square("g1")),
                             Move(str_to_square("d1"), str_to_square("h1"))

  };
  std::vector<Move> test_moves;
  board.pseudolegal_rook_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, RookMovesBlack) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 b - - 4 36");

  std::vector<Move> moves = {
      Move(str_to_square("a8"), str_to_square("a7")),
      Move(str_to_square("a8"), str_to_square("a6")),
      Move(str_to_square("a8"), str_to_square("a5")),
      Move(str_to_square("a8"), str_to_square("b8")),
      Move(str_to_square("a8"), str_to_square("c8")),
      Move(str_to_square("d8"), str_to_square("c8")),
      Move(str_to_square("d8"), str_to_square("b8")),
      Move(str_to_square("d8"), str_to_square("e8")),
      Move(str_to_square("d8"), str_to_square("d7")),
      Move(str_to_square("d8"), str_to_square("d6")),
      Move(str_to_square("d8"), str_to_square("d5")),
  };
  std::vector<Move> test_moves;
  board.pseudolegal_rook_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, QueenMovesWhite) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 w - - 4 36");

  std::vector<Move> moves = {Move(str_to_square("h4"), str_to_square("g3")),
                             Move(str_to_square("h4"), str_to_square("f2")),
                             Move(str_to_square("h4"), str_to_square("e1")),
                             Move(str_to_square("h4"), str_to_square("g4")),
                             Move(str_to_square("h4"), str_to_square("f4")),
                             Move(str_to_square("h4"), str_to_square("e4")),
                             Move(str_to_square("h4"), str_to_square("d4")),
                             Move(str_to_square("h4"), str_to_square("c4")),
                             Move(str_to_square("h4"), str_to_square("b4")),
                             Move(str_to_square("h4"), str_to_square("a4")),
                             Move(str_to_square("h4"), str_to_square("g5")),
                             Move(str_to_square("h4"), str_to_square("f6")),
                             Move(str_to_square("h4"), str_to_square("e7")),
                             Move(str_to_square("h4"), str_to_square("d8")),
                             Move(str_to_square("h4"), str_to_square("h5")),
                             Move(str_to_square("h4"), str_to_square("h6"))

  };
  std::vector<Move> test_moves;
  board.pseudolegal_queen_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, QueenMovesBlack) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 b - - 4 36");

  std::vector<Move> moves = {
      Move(str_to_square("b3"), str_to_square("a2")),
      Move(str_to_square("b3"), str_to_square("a3")),
      Move(str_to_square("b3"), str_to_square("b2")),
      Move(str_to_square("b3"), str_to_square("c2")),
      Move(str_to_square("b3"), str_to_square("d1")),
      Move(str_to_square("b3"), str_to_square("c3")),
      Move(str_to_square("b3"), str_to_square("b4")),
      Move(str_to_square("b3"), str_to_square("b5")),
      Move(str_to_square("b3"), str_to_square("b6")),
      Move(str_to_square("b3"), str_to_square("b7")),
      Move(str_to_square("b3"), str_to_square("b8")),
      Move(str_to_square("b3"), str_to_square("c4")),
      Move(str_to_square("b3"), str_to_square("d5")),
  };
  std::vector<Move> test_moves;
  board.pseudolegal_queen_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, KingMovesWhite) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 w - - 4 36");

  std::vector<Move> moves = {Move(str_to_square("h2"), str_to_square("h1")),
                             Move(str_to_square("h2"), str_to_square("g1")),
                             Move(str_to_square("h2"), str_to_square("g3"))};
  std::vector<Move> test_moves;
  board.pseudolegal_king_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()))
      << ::testing::PrintToString(test_moves) << '\n'
      << ::testing::PrintToString(moves);
}

TEST(PseudoLegalMoves, KingMovesBlack) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 b - - 4 36");

  std::vector<Move> moves = {
      Move(str_to_square("h7"), str_to_square("h8")),
      Move(str_to_square("h7"), str_to_square("g6")),
  };
  std::vector<Move> test_moves;
  board.pseudolegal_king_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, KnightMovesWhite) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 w - - 4 36");

  std::vector<Move> moves = {
      Move(str_to_square("f5"), str_to_square("e7")),
      Move(str_to_square("f5"), str_to_square("g7")),
      Move(str_to_square("f5"), str_to_square("h6")),
      Move(str_to_square("f5"), str_to_square("g3")),
      Move(str_to_square("f5"), str_to_square("e3")),
      Move(str_to_square("f5"), str_to_square("d4")),
      Move(str_to_square("f5"), str_to_square("d6")),
  };
  std::vector<Move> test_moves;
  board.pseudolegal_knight_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()))
      << ::testing::PrintToString(test_moves) << '\n'
      << ::testing::PrintToString(moves);
}

TEST(PseudoLegalMoves, KnightMovesBlack) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 b - - 4 36");

  std::vector<Move> moves = {
      Move(str_to_square("g8"), str_to_square("e7")),
      Move(str_to_square("g8"), str_to_square("f6")),
  };
  std::vector<Move> test_moves;
  board.pseudolegal_knight_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, White) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 w - - 4 36");

  std::vector<Move> moves = {// Bishop moves.
                             Move(str_to_square("d2"), str_to_square("c1")),
                             Move(str_to_square("d2"), str_to_square("e3")),
                             Move(str_to_square("d2"), str_to_square("f4")),
                             Move(str_to_square("d2"), str_to_square("g5")),
                             Move(str_to_square("d2"), str_to_square("e1")),
                             Move(str_to_square("d2"), str_to_square("h6")),
                             // Rook moves.
                             Move(str_to_square("a1"), str_to_square("a2")),
                             Move(str_to_square("a1"), str_to_square("a3")),
                             Move(str_to_square("a1"), str_to_square("b1")),
                             Move(str_to_square("a1"), str_to_square("c1")),
                             Move(str_to_square("a1"), str_to_square("a4")),
                             Move(str_to_square("d1"), str_to_square("c1")),
                             Move(str_to_square("d1"), str_to_square("b1")),
                             Move(str_to_square("d1"), str_to_square("e1")),
                             Move(str_to_square("d1"), str_to_square("f1")),
                             Move(str_to_square("d1"), str_to_square("g1")),
                             Move(str_to_square("d1"), str_to_square("h1")),
                             // Queen moves.
                             Move(str_to_square("h4"), str_to_square("g3")),
                             Move(str_to_square("h4"), str_to_square("f2")),
                             Move(str_to_square("h4"), str_to_square("e1")),
                             Move(str_to_square("h4"), str_to_square("g4")),
                             Move(str_to_square("h4"), str_to_square("f4")),
                             Move(str_to_square("h4"), str_to_square("e4")),
                             Move(str_to_square("h4"), str_to_square("d4")),
                             Move(str_to_square("h4"), str_to_square("c4")),
                             Move(str_to_square("h4"), str_to_square("b4")),
                             Move(str_to_square("h4"), str_to_square("a4")),
                             Move(str_to_square("h4"), str_to_square("g5")),
                             Move(str_to_square("h4"), str_to_square("f6")),
                             Move(str_to_square("h4"), str_to_square("e7")),
                             Move(str_to_square("h4"), str_to_square("d8")),
                             Move(str_to_square("h4"), str_to_square("h5")),
                             Move(str_to_square("h4"), str_to_square("h6")),
                             // King moves.
                             Move(str_to_square("h2"), str_to_square("h1")),
                             Move(str_to_square("h2"), str_to_square("g1")),
                             Move(str_to_square("h2"), str_to_square("g3")),
                             Move(str_to_square("f5"), str_to_square("e7")),
                             // Knight moves.
                             Move(str_to_square("f5"), str_to_square("g7")),
                             Move(str_to_square("f5"), str_to_square("h6")),
                             Move(str_to_square("f5"), str_to_square("g3")),
                             Move(str_to_square("f5"), str_to_square("e3")),
                             Move(str_to_square("f5"), str_to_square("d4")),
                             Move(str_to_square("f5"), str_to_square("d6")),
                             // Pawn moves.
                             Move(str_to_square("c3"), str_to_square("c4")),
                             Move(str_to_square("d5"), str_to_square("d6")),
                             Move(str_to_square("f3"), str_to_square("f4")),
                             Move(str_to_square("g2"), str_to_square("g3")),
                             Move(str_to_square("g2"), str_to_square("g4"))};
  auto test_moves = board.pseudolegal_moves(Color::white);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, Black) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 b - - 4 36");

  std::vector<Move> moves = {// Bishop moves.
                             Move(str_to_square("f8"), str_to_square("e7")),
                             Move(str_to_square("f8"), str_to_square("d6")),
                             Move(str_to_square("f8"), str_to_square("c5")),
                             Move(str_to_square("f8"), str_to_square("b4")),
                             Move(str_to_square("f8"), str_to_square("a3")),
                             // Rook moves.
                             Move(str_to_square("a8"), str_to_square("a7")),
                             Move(str_to_square("a8"), str_to_square("a6")),
                             Move(str_to_square("a8"), str_to_square("a5")),
                             Move(str_to_square("a8"), str_to_square("b8")),
                             Move(str_to_square("a8"), str_to_square("c8")),
                             Move(str_to_square("d8"), str_to_square("c8")),
                             Move(str_to_square("d8"), str_to_square("b8")),
                             Move(str_to_square("d8"), str_to_square("e8")),
                             Move(str_to_square("d8"), str_to_square("d7")),
                             Move(str_to_square("d8"), str_to_square("d6")),
                             Move(str_to_square("d8"), str_to_square("d5")),
                             // Queen moves.
                             Move(str_to_square("b3"), str_to_square("a2")),
                             Move(str_to_square("b3"), str_to_square("a3")),
                             Move(str_to_square("b3"), str_to_square("b2")),
                             Move(str_to_square("b3"), str_to_square("c2")),
                             Move(str_to_square("b3"), str_to_square("d1")),
                             Move(str_to_square("b3"), str_to_square("c3")),
                             Move(str_to_square("b3"), str_to_square("b4")),
                             Move(str_to_square("b3"), str_to_square("b5")),
                             Move(str_to_square("b3"), str_to_square("b6")),
                             Move(str_to_square("b3"), str_to_square("b7")),
                             Move(str_to_square("b3"), str_to_square("b8")),
                             Move(str_to_square("b3"), str_to_square("c4")),
                             Move(str_to_square("b3"), str_to_square("d5")),
                             // King moves.
                             Move(str_to_square("h7"), str_to_square("h8")),
                             Move(str_to_square("h7"), str_to_square("g6")),
                             // Knight moves.
                             Move(str_to_square("g8"), str_to_square("e7")),
                             Move(str_to_square("g8"), str_to_square("f6")),
                             // Pawn moves.
                             Move(str_to_square("a4"), str_to_square("a3")),
                             Move(str_to_square("e5"), str_to_square("e4")),
                             Move(str_to_square("f7"), str_to_square("f6")),
                             Move(str_to_square("g7"), str_to_square("g6")),
                             Move(str_to_square("g7"), str_to_square("g5")),
                             Move(str_to_square("h6"), str_to_square("h5"))

  };
  auto test_moves = board.pseudolegal_moves(Color::black);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(AttackMoves, Simple) {
  Board board("4k3/8/8/8/4P3/8/8/2K5 w - - 0 1");

  Bitboard bb = str_to_square("b1") | str_to_square("d1") |
                str_to_square("b2") | str_to_square("c2") |
                str_to_square("d2") | str_to_square("e5");
  EXPECT_EQ(board.all_dst_squares(Color::white), bb);
}

TEST(SquareToStr, Simple) {
  EXPECT_EQ(square_to_str(str_to_square("a1")), "a1");
  EXPECT_EQ(square_to_str(str_to_square("e4")), "e4");
  EXPECT_EQ(square_to_str(str_to_square("a8")), "a8");
  EXPECT_EQ(square_to_str(str_to_square("h1")), "h1");
  EXPECT_EQ(square_to_str(str_to_square("h8")), "h8");
  EXPECT_EQ(square_to_str(str_to_square("d4")), "d4");
}

TEST(PseudoLegalMoves, SimplePawnMovesWhite) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");

  std::vector<Move> moves = {Move(str_to_square("g2"), str_to_square("g3")),
                             Move(str_to_square("f2"), str_to_square("f3")),
                             Move(str_to_square("a2"), str_to_square("a3")),
                             Move(str_to_square("d4"), str_to_square("d5"))

  };
  std::vector<Move> test_moves;
  board.pseudolegal_simple_pawn_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, SimplePawnMovesBlack) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");

  std::vector<Move> moves = {Move(str_to_square("h6"), str_to_square("h5")),
                             Move(str_to_square("c6"), str_to_square("c5")),
                             Move(str_to_square("f7"), str_to_square("f6")),
                             Move(str_to_square("b7"), str_to_square("b6")),
                             Move(str_to_square("a7"), str_to_square("a6"))

  };
  std::vector<Move> test_moves;
  board.pseudolegal_simple_pawn_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, TwoStepPawnMovesWhite) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");

  std::vector<Move> moves = {Move(str_to_square("f2"), str_to_square("f4")),
                             Move(str_to_square("a2"), str_to_square("a4"))};
  std::vector<Move> test_moves;
  board.pseudolegal_two_step_pawn_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, TwoStepPawnMovesBlack) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");

  std::vector<Move> moves = {Move(str_to_square("f7"), str_to_square("f5")),
                             Move(str_to_square("b7"), str_to_square("b5")),
                             Move(str_to_square("a7"), str_to_square("a5"))

  };
  std::vector<Move> test_moves;
  board.pseudolegal_two_step_pawn_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, NoEnPassantMovesWhite) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");

  std::vector<Move> test_moves;
  board.pseudolegal_en_passant_moves(Color::white, &test_moves);
  EXPECT_TRUE(test_moves.empty());
}

TEST(PseudoLegalMoves, OneEnPassantMoveWhite) {
  Board board("rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");

  std::vector<Move> moves = {Move(str_to_square("e5"), str_to_square("d6"))};
  std::vector<Move> test_moves;
  board.pseudolegal_en_passant_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, TwoEnPassantMovesWhite) {
  Board board("rnbqkbnr/1pp1pppp/8/2PpP3/p7/8/PP1P1PPP/RNBQKBNR w KQkq d6 0 5");

  std::vector<Move> moves = {Move(str_to_square("c5"), str_to_square("d6")),
                             Move(str_to_square("e5"), str_to_square("d6"))};
  std::vector<Move> test_moves;
  board.pseudolegal_en_passant_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, NoEnPassantMovesBlack) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");

  std::vector<Move> test_moves;
  board.pseudolegal_en_passant_moves(Color::black, &test_moves);
  EXPECT_TRUE(test_moves.empty());
}

TEST(PseudoLegalMoves, OneEnPassantMoveBlack) {
  Board board(
      "rnbqkbnr/ppp1p1pp/8/5p2/P2pP3/7P/1PPP1PP1/RNBQKBNR b KQkq e3 0 4");

  std::vector<Move> moves = {Move(str_to_square("d4"), str_to_square("e3"))};
  std::vector<Move> test_moves;
  board.pseudolegal_en_passant_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, TwoEnPassantMovesBlack) {
  Board board(
      "rnbqkbnr/ppp1p1pp/8/P7/3pPp2/7P/1PPP1PP1/RNBQKBNR b KQkq e3 0 5");

  std::vector<Move> moves = {Move(str_to_square("d4"), str_to_square("e3")),
                             Move(str_to_square("f4"), str_to_square("e3"))

  };
  std::vector<Move> test_moves;
  board.pseudolegal_en_passant_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, PromotionsWhite) {
  Board board("1Q1r1Q1q/PPP1PPPP/3P4/8/8/8/8/k2K4 w - - 0 1");

  std::vector<Move> moves = {
      Move(str_to_square("a7"), str_to_square("a8"), Piece::white_rook),
      Move(str_to_square("a7"), str_to_square("a8"), Piece::white_knight),
      Move(str_to_square("a7"), str_to_square("a8"), Piece::white_bishop),
      Move(str_to_square("a7"), str_to_square("a8"), Piece::white_queen),
      Move(str_to_square("c7"), str_to_square("c8"), Piece::white_rook),
      Move(str_to_square("c7"), str_to_square("c8"), Piece::white_knight),
      Move(str_to_square("c7"), str_to_square("c8"), Piece::white_bishop),
      Move(str_to_square("c7"), str_to_square("c8"), Piece::white_queen),
      Move(str_to_square("c7"), str_to_square("d8"), Piece::white_rook),
      Move(str_to_square("c7"), str_to_square("d8"), Piece::white_knight),
      Move(str_to_square("c7"), str_to_square("d8"), Piece::white_bishop),
      Move(str_to_square("c7"), str_to_square("d8"), Piece::white_queen),
      Move(str_to_square("e7"), str_to_square("e8"), Piece::white_rook),
      Move(str_to_square("e7"), str_to_square("e8"), Piece::white_knight),
      Move(str_to_square("e7"), str_to_square("e8"), Piece::white_bishop),
      Move(str_to_square("e7"), str_to_square("e8"), Piece::white_queen),
      Move(str_to_square("e7"), str_to_square("d8"), Piece::white_rook),
      Move(str_to_square("e7"), str_to_square("d8"), Piece::white_knight),
      Move(str_to_square("e7"), str_to_square("d8"), Piece::white_bishop),
      Move(str_to_square("e7"), str_to_square("d8"), Piece::white_queen),
      Move(str_to_square("g7"), str_to_square("g8"), Piece::white_rook),
      Move(str_to_square("g7"), str_to_square("g8"), Piece::white_knight),
      Move(str_to_square("g7"), str_to_square("g8"), Piece::white_bishop),
      Move(str_to_square("g7"), str_to_square("g8"), Piece::white_queen),
      Move(str_to_square("g7"), str_to_square("h8"), Piece::white_rook),
      Move(str_to_square("g7"), str_to_square("h8"), Piece::white_knight),
      Move(str_to_square("g7"), str_to_square("h8"), Piece::white_bishop),
      Move(str_to_square("g7"), str_to_square("h8"), Piece::white_queen),
  };
  std::vector<Move> test_moves;
  board.pseudolegal_promotions(Color::white, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, PromotionsBlack) {
  Board board("1K1k4/8/8/8/8/3p4/ppp1pppp/1q1R1q1Q b - - 0 1");

  std::vector<Move> moves = {
      Move(str_to_square("a2"), str_to_square("a1"), Piece::black_rook),
      Move(str_to_square("a2"), str_to_square("a1"), Piece::black_knight),
      Move(str_to_square("a2"), str_to_square("a1"), Piece::black_bishop),
      Move(str_to_square("a2"), str_to_square("a1"), Piece::black_queen),
      Move(str_to_square("c2"), str_to_square("c1"), Piece::black_rook),
      Move(str_to_square("c2"), str_to_square("c1"), Piece::black_knight),
      Move(str_to_square("c2"), str_to_square("c1"), Piece::black_bishop),
      Move(str_to_square("c2"), str_to_square("c1"), Piece::black_queen),
      Move(str_to_square("c2"), str_to_square("d1"), Piece::black_rook),
      Move(str_to_square("c2"), str_to_square("d1"), Piece::black_knight),
      Move(str_to_square("c2"), str_to_square("d1"), Piece::black_bishop),
      Move(str_to_square("c2"), str_to_square("d1"), Piece::black_queen),
      Move(str_to_square("e2"), str_to_square("e1"), Piece::black_rook),
      Move(str_to_square("e2"), str_to_square("e1"), Piece::black_knight),
      Move(str_to_square("e2"), str_to_square("e1"), Piece::black_bishop),
      Move(str_to_square("e2"), str_to_square("e1"), Piece::black_queen),
      Move(str_to_square("e2"), str_to_square("d1"), Piece::black_rook),
      Move(str_to_square("e2"), str_to_square("d1"), Piece::black_knight),
      Move(str_to_square("e2"), str_to_square("d1"), Piece::black_bishop),
      Move(str_to_square("e2"), str_to_square("d1"), Piece::black_queen),
      Move(str_to_square("g2"), str_to_square("g1"), Piece::black_rook),
      Move(str_to_square("g2"), str_to_square("g1"), Piece::black_knight),
      Move(str_to_square("g2"), str_to_square("g1"), Piece::black_bishop),
      Move(str_to_square("g2"), str_to_square("g1"), Piece::black_queen),
      Move(str_to_square("g2"), str_to_square("h1"), Piece::black_rook),
      Move(str_to_square("g2"), str_to_square("h1"), Piece::black_knight),
      Move(str_to_square("g2"), str_to_square("h1"), Piece::black_bishop),
      Move(str_to_square("g2"), str_to_square("h1"), Piece::black_queen),
  };
  std::vector<Move> test_moves;
  board.pseudolegal_promotions(Color::black, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, PawnCapturesWhite) {
  Board board(
      "rn1qk2r/Pp2pp1p/2p5/1p1pbbpn/2PPPPPN/8/1P5P/RNBQK2R w KQkq - 1 13");

  std::vector<Move> moves = {Move(str_to_square("g4"), str_to_square("h5")),
                             Move(str_to_square("g4"), str_to_square("f5")),
                             Move(str_to_square("f4"), str_to_square("g5")),
                             Move(str_to_square("f4"), str_to_square("e5")),
                             Move(str_to_square("e4"), str_to_square("f5")),
                             Move(str_to_square("e4"), str_to_square("d5")),
                             Move(str_to_square("d4"), str_to_square("e5")),
                             Move(str_to_square("c4"), str_to_square("d5")),
                             Move(str_to_square("c4"), str_to_square("b5"))};
  std::vector<Move> test_moves;
  board.pseudolegal_pawn_captures(Color::white, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, PawnCapturesBlack) {
  Board board(
      "rn1qk2r/Pp2pp1p/2p5/1p1pbbpn/2PPPPPN/8/1P5P/RNBQK2R b KQkq - 1 13");

  std::vector<Move> moves = {
      Move(str_to_square("g5"), str_to_square("h4")),
      Move(str_to_square("g5"), str_to_square("f4")),
      Move(str_to_square("d5"), str_to_square("e4")),
      Move(str_to_square("d5"), str_to_square("c4")),
      Move(str_to_square("b5"), str_to_square("c4")),
  };
  std::vector<Move> test_moves;
  board.pseudolegal_pawn_captures(Color::black, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, PawnsWhite) {
  Board board(
      "1n1qkbnr/1p2pppp/r7/1Ppp3P/3PP3/2N2N2/pPP2PP1/1RBQKB1R w Kk - 6 12");

  std::vector<Move> moves = {
      Move(str_to_square("g2"), str_to_square("g3")),
      Move(str_to_square("g2"), str_to_square("g4")),
      Move(str_to_square("b2"), str_to_square("b3")),
      Move(str_to_square("b2"), str_to_square("b4")),
      Move(str_to_square("e4"), str_to_square("e5")),
      Move(str_to_square("e4"), str_to_square("d5")),
      Move(str_to_square("d4"), str_to_square("c5")),
      Move(str_to_square("h5"), str_to_square("h6")),
      Move(str_to_square("b5"), str_to_square("b6")),
      Move(str_to_square("b5"), str_to_square("a6")),
  };
  std::vector<Move> test_moves;
  board.pseudolegal_pawn_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, PawnsBlack) {
  Board board(
      "1n1qkbnr/1p2pppp/r7/1Ppp3P/3PP3/2N2N2/pPP2PP1/1RBQKB1R b Kk - 6 12");

  std::vector<Move> moves = {
      Move(str_to_square("a2"), str_to_square("a1"), Piece::black_rook),
      Move(str_to_square("a2"), str_to_square("a1"), Piece::black_knight),
      Move(str_to_square("a2"), str_to_square("a1"), Piece::black_bishop),
      Move(str_to_square("a2"), str_to_square("a1"), Piece::black_queen),
      Move(str_to_square("a2"), str_to_square("b1"), Piece::black_rook),
      Move(str_to_square("a2"), str_to_square("b1"), Piece::black_knight),
      Move(str_to_square("a2"), str_to_square("b1"), Piece::black_bishop),
      Move(str_to_square("a2"), str_to_square("b1"), Piece::black_queen),
      Move(str_to_square("d5"), str_to_square("e4")),
      Move(str_to_square("c5"), str_to_square("d4")),
      Move(str_to_square("c5"), str_to_square("c4")),
      Move(str_to_square("h7"), str_to_square("h6")),
      Move(str_to_square("g7"), str_to_square("g6")),
      Move(str_to_square("g7"), str_to_square("g5")),
      Move(str_to_square("f7"), str_to_square("f6")),
      Move(str_to_square("f7"), str_to_square("f5")),
      Move(str_to_square("e7"), str_to_square("e6")),
      Move(str_to_square("e7"), str_to_square("e5")),
      Move(str_to_square("b7"), str_to_square("b6")),
  };
  std::vector<Move> test_moves;
  board.pseudolegal_pawn_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}
