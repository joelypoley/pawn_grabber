#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <cstdint>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

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

enum class Color { white, black };
enum class Piece { pawn, rook, knight, bishop, queen, king };

enum class MoveType {
  simple,
  en_passant,
  castle_kingside,
  castle_queenside,
  capture,
  two_step_pawn,
  promotion_to_rook,
  promotion_to_bishop,
  promotion_to_knight,
  promotion_to_queen
};

enum class Direction {
  north,
  south,
  east,
  west,
  northeast,
  northwest,
  southeast,
  southwest
};

const int board_size = 8;
// `lsb_bitboard is convenient for shift operations. The literal 1 has type int,
// which has width 32 on x86, so `Bitboard bb = (1 << 50)` is undefined
// behavior.
constexpr Bitboard lsb_bitboard = 1;

constexpr std::array<MoveType, 4> promotion_move_types = {
    MoveType::promotion_to_rook, MoveType::promotion_to_bishop,
    MoveType::promotion_to_knight, MoveType::promotion_to_queen};

constexpr std::array<Direction, 8> all_directions = {
    Direction::north,     Direction::south,     Direction::east,
    Direction::west,      Direction::northeast, Direction::northwest,
    Direction::southeast, Direction::southwest};

constexpr std::array<std::pair<Direction, Direction>, 8>
    knight_move_directions = {
        std::make_pair(Direction::north, Direction::northwest),
        std::make_pair(Direction::north, Direction::northeast),
        std::make_pair(Direction::west, Direction::northwest),
        std::make_pair(Direction::west, Direction::southwest),
        std::make_pair(Direction::east, Direction::northeast),
        std::make_pair(Direction::east, Direction::southeast),
        std::make_pair(Direction::south, Direction::southeast),
        std::make_pair(Direction::south, Direction::southwest)};

constexpr std::array<Direction, 4> bishop_move_directions = {
    Direction::northeast, Direction::northwest, Direction::southeast,
    Direction::southwest};

constexpr std::array<Direction, 4> rook_move_directions = {
    Direction::north, Direction::south, Direction::east, Direction::west};

struct Move;

// The Board struct stores the current board state. Each bitboard tracks all
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
// The Board struct also tracks castling rights, the side to move, the move
// number and the number of moves until the fifty move rule applies.
struct Board {
 public:
  // The default initializer intializes board to the starting position.
  Board();
  explicit Board(absl::string_view fen);
  Board(const Board& other) = default;

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
  bool is_whites_move_;
  absl::optional<Bitboard> en_passant_square_;
  bool white_has_right_to_castle_kingside_;
  bool white_has_right_to_castle_queenside_;
  bool black_has_right_to_castle_kingside_;
  bool black_has_right_to_castle_queenside_;
  int fifty_move_clock_;
  int num_moves_;

  // Returns an array of all bitboards.
  std::array<Bitboard*, 12> all_bitboards();

  // Prints the board using unicode chess and line drawing symbols.
  std::string to_pretty_str() const;
  // Returns a unicode symbol for the piece on a given file and rank. Returns a
  // space if there is no piece on that square.
  std::string occupiers_unicode_symbol(int file, int rank) const;
  // Pretty string for google test.
  friend void PrintTo(const Board& board, std::ostream* os);

  // Mask methods.
  //
  // Returns mask of all pieces with color `side`.
  Bitboard friends(Color side) const;
  // Retuns a mask of all pieces that are the opposite color of `side`.
  Bitboard enemies(Color side) const;
  // Returns a mask of all white pieces.
  Bitboard white_pieces() const;
  // Returns a mask of all black pieces.
  Bitboard black_pieces() const;
  // Returns a mask of all pieces.
  Bitboard all_pieces() const;
  // Returns a mask of all squares attacked by pawns of color `side`.
  Bitboard pawn_attack_squares(Color side) const;
  // Returns a mask of all squares attacked by `side`.
  Bitboard attack_squares(Color side) const;

  // Move generation methods.
  //
  // Pseudolegal moves are moves generated based on how the pieces move, without
  // checking if the king is in check.
  void append_pseudolegal_sliding_moves(Direction direction, Color side,
                                        Bitboard src_square, Piece piece_moving,
                                        std::vector<Move>* res) const;
  void append_pseudolegal_bishop_moves(Color side,
                                       std::vector<Move>* res) const;
  void append_pseudolegal_rook_moves(Color side, std::vector<Move>* res) const;
  void append_pseudolegal_queen_moves(Color side, std::vector<Move>* res) const;
  // "Simple" in this context means no two-step moves, no promotions, no en
  // passant and no captures.
  void append_pseudolegal_simple_pawn_moves(Color side,
                                            std::vector<Move>* res) const;
  void append_pseudolegal_two_step_pawn_moves(Color side,
                                              std::vector<Move>* res) const;
  void append_pseudolegal_en_passant_moves(Color side,
                                           std::vector<Move>* res) const;
  void append_pseudolegal_promotions(Color side, std::vector<Move>* res) const;
  void append_pseudolegal_pawn_captures(Color side,
                                        std::vector<Move>* res) const;
  void append_pseudolegal_pawn_moves(Color side, std::vector<Move>* res) const;
  void append_pseudolegal_king_moves(Color side, std::vector<Move>* res) const;
  void append_pseudolegal_knight_moves(Color side,
                                       std::vector<Move>* res) const;
  std::vector<Move> pseudolegal_moves(Color side) const;
  // Castling is not counted as a pseudolegal move. The castling_moves() method
  // uses is_castle_*_legal() methods to check if castling is legal.
  bool is_castle_kingside_legal() const;
  bool is_castle_queenside_legal() const;
  void append_castling_moves(std::vector<Move>* res) const;
  bool is_king_attacked(Color side) const;
  bool is_pseudolegal_move_legal(Move move) const;
  std::vector<Move> legal_moves() const;
  bool is_capture_move(const Move& move) const;
  std::vector<Move> legal_capture_moves() const;

  // Methods for performing moves.
  //
  // adjust_castling_rights() is called after the move is performed.
  void adjust_castling_rights(Move move);
  void remove_piece_on(Bitboard sq);
  void move_piece(Bitboard from, Bitboard to);
  void do_en_passant_move(Move move);
  void do_castle_move(Move move);
  void do_promotion_move(Move move);
  void do_capture_move(Move move);
  void do_two_step_pawn_move(Move move);
  void do_simple_move(Move move);
  void undo_move(Move move);
  void do_move(Move move);

  int static_evaluation() const;

  // Initialization helper methods.
  void zero_all_bitboards();
  void init_bitboards(absl::string_view pieces_fen);
  void init_is_whites_move(absl::string_view pieces_fen);
  void init_castling_rights(absl::string_view castling_rights_fen);
  void init_en_passant(absl::string_view algebraic_square);
};

bool operator==(const Board& lhs, const Board& rhs);

struct Move {
  Bitboard src_square_;
  Bitboard dst_square_;
  Piece piece_moving_;
  MoveType move_type_;
  Board board_state_;
  // TODO: Remove this default initializer?
  Move();
  Move(Bitboard p_src_square, Bitboard p_dst_square, Piece p_piece_moving,
       MoveType p_move_type, Board p_board_state);
  std::string to_pretty_str() const;
  friend void PrintTo(const Move& move, std::ostream* os);
};

bool operator==(const Move& lhs, const Move& rhs);

int evaluation(Board board, int depth, std::vector<Move>* pv);
int alpha_beta(Board board, int depth, int alpha, int beta,
               std::vector<Move>* pv);
int quiescence_search(Board board, int alpha, int beta, std::vector<Move>* pv);

int pop_count(Bitboard bb);
bool is_square(Bitboard);
int square_idx(Bitboard square);
int rank_idx(Bitboard square);
int file_idx(Bitboard square);
bool on_a_file(Bitboard square);
bool on_h_file(Bitboard square);

Bitboard north_of(Bitboard square);
Bitboard south_of(Bitboard square);
Bitboard east_of(Bitboard square);
Bitboard west_of(Bitboard square);
Bitboard northeast_of(Bitboard square);
Bitboard northwest_of(Bitboard square);
Bitboard southeast_of(Bitboard square);
Bitboard southwest_of(Bitboard square);
std::function<Bitboard(Bitboard)> direction_to_function(Direction direction);

Bitboard str_to_square(absl::string_view alegbraic_square);
std::string square_to_str(Bitboard sq);
Bitboard coordinates_to_square(int file, int rank);
std::vector<Bitboard> bitboard_split(Bitboard bb);
Color flip_color(Color color);
std::string bb_to_pretty_str(Bitboard bb);

Move castle_kingside_move(Color color, Board board);
Move castle_queenside_move(Color color, Board board);

int64_t number_of_moves(Board board, int half_move_depth);

#endif
