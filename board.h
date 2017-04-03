#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <array>

using namespace std;

/*
Each bit of a Bitboard represents a square. So the unsigned 64 bit integer
10000000 00000000 00000000 00000000 00000000 00000000 00000000 00000100
indicates that the a8 and f1 squares are set. This can be seen by stacking
consecutive blocks of 8 bits on top of each other, from most to least significant
bit. So our bitboard from before becomes:

8 | 1000 0000
7 | 0000 0000
6 | 0000 0000
5 | 0000 0000
4 | 0000 0000  
3 | 0000 0000
2 | 0000 0000
1 | 0000 0100
   -----------
    abcd efgh
*/
typedef uint64_t Bitboard;

typedef uint64_t Square;

typedef int File;

typedef int Rank;

typedef int Hash;

const Bitboard lsb_bitboard {1};

const int board_size {8};

const Square no_square {0xFFFFFFFFFFFFFFFF};

const string start_pos_fen {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};



enum PieceType {
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
	num_piece_types
};

enum CastlingRight {
	white_kingside,
	white_queenside,
	black_kingside,
	black_queenside,
	num_castling_rights
};

typedef array<Bitboard, PieceType::num_piece_types> PositionArray;
typedef array<bool, CastlingRight::num_castling_rights> CastlingArray;

enum class Color {
	white,
	black
};



PieceType ascii_to_piece_type(const char& c);
Square algebraic_to_square(const string& alegbraic_square);
Square coordinates_to_square(File file, Rank rank);
string piece_type_to_symbol(PieceType piece_type);


class Board {
public:
	Board();
	Board(string fen);
	PositionArray piece_bitboards;
	Color side_to_move;
	CastlingArray castling_rights;
	Square en_passant;
	int fifty_move;
	int num_moves;
	Hash pos_hash;

private:
	void init_pieces(const string& pieces_fen);
	void init_color(const string& side_to_move_fen);
	void init_castling_rights(const string& castling_rights_fen);
	void init_en_passant(const string& algebraic_square);
};

ostream& operator<<(ostream& os, const Board& board);
char file_to_ascii(const File& file);
char rank_to_ascii(const File& rank);

#endif