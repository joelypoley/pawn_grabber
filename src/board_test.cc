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
  EXPECT_EQ(str_to_square("e5"), north(e4));
  EXPECT_EQ(str_to_square("f5"), northeast(e4));
  EXPECT_EQ(str_to_square("d5"), northwest(e4));
  EXPECT_EQ(str_to_square("d4"), west(e4));
  EXPECT_EQ(str_to_square("f4"), east(e4));
  EXPECT_EQ(str_to_square("e3"), south(e4));
  EXPECT_EQ(str_to_square("d3"), southwest(e4));
  EXPECT_EQ(str_to_square("f3"), southeast(e4));
}

TEST(SquareDirections, A8) {
  Bitboard a8 = str_to_square("a8");
  EXPECT_EQ(0, north(a8));
  EXPECT_EQ(0, northeast(a8));
  EXPECT_EQ(0, northwest(a8));
  EXPECT_EQ(0, west(a8));
  EXPECT_EQ(str_to_square("b8"), east(a8));
  EXPECT_EQ(str_to_square("a7"), south(a8));
  EXPECT_EQ(0, southwest(a8));
  EXPECT_EQ(str_to_square("b7"), southeast(a8));
}

TEST(SquareDirections, A1) {
  Bitboard a1 = str_to_square("a1");
  EXPECT_EQ(str_to_square("a2"), north(a1));
  EXPECT_EQ(str_to_square("b2"), northeast(a1));
  EXPECT_EQ(0, northwest(a1));
  EXPECT_EQ(0, west(a1));
  EXPECT_EQ(str_to_square("b1"), east(a1));
  EXPECT_EQ(0, south(a1));
  EXPECT_EQ(0, southwest(a1));
  EXPECT_EQ(0, southeast(a1));
}

TEST(SquareDirections, H1) {
  Bitboard h1 = str_to_square("h1");
  EXPECT_EQ(str_to_square("h2"), north(h1));
  EXPECT_EQ(0, northeast(h1));
  EXPECT_EQ(str_to_square("g2"), northwest(h1));
  EXPECT_EQ(str_to_square("g1"), west(h1));
  EXPECT_EQ(0, east(h1));
  EXPECT_EQ(0, south(h1));
  EXPECT_EQ(0, southwest(h1));
  EXPECT_EQ(0, southeast(h1));
}

TEST(SquareDirections, H8) {
  Bitboard h8 = str_to_square("h8");
  EXPECT_EQ(0, north(h8));
  EXPECT_EQ(0, northeast(h8));
  EXPECT_EQ(0, northwest(h8));
  EXPECT_EQ(str_to_square("g8"), west(h8));
  EXPECT_EQ(0, east(h8));
  EXPECT_EQ(str_to_square("h7"), south(h8));
  EXPECT_EQ(str_to_square("g7"), southwest(h8));
  EXPECT_EQ(0, southeast(h8));
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
  EXPECT_EQ(start_board.side_to_move_, Color::white);
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
  EXPECT_EQ(board.side_to_move_, Color::black);
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
  EXPECT_EQ(board.side_to_move_, Color::white);
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
  EXPECT_EQ(board.side_to_move_, Color::black);
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
  EXPECT_EQ(board.side_to_move_, Color::black);
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
  EXPECT_EQ(board.side_to_move_, Color::white);
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
  EXPECT_EQ(board.side_to_move_, Color::white);
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
  EXPECT_EQ(board.side_to_move_, Color::white);
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
  board.pseudolegal_moves_in_direction(north, a1, &test_north_moves);
  EXPECT_TRUE(test_north_moves.empty());

  std::vector<Move> test_west_moves;
  board.pseudolegal_moves_in_direction(west, a1, &test_west_moves);
  EXPECT_TRUE(test_west_moves.empty());

  std::vector<Move> test_south_moves;
  board.pseudolegal_moves_in_direction(south, a1, &test_south_moves);
  EXPECT_TRUE(test_south_moves.empty());

  // TODO: Change to computed_east_moves and east_moves.
  std::vector<Move> east_moves = {
      Move(a1, str_to_square("b1")), Move(a1, str_to_square("c1")),
      Move(a1, str_to_square("d1")), Move(a1, str_to_square("e1"))};
  std::vector<Move> test_east_moves;
  board.pseudolegal_moves_in_direction(east, a1, &test_east_moves);
  EXPECT_EQ(test_east_moves, east_moves);
}

TEST(PseudoLegalMoves, F1Rook) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");
  const Bitboard f1 = str_to_square("f1");

  std::vector<Move> test_north_moves;
  board.pseudolegal_moves_in_direction(north, f1, &test_north_moves);
  EXPECT_TRUE(test_north_moves.empty());

  std::vector<Move> test_south_moves;
  board.pseudolegal_moves_in_direction(south, f1, &test_south_moves);
  EXPECT_TRUE(test_south_moves.empty());

  std::vector<Move> east_moves = {{f1, str_to_square("g1")}};
  std::vector<Move> test_east_moves;
  board.pseudolegal_moves_in_direction(east, f1, &test_east_moves);
  EXPECT_EQ(test_east_moves, east_moves);

  std::vector<Move> west_moves = {
      Move(f1, str_to_square("e1")), Move(f1, str_to_square("d1")),
      Move(f1, str_to_square("c1")), Move(f1, str_to_square("b1"))};
  std::vector<Move> test_west_moves;
  board.pseudolegal_moves_in_direction(west, f1, &test_west_moves);
  EXPECT_EQ(test_west_moves, west_moves);
}

TEST(PseudoLegalMoves, B3Queen) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");
  const Bitboard b3 = str_to_square("b3");

  std::vector<Move> test_east_moves;
  board.pseudolegal_moves_in_direction(east, b3, &test_east_moves);
  EXPECT_TRUE(test_east_moves.empty());

  std::vector<Move> test_southwest_moves;
  board.pseudolegal_moves_in_direction(southwest, b3, &test_southwest_moves);
  EXPECT_TRUE(test_southwest_moves.empty());

  std::vector<Move> test_south_moves;
  board.pseudolegal_moves_in_direction(south, b3, &test_south_moves);
  EXPECT_TRUE(test_south_moves.empty());

  std::vector<Move> north_moves = {
      Move(b3, str_to_square("b4")), Move(b3, str_to_square("b5")),
      Move(b3, str_to_square("b6")), Move(b3, str_to_square("b7"))};
  std::vector<Move> test_north_moves;
  board.pseudolegal_moves_in_direction(north, b3, &test_north_moves);
  EXPECT_EQ(test_north_moves, north_moves);

  std::vector<Move> northwest_moves = {Move(b3, str_to_square("a4"))};
  std::vector<Move> test_northwest_moves;
  board.pseudolegal_moves_in_direction(northwest, b3, &test_northwest_moves);
  EXPECT_EQ(test_northwest_moves, northwest_moves);

  std::vector<Move> northeast_moves = {
      Move(b3, str_to_square("c4")), Move(b3, str_to_square("d5")),
      Move(b3, str_to_square("e6")), Move(b3, str_to_square("f7"))};
  std::vector<Move> test_northeast_moves;
  board.pseudolegal_moves_in_direction(northeast, b3, &test_northeast_moves);
  EXPECT_EQ(test_northeast_moves, northeast_moves);

  std::vector<Move> west_moves = {Move(b3, str_to_square("a3"))};
  std::vector<Move> test_west_moves;
  board.pseudolegal_moves_in_direction(west, b3, &test_west_moves);
  EXPECT_EQ(test_west_moves, west_moves);

  std::vector<Move> southeast_moves = {
      Move(b3, str_to_square("c2")),
      Move(b3, str_to_square("d1")),
  };
  std::vector<Move> test_southeast_moves;
  board.pseudolegal_moves_in_direction(southeast, b3, &test_southeast_moves);
  EXPECT_EQ(test_southeast_moves, southeast_moves);
}

TEST(PseudoLegalMoves, A8Rook) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");
  const Bitboard a8 = str_to_square("a8");

  std::vector<Move> test_north_moves;
  board.pseudolegal_moves_in_direction(north, a8, &test_north_moves);
  EXPECT_TRUE(test_north_moves.empty());

  std::vector<Move> test_west_moves;
  board.pseudolegal_moves_in_direction(west, a8, &test_west_moves);
  EXPECT_TRUE(test_west_moves.empty());

  std::vector<Move> test_south_moves;
  board.pseudolegal_moves_in_direction(south, a8, &test_south_moves);
  EXPECT_TRUE(test_south_moves.empty());

  std::vector<Move> east_moves = {
      Move(a8, str_to_square("b8")), Move(a8, str_to_square("c8")),
      Move(a8, str_to_square("d8")), Move(a8, str_to_square("e8"))};
  std::vector<Move> test_east_moves;
  board.pseudolegal_moves_in_direction(east, a8, &test_east_moves);
  EXPECT_EQ(test_east_moves, east_moves);
}

TEST(PseudoLegalMoves, F8Rook) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");
  const Bitboard f8 = str_to_square("f8");

  std::vector<Move> test_north_moves;
  board.pseudolegal_moves_in_direction(north, f8, &test_north_moves);
  EXPECT_TRUE(test_north_moves.empty());

  std::vector<Move> test_east_moves;
  board.pseudolegal_moves_in_direction(east, f8, &test_east_moves);
  EXPECT_TRUE(test_east_moves.empty());

  std::vector<Move> test_south_moves;
  board.pseudolegal_moves_in_direction(south, f8, &test_south_moves);
  EXPECT_TRUE(test_south_moves.empty());

  std::vector<Move> west_moves = {
      Move(f8, str_to_square("e8")), Move(f8, str_to_square("d8")),
      Move(f8, str_to_square("c8")), Move(f8, str_to_square("b8"))};
  std::vector<Move> test_west_moves;
  board.pseudolegal_moves_in_direction(west, f8, &test_west_moves);
  EXPECT_EQ(test_west_moves, west_moves);
}

TEST(PseudoLegalMoves, G6Bishop) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");
  const Bitboard g6 = str_to_square("g6");

  std::vector<Move> test_northwest_moves;
  board.pseudolegal_moves_in_direction(northwest, g6, &test_northwest_moves);
  EXPECT_TRUE(test_northwest_moves.empty());

  std::vector<Move> northeast_moves = {Move(g6, str_to_square("h7"))};
  std::vector<Move> test_northeast_moves;
  board.pseudolegal_moves_in_direction(northeast, g6, &test_northeast_moves);
  EXPECT_EQ(test_northeast_moves, northeast_moves);

  std::vector<Move> southwest_moves = {Move(g6, str_to_square("f5"))};
  std::vector<Move> test_southwest_moves;
  board.pseudolegal_moves_in_direction(southwest, g6, &test_southwest_moves);
  EXPECT_EQ(test_southwest_moves, southwest_moves);

  std::vector<Move> southeast_moves = {Move(g6, str_to_square("h5"))};
  std::vector<Move> test_southeast_moves;
  board.pseudolegal_moves_in_direction(southeast, g6, &test_southeast_moves);
  EXPECT_EQ(test_southeast_moves, southeast_moves);
}

TEST(PseudoLegalMoves, H4Queen) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");
  const Bitboard h4 = str_to_square("h4");

  std::vector<Move> north_moves = {Move(h4, str_to_square("h5"))};
  std::vector<Move> test_north_moves;
  board.pseudolegal_moves_in_direction(north, h4, &test_north_moves);
  EXPECT_EQ(test_north_moves, north_moves);

  std::vector<Move> south_moves = {Move(h4, str_to_square("h3"))};
  std::vector<Move> test_south_moves;
  board.pseudolegal_moves_in_direction(south, h4, &test_south_moves);
  EXPECT_EQ(test_south_moves, south_moves);

  std::vector<Move> test_west_moves;
  board.pseudolegal_moves_in_direction(west, h4, &test_west_moves);
  EXPECT_TRUE(test_west_moves.empty());

  std::vector<Move> test_east_moves;
  board.pseudolegal_moves_in_direction(east, h4, &test_east_moves);
  EXPECT_TRUE(test_east_moves.empty());

  std::vector<Move> test_northeast_moves;
  board.pseudolegal_moves_in_direction(northeast, h4, &test_northeast_moves);
  EXPECT_TRUE(test_northeast_moves.empty());

  std::vector<Move> test_southeast_moves;
  board.pseudolegal_moves_in_direction(southeast, h4, &test_southeast_moves);
  EXPECT_TRUE(test_southeast_moves.empty());

  std::vector<Move> northwest_moves = {
      Move(h4, str_to_square("g5")),
      Move(h4, str_to_square("f6")),
      Move(h4, str_to_square("e7")),
      Move(h4, str_to_square("d8")),
  };
  std::vector<Move> test_northwest_moves;
  board.pseudolegal_moves_in_direction(northwest, h4, &test_northwest_moves);
  EXPECT_EQ(test_northwest_moves, northwest_moves);

  std::vector<Move> southwest_moves = {
      Move(h4, str_to_square("g3")),
      Move(h4, str_to_square("f2")),
  };
  std::vector<Move> test_southwest_moves;
  board.pseudolegal_moves_in_direction(southwest, h4, &test_southwest_moves);
  EXPECT_EQ(test_southwest_moves, southwest_moves);
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
  board.pseudolegal_simple_pawn_moves(&test_moves);
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
  board.pseudolegal_simple_pawn_moves(&test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, TwoStepPawnMovesWhite) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");

  std::vector<Move> moves = {Move(str_to_square("f2"), str_to_square("f4")),
                             Move(str_to_square("a2"), str_to_square("a4"))};
  std::vector<Move> test_moves;
  board.pseudolegal_two_step_pawn_moves(&test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, TwoStepPawnMovesBlack) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");

  std::vector<Move> moves = {Move(str_to_square("f7"), str_to_square("f5")),
                             Move(str_to_square("b7"), str_to_square("b5")),
                             Move(str_to_square("a7"), str_to_square("a5"))

  };
  std::vector<Move> test_moves;
  board.pseudolegal_two_step_pawn_moves(&test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, NoEnPassantMovesWhite) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");

  std::vector<Move> test_moves;
  board.pseudolegal_en_passant_moves(&test_moves);
  EXPECT_TRUE(test_moves.empty());
}

TEST(PseudoLegalMoves, OneEnPassantMoveWhite) {
  Board board("rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");

  std::vector<Move> moves = {Move(str_to_square("e5"), str_to_square("d6"))};
  std::vector<Move> test_moves;
  board.pseudolegal_en_passant_moves(&test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, TwoEnPassantMovesWhite) {
  Board board("rnbqkbnr/1pp1pppp/8/2PpP3/p7/8/PP1P1PPP/RNBQKBNR w KQkq d6 0 5");

  std::vector<Move> moves = {Move(str_to_square("c5"), str_to_square("d6")),
                             Move(str_to_square("e5"), str_to_square("d6"))};
  std::vector<Move> test_moves;
  board.pseudolegal_en_passant_moves(&test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, NoEnPassantMovesBlack) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");

  std::vector<Move> test_moves;
  board.pseudolegal_en_passant_moves(&test_moves);
  EXPECT_TRUE(test_moves.empty());
}

TEST(PseudoLegalMoves, OneEnPassantMoveBlack) {
  Board board(
      "rnbqkbnr/ppp1p1pp/8/5p2/P2pP3/7P/1PPP1PP1/RNBQKBNR b KQkq e3 0 4");

  std::vector<Move> moves = {Move(str_to_square("d4"), str_to_square("e3"))};
  std::vector<Move> test_moves;
  board.pseudolegal_en_passant_moves(&test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, TwoEnPassantMovesBlack) {
  Board board(
      "rnbqkbnr/ppp1p1pp/8/P7/3pPp2/7P/1PPP1PP1/RNBQKBNR b KQkq e3 0 5");

  std::vector<Move> moves = {Move(str_to_square("d4"), str_to_square("e3")),
                             Move(str_to_square("f4"), str_to_square("e3"))

  };
  std::vector<Move> test_moves;
  board.pseudolegal_en_passant_moves(&test_moves);
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
  board.pseudolegal_promotions(&test_moves);
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
  board.pseudolegal_promotions(&test_moves);
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
  board.pseudolegal_pawn_captures(&test_moves);
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
  board.pseudolegal_pawn_captures(&test_moves);
  EXPECT_EQ(test_moves, moves);
}
