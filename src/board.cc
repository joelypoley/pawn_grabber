#include "src/board.h"

#include <cassert>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "absl/algorithm/container.h"
#include "absl/base/internal/raw_logging.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"

namespace {
const std::string& get_start_fen() {
  const static std::string& start_fen = *new std::string(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  return start_fen;
}

const std::unordered_map<Piece, std::pair<char, std::string>>
get_piece_to_char_and_unicode() {
  const static auto& piece_to_char_and_unicode =
      *new std::unordered_map<Piece, std::pair<char, std::string>>{
          {Piece::white_pawn, {'P', "♙"}},    // U+2564
          {Piece::white_rook, {'R', "♖"}},    // U+2656
          {Piece::white_knight, {'N', "♘"}},  // U+2658
          {Piece::white_bishop, {'B', "♗"}},  // U+2657
          {Piece::white_queen, {'Q', "♕"}},   // U+2655
          {Piece::white_king, {'K', "♔"}},    // U+2654
          {Piece::black_pawn, {'p', "♟"}},    // U+265F
          {Piece::black_rook, {'r', "♜"}},    // U+265C
          {Piece::black_knight, {'n', "♞"}},  // U+265E
          {Piece::black_bishop, {'b', "♝"}},  // U+265D
          {Piece::black_queen, {'q', "♛"}},   // U+265B
          {Piece::black_king, {'k', "♚"}},    // U+265A
      };
  return piece_to_char_and_unicode;
}

const int board_size = 8;
const Bitboard lsb_bitboard = 1;
const Bitboard seventh_rank_mask = str_to_square("a7") | str_to_square("b7") |
                                   str_to_square("c7") | str_to_square("d7") |
                                   str_to_square("e7") | str_to_square("f7") |
                                   str_to_square("g7") | str_to_square("h7");
const Bitboard second_rank_mask = str_to_square("a2") | str_to_square("b2") |
                                  str_to_square("c2") | str_to_square("d2") |
                                  str_to_square("e2") | str_to_square("f2") |
                                  str_to_square("g2") | str_to_square("h2");
}  // namespace.

// Check that the bitboard has exactly one bit set.
bool is_square(Bitboard square) {
  const bool has_more_than_one_set_bit = (square & (square - 1));
  return square && !has_more_than_one_set_bit;
}

int square_idx(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  int res = -1;
  while (square) {
    square >>= 1;
    res += 1;
  }
  return res;
}

int rank_idx(Bitboard square) {
  const int idx = square_idx(square);
  return idx / 8;
}

int file_idx(Bitboard square) {
  const int idx = square_idx(square);
  return 7 - idx % 8;
}

bool on_a_file(Bitboard square) { return file_idx(square) == 0; }

bool on_h_file(Bitboard square) { return file_idx(square) == 7; }

bool on_first_rank(Bitboard square) { return rank_idx(square) == 0; }
bool on_eigth_rank(Bitboard square) { return rank_idx(square) == 7; }

Bitboard north_of(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  // TODO: Make sure right shifting off the end is not undefined behavior.
  return square << board_size;
}

Bitboard south_of(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  return square >> board_size;
}

Bitboard east_of(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  return on_h_file(square) ? 0 : square >> 1;
}

Bitboard west_of(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  return on_a_file(square) ? 0 : square << 1;
}

Bitboard northeast_of(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  const Bitboard north_square = north_of(square);
  return north_square ? east_of(north_square) : 0;
}

Bitboard northwest_of(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  const Bitboard north_square = north_of(square);
  return north_square ? west_of(north_square) : 0;
}

Bitboard southeast_of(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  const Bitboard south_square = south_of(square);
  return south_square ? east_of(south_square) : 0;
}

Bitboard southwest_of(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  const Bitboard south_square = south_of(square);
  return south_square ? west_of(south_square) : 0;
}

std::string Move::to_pretty_str() const {
  std::string promotion_str;
  if (promotion_piece) {
    const auto& piece_map = get_piece_to_char_and_unicode();
    promotion_str.push_back(
        piece_map.find(promotion_piece.value())->second.first);
  }
  return absl::StrCat(square_to_str(src_square), square_to_str(dst_square),
                      promotion_str);
}

void PrintTo(const Move& move, std::ostream* os) {
  *os << move.to_pretty_str();
}

Board::Board() : Board(get_start_fen()) {}

Board::Board(std::string_view fen) {
  std::vector<std::string_view> split_fen = absl::StrSplit(fen, " ");

  init_bitboards(split_fen[0]);
  init_is_whites_move(split_fen[1]);
  init_castling_rights(split_fen[2]);
  init_en_passant(split_fen[3]);
  ABSL_RAW_CHECK(absl::SimpleAtoi(split_fen[4], &fifty_move_clock_),
                 "FEN invalid: Fifty move not convertible to integer.");
  ABSL_RAW_CHECK(absl::SimpleAtoi(split_fen[5], &num_moves_),
                 "FEN invalid: Number of moves not convertible to integer.");
}

Board::Board(const Board& other)
    : white_pawns_(other.white_pawns_),
      white_rooks_(other.white_rooks_),
      white_knights_(other.white_knights_),
      white_bishops_(other.white_bishops_),
      white_queens_(other.white_queens_),
      white_king_(other.white_king_),
      black_pawns_(other.black_pawns_),
      black_rooks_(other.black_rooks_),
      black_knights_(other.black_knights_),
      black_bishops_(other.black_bishops_),
      black_queens_(other.black_queens_),
      black_king_(other.black_king_),
      is_whites_move_(other.is_whites_move_),
      en_passant_square_(other.en_passant_square_),
      white_has_right_to_castle_kingside_(
          other.white_has_right_to_castle_kingside_),
      white_has_right_to_castle_queenside_(
          other.white_has_right_to_castle_queenside_),
      black_has_right_to_castle_kingside_(
          other.black_has_right_to_castle_kingside_),
      black_has_right_to_castle_queenside_(
          other.black_has_right_to_castle_queenside_),
      fifty_move_clock_(other.fifty_move_clock_),
      num_moves_(other.num_moves_) {}

void Board::zero_all_bitboards() {
  white_pawns_ = 0;
  white_rooks_ = 0;
  white_knights_ = 0;
  white_bishops_ = 0;
  white_queens_ = 0;
  white_king_ = 0;
  black_pawns_ = 0;
  black_rooks_ = 0;
  black_knights_ = 0;
  black_bishops_ = 0;
  black_queens_ = 0;
  black_king_ = 0;
}

void Board::init_bitboards(const std::string_view pieces_fen) {
  zero_all_bitboards();
  int rank = board_size - 1;
  int file = 0;

  for (char c : pieces_fen) {
    Bitboard square_mask{lsb_bitboard
                         << (rank * board_size + board_size - file - 1)};
    switch (c) {
      // If the next characted is a number, skip that many files.
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
        file += (c - '0');
        break;
      case '/':
        rank -= 1;
        file = 0;
        break;
      case 'P':
        white_pawns_ |= square_mask;
        file += 1;
        break;
      case 'R':
        white_rooks_ |= square_mask;
        file += 1;
        break;
      case 'N':
        white_knights_ |= square_mask;
        file += 1;
        break;
      case 'B':
        white_bishops_ |= square_mask;
        file += 1;
        break;
      case 'Q':
        white_queens_ |= square_mask;
        file += 1;
        break;
      case 'K':
        white_king_ |= square_mask;
        file += 1;
        break;
      case 'p':
        black_pawns_ |= square_mask;
        file += 1;
        break;
      case 'r':
        black_rooks_ |= square_mask;
        file += 1;
        break;
      case 'n':
        black_knights_ |= square_mask;
        file += 1;
        break;
      case 'b':
        black_bishops_ |= square_mask;
        file += 1;
        break;
      case 'q':
        black_queens_ |= square_mask;
        file += 1;
        break;
      case 'k':
        black_king_ |= square_mask;
        file += 1;
        break;
      default:
        assert(false);
        break;
    }
  }
}

void Board::init_is_whites_move(const std::string_view side_to_move) {
  ABSL_RAW_CHECK(side_to_move == "w" || side_to_move == "b",
                 "Invalid FEN. Side to move must be either w or b");
  if (side_to_move == "w") {
    is_whites_move_ = true;
  } else if (side_to_move == "b") {
    is_whites_move_ = false;
  }
}

void Board::init_castling_rights(const std::string_view castling_rights_fen) {
  white_has_right_to_castle_kingside_ =
      (castling_rights_fen.find('K') != std::string::npos);
  white_has_right_to_castle_queenside_ =
      (castling_rights_fen.find('Q') != std::string::npos);
  black_has_right_to_castle_kingside_ =
      (castling_rights_fen.find('k') != std::string::npos);
  black_has_right_to_castle_queenside_ =
      (castling_rights_fen.find('q') != std::string::npos);
}

void Board::init_en_passant(const std::string_view algebraic_square) {
  if (algebraic_square == "-") {
    en_passant_square_ = std::nullopt;
  } else {
    en_passant_square_ = str_to_square(algebraic_square);
  }
}

std::string Board::to_pretty_str() const {
  const std::string top_left_corner = "┌";      // U+250C
  const std::string top_right_corner = "┐";     // U+2510
  const std::string bottom_left_corner = "└";   // U+2514
  const std::string bottom_right_corner = "┘";  // U+2518
  const std::string horizontal = "─";           // U+2500
  const std::string vertical = "│";             // U+2502
  const std::string left_side_tee = "├";        // U+251C
  const std::string right_side_tee = "┤";       // U+2524
  const std::string top_tee = "┬";              // U+252C
  const std::string bottom_tee = "┴";           // U+2534
  const std::string cross = "┼";                // U+253C

  std::string res;

  // do top of board
  res.append("  ");
  res.append(top_left_corner);
  for (int i = 0; i < board_size - 1; ++i) {
    res.append(horizontal + horizontal + horizontal);
    res.append(top_tee);
  }
  res.append(horizontal + horizontal + horizontal);
  res.append(top_right_corner);
  res.append("\n");

  // do rows
  for (int rank = board_size - 1; rank >= 0; --rank) {
    // print a row of pieces
    res.append(std::string(1, '1' + rank));
    res.append(" ");
    res.append(vertical);
    for (int file = 0; file < board_size; ++file) {
      res.append(" ");
      res.append(square_to_unicode(coordinates_to_square(file, rank)));
      res.append(" ");
      res.append(vertical);
    }
    res.append("\n");
    // print a dividing line (unless we're at the last rank)
    if (rank != 0) {
      res.append("  ");
      res.append(left_side_tee);
      for (int i = 0; i < board_size - 1; ++i) {
        res.append(horizontal + horizontal + horizontal + cross);
      }
      res.append(horizontal + horizontal + horizontal + right_side_tee);
      res.append("\n");
    }
  }

  // do bottom of board
  res.append("  ");
  res.append(bottom_left_corner);
  for (int i = 0; i < board_size - 1; ++i) {
    res.append(horizontal + horizontal + horizontal);
    res.append(bottom_tee);
  }
  res.append(horizontal + horizontal + horizontal);
  res.append(bottom_right_corner);
  res.append("\n");

  res.append("   ");
  for (char i = 0; i < board_size - 1; ++i) {
    res.append(" ");
    res.append(std::string(1, i + 'a'));
    res.append("  ");
  }
  res.append(" ");
  res.append("h");
  res.append("\n");

  return res;
}

void PrintTo(const Board& board, std::ostream* os) {
  *os << board.to_pretty_str();
}

std::string Board::square_to_unicode(Bitboard square) const {
  if (square & white_pawns_) {
    return "♙";  // U+2564
  } else if (square & white_rooks_) {
    return "♖";  // U+2656
  } else if (square & white_knights_) {
    return "♘";  // U+2658
  } else if (square & white_bishops_) {
    return "♗";  // U+2657
  } else if (square & white_queens_) {
    return "♕";  // U+2655
  } else if (square & white_king_) {
    return "♔";  // U+2654
  } else if (square & black_pawns_) {
    return "♟";  // U+265F
  } else if (square & black_rooks_) {
    return "♜";  // U+265C
  } else if (square & black_knights_) {
    return "♞";  // U+265E
  } else if (square & black_bishops_) {
    return "♝";  // U+265D
  } else if (square & black_queens_) {
    return "♛";  // U+265B
  } else if (square & black_king_) {
    return "♚";  // U+265A
  } else {
    return " ";
  }
}

void Board::pseudolegal_sliding_moves(Direction direction, Color side,
                                      Bitboard src_square,
                                      std::vector<Move>* res_ptr) const {
  std::vector<Move>& res = *res_ptr;
  Bitboard friends = side == Color::white ? white_pieces() : black_pieces();
  Bitboard enemies = side == Color::white ? black_pieces() : white_pieces();
  ABSL_RAW_CHECK(src_square & friends,
                 "src_square must have a piece with the correct color on it.");

  std::vector<Bitboard> dst_squares;
  auto direction_fn = direction_to_function(direction);
  Bitboard curr_square = direction_fn(src_square);
  Bitboard all_pieces_mask = friends | enemies;
  while (curr_square && !(curr_square & all_pieces_mask)) {
    res.emplace_back(src_square, curr_square);
    curr_square = direction_fn(curr_square);
  }
  if (curr_square & enemies) {
    res.emplace_back(src_square, curr_square);
  }
}

void Board::pseudolegal_bishop_moves(Color side,
                                     std::vector<Move>* res_ptr) const {
  Bitboard bishops = is_whites_move_ ? white_bishops_ : black_bishops_;
  for (Bitboard bishop : bitboard_split(bishops)) {
    pseudolegal_sliding_moves(Direction::northeast, side, bishop, res_ptr);
    pseudolegal_sliding_moves(Direction::northwest, side, bishop, res_ptr);
    pseudolegal_sliding_moves(Direction::southeast, side, bishop, res_ptr);
    pseudolegal_sliding_moves(Direction::southwest, side, bishop, res_ptr);
  }
}

void Board::pseudolegal_rook_moves(Color side,
                                   std::vector<Move>* res_ptr) const {
  Bitboard rooks = side == Color::white ? white_rooks_ : black_rooks_;
  for (Bitboard rook : bitboard_split(rooks)) {
    pseudolegal_sliding_moves(Direction::north, side, rook, res_ptr);
    pseudolegal_sliding_moves(Direction::south, side, rook, res_ptr);
    pseudolegal_sliding_moves(Direction::east, side, rook, res_ptr);
    pseudolegal_sliding_moves(Direction::west, side, rook, res_ptr);
  }
}

void Board::pseudolegal_queen_moves(Color side,
                                    std::vector<Move>* res_ptr) const {
  Bitboard queens = side == Color::white ? white_queens_ : black_queens_;
  for (Bitboard queen : bitboard_split(queens)) {
    pseudolegal_sliding_moves(Direction::north, side, queen, res_ptr);
    pseudolegal_sliding_moves(Direction::northeast, side, queen, res_ptr);
    pseudolegal_sliding_moves(Direction::northwest, side, queen, res_ptr);
    pseudolegal_sliding_moves(Direction::southeast, side, queen, res_ptr);
    pseudolegal_sliding_moves(Direction::southwest, side, queen, res_ptr);
    pseudolegal_sliding_moves(Direction::south, side, queen, res_ptr);
    pseudolegal_sliding_moves(Direction::east, side, queen, res_ptr);
    pseudolegal_sliding_moves(Direction::west, side, queen, res_ptr);
  }
}

void Board::pseudolegal_simple_pawn_moves(Color side,
                                          std::vector<Move>* res_ptr) const {
  std::vector<Move>& res = *res_ptr;
  if (side == Color::white) {
    const Bitboard white_pawns_excluding_seventh =
        white_pawns_ & (~seventh_rank_mask);
    for (Bitboard single_pawn : bitboard_split(white_pawns_excluding_seventh)) {
      const Bitboard north_of_pawn = north_of(single_pawn);
      const bool blocked = all_pieces() & north_of_pawn;
      if (!blocked) {
        res.emplace_back(single_pawn, north_of_pawn);
      }
    }
  } else {
    const Bitboard black_pawns_excluding_second =
        black_pawns_ & (~second_rank_mask);
    for (Bitboard single_pawn : bitboard_split(black_pawns_excluding_second)) {
      const Bitboard south_of_pawn = south_of(single_pawn);
      const bool blocked = all_pieces() & south_of_pawn;
      if (!blocked) {
        res.emplace_back(single_pawn, south_of_pawn);
      }
    }
  }
}

void Board::pseudolegal_two_step_pawn_moves(Color side,
                                            std::vector<Move>* res_ptr) const {
  std::vector<Move>& res = *res_ptr;
  if (side == Color::white) {
    const Bitboard white_pawns_on_second = white_pawns_ & second_rank_mask;
    for (Bitboard single_pawn : bitboard_split(white_pawns_on_second)) {
      const Bitboard north_of_pawn = north_of(single_pawn);
      const Bitboard two_north_of_pawn = north_of(north_of_pawn);
      const bool blocked =
          (all_pieces() & north_of_pawn) || (all_pieces() & two_north_of_pawn);
      if (!blocked) {
        res.emplace_back(single_pawn, two_north_of_pawn);
      }
    }
  } else {
    const Bitboard black_pawns_on_seventh = black_pawns_ & seventh_rank_mask;
    for (Bitboard single_pawn : bitboard_split(black_pawns_on_seventh)) {
      const Bitboard south_of_pawn = south_of(single_pawn);
      const Bitboard two_south_of_pawn = south_of(south_of_pawn);
      const bool blocked =
          (all_pieces() & south_of_pawn) || (all_pieces() & two_south_of_pawn);
      if (!blocked) {
        res.emplace_back(single_pawn, two_south_of_pawn);
      }
    }
  }
}

void Board::pseudolegal_en_passant_moves(Color side,
                                         std::vector<Move>* res_ptr) const {
  std::vector<Move>& res = *res_ptr;
  if (en_passant_square_) {
    if (side == Color::white) {
      Bitboard southeast_of_ep_square =
          southeast_of(en_passant_square_.value());
      Bitboard southwest_of_ep_square =
          southwest_of(en_passant_square_.value());
      if (southwest_of_ep_square & white_pieces()) {
        res.emplace_back(southwest_of_ep_square, en_passant_square_.value());
      }
      if (southeast_of_ep_square & white_pieces()) {
        res.emplace_back(southeast_of_ep_square, en_passant_square_.value());
      }
    } else {
      Bitboard northeast_of_ep_square =
          northeast_of(en_passant_square_.value());
      Bitboard northwest_of_ep_square =
          northwest_of(en_passant_square_.value());
      if (northwest_of_ep_square & black_pieces()) {
        res.emplace_back(northwest_of_ep_square, en_passant_square_.value());
      }
      if (northeast_of_ep_square & black_pieces()) {
        res.emplace_back(northeast_of_ep_square, en_passant_square_.value());
      }
    }
  }
}

void Board::pseudolegal_promotions(Color side,
                                   std::vector<Move>* res_ptr) const {
  std::vector<Move>& res = *res_ptr;
  if (side == Color::white) {
    const Bitboard white_pawns_on_seventh = white_pawns_ & seventh_rank_mask;
    for (Bitboard single_pawn : bitboard_split(white_pawns_on_seventh)) {
      const Bitboard north_of_pawn = north_of(single_pawn);
      const bool blocked = all_pieces() & north_of_pawn;
      if (!blocked) {
        res.emplace_back(single_pawn, north_of_pawn, Piece::white_rook);
        res.emplace_back(single_pawn, north_of_pawn, Piece::white_knight);
        res.emplace_back(single_pawn, north_of_pawn, Piece::white_bishop);
        res.emplace_back(single_pawn, north_of_pawn, Piece::white_queen);
      }

      const Bitboard northeast_of_pawn = northeast_of(single_pawn);
      const bool black_piece_northeast = black_pieces() & northeast_of_pawn;
      if (black_piece_northeast) {
        res.emplace_back(single_pawn, northeast_of_pawn, Piece::white_rook);
        res.emplace_back(single_pawn, northeast_of_pawn, Piece::white_knight);
        res.emplace_back(single_pawn, northeast_of_pawn, Piece::white_bishop);
        res.emplace_back(single_pawn, northeast_of_pawn, Piece::white_queen);
      }

      const Bitboard northwest_of_pawn = northwest_of(single_pawn);
      const bool black_piece_northwest = black_pieces() & northwest_of_pawn;
      if (black_piece_northwest) {
        res.emplace_back(single_pawn, northwest_of_pawn, Piece::white_rook);
        res.emplace_back(single_pawn, northwest_of_pawn, Piece::white_knight);
        res.emplace_back(single_pawn, northwest_of_pawn, Piece::white_bishop);
        res.emplace_back(single_pawn, northwest_of_pawn, Piece::white_queen);
      }
    }
  } else {
    const Bitboard black_pawns_on_second = black_pawns_ & second_rank_mask;
    for (Bitboard single_pawn : bitboard_split(black_pawns_on_second)) {
      const Bitboard south_of_pawn = south_of(single_pawn);
      const bool blocked = all_pieces() & south_of_pawn;
      if (!blocked) {
        res.emplace_back(single_pawn, south_of_pawn, Piece::black_rook);
        res.emplace_back(single_pawn, south_of_pawn, Piece::black_knight);
        res.emplace_back(single_pawn, south_of_pawn, Piece::black_bishop);
        res.emplace_back(single_pawn, south_of_pawn, Piece::black_queen);
      }

      const Bitboard southeast_of_pawn = southeast_of(single_pawn);
      const bool white_piece_southeast = white_pieces() & southeast_of_pawn;
      if (white_piece_southeast) {
        res.emplace_back(single_pawn, southeast_of_pawn, Piece::black_rook);
        res.emplace_back(single_pawn, southeast_of_pawn, Piece::black_knight);
        res.emplace_back(single_pawn, southeast_of_pawn, Piece::black_bishop);
        res.emplace_back(single_pawn, southeast_of_pawn, Piece::black_queen);
      }

      const Bitboard southwest_of_pawn = southwest_of(single_pawn);
      const bool white_piece_southwest = white_pieces() & southwest_of_pawn;
      if (white_piece_southwest) {
        res.emplace_back(single_pawn, southwest_of_pawn, Piece::black_rook);
        res.emplace_back(single_pawn, southwest_of_pawn, Piece::black_knight);
        res.emplace_back(single_pawn, southwest_of_pawn, Piece::black_bishop);
        res.emplace_back(single_pawn, southwest_of_pawn, Piece::black_queen);
      }
    }
  }
}

void Board::pseudolegal_pawn_captures(Color side,
                                      std::vector<Move>* res_ptr) const {
  std::vector<Move>& res = *res_ptr;
  if (side == Color::white) {
    const Bitboard white_pawns_excluding_seventh =
        white_pawns_ & (~seventh_rank_mask);
    for (Bitboard single_pawn : bitboard_split(white_pawns_excluding_seventh)) {
      const Bitboard northeast_of_pawn = northeast_of(single_pawn);
      const bool can_capture_northeast = black_pieces() & northeast_of_pawn;
      if (can_capture_northeast) {
        res.emplace_back(single_pawn, northeast_of_pawn);
      }

      const Bitboard northwest_of_pawn = northwest_of(single_pawn);
      const bool can_capture_northwest = black_pieces() & northwest_of_pawn;
      if (can_capture_northwest) {
        res.emplace_back(single_pawn, northwest_of_pawn);
      }
    }
  } else {
    const Bitboard black_pawns_excluding_seventh =
        black_pawns_ & (~second_rank_mask);
    for (Bitboard single_pawn : bitboard_split(black_pawns_excluding_seventh)) {
      const Bitboard southeast_of_pawn = southeast_of(single_pawn);
      const bool can_capture_southeast = white_pieces() & southeast_of_pawn;
      if (can_capture_southeast) {
        res.emplace_back(single_pawn, southeast_of_pawn);
      }

      const Bitboard southwest_of_pawn = southwest_of(single_pawn);
      const bool can_capture_southwest = white_pieces() & southwest_of_pawn;
      if (can_capture_southwest) {
        res.emplace_back(single_pawn, southwest_of_pawn);
      }
    }
  }
}

void Board::pseudolegal_pawn_moves(Color side,
                                   std::vector<Move>* res_ptr) const {
  pseudolegal_simple_pawn_moves(side, res_ptr);
  pseudolegal_two_step_pawn_moves(side, res_ptr);
  pseudolegal_pawn_captures(side, res_ptr);
  pseudolegal_en_passant_moves(side, res_ptr);
  pseudolegal_promotions(side, res_ptr);
}

void Board::pseudolegal_king_moves(Color side,
                                   std::vector<Move>* res_ptr) const {
  Bitboard king = side == Color::white ? white_king_ : black_king_;
  Bitboard friends = side == Color::white ? white_pieces() : black_pieces();

  Bitboard north_of_king = north_of(king);
  bool friendly_piece_north_of_king = north_of_king & friends;
  if (!friendly_piece_north_of_king && north_of_king) {
    res_ptr->emplace_back(king, north_of_king);
  }

  Bitboard south_of_king = south_of(king);
  bool friendly_piece_south_of_king = south_of_king & friends;
  if (!friendly_piece_south_of_king && south_of_king) {
    res_ptr->emplace_back(king, south_of_king);
  }

  Bitboard east_of_king = east_of(king);
  bool friendly_piece_east_of_king = east_of_king & friends;
  if (!friendly_piece_east_of_king && east_of_king) {
    res_ptr->emplace_back(king, east_of_king);
  }

  Bitboard west_of_king = west_of(king);
  bool friendly_piece_west_of_king = west_of_king & friends;
  if (!friendly_piece_west_of_king && west_of_king) {
    res_ptr->emplace_back(king, west_of_king);
  }

  Bitboard northeast_of_king = northeast_of(king);
  bool friendly_piece_northeast_of_king = northeast_of_king & friends;
  if (!friendly_piece_northeast_of_king && northeast_of_king) {
    res_ptr->emplace_back(king, northeast_of_king);
  }

  Bitboard northwest_of_king = northwest_of(king);
  bool friendly_piece_northwest_of_king = northwest_of_king & friends;
  if (!friendly_piece_northwest_of_king && northwest_of_king) {
    res_ptr->emplace_back(king, northwest_of_king);
  }

  Bitboard southeast_of_king = southeast_of(king);
  bool friendly_piece_southeast_of_king = southeast_of_king & friends;
  if (!friendly_piece_southeast_of_king && southeast_of_king) {
    res_ptr->emplace_back(king, southeast_of_king);
  }

  Bitboard southwest_of_king = southwest_of(king);
  bool friendly_piece_southwest_of_king = southwest_of_king & friends;
  if (!friendly_piece_southwest_of_king && southwest_of_king) {
    res_ptr->emplace_back(king, southwest_of_king);
  }
}

void Board::pseudolegal_knight_moves(Color side,
                                     std::vector<Move>* res_ptr) const {
  Bitboard knights = side == Color::white ? white_knights_ : black_knights_;
  Bitboard friends = side == Color::white ? white_pieces() : black_pieces();

  for (Bitboard knight : bitboard_split(knights)) {
    Bitboard n_knight = north_of(knight);
    if (n_knight) {
      Bitboard ne_n_knight = northeast_of(n_knight);
      bool friendly_piece_ne_n_knight = ne_n_knight & friends;
      if (!friendly_piece_ne_n_knight && ne_n_knight) {
        res_ptr->emplace_back(knight, ne_n_knight);
      }
      Bitboard nw_n_knight = northwest_of(n_knight);
      bool friendly_piece_nw_n_knight = nw_n_knight & friends;
      if (!friendly_piece_nw_n_knight && nw_n_knight) {
        res_ptr->emplace_back(knight, nw_n_knight);
      }
    }

    Bitboard s_knight = south_of(knight);
    if (s_knight) {
      Bitboard se_s_knight = southeast_of(s_knight);
      bool friendly_piece_se_s_knight = se_s_knight & friends;
      if (!friendly_piece_se_s_knight && se_s_knight) {
        res_ptr->emplace_back(knight, se_s_knight);
      }
      Bitboard sw_s_knight = southwest_of(s_knight);
      bool friendly_piece_sw_s_knight = sw_s_knight & friends;
      if (!friendly_piece_sw_s_knight && sw_s_knight) {
        res_ptr->emplace_back(knight, sw_s_knight);
      }
    }

    Bitboard e_knight = east_of(knight);
    if (e_knight) {
      Bitboard ne_e_knight = northeast_of(e_knight);
      bool friendly_piece_ne_e_knight = ne_e_knight & friends;
      if (!friendly_piece_ne_e_knight && ne_e_knight) {
        res_ptr->emplace_back(knight, ne_e_knight);
      }
      Bitboard se_e_knight = southeast_of(e_knight);
      bool friendly_piece_se_e_knight = se_e_knight & friends;
      if (!friendly_piece_se_e_knight && se_e_knight) {
        res_ptr->emplace_back(knight, se_e_knight);
      }
    }

    Bitboard w_knight = west_of(knight);
    if (w_knight) {
      Bitboard nw_w_knight = northwest_of(w_knight);
      bool friendly_piecw_nw_w_knight = nw_w_knight & friends;
      if (!friendly_piecw_nw_w_knight && nw_w_knight) {
        res_ptr->emplace_back(knight, nw_w_knight);
      }
      Bitboard sw_w_knight = southwest_of(w_knight);
      bool friendly_piecw_sw_w_knight = sw_w_knight & friends;
      if (!friendly_piecw_sw_w_knight && sw_w_knight) {
        res_ptr->emplace_back(knight, sw_w_knight);
      }
    }
  }
}

std::vector<Move> Board::pseudolegal_moves(Color side) const {
  std::vector<Move> res;
  pseudolegal_bishop_moves(side, &res);
  pseudolegal_rook_moves(side, &res);
  pseudolegal_queen_moves(side, &res);
  pseudolegal_pawn_moves(side, &res);
  pseudolegal_king_moves(side, &res);
  pseudolegal_knight_moves(side, &res);
  return res;
}

Bitboard Board::all_dst_squares(Color side) const {
  std::vector<Move> moves = pseudolegal_moves(side);
  Bitboard res = absl::c_accumulate(
      moves, uint64_t(0),
      [](Bitboard acc, Move move) { return acc | move.dst_square; });
  return res;
}

Bitboard str_to_square(const std::string_view algebraic_square) {
  const char file_char = algebraic_square[0];
  const char rank_char = algebraic_square[1];
  ABSL_RAW_CHECK('a' <= file_char && file_char <= 'h', "Invalid file.");
  ABSL_RAW_CHECK('1' <= rank_char && rank_char <= '8', "Invalid rank.");
  const int file = file_char - 'a';
  const int rank = rank_char - 1;
  return coordinates_to_square(file, rank);
}

std::string square_to_str(Bitboard sq) {
  ABSL_RAW_CHECK(is_square(sq), "Not a square.");
  char rank = rank_idx(sq);
  char file = file_idx(sq);
  std::string res;
  res.push_back('a' + file);
  res.push_back('1' + rank);
  return res;
}

Bitboard coordinates_to_square(int file, int rank) {
  return lsb_bitboard << (rank * board_size + board_size - file - 1);
}

Bitboard Board::white_pieces() const {
  return white_pawns_ | white_knights_ | white_bishops_ | white_rooks_ |
         white_queens_ | white_king_;
}

Bitboard Board::black_pieces() const {
  return black_pawns_ | black_knights_ | black_bishops_ | black_rooks_ |
         black_queens_ | black_king_;
}

Bitboard Board::all_pieces() const { return white_pieces() | black_pieces(); }

std::vector<Bitboard> bitboard_split(Bitboard bb) {
  std::vector<Bitboard> res;
  while (bb) {
    const Bitboard bb_with_lsb_cleared = bb & bb - 1;
    const Bitboard bb_with_only_lsb = bb ^ bb_with_lsb_cleared;
    res.push_back(bb_with_only_lsb);
    bb = bb_with_lsb_cleared;
  }
  return res;
}

std::function<Bitboard(Bitboard)> direction_to_function(Direction direction) {
  switch (direction) {
    case Direction::north:
      return north_of;
    case Direction::south:
      return south_of;
    case Direction::east:
      return east_of;
    case Direction::west:
      return west_of;
    case Direction::northeast:
      return northeast_of;
    case Direction::northwest:
      return northwest_of;
    case Direction::southeast:
      return southeast_of;
    case Direction::southwest:
      return southwest_of;
  }
}