#include "board.h"

#include <array>
#include <functional>
#include <iostream>
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

// TODO: Change these all to functions returning their static local variables to
// avoid problems with global initialization.

const Bitboard seventh_rank_mask = str_to_square("a7") | str_to_square("b7") |
                                   str_to_square("c7") | str_to_square("d7") |
                                   str_to_square("e7") | str_to_square("f7") |
                                   str_to_square("g7") | str_to_square("h7");
const Bitboard second_rank_mask = str_to_square("a2") | str_to_square("b2") |
                                  str_to_square("c2") | str_to_square("d2") |
                                  str_to_square("e2") | str_to_square("f2") |
                                  str_to_square("g2") | str_to_square("h2");
const Bitboard third_rank_mask = str_to_square("a3") | str_to_square("b3") |
                                 str_to_square("c3") | str_to_square("d3") |
                                 str_to_square("e3") | str_to_square("f3") |
                                 str_to_square("g3") | str_to_square("h3");
const Bitboard white_castle_kingside_mask =
    str_to_square("f1") | str_to_square("g1");
const Bitboard white_castle_queenside_mask =
    str_to_square("d1") | str_to_square("c1");
const Bitboard black_castle_kingside_mask =
    str_to_square("f8") | str_to_square("g8");
const Bitboard black_castle_queenside_mask =
    str_to_square("d8") | str_to_square("c8");

}  // namespace.

Board::Board() : Board(get_start_fen()) {}

Board::Board(absl::string_view fen) {
  std::vector<absl::string_view> split_fen = absl::StrSplit(fen, " ");

  init_bitboards(split_fen[0]);
  init_is_whites_move(split_fen[1]);
  init_castling_rights(split_fen[2]);
  init_en_passant(split_fen[3]);
  ABSL_RAW_CHECK(absl::SimpleAtoi(split_fen[4], &fifty_move_clock_),
                 "FEN invalid: Fifty move clock not convertible to integer.");
  ABSL_RAW_CHECK(absl::SimpleAtoi(split_fen[5], &num_moves_),
                 "FEN invalid: Number of moves not convertible to integer.");
}

std::array<Bitboard*, 12> Board::all_bitboards() {
  return {&white_pawns_,   &white_rooks_,   &white_bishops_, &white_knights_,
          &white_queens_,  &white_king_,    &black_pawns_,   &black_rooks_,
          &black_bishops_, &black_knights_, &black_queens_,  &black_king_};
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
    res.append(absl::StrCat(horizontal, horizontal, horizontal));
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
      res.append(occupiers_unicode_symbol(file, rank));
      res.append(" ");
      res.append(vertical);
    }
    res.append("\n");
    // print a dividing line (unless we're at the last rank)
    if (rank != 0) {
      res.append("  ");
      res.append(left_side_tee);
      for (int i = 0; i < board_size - 1; ++i) {
        res.append(absl::StrCat(horizontal, horizontal, horizontal, cross));
      }
      res.append(
          absl::StrCat(horizontal, horizontal, horizontal, right_side_tee));
      res.append("\n");
    }
  }

  // do bottom of board
  res.append("  ");
  res.append(bottom_left_corner);
  for (int i = 0; i < board_size - 1; ++i) {
    res.append(absl::StrCat(horizontal, horizontal, horizontal));
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

std::string Board::occupiers_unicode_symbol(int file, int rank) const {
  Bitboard square = coordinates_to_square(file, rank);
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

void PrintTo(const Board& board, std::ostream* os) {
  *os << board.to_pretty_str();
}

Bitboard Board::friends(Color side) const {
  return side == Color::white ? white_pieces() : black_pieces();
}

Bitboard Board::enemies(Color side) const {
  return side == Color::white ? black_pieces() : white_pieces();
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

Bitboard Board::pawn_attack_squares(Color side) const {
  Bitboard res = 0;
  if (side == Color::white) {
    for (Bitboard single_pawn : bitboard_split(white_pawns_)) {
      const Bitboard northeast_of_pawn = northeast_of(single_pawn);
      res |= northeast_of_pawn;
      const Bitboard northwest_of_pawn = northwest_of(single_pawn);
      res |= northwest_of_pawn;
    }
  } else {
    for (Bitboard single_pawn : bitboard_split(black_pawns_)) {
      const Bitboard southeast_of_pawn = southeast_of(single_pawn);
      res |= southeast_of_pawn;
      const Bitboard southwest_of_pawn = southwest_of(single_pawn);
      res |= southwest_of_pawn;
    }
  }
  return res;
}

Bitboard Board::attack_squares(Color side) const {
  std::vector<Move> moves = pseudolegal_moves(side);
  const Bitboard without_pawns_attack_squares = absl::c_accumulate(
      moves, uint64_t(0),
      [](Bitboard acc, Move move) { return acc | move.dst_square_; });
  return without_pawns_attack_squares | pawn_attack_squares(side);
}

void Board::append_pseudolegal_sliding_moves(Direction direction, Color side,
                                             Bitboard src_square,
                                             Piece piece_moving,
                                             std::vector<Move>* res) const {
  ABSL_RAW_CHECK(src_square & friends(side),
                 "src_square must have a piece with the correct color on it.");

  std::vector<Bitboard> dst_squares;
  auto direction_fn = direction_to_function(direction);
  Bitboard curr_square = direction_fn(src_square);
  const Bitboard all_pieces_mask = all_pieces();
  while (curr_square && !(curr_square & all_pieces_mask)) {
    res->emplace_back(src_square, curr_square, piece_moving, MoveType::simple,
                     *this);
    curr_square = direction_fn(curr_square);
  }
  if (curr_square & enemies(side)) {
    res->emplace_back(src_square, curr_square, piece_moving, MoveType::capture,
                     *this);
  }
}

void Board::append_pseudolegal_bishop_moves(Color side,
                                            std::vector<Move>* res) const {
  const Bitboard bishops =
      side == Color::white ? white_bishops_ : black_bishops_;
  for (Bitboard bishop_sq : bitboard_split(bishops)) {
    for (Direction dir : bishop_move_directions) {
      append_pseudolegal_sliding_moves(dir, side, bishop_sq, Piece::bishop,
                                       res);
    }
  }
}

void Board::append_pseudolegal_rook_moves(Color side,
                                          std::vector<Move>* res) const {
  const Bitboard rooks = side == Color::white ? white_rooks_ : black_rooks_;
  for (Bitboard rook_sq : bitboard_split(rooks)) {
    for (Direction dir : rook_move_directions) {
      append_pseudolegal_sliding_moves(dir, side, rook_sq, Piece::rook,
                                       res);
    }
  }
}

void Board::append_pseudolegal_queen_moves(Color side,
                                           std::vector<Move>* res) const {
  const Bitboard queens = side == Color::white ? white_queens_ : black_queens_;
  for (Bitboard queen_sq : bitboard_split(queens)) {
    for (Direction dir : all_directions) {
      append_pseudolegal_sliding_moves(dir, side, queen_sq, Piece::queen,
                                       res);
    }
  }
}

void Board::append_pseudolegal_simple_pawn_moves(
    Color side, std::vector<Move>* res) const {
  if (side == Color::white) {
    const Bitboard white_pawns_excluding_seventh =
        white_pawns_ & (~seventh_rank_mask);
    for (Bitboard single_pawn : bitboard_split(white_pawns_excluding_seventh)) {
      const Bitboard north_of_pawn = north_of(single_pawn);
      const bool blocked = static_cast<bool>(all_pieces() & north_of_pawn);
      if (!blocked) {
        res->emplace_back(single_pawn, north_of_pawn, Piece::pawn,
                         MoveType::simple, *this);
      }
    }
  } else {
    const Bitboard black_pawns_excluding_second =
        black_pawns_ & (~second_rank_mask);
    for (Bitboard single_pawn : bitboard_split(black_pawns_excluding_second)) {
      const Bitboard south_of_pawn = south_of(single_pawn);
      const bool blocked = static_cast<bool>(all_pieces() & south_of_pawn);
      if (!blocked) {
        res->emplace_back(single_pawn, south_of_pawn, Piece::pawn,
                         MoveType::simple, *this);
      }
    }
  }
}

void Board::append_pseudolegal_two_step_pawn_moves(
    Color side, std::vector<Move>* res) const {
  if (side == Color::white) {
    const Bitboard white_pawns_on_second = white_pawns_ & second_rank_mask;
    for (Bitboard single_pawn : bitboard_split(white_pawns_on_second)) {
      const Bitboard north_of_pawn = north_of(single_pawn);
      const Bitboard two_north_of_pawn = north_of(north_of_pawn);
      const bool blocked =
          (all_pieces() & north_of_pawn) || (all_pieces() & two_north_of_pawn);
      if (!blocked) {
        res->emplace_back(single_pawn, two_north_of_pawn, Piece::pawn,
                         MoveType::two_step_pawn, *this);
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
        res->emplace_back(single_pawn, two_south_of_pawn, Piece::pawn,
                         MoveType::two_step_pawn, *this);
      }
    }
  }
}

void Board::append_pseudolegal_en_passant_moves(
    Color side, std::vector<Move>* res) const {
  if (en_passant_square_) {
    if (side == Color::white) {
      Bitboard southeast_of_ep_square =
          southeast_of(en_passant_square_.value());
      Bitboard southwest_of_ep_square =
          southwest_of(en_passant_square_.value());
      if (southwest_of_ep_square & white_pawns_) {
        res->emplace_back(southwest_of_ep_square, en_passant_square_.value(),
                         Piece::pawn, MoveType::en_passant, *this);
      }
      if (southeast_of_ep_square & white_pawns_) {
        res->emplace_back(southeast_of_ep_square, en_passant_square_.value(),
                         Piece::pawn, MoveType::en_passant, *this);
      }
    } else {
      Bitboard northeast_of_ep_square =
          northeast_of(en_passant_square_.value());
      Bitboard northwest_of_ep_square =
          northwest_of(en_passant_square_.value());
      if (northwest_of_ep_square & black_pawns_) {
        res->emplace_back(northwest_of_ep_square, en_passant_square_.value(),
                         Piece::pawn, MoveType::en_passant, *this);
      }
      if (northeast_of_ep_square & black_pawns_) {
        res->emplace_back(northeast_of_ep_square, en_passant_square_.value(),
                         Piece::pawn, MoveType::en_passant, *this);
      }
    }
  }
}

void Board::append_pseudolegal_promotions(Color side,
                                          std::vector<Move>* res) const {
  if (side == Color::white) {
    const Bitboard white_pawns_on_seventh = white_pawns_ & seventh_rank_mask;
    for (Bitboard single_pawn : bitboard_split(white_pawns_on_seventh)) {
      for (MoveType promotion_piece : promotion_move_types) {
        const Bitboard north_of_pawn = north_of(single_pawn);
        const bool blocked = static_cast<bool>(all_pieces() & north_of_pawn);
        if (!blocked) {
          res->emplace_back(single_pawn, north_of_pawn, Piece::pawn,
                           promotion_piece, *this);
        }

        const Bitboard northeast_of_pawn = northeast_of(single_pawn);
        const bool black_piece_northeast =
            static_cast<bool>(black_pieces() & northeast_of_pawn);
        if (black_piece_northeast) {
          res->emplace_back(single_pawn, northeast_of_pawn, Piece::pawn,
                           promotion_piece, *this);
        }

        const Bitboard northwest_of_pawn = northwest_of(single_pawn);
        const bool black_piece_northwest =
            static_cast<bool>(black_pieces() & northwest_of_pawn);
        if (black_piece_northwest) {
          res->emplace_back(single_pawn, northwest_of_pawn, Piece::pawn,
                           promotion_piece, *this);
        }
      }
    }
  } else {
    const Bitboard black_pawns_on_second = black_pawns_ & second_rank_mask;
    for (Bitboard single_pawn : bitboard_split(black_pawns_on_second)) {
      for (MoveType promotion_piece : promotion_move_types) {
        const Bitboard south_of_pawn = south_of(single_pawn);
        const bool blocked = static_cast<bool>(all_pieces() & south_of_pawn);
        if (!blocked) {
          res->emplace_back(single_pawn, south_of_pawn, Piece::pawn,
                           promotion_piece, *this);
        }

        const Bitboard southeast_of_pawn = southeast_of(single_pawn);
        const bool white_piece_southeast =
            static_cast<bool>(white_pieces() & southeast_of_pawn);
        if (white_piece_southeast) {
          res->emplace_back(single_pawn, southeast_of_pawn, Piece::pawn,
                           promotion_piece, *this);
        }

        const Bitboard southwest_of_pawn = southwest_of(single_pawn);
        const bool white_piece_southwest =
            static_cast<bool>(white_pieces() & southwest_of_pawn);
        if (white_piece_southwest) {
          res->emplace_back(single_pawn, southwest_of_pawn, Piece::pawn,
                           promotion_piece, *this);
        }
      }
    }
  }
}

void Board::append_pseudolegal_pawn_captures(Color side,
                                             std::vector<Move>* res) const {
  if (side == Color::white) {
    const Bitboard white_pawns_excluding_seventh =
        white_pawns_ & (~seventh_rank_mask);
    for (Bitboard single_pawn : bitboard_split(white_pawns_excluding_seventh)) {
      const Bitboard northeast_of_pawn = northeast_of(single_pawn);
      const bool can_capture_northeast =
          static_cast<bool>(black_pieces() & northeast_of_pawn);
      if (can_capture_northeast) {
        res->emplace_back(single_pawn, northeast_of_pawn, Piece::pawn,
                         MoveType::capture, *this);
      }

      const Bitboard northwest_of_pawn = northwest_of(single_pawn);
      const bool can_capture_northwest =
          static_cast<bool>(black_pieces() & northwest_of_pawn);
      if (can_capture_northwest) {
        res->emplace_back(single_pawn, northwest_of_pawn, Piece::pawn,
                         MoveType::capture, *this);
      }
    }
  } else {
    const Bitboard black_pawns_excluding_seventh =
        black_pawns_ & (~second_rank_mask);
    for (Bitboard single_pawn : bitboard_split(black_pawns_excluding_seventh)) {
      const Bitboard southeast_of_pawn = southeast_of(single_pawn);
      const bool can_capture_southeast =
          static_cast<bool>(white_pieces() & southeast_of_pawn);
      if (can_capture_southeast) {
        res->emplace_back(single_pawn, southeast_of_pawn, Piece::pawn,
                         MoveType::capture, *this);
      }

      const Bitboard southwest_of_pawn = southwest_of(single_pawn);
      const bool can_capture_southwest =
          static_cast<bool>(white_pieces() & southwest_of_pawn);
      if (can_capture_southwest) {
        res->emplace_back(single_pawn, southwest_of_pawn, Piece::pawn,
                         MoveType::capture, *this);
      }
    }
  }
}

void Board::append_pseudolegal_pawn_moves(Color side,
                                          std::vector<Move>* res) const {
  append_pseudolegal_simple_pawn_moves(side, res);
  append_pseudolegal_two_step_pawn_moves(side, res);
  append_pseudolegal_pawn_captures(side, res);
  append_pseudolegal_en_passant_moves(side, res);
  append_pseudolegal_promotions(side, res);
}

void Board::append_pseudolegal_king_moves(Color side,
                                          std::vector<Move>* res) const {
  const Bitboard king_sq = side == Color::white ? white_king_ : black_king_;
  const Bitboard friends_mask = friends(side);

  for (Direction dir : all_directions) {
    auto dir_fn = direction_to_function(dir);
    const Bitboard dst_square = dir_fn(king_sq);
    const bool friendly_piece_on_dst_square =
        static_cast<bool>(dst_square & friends_mask);
    if (dst_square && !friendly_piece_on_dst_square) {
      const bool enemy_piece_on_dst_square =
          static_cast<bool>(dst_square & enemies(side));
      const MoveType move_type =
          enemy_piece_on_dst_square ? MoveType::capture : MoveType::simple;
      res->emplace_back(king_sq, dst_square, Piece::king, move_type, *this);
    }
  }
}

void Board::append_pseudolegal_knight_moves(Color side,
                                            std::vector<Move>* res) const {
  const Bitboard knights = side == Color::white ? white_knights_ : black_knights_;
  const Bitboard friends_mask = friends(side);

  for (Bitboard knight_sq : bitboard_split(knights)) {
    for (auto dir1_dir2 : knight_move_directions) {
      auto dir1_fn = direction_to_function(dir1_dir2.first);
      auto dir2_fn = direction_to_function(dir1_dir2.second);
      const Bitboard first_step = dir1_fn(knight_sq);
      if (first_step) {
        const Bitboard dst_square = dir2_fn(first_step);
        const bool friendly_piece_on_dst_square =
            static_cast<bool>(dst_square & friends_mask);
        if (dst_square && !friendly_piece_on_dst_square) {
          const bool enemy_piece_on_dst_square =
              static_cast<bool>(dst_square & enemies(side));
          const MoveType move_type =
              enemy_piece_on_dst_square ? MoveType::capture : MoveType::simple;
          res->emplace_back(knight_sq, dst_square, Piece::knight, move_type,
                                *this);
        }
      }
    }
  }
}

std::vector<Move> Board::pseudolegal_moves(Color side) const {
  std::vector<Move> res;
  append_pseudolegal_bishop_moves(side, &res);
  append_pseudolegal_rook_moves(side, &res);
  append_pseudolegal_queen_moves(side, &res);
  append_pseudolegal_pawn_moves(side, &res);
  append_pseudolegal_king_moves(side, &res);
  append_pseudolegal_knight_moves(side, &res);
  return res;
}

bool Board::is_castle_kingside_legal() const {
  const bool has_right_to_castle = is_whites_move_
                                 ? white_has_right_to_castle_kingside_
                                 : black_has_right_to_castle_kingside_;
  const Color side_to_move = is_whites_move_ ? Color::white : Color::black;
  if (!has_right_to_castle || is_king_attacked(side_to_move)) {
    return false;
  }

  const Bitboard castle_squares =
      is_whites_move_ ? white_castle_kingside_mask : black_castle_kingside_mask;
  const Bitboard enemy_attack_squares = attack_squares(flip_color(side_to_move));
  const bool castle_squares_clear = !(castle_squares & all_pieces());
  const bool castle_squares_attacked =
      static_cast<const bool>(castle_squares & enemy_attack_squares);

  return castle_squares_clear && !castle_squares_attacked;
}

bool Board::is_castle_queenside_legal() const {
  const bool has_right_to_castle = is_whites_move_
                                       ? white_has_right_to_castle_queenside_
                                       : black_has_right_to_castle_queenside_;
  const Color side_to_move = is_whites_move_ ? Color::white : Color::black;
  if (!has_right_to_castle || is_king_attacked(side_to_move)) {
    return false;
  }

  const Bitboard castle_squares = is_whites_move_ ? white_castle_queenside_mask
                                                  : black_castle_queenside_mask;
  const Bitboard enemy_attack_squares = is_whites_move_
                                            ? attack_squares(Color::black)
                                            : attack_squares(Color::white);
  const bool castle_squares_attacked =
      static_cast<const bool>(castle_squares & enemy_attack_squares);
  // When castling queenside the square on the b file can be attacked but must
  // not be occupied.
  const Bitboard b_file_square =
      is_whites_move_ ? str_to_square("b1") : str_to_square("b8");
  const Bitboard castle_squares_with_b_file = castle_squares | b_file_square;
  const bool castle_squares_blocked =
      static_cast<const bool>(castle_squares_with_b_file & all_pieces());

  return !castle_squares_blocked && !castle_squares_attacked;
}

void Board::append_castling_moves(std::vector<Move>* res) const {
  const Color side_to_move = is_whites_move_ ? Color::white : Color::black;
  if (is_castle_kingside_legal()) {
    res->push_back(castle_kingside_move(side_to_move, *this));
  }
  if (is_castle_queenside_legal()) {
    res->push_back(castle_queenside_move(side_to_move, *this));
  }
}

bool Board::is_king_attacked(Color side) const {
  const Bitboard king = side == Color::white ? white_king_ : black_king_;
  return static_cast<bool>(king & attack_squares(flip_color(side)));
}

bool Board::is_pseudolegal_move_legal(Move move) {
  // // Copying the board is not very efficient but allows this method to be const.
  // Board this_copy(*this);
  // const Color side_to_move = this_copy.is_whites_move_ ? Color::white : Color::black;
  // this_copy.do_move(move);
  // return !this_copy.is_king_attacked(side_to_move);
  const Color side_to_move = is_whites_move_ ? Color::white : Color::black;
  do_move(move);
  const bool res = !is_king_attacked(side_to_move);
  undo_move(move);
  return res;
}

std::vector<Move> Board::legal_moves() {
  const Color side_to_move = is_whites_move_ ? Color::white : Color::black;
  std::vector<Move> res = pseudolegal_moves(side_to_move);
  auto last_it = std::remove_if(res.begin(), res.end(), [this](Move move) {
    return !is_pseudolegal_move_legal(move);
  });

  res.erase(last_it, res.end());
  append_castling_moves(&res);
  return res;
}

// Remember to reset ep square and change castling rights after all of these.

void Board::remove_piece_on(Bitboard sq) {
  for (Bitboard* bb_ptr : all_bitboards()) {
    if (sq & *bb_ptr) {
      *bb_ptr ^= sq;
      break;
    }
  }
}

void Board::do_en_passant_move(Move move) {
  ABSL_RAW_CHECK(en_passant_square_ == move.dst_square_,
                 "Move type is en passant. But the e.p. square is not set.");
  Bitboard enemy_pawn_square = move.dst_square_ & third_rank_mask
                                   ? north_of(move.dst_square_)
                                   : south_of(move.dst_square_);
  remove_piece_on(enemy_pawn_square);
  do_simple_move(Move(move.src_square_, move.dst_square_, Piece::pawn,
                      MoveType::simple, *this));
}

void Board::do_castle_move(Move move) {
  do_simple_move(move);
  switch (move.move_type_) {
    case MoveType::castle_kingside:
      if (is_whites_move_) {
        // Using do_simple_move is a bit of a hack.
        ABSL_RAW_CHECK(white_rooks_ & str_to_square("h1"), "No rook here.");
        do_simple_move(Move(str_to_square("h1"), str_to_square("f1"),
                            Piece::rook, MoveType::simple, *this));
      } else {
        ABSL_RAW_CHECK(black_rooks_ & str_to_square("h8"), "No rook here.");
        do_simple_move(Move(str_to_square("h8"), str_to_square("f8"),
                            Piece::rook, MoveType::simple, *this));
      }
      break;
    case MoveType::castle_queenside:
      if (is_whites_move_) {
        ABSL_RAW_CHECK(white_rooks_ & str_to_square("a1"), "No rook here.");
        do_simple_move(Move(str_to_square("a1"), str_to_square("d1"),
                            Piece::rook, MoveType::simple, *this));
      } else {
        ABSL_RAW_CHECK(black_rooks_ & str_to_square("a8"), "No rook here.");
        do_simple_move(Move(str_to_square("a8"), str_to_square("d8"),
                            Piece::rook, MoveType::simple, *this));
      }
      break;
    default:
      ABSL_RAW_CHECK(false, "Castle move has wrong move type.");
  }
}

void Board::do_promotion_move(Move move) {
  // Ugly hack.
  if (move.dst_square_ == str_to_square("a1")) {
    white_has_right_to_castle_queenside_ = false;
  }
  if (move.dst_square_ == str_to_square("a8")) {
    black_has_right_to_castle_queenside_ = false;
  }
  if (move.dst_square_ == str_to_square("h1")) {
    white_has_right_to_castle_kingside_ = false;
  }
  if (move.dst_square_ == str_to_square("h8")) {
    black_has_right_to_castle_kingside_ = false;
  }
  remove_piece_on(move.src_square_);
  remove_piece_on(move.dst_square_);
  switch (move.move_type_) {
    case MoveType::promotion_to_rook:
      if (is_whites_move_) {
        white_rooks_ |= move.dst_square_;
      } else {
        black_rooks_ |= move.dst_square_;
      }
      break;
    case MoveType::promotion_to_bishop:
      if (is_whites_move_) {
        white_bishops_ |= move.dst_square_;
      } else {
        black_bishops_ |= move.dst_square_;
      }
      break;
    case MoveType::promotion_to_knight:
      if (is_whites_move_) {
        white_knights_ |= move.dst_square_;
      } else {
        black_knights_ |= move.dst_square_;
      }
      break;
    case MoveType::promotion_to_queen:
      if (is_whites_move_) {
        white_queens_ |= move.dst_square_;
      } else {
        black_queens_ |= move.dst_square_;
      }
      break;
    default:
      ABSL_RAW_CHECK(false, "Promotion move has wrong move type.");
  }
  en_passant_square_ = absl::nullopt;
}

void Board::do_capture_move(Move move) {
  remove_piece_on(move.dst_square_);
  do_simple_move(move);
}

void Board::do_simple_move(Move move) {
  // Ugly hack.
  if (move.dst_square_ == str_to_square("a1")) {
    white_has_right_to_castle_queenside_ = false;
  }
  if (move.dst_square_ == str_to_square("a8")) {
    black_has_right_to_castle_queenside_ = false;
  }
  if (move.dst_square_ == str_to_square("h1")) {
    white_has_right_to_castle_kingside_ = false;
  }
  if (move.dst_square_ == str_to_square("h8")) {
    black_has_right_to_castle_kingside_ = false;
  }
  bool found = false;
  for (Bitboard* bb_ptr : all_bitboards()) {
    if (move.src_square_ & *bb_ptr) {
      *bb_ptr ^= move.src_square_;
      *bb_ptr |= move.dst_square_;
      found = true;
      break;
    }
  }
  ABSL_RAW_CHECK(found, "Move not valid");
  if (move.move_type_ == MoveType::two_step_pawn) {
    en_passant_square_ = move.src_square_ & second_rank_mask
                             ? north_of(move.src_square_)
                             : south_of(move.src_square_);
  } else {
    en_passant_square_ = absl::nullopt;
  }
  if (move.piece_moving_ == Piece::king) {
    if (move.src_square_ == str_to_square("e1")) {
      white_has_right_to_castle_kingside_ = false;
      white_has_right_to_castle_queenside_ = false;
    } else if (move.src_square_ == str_to_square("e8")) {
      black_has_right_to_castle_kingside_ = false;
      black_has_right_to_castle_queenside_ = false;
    }
  }
  if (move.piece_moving_ == Piece::rook) {
    if (move.src_square_ == str_to_square("a1")) {
      white_has_right_to_castle_queenside_ = false;
    } else if (move.src_square_ == str_to_square("h1")) {
      white_has_right_to_castle_kingside_ = false;
    } else if (move.src_square_ == str_to_square("a8")) {
      black_has_right_to_castle_queenside_ = false;
    } else if (move.src_square_ == str_to_square("h8")) {
      black_has_right_to_castle_kingside_ = false;
    }
  }
}

void Board::undo_move(Move move) { *this = move.board_state_; }

void Board::do_move(Move move) {
  ABSL_RAW_CHECK(is_square(move.src_square_) && is_square(move.dst_square_),
                 "Not a valid move.");
  // Ugly hack.
  if (move.dst_square_ == str_to_square("a1")) {
    white_has_right_to_castle_queenside_ = false;
  }
  if (move.dst_square_ == str_to_square("a8")) {
    black_has_right_to_castle_queenside_ = false;
  }
  if (move.dst_square_ == str_to_square("h1")) {
    white_has_right_to_castle_kingside_ = false;
  }
  if (move.dst_square_ == str_to_square("h8")) {
    black_has_right_to_castle_kingside_ = false;
  }
  // std::string b = to_pretty_str();
  // b.append(is_whites_move_ ? "White to move\n" : "Black to move\n");
  // b.append(is_king_attacked(is_whites_move_ ? Color::white : Color::black) ?
  // "King is attacked\n" : "King is not attacked\n");
  // b.append(bb_to_pretty_str(attack_squares(is_whites_move_ ? Color::black :
  // Color::white)));

  switch (move.move_type_) {
    case MoveType::simple:
    case MoveType::two_step_pawn:
      do_simple_move(move);
      break;
    case MoveType::capture:
      do_capture_move(move);
      break;
    case MoveType::en_passant:
      do_en_passant_move(move);
      break;
    case MoveType::castle_kingside:
    case MoveType::castle_queenside:
      // b.append(is_king_attacked(is_whites_move_ ? Color::white :
      // Color::black) ? "King is attacked\n" : "King is not attacked\n");
      ABSL_RAW_CHECK(
          !is_king_attacked(is_whites_move_ ? Color::white : Color::black),
          "Castling while in check.");
      do_castle_move(move);
      ABSL_RAW_CHECK(
          !is_king_attacked(is_whites_move_ ? Color::white : Color::black),
          "Castled into check");
      break;
    case MoveType::promotion_to_rook:
    case MoveType::promotion_to_bishop:
    case MoveType::promotion_to_knight:
    case MoveType::promotion_to_queen:
      do_promotion_move(move);
      break;
  }
  is_whites_move_ = !is_whites_move_;
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

void Board::init_bitboards(const absl::string_view pieces_fen) {
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

void Board::init_is_whites_move(const absl::string_view side_to_move) {
  ABSL_RAW_CHECK(side_to_move == "w" || side_to_move == "b",
                 "Invalid FEN. Side to move must be either w or b");
  if (side_to_move == "w") {
    is_whites_move_ = true;
  } else if (side_to_move == "b") {
    is_whites_move_ = false;
  }
}

void Board::init_castling_rights(const absl::string_view castling_rights_fen) {
  white_has_right_to_castle_kingside_ =
      (castling_rights_fen.find('K') != std::string::npos);
  white_has_right_to_castle_queenside_ =
      (castling_rights_fen.find('Q') != std::string::npos);
  black_has_right_to_castle_kingside_ =
      (castling_rights_fen.find('k') != std::string::npos);
  black_has_right_to_castle_queenside_ =
      (castling_rights_fen.find('q') != std::string::npos);
}

void Board::init_en_passant(const absl::string_view algebraic_square) {
  if (algebraic_square == "-") {
    en_passant_square_ = absl::nullopt;
  } else {
    en_passant_square_ = str_to_square(algebraic_square);
  }
}

bool operator==(const Board& lhs, const Board& rhs) {
  return std::tie(lhs.white_pawns_, lhs.white_rooks_, lhs.white_knights_,
                  lhs.white_bishops_, lhs.white_queens_, lhs.white_king_,
                  lhs.black_pawns_, lhs.black_rooks_, lhs.black_knights_,
                  lhs.black_bishops_, lhs.black_queens_, lhs.black_king_,
                  lhs.is_whites_move_, lhs.en_passant_square_,
                  lhs.white_has_right_to_castle_kingside_,
                  lhs.white_has_right_to_castle_queenside_,
                  lhs.black_has_right_to_castle_kingside_,
                  lhs.black_has_right_to_castle_queenside_,
                  lhs.fifty_move_clock_, lhs.num_moves_) ==
         std::tie(rhs.white_pawns_, rhs.white_rooks_, rhs.white_knights_,
                  rhs.white_bishops_, rhs.white_queens_, rhs.white_king_,
                  rhs.black_pawns_, rhs.black_rooks_, rhs.black_knights_,
                  rhs.black_bishops_, rhs.black_queens_, rhs.black_king_,
                  rhs.is_whites_move_, rhs.en_passant_square_,
                  rhs.white_has_right_to_castle_kingside_,
                  rhs.white_has_right_to_castle_queenside_,
                  rhs.black_has_right_to_castle_kingside_,
                  rhs.black_has_right_to_castle_queenside_,
                  rhs.fifty_move_clock_, rhs.num_moves_);
}

Move::Move()
    : src_square_(0),
      dst_square_(0),
      piece_moving_(Piece::pawn),
      move_type_(MoveType::simple),
      board_state_(Board()) {}

Move::Move(Bitboard p_src_square, Bitboard p_dst_square, Piece p_piece_moving,
           MoveType p_move_type, Board p_board_state)
    : src_square_(p_src_square),
      dst_square_(p_dst_square),
      piece_moving_(p_piece_moving),
      move_type_(p_move_type),
      board_state_(p_board_state) {}

std::string Move::to_pretty_str() const {
  return absl::StrCat(square_to_str(src_square_), square_to_str(dst_square_));
}

void PrintTo(const Move& move, std::ostream* os) {
  *os << move.to_pretty_str();
}

bool operator==(const Move& lhs, const Move& rhs) {
  return std::tie(lhs.src_square_, lhs.dst_square_, lhs.piece_moving_,
                  lhs.move_type_, lhs.board_state_) ==
         std::tie(rhs.src_square_, rhs.dst_square_, rhs.piece_moving_,
                  rhs.move_type_, rhs.board_state_);
}

// Check that the bitboard has exactly one bit set.
bool is_square(Bitboard square) {
  const bool has_more_than_one_set_bit =
      static_cast<const bool>(square & (square - 1));
  return square && !has_more_than_one_set_bit;
}

int square_idx(Bitboard square) {
  // ABSL_RAW_CHECK(is_square(square), "Is not square.");
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

Bitboard north_of(Bitboard square) {
  ABSL_RAW_CHECK(is_square(square), "Is not square.");
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

Bitboard str_to_square(const absl::string_view algebraic_square) {
  const char file_char = algebraic_square[0];
  const char rank_char = algebraic_square[1];
  ABSL_RAW_CHECK('a' <= file_char && file_char <= 'h', "Invalid file.");
  ABSL_RAW_CHECK('1' <= rank_char && rank_char <= '8', "Invalid rank.");
  const int file = file_char - 'a';
  const int rank = rank_char - '1';
  return coordinates_to_square(file, rank);
}

std::string square_to_str(Bitboard sq) {
  ABSL_RAW_CHECK(is_square(sq), "Not a square.");
  char rank = static_cast<char>(rank_idx(sq));
  char file = static_cast<char>(file_idx(sq));
  std::string res;
  res.push_back('a' + file);
  res.push_back('1' + rank);
  return res;
}

Bitboard coordinates_to_square(int file, int rank) {
  return lsb_bitboard << (rank * board_size + board_size - file - 1);
}

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

Color flip_color(Color color) {
  return color == Color::white ? Color::black : Color::white;
}

std::string bb_to_pretty_str(Bitboard bb) {
  Board board;
  board.white_pawns_ = 0;
  board.white_rooks_ = 0;
  board.white_knights_ = 0;
  board.white_bishops_ = 0;
  board.white_queens_ = 0;
  board.white_king_ = 0;
  board.black_pawns_ = 0;
  board.black_rooks_ = 0;
  board.black_knights_ = 0;
  board.black_bishops_ = 0;
  board.black_queens_ = 0;
  board.black_king_ = 0;
  board.black_pawns_ = bb;
  return board.to_pretty_str();
}

Move castle_kingside_move(Color color, Board board) {
  if (color == Color::white) {
    return Move(str_to_square("e1"), str_to_square("g1"), Piece::king,
                MoveType::castle_kingside, board);
  } else {
    return Move(str_to_square("e8"), str_to_square("g8"), Piece::king,
                MoveType::castle_kingside, board);
  }
}

Move castle_queenside_move(Color color, Board board) {
  if (color == Color::white) {
    return Move(str_to_square("e1"), str_to_square("c1"), Piece::king,
                MoveType::castle_queenside, board);
  } else {
    return Move(str_to_square("e8"), str_to_square("c8"), Piece::king,
                MoveType::castle_queenside, board);
  }
}

int number_of_moves(Board board, int half_move_depth) {
  if (half_move_depth == 0) {
    return 1;
  }
  int res = 0;
  std::vector<Move> moves = board.legal_moves();
  for (Move move : moves) {
    board.do_move(move);
    res += number_of_moves(board, half_move_depth - 1);
    board.undo_move(move);
  }
  return res;
}
