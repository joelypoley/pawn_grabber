#include "board.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>

#include "absl/algorithm/container.h"
#include "absl/strings/str_cat.h"
#include "absl/types/optional.h"
#include "gtest/gtest.h"

TEST(SquareUtils, IsSquare) {
  EXPECT_FALSE(is_square(0));
  EXPECT_TRUE(is_square(str_to_square("a1")));
  EXPECT_FALSE(is_square(str_to_square("a1") | str_to_square("a2")));
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
  EXPECT_EQ(start_board.en_passant_square_, absl::nullopt);
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
  EXPECT_EQ(board.en_passant_square_, absl::nullopt);
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
  EXPECT_EQ(board.en_passant_square_, absl::nullopt);
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
  EXPECT_EQ(board.en_passant_square_, absl::nullopt);
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
  EXPECT_EQ(board.en_passant_square_, absl::nullopt);
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
  EXPECT_EQ(board.en_passant_square_, absl::nullopt);
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
  absl::c_transform(white_squares, white_squares_bb.begin(), str_to_square);
  Bitboard bb =
      absl::c_accumulate(white_squares_bb, Bitboard(0),
                         [](Bitboard bb1, Bitboard bb2) { return bb1 | bb2; });
  EXPECT_EQ(board.white_pieces(), bb);
}

TEST(PseudoLegalMoves, A1Rook) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");
  const Bitboard a1 = str_to_square("a1");

  std::vector<Move> computed_north_moves;
  board.append_pseudolegal_sliding_moves(Direction::north, Color::white, a1,
                                         Piece::rook, &computed_north_moves);
  EXPECT_TRUE(computed_north_moves.empty());

  std::vector<Move> computed_west_moves;
  board.append_pseudolegal_sliding_moves(Direction::west, Color::white, a1,
                                         Piece::rook, &computed_west_moves);
  EXPECT_TRUE(computed_west_moves.empty());

  std::vector<Move> computed_south_moves;
  board.append_pseudolegal_sliding_moves(Direction::south, Color::white, a1,
                                         Piece::rook, &computed_south_moves);
  EXPECT_TRUE(computed_south_moves.empty());

  std::vector<Move> correct_east_moves = {
      Move(a1, str_to_square("b1"), Piece::rook, MoveType::simple, board),
      Move(a1, str_to_square("c1"), Piece::rook, MoveType::simple, board),
      Move(a1, str_to_square("d1"), Piece::rook, MoveType::simple, board),
      Move(a1, str_to_square("e1"), Piece::rook, MoveType::simple, board)};
  std::vector<Move> computed_east_moves;
  board.append_pseudolegal_sliding_moves(Direction::east, Color::white, a1,
                                         Piece::rook, &computed_east_moves);
  EXPECT_EQ(computed_east_moves, correct_east_moves);
}

TEST(PseudoLegalMoves, F1Rook) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");
  const Bitboard f1 = str_to_square("f1");

  std::vector<Move> computer_north_moves;
  board.append_pseudolegal_sliding_moves(Direction::north, Color::white, f1,
                                         Piece::rook, &computer_north_moves);
  EXPECT_TRUE(computer_north_moves.empty());

  std::vector<Move> computer_south_moves;
  board.append_pseudolegal_sliding_moves(Direction::south, Color::white, f1,
                                         Piece::rook, &computer_south_moves);
  EXPECT_TRUE(computer_south_moves.empty());

  std::vector<Move> correct_east_moves = {
      Move(f1, str_to_square("g1"), Piece::rook, MoveType::simple, board)};
  std::vector<Move> computer_east_moves;
  board.append_pseudolegal_sliding_moves(Direction::east, Color::white, f1,
                                         Piece::rook, &computer_east_moves);
  EXPECT_EQ(computer_east_moves, correct_east_moves);

  std::vector<Move> correct_west_moves = {
      Move(f1, str_to_square("e1"), Piece::rook, MoveType::simple, board),
      Move(f1, str_to_square("d1"), Piece::rook, MoveType::simple, board),
      Move(f1, str_to_square("c1"), Piece::rook, MoveType::simple, board),
      Move(f1, str_to_square("b1"), Piece::rook, MoveType::simple, board)};
  std::vector<Move> computer_west_moves;
  board.append_pseudolegal_sliding_moves(Direction::west, Color::white, f1,
                                         Piece::rook, &computer_west_moves);
  EXPECT_EQ(computer_west_moves, correct_west_moves);
}

TEST(PseudoLegalMoves, B3Queen) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");
  const Bitboard b3 = str_to_square("b3");

  std::vector<Move> computed_east_moves;
  board.append_pseudolegal_sliding_moves(Direction::east, Color::white, b3,
                                         Piece::queen, &computed_east_moves);
  EXPECT_TRUE(computed_east_moves.empty());

  std::vector<Move> computed_southwest_moves;
  board.append_pseudolegal_sliding_moves(Direction::southwest, Color::white, b3,
                                         Piece::queen,
                                         &computed_southwest_moves);
  EXPECT_TRUE(computed_southwest_moves.empty());

  std::vector<Move> computed_south_moves;
  board.append_pseudolegal_sliding_moves(Direction::south, Color::white, b3,
                                         Piece::queen, &computed_south_moves);
  EXPECT_TRUE(computed_south_moves.empty());

  std::vector<Move> correct_north_moves = {
      Move(b3, str_to_square("b4"), Piece::queen, MoveType::simple, board),
      Move(b3, str_to_square("b5"), Piece::queen, MoveType::simple, board),
      Move(b3, str_to_square("b6"), Piece::queen, MoveType::simple, board),
      Move(b3, str_to_square("b7"), Piece::queen, MoveType::capture, board)};
  std::vector<Move> computed_north_moves;
  board.append_pseudolegal_sliding_moves(Direction::north, Color::white, b3,
                                         Piece::queen, &computed_north_moves);
  EXPECT_EQ(computed_north_moves, correct_north_moves);

  std::vector<Move> correct_northwest_moves = {
      Move(b3, str_to_square("a4"), Piece::queen, MoveType::simple, board)};
  std::vector<Move> computed_northwest_moves;
  board.append_pseudolegal_sliding_moves(Direction::northwest, Color::white, b3,
                                         Piece::queen,
                                         &computed_northwest_moves);
  EXPECT_EQ(computed_northwest_moves, correct_northwest_moves);

  std::vector<Move> correct_northeast_moves = {
      Move(b3, str_to_square("c4"), Piece::queen, MoveType::simple, board),
      Move(b3, str_to_square("d5"), Piece::queen, MoveType::simple, board),
      Move(b3, str_to_square("e6"), Piece::queen, MoveType::simple, board),
      Move(b3, str_to_square("f7"), Piece::queen, MoveType::capture, board)};
  std::vector<Move> computed_northeast_moves;
  board.append_pseudolegal_sliding_moves(Direction::northeast, Color::white, b3,
                                         Piece::queen,
                                         &computed_northeast_moves);
  EXPECT_EQ(computed_northeast_moves, correct_northeast_moves);

  std::vector<Move> correct_west_moves = {
      Move(b3, str_to_square("a3"), Piece::queen, MoveType::simple, board)};
  std::vector<Move> computed_west_moves;
  board.append_pseudolegal_sliding_moves(Direction::west, Color::white, b3,
                                         Piece::queen, &computed_west_moves);
  EXPECT_EQ(computed_west_moves, correct_west_moves);

  std::vector<Move> correct_southeast_moves = {
      Move(b3, str_to_square("c2"), Piece::queen, MoveType::simple, board),
      Move(b3, str_to_square("d1"), Piece::queen, MoveType::simple, board),
  };
  std::vector<Move> computed_southeast_moves;
  board.append_pseudolegal_sliding_moves(Direction::southeast, Color::white, b3,
                                         Piece::queen,
                                         &computed_southeast_moves);
  EXPECT_EQ(computed_southeast_moves, correct_southeast_moves);
}

TEST(PseudoLegalMoves, A8Rook) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");
  const Bitboard a8 = str_to_square("a8");

  std::vector<Move> computed_north_moves;
  board.append_pseudolegal_sliding_moves(Direction::north, Color::black, a8,
                                         Piece::rook, &computed_north_moves);
  EXPECT_TRUE(computed_north_moves.empty());

  std::vector<Move> computed_west_moves;
  board.append_pseudolegal_sliding_moves(Direction::west, Color::black, a8,
                                         Piece::rook, &computed_west_moves);
  EXPECT_TRUE(computed_west_moves.empty());

  std::vector<Move> computed_south_moves;
  board.append_pseudolegal_sliding_moves(Direction::south, Color::black, a8,
                                         Piece::rook, &computed_south_moves);
  EXPECT_TRUE(computed_south_moves.empty());

  std::vector<Move> correct_east_moves = {
      Move(a8, str_to_square("b8"), Piece::rook, MoveType::simple, board),
      Move(a8, str_to_square("c8"), Piece::rook, MoveType::simple, board),
      Move(a8, str_to_square("d8"), Piece::rook, MoveType::simple, board),
      Move(a8, str_to_square("e8"), Piece::rook, MoveType::simple, board)};
  std::vector<Move> computed_east_moves;
  board.append_pseudolegal_sliding_moves(Direction::east, Color::black, a8,
                                         Piece::rook, &computed_east_moves);
  EXPECT_EQ(computed_east_moves, correct_east_moves);
}

TEST(PseudoLegalMoves, F8Rook) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");
  const Bitboard f8 = str_to_square("f8");

  std::vector<Move> computed_north_moves;
  board.append_pseudolegal_sliding_moves(Direction::north, Color::black, f8,
                                         Piece::rook, &computed_north_moves);
  EXPECT_TRUE(computed_north_moves.empty());

  std::vector<Move> computed_east_moves;
  board.append_pseudolegal_sliding_moves(Direction::east, Color::black, f8,
                                         Piece::rook, &computed_east_moves);
  EXPECT_TRUE(computed_east_moves.empty());

  std::vector<Move> computed_south_moves;
  board.append_pseudolegal_sliding_moves(Direction::south, Color::black, f8,
                                         Piece::rook, &computed_south_moves);
  EXPECT_TRUE(computed_south_moves.empty());

  std::vector<Move> correct_west_moves = {
      Move(f8, str_to_square("e8"), Piece::rook, MoveType::simple, board),
      Move(f8, str_to_square("d8"), Piece::rook, MoveType::simple, board),
      Move(f8, str_to_square("c8"), Piece::rook, MoveType::simple, board),
      Move(f8, str_to_square("b8"), Piece::rook, MoveType::simple, board)};
  std::vector<Move> computed_west_moves;
  board.append_pseudolegal_sliding_moves(Direction::west, Color::black, f8,
                                         Piece::rook, &computed_west_moves);
  EXPECT_EQ(computed_west_moves, correct_west_moves);
}

TEST(PseudoLegalMoves, G6Bishop) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");
  const Bitboard g6 = str_to_square("g6");

  std::vector<Move> computed_northwest_moves;
  board.append_pseudolegal_sliding_moves(Direction::northwest, Color::black, g6,
                                         Piece::bishop,
                                         &computed_northwest_moves);
  EXPECT_TRUE(computed_northwest_moves.empty());

  std::vector<Move> northeast_moves = {
      Move(g6, str_to_square("h7"), Piece::bishop, MoveType::simple, board)};
  std::vector<Move> computed_northeast_moves;
  board.append_pseudolegal_sliding_moves(Direction::northeast, Color::black, g6,
                                         Piece::bishop,
                                         &computed_northeast_moves);
  EXPECT_EQ(computed_northeast_moves, northeast_moves);

  std::vector<Move> southwest_moves = {
      Move(g6, str_to_square("f5"), Piece::bishop, MoveType::simple, board)};
  std::vector<Move> computed_southwest_moves;
  board.append_pseudolegal_sliding_moves(Direction::southwest, Color::black, g6,
                                         Piece::bishop,
                                         &computed_southwest_moves);
  EXPECT_EQ(computed_southwest_moves, southwest_moves);

  std::vector<Move> southeast_moves = {
      Move(g6, str_to_square("h5"), Piece::bishop, MoveType::simple, board)};
  std::vector<Move> computed_southeast_moves;
  board.append_pseudolegal_sliding_moves(Direction::southeast, Color::black, g6,
                                         Piece::bishop,
                                         &computed_southeast_moves);
  EXPECT_EQ(computed_southeast_moves, southeast_moves);
}

TEST(PseudoLegalMoves, H4Queen) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");
  const Bitboard h4 = str_to_square("h4");

  std::vector<Move> correct_north_moves = {
      Move(h4, str_to_square("h5"), Piece::queen, MoveType::simple, board)};
  std::vector<Move> computed_north_moves;
  board.append_pseudolegal_sliding_moves(Direction::north, Color::black, h4,
                                         Piece::queen, &computed_north_moves);
  EXPECT_EQ(computed_north_moves, correct_north_moves);

  std::vector<Move> correct_south_moves = {
      Move(h4, str_to_square("h3"), Piece::queen, MoveType::capture, board)};
  std::vector<Move> computed_south_moves;
  board.append_pseudolegal_sliding_moves(Direction::south, Color::black, h4,
                                         Piece::queen, &computed_south_moves);
  EXPECT_EQ(computed_south_moves, correct_south_moves);

  std::vector<Move> computed_west_moves;
  board.append_pseudolegal_sliding_moves(Direction::west, Color::black, h4,
                                         Piece::queen, &computed_west_moves);
  EXPECT_TRUE(computed_west_moves.empty());

  std::vector<Move> computed_east_moves;
  board.append_pseudolegal_sliding_moves(Direction::east, Color::black, h4,
                                         Piece::queen, &computed_east_moves);
  EXPECT_TRUE(computed_east_moves.empty());

  std::vector<Move> computed_northeast_moves;
  board.append_pseudolegal_sliding_moves(Direction::northeast, Color::black, h4,
                                         Piece::queen,
                                         &computed_northeast_moves);
  EXPECT_TRUE(computed_northeast_moves.empty());

  std::vector<Move> computed_southeast_moves;
  board.append_pseudolegal_sliding_moves(Direction::southeast, Color::black, h4,
                                         Piece::queen,
                                         &computed_southeast_moves);
  EXPECT_TRUE(computed_southeast_moves.empty());

  std::vector<Move> correct_northwest_moves = {
      Move(h4, str_to_square("g5"), Piece::queen, MoveType::simple, board),
      Move(h4, str_to_square("f6"), Piece::queen, MoveType::simple, board),
      Move(h4, str_to_square("e7"), Piece::queen, MoveType::simple, board),
      Move(h4, str_to_square("d8"), Piece::queen, MoveType::simple, board),
  };
  std::vector<Move> computed_northwest_moves;
  board.append_pseudolegal_sliding_moves(Direction::northwest, Color::black, h4,
                                         Piece::queen,
                                         &computed_northwest_moves);
  EXPECT_EQ(computed_northwest_moves, correct_northwest_moves);

  std::vector<Move> correct_southwest_moves = {
      Move(h4, str_to_square("g3"), Piece::queen, MoveType::simple, board),
      Move(h4, str_to_square("f2"), Piece::queen, MoveType::capture, board),
  };
  std::vector<Move> computed_southwest_moves;
  board.append_pseudolegal_sliding_moves(Direction::southwest, Color::black, h4,
                                         Piece::queen,
                                         &computed_southwest_moves);
  EXPECT_EQ(computed_southwest_moves, correct_southwest_moves);
}

TEST(PseudoLegalMoves, BishopMovesWhite) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 w - - 4 36");

  std::vector<Move> correct_moves = {
      Move(str_to_square("d2"), str_to_square("c1"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("d2"), str_to_square("e3"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("d2"), str_to_square("f4"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("d2"), str_to_square("g5"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("d2"), str_to_square("e1"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("d2"), str_to_square("h6"), Piece::bishop,
           MoveType::capture, board)

  };
  std::vector<Move> computed_moves;
  board.append_pseudolegal_bishop_moves(Color::white, &computed_moves);
  EXPECT_EQ(computed_moves.size(), correct_moves.size());
  EXPECT_TRUE(std::is_permutation(computed_moves.begin(), computed_moves.end(),
                                  correct_moves.begin()));
}

TEST(PseudoLegalMoves, BishopMovesBlack) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 b - - 4 36");

  std::vector<Move> correct_moves = {
      Move(str_to_square("f8"), str_to_square("e7"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("f8"), str_to_square("d6"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("f8"), str_to_square("c5"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("f8"), str_to_square("b4"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("f8"), str_to_square("a3"), Piece::bishop,
           MoveType::simple, board)};
  std::vector<Move> computed_moves;
  board.append_pseudolegal_bishop_moves(Color::black, &computed_moves);
  EXPECT_EQ(computed_moves.size(), correct_moves.size());
  EXPECT_TRUE(std::is_permutation(computed_moves.begin(), computed_moves.end(),
                                  correct_moves.begin()));
}

TEST(PseudoLegalMoves, RookMovesWhite) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 w - - 4 36");

  std::vector<Move> correct_moves = {
      Move(str_to_square("a1"), str_to_square("a2"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a1"), str_to_square("a3"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a1"), str_to_square("b1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a1"), str_to_square("c1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a1"), str_to_square("a4"), Piece::rook,
           MoveType::capture, board),
      Move(str_to_square("d1"), str_to_square("c1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d1"), str_to_square("b1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d1"), str_to_square("e1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d1"), str_to_square("f1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d1"), str_to_square("g1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d1"), str_to_square("h1"), Piece::rook,
           MoveType::simple, board)

  };
  std::vector<Move> computed_moves;
  board.append_pseudolegal_rook_moves(Color::white, &computed_moves);
  EXPECT_EQ(computed_moves.size(), correct_moves.size());
  EXPECT_TRUE(std::is_permutation(computed_moves.begin(), computed_moves.end(),
                                  correct_moves.begin()));
}

TEST(PseudoLegalMoves, RookMovesBlack) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 b - - 4 36");

  std::vector<Move> correct_moves = {
      Move(str_to_square("a8"), str_to_square("a7"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a8"), str_to_square("a6"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a8"), str_to_square("a5"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a8"), str_to_square("b8"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a8"), str_to_square("c8"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d8"), str_to_square("c8"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d8"), str_to_square("b8"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d8"), str_to_square("e8"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d8"), str_to_square("d7"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d8"), str_to_square("d6"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d8"), str_to_square("d5"), Piece::rook,
           MoveType::capture, board),
  };
  std::vector<Move> computed_moves;
  board.append_pseudolegal_rook_moves(Color::black, &computed_moves);
  EXPECT_EQ(computed_moves.size(), correct_moves.size());
  EXPECT_TRUE(std::is_permutation(computed_moves.begin(), computed_moves.end(),
                                  correct_moves.begin()));
}

TEST(PseudoLegalMoves, QueenMovesWhite) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 w - - 4 36");

  std::vector<Move> correct_moves = {
      Move(str_to_square("h4"), str_to_square("g3"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("f2"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("e1"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("g4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("f4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("e4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("d4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("c4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("b4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("a4"), Piece::queen,
           MoveType::capture, board),
      Move(str_to_square("h4"), str_to_square("g5"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("f6"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("e7"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("d8"), Piece::queen,
           MoveType::capture, board),
      Move(str_to_square("h4"), str_to_square("h5"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("h6"), Piece::queen,
           MoveType::capture, board)

  };
  std::vector<Move> computed_moves;
  board.append_pseudolegal_queen_moves(Color::white, &computed_moves);
  EXPECT_EQ(computed_moves.size(), correct_moves.size());
  EXPECT_TRUE(std::is_permutation(computed_moves.begin(), computed_moves.end(),
                                  correct_moves.begin()));
}

TEST(PseudoLegalMoves, QueenMovesBlack) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 b - - 4 36");

  std::vector<Move> correct_moves = {
      Move(str_to_square("b3"), str_to_square("a2"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("a3"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("b2"), Piece::queen,
           MoveType::capture, board),
      Move(str_to_square("b3"), str_to_square("c2"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("d1"), Piece::queen,
           MoveType::capture, board),
      Move(str_to_square("b3"), str_to_square("c3"), Piece::queen,
           MoveType::capture, board),
      Move(str_to_square("b3"), str_to_square("b4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("b5"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("b6"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("b7"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("b8"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("c4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("d5"), Piece::queen,
           MoveType::capture, board),
  };
  std::vector<Move> computed_moves;
  board.append_pseudolegal_queen_moves(Color::black, &computed_moves);
  EXPECT_EQ(computed_moves.size(), correct_moves.size());
  EXPECT_TRUE(std::is_permutation(computed_moves.begin(), computed_moves.end(),
                                  correct_moves.begin()));
}

TEST(PseudoLegalMoves, KingMovesWhite) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 w - - 4 36");

  std::vector<Move> correct_moves = {
      Move(str_to_square("h2"), str_to_square("h1"), Piece::king,
           MoveType::simple, board),
      Move(str_to_square("h2"), str_to_square("g1"), Piece::king,
           MoveType::simple, board),
      Move(str_to_square("h2"), str_to_square("g3"), Piece::king,
           MoveType::simple, board)};
  std::vector<Move> computed_moves;
  board.append_pseudolegal_king_moves(Color::white, &computed_moves);
  EXPECT_EQ(computed_moves.size(), correct_moves.size());
  EXPECT_TRUE(std::is_permutation(computed_moves.begin(), computed_moves.end(),
                                  correct_moves.begin()))
      << ::testing::PrintToString(computed_moves) << '\n'
      << ::testing::PrintToString(correct_moves);
}

TEST(PseudoLegalMoves, KingMovesBlack) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 b - - 4 36");

  std::vector<Move> correct_moves = {
      Move(str_to_square("h7"), str_to_square("h8"), Piece::king,
           MoveType::simple, board),
      Move(str_to_square("h7"), str_to_square("g6"), Piece::king,
           MoveType::simple, board),
  };
  std::vector<Move> computed_moves;
  board.append_pseudolegal_king_moves(Color::black, &computed_moves);
  EXPECT_EQ(computed_moves.size(), correct_moves.size());
  EXPECT_TRUE(std::is_permutation(computed_moves.begin(), computed_moves.end(),
                                  correct_moves.begin()));
}

TEST(PseudoLegalMoves, KnightMovesWhite) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 w - - 4 36");

  std::vector<Move> correct_moves = {
      Move(str_to_square("f5"), str_to_square("e7"), Piece::knight,
           MoveType::simple, board),
      Move(str_to_square("f5"), str_to_square("g7"), Piece::knight,
           MoveType::capture, board),
      Move(str_to_square("f5"), str_to_square("h6"), Piece::knight,
           MoveType::capture, board),
      Move(str_to_square("f5"), str_to_square("g3"), Piece::knight,
           MoveType::simple, board),
      Move(str_to_square("f5"), str_to_square("e3"), Piece::knight,
           MoveType::simple, board),
      Move(str_to_square("f5"), str_to_square("d4"), Piece::knight,
           MoveType::simple, board),
      Move(str_to_square("f5"), str_to_square("d6"), Piece::knight,
           MoveType::simple, board),
  };
  std::vector<Move> computed_moves;
  board.append_pseudolegal_knight_moves(Color::white, &computed_moves);
  EXPECT_EQ(computed_moves.size(), correct_moves.size());
  EXPECT_TRUE(std::is_permutation(computed_moves.begin(), computed_moves.end(),
                                  correct_moves.begin()))
      << ::testing::PrintToString(computed_moves) << '\n'
      << ::testing::PrintToString(correct_moves);
}

TEST(PseudoLegalMoves, KnightMovesBlack) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 b - - 4 36");

  std::vector<Move> correct_moves = {
      Move(str_to_square("g8"), str_to_square("e7"), Piece::knight,
           MoveType::simple, board),
      Move(str_to_square("g8"), str_to_square("f6"), Piece::knight,
           MoveType::simple, board),
  };
  std::vector<Move> computed_moves;
  board.append_pseudolegal_knight_moves(Color::black, &computed_moves);
  EXPECT_EQ(computed_moves.size(), correct_moves.size());
  EXPECT_TRUE(std::is_permutation(computed_moves.begin(), computed_moves.end(),
                                  correct_moves.begin()));
}

TEST(PseudoLegalMoves, White) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 w - - 4 36");

  std::vector<Move> correct_moves = {
      // Bishop moves.
      Move(str_to_square("d2"), str_to_square("c1"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("d2"), str_to_square("e3"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("d2"), str_to_square("f4"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("d2"), str_to_square("g5"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("d2"), str_to_square("e1"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("d2"), str_to_square("h6"), Piece::bishop,
           MoveType::capture, board),
      // Rook moves.
      Move(str_to_square("a1"), str_to_square("a2"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a1"), str_to_square("a3"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a1"), str_to_square("b1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a1"), str_to_square("c1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a1"), str_to_square("a4"), Piece::rook,
           MoveType::capture, board),
      Move(str_to_square("d1"), str_to_square("c1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d1"), str_to_square("b1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d1"), str_to_square("e1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d1"), str_to_square("f1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d1"), str_to_square("g1"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d1"), str_to_square("h1"), Piece::rook,
           MoveType::simple, board),
      // Queen moves.
      Move(str_to_square("h4"), str_to_square("g3"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("f2"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("e1"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("g4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("f4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("e4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("d4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("c4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("b4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("a4"), Piece::queen,
           MoveType::capture, board),
      Move(str_to_square("h4"), str_to_square("g5"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("f6"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("e7"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("d8"), Piece::queen,
           MoveType::capture, board),
      Move(str_to_square("h4"), str_to_square("h5"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("h4"), str_to_square("h6"), Piece::queen,
           MoveType::capture, board),
      // King moves.
      Move(str_to_square("h2"), str_to_square("h1"), Piece::king,
           MoveType::simple, board),
      Move(str_to_square("h2"), str_to_square("g1"), Piece::king,
           MoveType::simple, board),
      Move(str_to_square("h2"), str_to_square("g3"), Piece::king,
           MoveType::simple, board),
      // Knight moves.
      Move(str_to_square("f5"), str_to_square("e7"), Piece::knight,
           MoveType::simple, board),
      Move(str_to_square("f5"), str_to_square("g7"), Piece::knight,
           MoveType::capture, board),
      Move(str_to_square("f5"), str_to_square("h6"), Piece::knight,
           MoveType::capture, board),
      Move(str_to_square("f5"), str_to_square("g3"), Piece::knight,
           MoveType::simple, board),
      Move(str_to_square("f5"), str_to_square("e3"), Piece::knight,
           MoveType::simple, board),
      Move(str_to_square("f5"), str_to_square("d4"), Piece::knight,
           MoveType::simple, board),
      Move(str_to_square("f5"), str_to_square("d6"), Piece::knight,
           MoveType::simple, board),
      // Pawn moves.
      Move(str_to_square("c3"), str_to_square("c4"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("d5"), str_to_square("d6"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("f3"), str_to_square("f4"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("g2"), str_to_square("g3"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("g2"), str_to_square("g4"), Piece::pawn,
           MoveType::two_step_pawn, board)};
  auto computed_moves = board.pseudolegal_moves(Color::white);
  EXPECT_EQ(computed_moves.size(), correct_moves.size());
  EXPECT_TRUE(std::is_permutation(computed_moves.begin(), computed_moves.end(),
                                  correct_moves.begin()))
      << ::testing::PrintToString(computed_moves)
      << ::testing::PrintToString(correct_moves);
}

TEST(PseudoLegalMoves, Black) {
  Board board("r2r1bn1/5ppk/7p/3PpN2/p6Q/1qP2P1P/1P1B2PK/R2R4 b - - 4 36");

  std::vector<Move> correct_moves = {
      // Bishop moves.
      Move(str_to_square("f8"), str_to_square("e7"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("f8"), str_to_square("d6"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("f8"), str_to_square("c5"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("f8"), str_to_square("b4"), Piece::bishop,
           MoveType::simple, board),
      Move(str_to_square("f8"), str_to_square("a3"), Piece::bishop,
           MoveType::simple, board),
      // Rook moves.
      Move(str_to_square("a8"), str_to_square("a7"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a8"), str_to_square("a6"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a8"), str_to_square("a5"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a8"), str_to_square("b8"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("a8"), str_to_square("c8"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d8"), str_to_square("c8"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d8"), str_to_square("b8"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d8"), str_to_square("e8"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d8"), str_to_square("d7"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d8"), str_to_square("d6"), Piece::rook,
           MoveType::simple, board),
      Move(str_to_square("d8"), str_to_square("d5"), Piece::rook,
           MoveType::capture, board),
      // Queen moves.
      Move(str_to_square("b3"), str_to_square("a2"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("a3"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("b2"), Piece::queen,
           MoveType::capture, board),
      Move(str_to_square("b3"), str_to_square("c2"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("d1"), Piece::queen,
           MoveType::capture, board),
      Move(str_to_square("b3"), str_to_square("c3"), Piece::queen,
           MoveType::capture, board),
      Move(str_to_square("b3"), str_to_square("b4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("b5"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("b6"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("b7"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("b8"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("c4"), Piece::queen,
           MoveType::simple, board),
      Move(str_to_square("b3"), str_to_square("d5"), Piece::queen,
           MoveType::capture, board),
      // King moves.
      Move(str_to_square("h7"), str_to_square("h8"), Piece::king,
           MoveType::simple, board),
      Move(str_to_square("h7"), str_to_square("g6"), Piece::king,
           MoveType::simple, board),
      // Knight moves.
      Move(str_to_square("g8"), str_to_square("e7"), Piece::knight,
           MoveType::simple, board),
      Move(str_to_square("g8"), str_to_square("f6"), Piece::knight,
           MoveType::simple, board),
      // Pawn moves.
      Move(str_to_square("a4"), str_to_square("a3"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("e5"), str_to_square("e4"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("f7"), str_to_square("f6"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("g7"), str_to_square("g6"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("g7"), str_to_square("g5"), Piece::pawn,
           MoveType::two_step_pawn, board),
      Move(str_to_square("h6"), str_to_square("h5"), Piece::pawn,
           MoveType::simple, board)

  };
  auto computed_moves = board.pseudolegal_moves(Color::black);
  EXPECT_EQ(computed_moves.size(), correct_moves.size());
  EXPECT_TRUE(std::is_permutation(computed_moves.begin(), computed_moves.end(),
                                  correct_moves.begin()));
}

TEST(AttackMoves, Simple) {
  Board board("4k3/8/8/8/4P3/8/8/2K5 w - - 0 1");

  Bitboard bb = str_to_square("b1") | str_to_square("d1") |
                str_to_square("b2") | str_to_square("c2") |
                str_to_square("d2") | str_to_square("e5") |
                str_to_square("d5") | str_to_square("f5");
  EXPECT_EQ(board.attack_squares(Color::white), bb);
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

  std::vector<Move> moves = {Move(str_to_square("g2"), str_to_square("g3"),
                                  Piece::pawn, MoveType::simple, board),
                             Move(str_to_square("f2"), str_to_square("f3"),
                                  Piece::pawn, MoveType::simple, board),
                             Move(str_to_square("a2"), str_to_square("a3"),
                                  Piece::pawn, MoveType::simple, board),
                             Move(str_to_square("d4"), str_to_square("d5"),
                                  Piece::pawn, MoveType::simple, board)};
  std::vector<Move> test_moves;
  board.append_pseudolegal_simple_pawn_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, SimplePawnMovesBlack) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");

  std::vector<Move> moves = {Move(str_to_square("h6"), str_to_square("h5"),
                                  Piece::pawn, MoveType::simple, board),
                             Move(str_to_square("c6"), str_to_square("c5"),
                                  Piece::pawn, MoveType::simple, board),
                             Move(str_to_square("f7"), str_to_square("f6"),
                                  Piece::pawn, MoveType::simple, board),
                             Move(str_to_square("b7"), str_to_square("b6"),
                                  Piece::pawn, MoveType::simple, board),
                             Move(str_to_square("a7"), str_to_square("a6"),
                                  Piece::pawn, MoveType::simple, board)};
  std::vector<Move> test_moves;
  board.append_pseudolegal_simple_pawn_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, TwoStepPawnMovesWhite) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");

  std::vector<Move> moves = {Move(str_to_square("f2"), str_to_square("f4"),
                                  Piece::pawn, MoveType::two_step_pawn, board),
                             Move(str_to_square("a2"), str_to_square("a4"),
                                  Piece::pawn, MoveType::two_step_pawn, board)};
  std::vector<Move> test_moves;
  board.append_pseudolegal_two_step_pawn_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, TwoStepPawnMovesBlack) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");

  std::vector<Move> moves = {Move(str_to_square("f7"), str_to_square("f5"),
                                  Piece::pawn, MoveType::two_step_pawn, board),
                             Move(str_to_square("b7"), str_to_square("b5"),
                                  Piece::pawn, MoveType::two_step_pawn, board),
                             Move(str_to_square("a7"), str_to_square("a5"),
                                  Piece::pawn, MoveType::two_step_pawn, board)

  };
  std::vector<Move> test_moves;
  board.append_pseudolegal_two_step_pawn_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, NoEnPassantMovesWhite) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K w - - 1 18");

  std::vector<Move> test_moves;
  board.append_pseudolegal_en_passant_moves(Color::white, &test_moves);
  EXPECT_TRUE(test_moves.empty());
}

TEST(PseudoLegalMoves, OneEnPassantMoveWhite) {
  Board board("rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");

  std::vector<Move> moves = {Move(str_to_square("e5"), str_to_square("d6"),
                                  Piece::pawn, MoveType::en_passant, board)};
  std::vector<Move> test_moves;
  board.append_pseudolegal_en_passant_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, TwoEnPassantMovesWhite) {
  Board board("rnbqkbnr/1pp1pppp/8/2PpP3/p7/8/PP1P1PPP/RNBQKBNR w KQkq d6 0 5");

  std::vector<Move> moves = {Move(str_to_square("c5"), str_to_square("d6"),
                                  Piece::pawn, MoveType::en_passant, board),
                             Move(str_to_square("e5"), str_to_square("d6"),
                                  Piece::pawn, MoveType::en_passant, board)};
  std::vector<Move> test_moves;
  board.append_pseudolegal_en_passant_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, NoEnPassantMovesBlack) {
  Board board("r4rk1/pp3pp1/2p3bp/8/3Pp1nq/1QN1P2P/PP1N1PP1/R4R1K b - - 1 18");

  std::vector<Move> test_moves;
  board.append_pseudolegal_en_passant_moves(Color::black, &test_moves);
  EXPECT_TRUE(test_moves.empty());
}

TEST(PseudoLegalMoves, OneEnPassantMoveBlack) {
  Board board(
      "rnbqkbnr/ppp1p1pp/8/5p2/P2pP3/7P/1PPP1PP1/RNBQKBNR b KQkq e3 0 4");

  std::vector<Move> moves = {Move(str_to_square("d4"), str_to_square("e3"),
                                  Piece::pawn, MoveType::en_passant, board)};
  std::vector<Move> test_moves;
  board.append_pseudolegal_en_passant_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, TwoEnPassantMovesBlack) {
  Board board(
      "rnbqkbnr/ppp1p1pp/8/P7/3pPp2/7P/1PPP1PP1/RNBQKBNR b KQkq e3 0 5");

  std::vector<Move> moves = {Move(str_to_square("d4"), str_to_square("e3"),
                                  Piece::pawn, MoveType::en_passant, board),
                             Move(str_to_square("f4"), str_to_square("e3"),
                                  Piece::pawn, MoveType::en_passant, board)

  };
  std::vector<Move> test_moves;
  board.append_pseudolegal_en_passant_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, PromotionsWhite) {
  Board board("1Q1r1Q1q/PPP1PPPP/3P4/8/8/8/8/k2K4 w - - 0 1");

  std::vector<Move> moves = {
      Move(str_to_square("a7"), str_to_square("a8"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("a7"), str_to_square("a8"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("a7"), str_to_square("a8"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("a7"), str_to_square("a8"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("c7"), str_to_square("c8"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("c7"), str_to_square("c8"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("c7"), str_to_square("c8"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("c7"), str_to_square("c8"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("c7"), str_to_square("d8"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("c7"), str_to_square("d8"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("c7"), str_to_square("d8"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("c7"), str_to_square("d8"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("e7"), str_to_square("e8"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("e7"), str_to_square("e8"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("e7"), str_to_square("e8"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("e7"), str_to_square("e8"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("e7"), str_to_square("d8"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("e7"), str_to_square("d8"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("e7"), str_to_square("d8"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("e7"), str_to_square("d8"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("g7"), str_to_square("g8"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("g7"), str_to_square("g8"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("g7"), str_to_square("g8"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("g7"), str_to_square("g8"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("g7"), str_to_square("h8"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("g7"), str_to_square("h8"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("g7"), str_to_square("h8"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("g7"), str_to_square("h8"), Piece::pawn,
           MoveType::promotion_to_queen, board),
  };
  std::vector<Move> test_moves;
  board.append_pseudolegal_promotions(Color::white, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, PromotionsBlack) {
  Board board("1K1k4/8/8/8/8/3p4/ppp1pppp/1q1R1q1Q b - - 0 1");

  std::vector<Move> moves = {
      Move(str_to_square("a2"), str_to_square("a1"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("a2"), str_to_square("a1"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("a2"), str_to_square("a1"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("a2"), str_to_square("a1"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("c2"), str_to_square("c1"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("c2"), str_to_square("c1"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("c2"), str_to_square("c1"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("c2"), str_to_square("c1"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("c2"), str_to_square("d1"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("c2"), str_to_square("d1"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("c2"), str_to_square("d1"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("c2"), str_to_square("d1"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("e2"), str_to_square("e1"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("e2"), str_to_square("e1"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("e2"), str_to_square("e1"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("e2"), str_to_square("e1"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("e2"), str_to_square("d1"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("e2"), str_to_square("d1"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("e2"), str_to_square("d1"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("e2"), str_to_square("d1"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("g2"), str_to_square("g1"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("g2"), str_to_square("g1"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("g2"), str_to_square("g1"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("g2"), str_to_square("g1"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("g2"), str_to_square("h1"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("g2"), str_to_square("h1"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("g2"), str_to_square("h1"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("g2"), str_to_square("h1"), Piece::pawn,
           MoveType::promotion_to_queen, board),
  };
  std::vector<Move> test_moves;
  board.append_pseudolegal_promotions(Color::black, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, PawnCapturesWhite) {
  Board board(
      "rn1qk2r/Pp2pp1p/2p5/1p1pbbpn/2PPPPPN/8/1P5P/RNBQK2R w KQkq - 1 13");

  std::vector<Move> moves = {Move(str_to_square("g4"), str_to_square("h5"),
                                  Piece::pawn, MoveType::capture, board),
                             Move(str_to_square("g4"), str_to_square("f5"),
                                  Piece::pawn, MoveType::capture, board),
                             Move(str_to_square("f4"), str_to_square("g5"),
                                  Piece::pawn, MoveType::capture, board),
                             Move(str_to_square("f4"), str_to_square("e5"),
                                  Piece::pawn, MoveType::capture, board),
                             Move(str_to_square("e4"), str_to_square("f5"),
                                  Piece::pawn, MoveType::capture, board),
                             Move(str_to_square("e4"), str_to_square("d5"),
                                  Piece::pawn, MoveType::capture, board),
                             Move(str_to_square("d4"), str_to_square("e5"),
                                  Piece::pawn, MoveType::capture, board),
                             Move(str_to_square("c4"), str_to_square("d5"),
                                  Piece::pawn, MoveType::capture, board),
                             Move(str_to_square("c4"), str_to_square("b5"),
                                  Piece::pawn, MoveType::capture, board)};
  std::vector<Move> test_moves;
  board.append_pseudolegal_pawn_captures(Color::white, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, PawnCapturesBlack) {
  Board board(
      "rn1qk2r/Pp2pp1p/2p5/1p1pbbpn/2PPPPPN/8/1P5P/RNBQK2R b KQkq - 1 13");

  std::vector<Move> moves = {
      Move(str_to_square("g5"), str_to_square("h4"), Piece::pawn,
           MoveType::capture, board),
      Move(str_to_square("g5"), str_to_square("f4"), Piece::pawn,
           MoveType::capture, board),
      Move(str_to_square("d5"), str_to_square("e4"), Piece::pawn,
           MoveType::capture, board),
      Move(str_to_square("d5"), str_to_square("c4"), Piece::pawn,
           MoveType::capture, board),
      Move(str_to_square("b5"), str_to_square("c4"), Piece::pawn,
           MoveType::capture, board),
  };
  std::vector<Move> test_moves;
  board.append_pseudolegal_pawn_captures(Color::black, &test_moves);
  EXPECT_EQ(test_moves, moves);
}

TEST(PseudoLegalMoves, PawnsWhite) {
  Board board(
      "1n1qkbnr/1p2pppp/r7/1Ppp3P/3PP3/2N2N2/pPP2PP1/1RBQKB1R w Kk - 6 12");

  std::vector<Move> moves = {
      Move(str_to_square("g2"), str_to_square("g3"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("g2"), str_to_square("g4"), Piece::pawn,
           MoveType::two_step_pawn, board),
      Move(str_to_square("b2"), str_to_square("b3"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("b2"), str_to_square("b4"), Piece::pawn,
           MoveType::two_step_pawn, board),
      Move(str_to_square("e4"), str_to_square("e5"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("e4"), str_to_square("d5"), Piece::pawn,
           MoveType::capture, board),
      Move(str_to_square("d4"), str_to_square("c5"), Piece::pawn,
           MoveType::capture, board),
      Move(str_to_square("h5"), str_to_square("h6"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("b5"), str_to_square("b6"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("b5"), str_to_square("a6"), Piece::pawn,
           MoveType::capture, board),
  };
  std::vector<Move> test_moves;
  board.append_pseudolegal_pawn_moves(Color::white, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(PseudoLegalMoves, PawnsBlack) {
  Board board(
      "1n1qkbnr/1p2pppp/r7/1Ppp3P/3PP3/2N2N2/pPP2PP1/1RBQKB1R b Kk - 6 12");

  std::vector<Move> moves = {
      Move(str_to_square("a2"), str_to_square("a1"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("a2"), str_to_square("a1"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("a2"), str_to_square("a1"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("a2"), str_to_square("a1"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("a2"), str_to_square("b1"), Piece::pawn,
           MoveType::promotion_to_rook, board),
      Move(str_to_square("a2"), str_to_square("b1"), Piece::pawn,
           MoveType::promotion_to_knight, board),
      Move(str_to_square("a2"), str_to_square("b1"), Piece::pawn,
           MoveType::promotion_to_bishop, board),
      Move(str_to_square("a2"), str_to_square("b1"), Piece::pawn,
           MoveType::promotion_to_queen, board),
      Move(str_to_square("d5"), str_to_square("e4"), Piece::pawn,
           MoveType::capture, board),
      Move(str_to_square("c5"), str_to_square("d4"), Piece::pawn,
           MoveType::capture, board),
      Move(str_to_square("c5"), str_to_square("c4"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("h7"), str_to_square("h6"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("g7"), str_to_square("g6"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("g7"), str_to_square("g5"), Piece::pawn,
           MoveType::two_step_pawn, board),
      Move(str_to_square("f7"), str_to_square("f6"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("f7"), str_to_square("f5"), Piece::pawn,
           MoveType::two_step_pawn, board),
      Move(str_to_square("e7"), str_to_square("e6"), Piece::pawn,
           MoveType::simple, board),
      Move(str_to_square("e7"), str_to_square("e5"), Piece::pawn,
           MoveType::two_step_pawn, board),
      Move(str_to_square("b7"), str_to_square("b6"), Piece::pawn,
           MoveType::simple, board),
  };
  std::vector<Move> test_moves;
  board.append_pseudolegal_pawn_moves(Color::black, &test_moves);
  EXPECT_EQ(test_moves.size(), moves.size());
  EXPECT_TRUE(
      std::is_permutation(test_moves.begin(), test_moves.end(), moves.begin()));
}

TEST(IsKingAttacked, Simple) {
  Board board_1 =
      Board("rnb1kbnr/pppp1ppp/4p3/8/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 0 1");
  EXPECT_TRUE(board_1.is_king_attacked(Color::white));
  EXPECT_FALSE(board_1.is_king_attacked(Color::black));
  Board board_2 = Board("8/6bp/p1k1p3/8/N2Pp3/2P1P3/6R1/3r3K w - - 1 36");
  EXPECT_TRUE(board_2.is_king_attacked(Color::white));
  EXPECT_FALSE(board_2.is_king_attacked(Color::black));
  Board board_3 =
      Board("rn2k2r/1pN1bppp/4pn2/3p3q/1P6/P1B1P3/2P2PPP/R2Q1RK1 b kq - 3 15");
  EXPECT_FALSE(board_3.is_king_attacked(Color::white));
  EXPECT_TRUE(board_3.is_king_attacked(Color::black));
  Board board_4(
      "1n1qkbnr/1p2pppp/r7/1Ppp3P/3PP3/2N2N2/pPP2PP1/1RBQKB1R b Kk - 6 12");
  EXPECT_FALSE(board_4.is_king_attacked(Color::white));
  EXPECT_FALSE(board_4.is_king_attacked(Color::black));
}

TEST(CanCastle, White) {
  Board board_1 = Board(
      "rnbqkb1r/1pp2ppp/3pp3/p1n3B1/5PP1/3P1N1B/PPPQP2P/RN2K2R w KQkq - 1 8");
  EXPECT_TRUE(board_1.is_castle_kingside_legal());
  EXPECT_FALSE(board_1.is_castle_queenside_legal());
  Board board_2 = Board(
      "rnbqkb1r/1pp2ppp/3pp3/p5B1/5PP1/2NP1NnB/PPPQP2P/R3K2R w KQkq - 4 10");
  EXPECT_FALSE(board_2.is_castle_kingside_legal());
  EXPECT_TRUE(board_2.is_castle_queenside_legal());
  Board board_3 = Board(
      "rnbqkb1r/1pp2ppp/3pp3/p5B1/P4PP1/2NP1N1B/1PPQPn1P/R3K2R w KQkq - 1 12");
  EXPECT_TRUE(board_3.is_castle_kingside_legal());
  EXPECT_FALSE(board_3.is_castle_queenside_legal());
  Board board_4(
      "rnbqkb1r/1pp2ppp/1P1pp3/p5B1/P4PP1/n2P1N1B/2PQP2P/R3K2R w KQkq - 1 16");
  EXPECT_TRUE(board_4.is_castle_kingside_legal());
  EXPECT_TRUE(board_4.is_castle_queenside_legal());
}

TEST(CanCastle, Black) {
  Board board_1 = Board(
      "rnbqk2r/1pp2ppp/4pb2/pQ1p2B1/P2PNPP1/5N1B/1PP1Pn1P/R3K2R b KQkq - 1 15");
  EXPECT_FALSE(board_1.is_castle_kingside_legal());
  EXPECT_FALSE(board_1.is_castle_queenside_legal());
  Board board_2 = Board(
      "r3k2r/1Qpbqppp/2n1pb2/p2p2B1/P2P1PP1/2N2N1B/1PP1Pn1P/R3K2R b KQkq - 3 "
      "18");
  EXPECT_TRUE(board_2.is_castle_kingside_legal());
  EXPECT_FALSE(board_2.is_castle_queenside_legal());
  Board board_3 = Board(
      "r3k2r/Q1pbqppp/2n1pb2/p2p2B1/P2P1PP1/2N2N1B/1PP1Pn1P/R3K2R b KQkq - 4 "
      "18");
  EXPECT_TRUE(board_3.is_castle_kingside_legal());
  EXPECT_TRUE(board_3.is_castle_queenside_legal());
  Board board_4(
      "r3k2r/Q1pbqppp/2n1pb2/p2p2B1/P2P1PP1/2N2N1B/1PP1Pn1P/R3K2R b KQkq - 4 "
      "18");
  EXPECT_TRUE(board_4.is_castle_kingside_legal());
  EXPECT_TRUE(board_4.is_castle_queenside_legal());
}

TEST(CanCastle, Perft) {
  // This position exposed a bug in how attack squares were calculated. It was
  // found from a perft test.
  Board board = Board(
      "r3k2r/p1ppPpb1/bn1q1np1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 2");
  EXPECT_FALSE(board.is_castle_kingside_legal());
  EXPECT_FALSE(board.is_castle_queenside_legal());
}

TEST(Perft, StartPosition) {
  Board board = Board();
  ASSERT_EQ(number_of_moves(board, 0), 1);
  ASSERT_EQ(number_of_moves(board, 1), 20);
  ASSERT_EQ(number_of_moves(board, 2), 400);
  // ASSERT_EQ(number_of_moves(board, 3), 8902);
  // ASSERT_EQ(number_of_moves(board, 4), 197281);
}

TEST(Perft, Kiwipete) {
  Board board = Board(
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
  ASSERT_EQ(number_of_moves(board, 0), 1);
  ASSERT_EQ(number_of_moves(board, 1), 48);
  ASSERT_EQ(number_of_moves(board, 2), 2039);
  // ASSERT_EQ(number_of_moves(board, 3), 97862);
  // ASSERT_EQ(number_of_moves(board, 4), 4085603);
}

TEST(Perft, Pos3) {
  Board board = Board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
  ASSERT_EQ(number_of_moves(board, 0), 1);
  ASSERT_EQ(number_of_moves(board, 1), 14);
  ASSERT_EQ(number_of_moves(board, 2), 191);
  // ASSERT_EQ(number_of_moves(board, 3), 2812);
  // ASSERT_EQ(number_of_moves(board, 4), 43238);
}

TEST(Perft, Pos4) {
  Board board =
      Board("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
  ASSERT_EQ(number_of_moves(board, 0), 1);
  ASSERT_EQ(number_of_moves(board, 1), 6);
  ASSERT_EQ(number_of_moves(board, 2), 264);
  // ASSERT_EQ(number_of_moves(board, 3), 9467);
  // ASSERT_EQ(number_of_moves(board, 4), 422333);
}

TEST(Perft, Pos5) {
  Board board =
      Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  ASSERT_EQ(number_of_moves(board, 0), 1);
  ASSERT_EQ(number_of_moves(board, 1), 44);
  ASSERT_EQ(number_of_moves(board, 2), 1486);
  // ASSERT_EQ(number_of_moves(board, 3), 62379);
  // ASSERT_EQ(number_of_moves(board, 4), 2103487);
}

TEST(Perft, Pos6) {
  Board board = Board(
      "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 "
      "10");
  ASSERT_EQ(number_of_moves(board, 0), 1);
  ASSERT_EQ(number_of_moves(board, 1), 46);
  ASSERT_EQ(number_of_moves(board, 2), 2079);
  // ASSERT_EQ(number_of_moves(board, 3), 89890);
  // ASSERT_EQ(number_of_moves(board, 4), 3894594);
}