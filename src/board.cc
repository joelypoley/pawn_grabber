#include "src/board.h"

#include <cassert>
#include <optional>
#include <string_view>

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

const int board_size = 8;
const Bitboard lsb_bitboard = 1;
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

Bitboard north(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  // TODO: Make sure right shifting off the end is not undefined behavior.
  return square << board_size;
}

Bitboard south(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  return square >> board_size;
}

Bitboard east(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  return on_h_file(square) ? 0 : square >> 1;
}

Bitboard west(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  return on_a_file(square) ? 0 : square << 1;
}

Bitboard northeast(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  const Bitboard north_square = north(square);
  return north_square ? east(north_square) : 0;
}

Bitboard northwest(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  const Bitboard north_square = north(square);
  return north_square ? west(north_square) : 0;
}

Bitboard southeast(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  const Bitboard south_square = south(square);
  return south_square ? east(south_square) : 0;
}

Bitboard southwest(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
  const Bitboard south_square = south(square);
  return south_square ? west(south_square) : 0;
}

Board::Board() : Board(get_start_fen()) {}

Board::Board(std::string_view fen) {
  std::vector<std::string_view> split_fen = absl::StrSplit(fen, " ");

  init_bitboards(split_fen[0]);
  init_side_to_move(split_fen[1]);
  init_castling_rights(split_fen[2]);
  init_en_passant(split_fen[3]);
  ABSL_RAW_CHECK(absl::SimpleAtoi(split_fen[4], &fifty_move_clock_),
                 "FEN invalid: Fifty move not convertible to integer.");
  ABSL_RAW_CHECK(absl::SimpleAtoi(split_fen[5], &num_moves_),
                 "FEN invalid: Number of moves not convertible to integer.");
}

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

void Board::init_side_to_move(const std::string_view side_to_move) {
  if (side_to_move == "w") {
    side_to_move_ = Color::white;
  } else if (side_to_move == "b") {
    side_to_move_ = Color::black;
  } else {
    assert(false);
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
    en_passant_square_ = algebraic_to_square(algebraic_square);
  }
}

std::string Board::to_pretty_str() {
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

  std::vector<std::string> res;

  // do top of board
  res.emplace_back("  ");
  res.emplace_back(top_left_corner);
  for (int i = 0; i < board_size - 1; ++i) {
    res.emplace_back(horizontal + horizontal + horizontal);
    res.emplace_back(top_tee);
  }
  res.emplace_back(horizontal + horizontal + horizontal);
  res.emplace_back(top_right_corner);
  res.emplace_back("\n");

  // do rows
  for (int rank = board_size - 1; rank >= 0; --rank) {
    // print a row of pieces
    res.emplace_back(std::string(1, '1' + rank));
    res.emplace_back(" ");
    res.emplace_back(vertical);
    for (int file = 0; file < board_size; ++file) {
      res.emplace_back(" ");
      res.emplace_back(square_to_unicode(coordinates_to_square(file, rank)));
      res.emplace_back(" ");
      res.emplace_back(vertical);
    }
    res.emplace_back("\n");
    // print a dividing line (unless we're at the last rank)
    if (rank != 0) {
      res.emplace_back("  ");
      res.emplace_back(left_side_tee);
      for (int i = 0; i < board_size - 1; ++i) {
        res.emplace_back(horizontal + horizontal + horizontal + cross);
      }
      res.emplace_back(horizontal + horizontal + horizontal + right_side_tee);
      res.emplace_back("\n");
    }
  }

  // do bottom of board
  res.emplace_back("  ");
  res.emplace_back(bottom_left_corner);
  for (int i = 0; i < board_size - 1; ++i) {
    res.emplace_back(horizontal + horizontal + horizontal);
    res.emplace_back(bottom_tee);
  }
  res.emplace_back(horizontal + horizontal + horizontal);
  res.emplace_back(bottom_right_corner);
  res.emplace_back("\n");

  res.emplace_back("   ");
  for (char i = 0; i < board_size - 1; ++i) {
    res.emplace_back(" ");
    res.emplace_back(std::string(1, i + 'a'));
    res.emplace_back("  ");
  }
  res.emplace_back(" ");
  res.emplace_back("h");
  res.emplace_back("\n");

  return absl::StrJoin(res, "");
}

std::string Board::square_to_unicode(Bitboard square) {
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

Bitboard algebraic_to_square(const std::string_view algebraic_square) {
  const int file = algebraic_square[0] - 'a';
  const int rank = algebraic_square[1] - 1;
  return coordinates_to_square(file, rank);
}

Bitboard coordinates_to_square(int file, int rank) {
  return lsb_bitboard << (rank * board_size + board_size - file - 1);
}

Bitboard Board::white_pieces() {
  return white_pawns_ | white_knights_ | white_bishops_ | white_rooks_ |
         white_queens_ | white_king_;
}

Bitboard Board::black_pieces() {
  return black_pawns_ | black_knights_ | black_bishops_ | black_rooks_ |
         black_queens_ | black_king_;
}
