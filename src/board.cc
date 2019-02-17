#include <exception>
#include <iostream>
#include <sstream>
#include <string>

#include "src/board.h"

using namespace std;

Board::Board() : Board(start_pos_fen) {}

Board::Board(string fen) {
  istringstream fen_stream{fen};

  string pieces_fen;
  fen_stream >> pieces_fen;

  init_pieces(pieces_fen);

  string side_to_move_fen;
  fen_stream >> side_to_move_fen;
  init_color(side_to_move_fen);

  string castling_rights_fen;
  fen_stream >> castling_rights_fen;
  init_castling_rights(castling_rights_fen);

  string en_passant_fen;
  fen_stream >> en_passant_fen;
  init_en_passant(en_passant_fen);

  fen_stream >> fifty_move;

  fen_stream >> num_moves;
}

void Board::init_pieces(const string& pieces_fen) {
  for (auto& bb : piece_bitboards) {
    bb = 0;
  }  // TODO: move to init pieces
  Rank rank = board_size - 1;
  File file = 0;

  for (auto c : pieces_fen) {
    Square square_mask{lsb_bitboard
                       << (rank * board_size + board_size - file - 1)};
    switch (c) {
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
      default:
        PieceType piece_type = ascii_to_piece_type(c);
        piece_bitboards[piece_type] |= square_mask;
        file += 1;
        break;
    }
  }
}

void Board::init_color(const string& side_to_move_fen) {
  if (side_to_move_fen == "w") {
    side_to_move = Color::white;
  } else if (side_to_move_fen == "b") {
    side_to_move = Color::black;
  } else {
    throw exception();
  }
}

void Board::init_castling_rights(const string& castling_rights_fen) {
  for (auto& x : castling_rights) {
    x = false;
  }

  if (castling_rights_fen.find("K") != string::npos) {
    castling_rights[white_kingside] = true;
  }
  if (castling_rights_fen.find("Q") != string::npos) {
    castling_rights[white_queenside] = true;
  }
  if (castling_rights_fen.find("k") != string::npos) {
    castling_rights[black_kingside] = true;
  }
  if (castling_rights_fen.find("q") != string::npos) {
    castling_rights[black_queenside] = true;
  }
}

void Board::init_en_passant(const string& algebraic_square) {
  if (algebraic_square == "-") {
    en_passant = no_square;
  } else {
    en_passant = algebraic_to_square(algebraic_square);
  }
}

char file_to_ascii(const File& file) {
  if ((0 <= file) && (file < board_size)) {
    return 'a' + file;
  } else {
    throw exception();
  }
}

char rank_to_ascii(const File& rank) {
  if ((0 <= rank) && (rank < board_size)) {
    return '1' + rank;
  } else {
    throw exception();
  }
}

ostream& operator<<(ostream& os, const Board& board) {
  string top_left_corner{"┌"};      // U+250C
  string top_right_corner{"┐"};     // U+2510
  string bottom_left_corner{"└"};   // U+2514
  string bottom_right_corner{"┘"};  // U+2518
  string horizontal{"─"};           // U+2500
  string vertical{"│"};             // U+2502
  string left_side_tee{"├"};        // U+251C
  string right_side_tee{"┤"};       // U+2524
  string top_tee{"┬"};              // U+252C
  string bottom_tee{"┴"};           // U+2534
  string cross{"┼"};                // U+253C

  // do top of board
  os << ' ' << ' ' << top_left_corner;
  for (int i = 0; i < board_size - 1; ++i) {
    os << horizontal << horizontal << horizontal << top_tee;
  }
  os << horizontal << horizontal << horizontal << top_right_corner;
  os << endl;

  // do rows
  string curr_piece{' '};
  for (Rank rank = board_size - 1; rank >= 0; --rank) {
    // print a row of pieces
    os << rank_to_ascii(rank) << ' ' << vertical;
    for (File file = 0; file < board_size; ++file) {
      for (int i = 0; i < board.piece_bitboards.size(); ++i) {
        if ((coordinates_to_square(file, rank) & board.piece_bitboards[i]) !=
            0) {
          curr_piece = piece_type_to_symbol(PieceType(i));
          break;
        }
      }
      os << ' ' << curr_piece << ' ' << vertical;
      curr_piece = ' ';
    }
    os << endl;
    // print a dividing line (unless where at the last rank)

    if (rank != 0) {
      os << ' ' << ' ';
      os << left_side_tee;
      for (int i = 0; i < board_size - 1; ++i) {
        os << horizontal << horizontal << horizontal << cross;
      }
      os << horizontal << horizontal << horizontal << right_side_tee;
      os << endl;
    }
  }

  // do bottom of board
  os << ' ' << ' ' << bottom_left_corner;
  for (int i = 0; i < board_size - 1; ++i) {
    os << horizontal << horizontal << horizontal << bottom_tee;
  }
  os << horizontal << horizontal << horizontal << bottom_right_corner;
  os << endl;

  // TODO: make this better
  // do coordinates
  os << ' ' << ' ' << ' ';
  for (char i = 0; i < board_size - 1; ++i) {
    os << ' ' << file_to_ascii(i) << ' ' << ' ';
  }
  os << ' ' << 'h' << ' ' << ' ';
  os << endl;

  return os;
}

PieceType ascii_to_piece_type(const char& c) {
  switch (c) {
    case 'P':
      return PieceType::white_pawn;
    case 'R':
      return PieceType::white_rook;
    case 'N':
      return PieceType::white_knight;
    case 'B':
      return PieceType::white_bishop;
    case 'Q':
      return PieceType::white_queen;
    case 'K':
      return PieceType::white_king;
    case 'p':
      return PieceType::black_pawn;
    case 'r':
      return PieceType::black_rook;
    case 'n':
      return PieceType::black_knight;
    case 'b':
      return PieceType::black_bishop;
    case 'q':
      return PieceType::black_queen;
    case 'k':
      return PieceType::black_king;
    default:
      throw exception();
  }
}

Square algebraic_to_square(const string& algebraic_square) {
  File file{algebraic_square[0] - 'a'};
  Rank rank{algebraic_square[1] - 1};

  Square square{lsb_bitboard << (rank * board_size + board_size - file - 1)};

  return square;
}

Square coordinates_to_square(File file, Rank rank) {
  Square square{lsb_bitboard << (rank * board_size + board_size - file - 1)};
  return square;
}

string piece_type_to_symbol(PieceType piece_type) {
                           switch (piece_type) {
    case white_pawn:
      return "♙";  // U+2564
    case white_rook:
      return "♖";  // U+2656
    case white_knight:
      return "♘";  // U+2658
    case white_bishop:
      return "♗";  // U+2657
    case white_queen:
      return "♕";  // U+2655
    case white_king:
      return "♔";  // U+2654
    case black_pawn:
      return "♟";  // U+265F
    case black_rook:
      return "♜";  // U+265C
    case black_knight:
      return "♞";  // U+265E
    case black_bishop:
      return "♝";  // U+265D
    case black_queen:
      return "♛";  // U+265B
    case black_king:
      return "♚";  // U+265A
    default:
      throw exception();
  }
}
