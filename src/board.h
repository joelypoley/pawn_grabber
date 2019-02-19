#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <optional>
#include <string>

// Each bit of a Bitboard represents a square. The a8 square is the most
// significant bit and the h1 square is the least significant bit. So the
// unsigned 64 bit integer 10000000 00000000 00000000 00000000 00000000 00000000
// 00000000 00000100 represents the board:

// 8 | 1000 0000
// 7 | 0000 0000
// 6 | 0000 0000
// 5 | 0000 0000
// 4 | 0000 0000
// 3 | 0000 0000
// 2 | 0000 0000
// 1 | 0000 0100
//    -----------
//     abcd efgh
typedef uint64_t Bitboard;

enum class Piece {
  white_pawn,
  white_rook,
  white_knight,
  white_bishop,
  white_queen,
  white_king,
  black_pawn,
  black_rook,
  black_knight,
  black_bishop,
  black_queen,
  black_king,
};

struct Move {
  Bitboard src_square;
  Bitboard dst_square;
  std::optional<Piece> promotion_piece;
  Move() : src_square(0), dst_square(0), promotion_piece(std::nullopt) {}
  Move(Bitboard p_src_square, Bitboard p_dst_square)
      : src_square(p_src_square),
        dst_square(p_dst_square),
        promotion_piece(std::nullopt) {}
  Move(Bitboard p_src_square, Bitboard p_dst_square,
       std::optional<Piece> p_promotion_piece)
      : src_square(p_src_square),
        dst_square(p_dst_square),
        promotion_piece(p_promotion_piece) {}
};

bool operator==(const Move& lhs, const Move& rhs) {
  return lhs.src_square == rhs.src_square && lhs.dst_square == rhs.dst_square &&
         lhs.promotion_piece == rhs.promotion_piece;
}

bool is_square(Bitboard);
int square_idx(Bitboard square);
int rank_idx(Bitboard square);
int file_idx(Bitboard square);
bool on_a_file(Bitboard square);
bool on_h_file(Bitboard square);
bool on_first_rank(Bitboard square);
bool on_eigth_rank(Bitboard square);
Bitboard north(Bitboard square);
Bitboard south(Bitboard square);
Bitboard east(Bitboard square);
Bitboard west(Bitboard square);
Bitboard northeast(Bitboard square);
Bitboard northwest(Bitboard square);
Bitboard southeast(Bitboard square);
Bitboard southwest(Bitboard square);

enum class Color { white, black };

// The Board class stores the current board state. Each bitboard tracks all
// places on the board there is a (piece, color) pair.
//
// For example, the Bitboards of the following position
//
//   ┌───┬───┬───┬───┬───┬───┬───┬───┐
// 8 │   │   │   │   │ ♚ │   │   │   │
//   ├───┼───┼───┼───┼───┼───┼───┼───┤
// 7 │   │   │   │   │   │   │   │   │
//   ├───┼───┼───┼───┼───┼───┼───┼───┤
// 6 │   │   │   │   │   │   │   │   │
//   ├───┼───┼───┼───┼───┼───┼───┼───┤
// 5 │   │   │   │   │   │   │   │   │
//   ├───┼───┼───┼───┼───┼───┼───┼───┤
// 4 │   │   │   │   │   │   │   │   │
//   ├───┼───┼───┼───┼───┼───┼───┼───┤
// 3 │   │   │   │   │   │   │   │   │
//   ├───┼───┼───┼───┼───┼───┼───┼───┤
// 2 │   │   │   │   │   │   │   │   │
//   ├───┼───┼───┼───┼───┼───┼───┼───┤
// 1 │   │   │   │   │ ♔ │   │   │   │
//   └───┴───┴───┴───┴───┴───┴───┴───┘
//     a   b   c   d   e   f   g   h
//
// are:
//
// white_pawns_ == 0x0
// white_rooks_ == 0x0
// white_knights_ == 0x0
// white_bishops_ == 0x0
// white_queens_ == 0x0
// white_king_ == 0x8
// black_pawns_ == 0x0
// black_rooks_ == 0x0
// black_knights_ == 0x0
// black_bishops_ == 0x0
// black_queens_ == 0x0
// black_king_ == 0x800000000000000.
//
// The Board class also tracks castling rights, the side to move, the move
// number and the number of moves until the fifty move rule applies.
class Board {
 public:
  Board();
  Board(std::string_view fen);
  std::string to_pretty_str();
  std::vector<Move> legal_moves();
  Bitboard white_pawns_;
  Bitboard white_rooks_;
  Bitboard white_knights_;
  Bitboard white_bishops_;
  Bitboard white_queens_;
  Bitboard white_king_;
  Bitboard black_pawns_;
  Bitboard black_rooks_;
  Bitboard black_knights_;
  Bitboard black_bishops_;
  Bitboard black_queens_;
  Bitboard black_king_;
  Color side_to_move_;
  std::optional<Bitboard> en_passant_square_;
  bool white_has_right_to_castle_kingside_;
  bool white_has_right_to_castle_queenside_;
  bool black_has_right_to_castle_kingside_;
  bool black_has_right_to_castle_queenside_;
  int fifty_move_clock_;
  int num_moves_;
  Bitboard white_pieces();
  Bitboard black_pieces();
  // The pseudo prefix refers to the fact that these functions generate
  // pesudolegal moves. We must also check if the king is in check before
  // generating all legal moves.
  std::vector<Move> pseudolegal_moves_in_direction(
      std::function<Bitboard(Bitboard)> direction_fn, Bitboard src_square);

 private:
  void zero_all_bitboards();
  void init_bitboards(const std::string_view pieces_fen);
  void init_side_to_move(const std::string_view pieces_fen);
  void init_color(const std::string_view side_to_move_fen);
  void init_castling_rights(const std::string_view castling_rights_fen);
  void init_en_passant(const std::string_view algebraic_square);
  void init_fifty_move_clock(const std::string_view num_half_moves_fen);
  void init_num_moves(const std::string_view num_moves_fen);
  std::string square_to_unicode(Bitboard square);

  std::vector<Move> pseudo_pawn_moves_excluding_promo();
};

Bitboard str_to_square(const std::string_view alegbraic_square);
std::string square_to_str(Bitboard sq);
Bitboard coordinates_to_square(int file, int rank);

#endif
