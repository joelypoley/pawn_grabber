#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <array>

using namespace std;

// Each bit of a Bitboard represents a square. So the unsigned 64 bit integer
// 10000000 00000000 00000000 00000000 00000000 00000000 00000000 00000100
// indicates that the a8 and f1 squares are set. This can be seen by stacking
// consecutive blocks of 8 bits on top of each other, from most to least significant
// bit. So our bitboard from before becomes:
//
// 8 | 10000000
// 7 | 00000000
// 6 | 00000000
// 5 | 00000000
// 4 | 00000000  
// 3 | 00000000
// 2 | 00000000
// 1 | 00000100
//    -----------
//     abcdefgh

typedef uint64_t Bitboard;

const Bitboard lsb_bitboard {1};

const int board_size {8};

enum File {
	a_file,
	b_file,
	c_file,
	d_file,
	e_file,
	f_file,
	g_file,
	h_file,
};

enum Rank {
	rank_1,
	rank_2,
	rank_3,
	rank_4,
	rank_5,
	rank_6,
	rank_7,
	rank_8
};

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
PieceType ascii_to_piece_type(char c);

class Board {
public:
	Board(string fen);

	array<Bitboard, PieceType::num_piece_types> piece_bitboards;

};

#endif