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
  EXPECT_TRUE(is_square(algebraic_to_square("a1")));
  EXPECT_FALSE(is_square(algebraic_to_square("a1") |
                         is_square(algebraic_to_square("a2"))));
}

TEST(SquareUtils, SquareIndex) {
  EXPECT_EQ(square_idx(algebraic_to_square("h1")), 0);
  EXPECT_EQ(square_idx(algebraic_to_square("a8")), 63);
  EXPECT_EQ(square_idx(algebraic_to_square("a1")), 7);
}

TEST(SquareUtils, RankIndex) {
  EXPECT_EQ(rank_idx(algebraic_to_square("h1")), 0);
  EXPECT_EQ(rank_idx(algebraic_to_square("h2")), 1);
  EXPECT_EQ(rank_idx(algebraic_to_square("a8")), 7);
  EXPECT_EQ(rank_idx(algebraic_to_square("e4")), 3);
}

TEST(SquareUtils, FileIndex) {
  EXPECT_EQ(file_idx(algebraic_to_square("h1")), 7);
  EXPECT_EQ(file_idx(algebraic_to_square("a8")), 0);
  EXPECT_EQ(file_idx(algebraic_to_square("e4")), 4);
}

TEST(SquareUtils, OnAFile) {
  EXPECT_TRUE(on_a_file(algebraic_to_square("a1")));
  EXPECT_TRUE(on_a_file(algebraic_to_square("a8")));
  EXPECT_FALSE(on_a_file(algebraic_to_square("b4")));
  EXPECT_FALSE(on_a_file(algebraic_to_square("e4")));
  EXPECT_FALSE(on_a_file(algebraic_to_square("h4")));
}

TEST(SquareUtils, OnHFile) {
  EXPECT_TRUE(on_h_file(algebraic_to_square("h1")));
  EXPECT_TRUE(on_h_file(algebraic_to_square("h8")));
  EXPECT_FALSE(on_h_file(algebraic_to_square("e4")));
  EXPECT_FALSE(on_h_file(algebraic_to_square("g4")));
  EXPECT_FALSE(on_h_file(algebraic_to_square("g8")));
}

TEST(SquareDirections, E4) {
  Bitboard e4 = algebraic_to_square("e4");
  EXPECT_EQ(algebraic_to_square("e5"), north(e4));
  EXPECT_EQ(algebraic_to_square("f5"), northeast(e4));
  EXPECT_EQ(algebraic_to_square("d5"), northwest(e4));
  EXPECT_EQ(algebraic_to_square("d4"), west(e4));
  EXPECT_EQ(algebraic_to_square("f4"), east(e4));
  EXPECT_EQ(algebraic_to_square("e3"), south(e4));
  EXPECT_EQ(algebraic_to_square("d3"), southwest(e4));
  EXPECT_EQ(algebraic_to_square("f3"), southeast(e4));
}

TEST(SquareDirections, A8) {
  Bitboard a8 = algebraic_to_square("a8");
  EXPECT_EQ(0, north(a8));
  EXPECT_EQ(0, northeast(a8));
  EXPECT_EQ(0, northwest(a8));
  EXPECT_EQ(0, west(a8));
  EXPECT_EQ(algebraic_to_square("b8"), east(a8));
  EXPECT_EQ(algebraic_to_square("a7"), south(a8));
  EXPECT_EQ(0, southwest(a8));
  EXPECT_EQ(algebraic_to_square("b7"), southeast(a8));
}

TEST(SquareDirections, A1) {
  Bitboard a1 = algebraic_to_square("a1");
  EXPECT_EQ(algebraic_to_square("a2"), north(a1));
  EXPECT_EQ(algebraic_to_square("b2"), northeast(a1));
  EXPECT_EQ(0, northwest(a1));
  EXPECT_EQ(0, west(a1));
  EXPECT_EQ(algebraic_to_square("b1"), east(a1));
  EXPECT_EQ(0, south(a1));
  EXPECT_EQ(0, southwest(a1));
  EXPECT_EQ(0, southeast(a1));
}

TEST(SquareDirections, H1) {
  Bitboard h1 = algebraic_to_square("h1");
  EXPECT_EQ(algebraic_to_square("h2"), north(h1));
  EXPECT_EQ(0, northeast(h1));
  EXPECT_EQ(algebraic_to_square("g2"), northwest(h1));
  EXPECT_EQ(algebraic_to_square("g1"), west(h1));
  EXPECT_EQ(0, east(h1));
  EXPECT_EQ(0, south(h1));
  EXPECT_EQ(0, southwest(h1));
  EXPECT_EQ(0, southeast(h1));
}

TEST(SquareDirections, H8) {
  Bitboard h8 = algebraic_to_square("h8");
  EXPECT_EQ(0, north(h8));
  EXPECT_EQ(0, northeast(h8));
  EXPECT_EQ(0, northwest(h8));
  EXPECT_EQ(algebraic_to_square("g8"), west(h8));
  EXPECT_EQ(0, east(h8));
  EXPECT_EQ(algebraic_to_square("h7"), south(h8));
  EXPECT_EQ(algebraic_to_square("g7"), southwest(h8));
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
  EXPECT_EQ(board.white_pawns_,
            algebraic_to_square("g2") | algebraic_to_square("f2") |
                algebraic_to_square("b2") | algebraic_to_square("a2") |
                algebraic_to_square("h3") | algebraic_to_square("e3") |
                algebraic_to_square("d4"));
  EXPECT_EQ(board.white_rooks_,
            algebraic_to_square("a1") | algebraic_to_square("f1"));
  EXPECT_EQ(board.white_knights_,
            algebraic_to_square("c3") | algebraic_to_square("d2"));
  EXPECT_EQ(board.white_bishops_, 0);
  EXPECT_EQ(board.white_queens_, algebraic_to_square("b3"));
  EXPECT_EQ(board.white_king_, algebraic_to_square("h1"));
  EXPECT_EQ(board.black_pawns_,
            algebraic_to_square("a7") | algebraic_to_square("b7") |
                algebraic_to_square("c6") | algebraic_to_square("e4") |
                algebraic_to_square("f7") | algebraic_to_square("g7") |
                algebraic_to_square("h6"));
  EXPECT_EQ(board.black_rooks_,
            algebraic_to_square("a8") | algebraic_to_square("f8"));
  EXPECT_EQ(board.black_knights_, algebraic_to_square("g4"));
  EXPECT_EQ(board.black_bishops_, algebraic_to_square("g6"));
  EXPECT_EQ(board.black_queens_, algebraic_to_square("h4"));
  EXPECT_EQ(board.black_king_, algebraic_to_square("g8"));
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
                 white_squares_bb.begin(), algebraic_to_square);
  Bitboard bb =
      std::reduce(white_squares_bb.begin(), white_squares_bb.end(), 0,
                  [](Bitboard bb1, Bitboard bb2) { return bb1 | bb2; });
  EXPECT_EQ(board.white_pieces(), bb);
}

TEST(Slide, A1Rook) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");
  const Bitboard a1 = algebraic_to_square("a1");
  EXPECT_TRUE(board.pseudolegal_moves_in_direction(north, a1).empty());
  EXPECT_TRUE(board.pseudolegal_moves_in_direction(west, a1).empty());
  EXPECT_TRUE(board.pseudolegal_moves_in_direction(south, a1).empty());
  std::vector<Move> east_moves = {{a1, algebraic_to_square("b1")},
                                  {a1, algebraic_to_square("c1")},
                                  {a1, algebraic_to_square("d1")},
                                  {a1, algebraic_to_square("e1")}};
  EXPECT_EQ(board.pseudolegal_moves_in_direction(east, a1), east_moves);
}
