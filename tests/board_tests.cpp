#include <string>
#include <array>

#include "../board.h"
#include "gtest/gtest.h"

using namespace std;


TEST (BoardConstructor, Startposition) {
	string start_fen {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};

	Board start_board {Board(start_fen)};

	array<Bitboard, num_piece_types> start_bitboards = {
		0xFF00,  	// white pawns
		0x81, 		// white rooks
		0x42, 		// white knights
		0x24, 		// white bishops
		0x10, 		// white queens
		0x8, 		// white king
		0xFF000000000000, 	// black pawns
		0x8100000000000000, // black rooks
		0x4200000000000000, // black knights
		0x2400000000000000, // black bishops
		0x1000000000000000, 	// black queens
		0x800000000000000 	// black king
	};

	EXPECT_EQ(start_board.piece_bitboards, start_bitboards);
}

TEST (BoardConstructor, KingVsKing) {
	string start_fen {"4k3/8/8/8/8/8/8/4K3 w - - 0 1"};

	Board start_board = Board(start_fen);

	array<Bitboard, num_piece_types> start_bitboards = {
		0x0,  	// white pawns
		0x0, 		// white rooks
		0x0, 		// white knights
		0x0, 		// white bishops
		0x0, 		// white queens
		0x8, 		// white king
		0x0, 	// black pawns
		0x0, // black rooks
		0x0, // black knights
		0x0, // black bishops
		0x0, 	// black queens
		0x800000000000000 	// black king
	};

	EXPECT_EQ(start_board.piece_bitboards, start_bitboards);
}

