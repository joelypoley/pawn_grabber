#include <string>
#include <array>

#include "../board.h"
#include "../transposition_table.h"
#include "gtest/gtest.h"

using namespace std;
//TODO: Add some middle game positions
//TODO: Check the zobrist hash for more positions. 

TEST (BoardConstructor, Startposition) {
	Board start_board;

	PositionArray start_bitboards = {
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

	CastlingArray correct_rights {
		true,	// white kingside
		true,	// white queenside
		true,	// black kingside
		true	// black queenside
	};

	Square correct_en_passant {no_square};
	int correct_fifty_move {0};
	int correct_num_moves {1};

	EXPECT_EQ(start_board.piece_bitboards, start_bitboards);
	EXPECT_EQ(start_board.side_to_move, Color::white);
	EXPECT_EQ(start_board.castling_rights, correct_rights);
	EXPECT_EQ(start_board.en_passant, correct_en_passant);
	EXPECT_EQ(start_board.fifty_move, correct_fifty_move);
	EXPECT_EQ(start_board.num_moves, correct_num_moves);
}

TEST (BoardConstructor, KingsPawn) {
	string pos_fen {"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"};

	Board start_board(pos_fen);

	PositionArray start_bitboards = {
		0x800F700,  	// white pawns
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

	CastlingArray correct_rights {
		true,	// white kingside
		true,	// white queenside
		true,	// black kingside
		true	// black queenside
	};

	Square correct_en_passant {0x80000};
	int correct_fifty_move {0};
	int correct_num_moves {1};

	EXPECT_EQ(start_board.piece_bitboards, start_bitboards);
	EXPECT_EQ(start_board.side_to_move, Color::black);
	EXPECT_EQ(start_board.castling_rights, correct_rights);
	EXPECT_EQ(start_board.en_passant, correct_en_passant);
	EXPECT_EQ(start_board.fifty_move, correct_fifty_move);
	EXPECT_EQ(start_board.num_moves, correct_num_moves);
}

TEST (BoardConstructor, Sicilian) {
	string pos_fen {"rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"};

	Board start_board(pos_fen);

	PositionArray start_bitboards {
		0x800F700,  	// white pawns
		0x81, 		// white rooks
		0x42, 		// white knights
		0x24, 		// white bishops
		0x10, 		// white queens
		0x8, 		// white king
		0xDF002000000000, 	// black pawns
		0x8100000000000000, // black rooks
		0x4200000000000000, // black knights
		0x2400000000000000, // black bishops
		0x1000000000000000, 	// black queens
		0x800000000000000 	// black king
	};

	CastlingArray correct_rights {
		true,	// white kingside
		true,	// white queenside
		true,	// black kingside
		true	// black queenside
	};

	Square correct_en_passant {0x200000000000};
	int correct_fifty_move {0};
	int correct_num_moves {2};

	EXPECT_EQ(start_board.piece_bitboards, start_bitboards);
	EXPECT_EQ(start_board.side_to_move, Color::white);
	EXPECT_EQ(start_board.castling_rights, correct_rights);
	EXPECT_EQ(start_board.en_passant, correct_en_passant);
	EXPECT_EQ(start_board.fifty_move, correct_fifty_move);
	EXPECT_EQ(start_board.num_moves, correct_num_moves);
}

TEST (BoardConstructor, Nf3Sicilian) {
	string pos_fen {"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"};

	Board start_board(pos_fen);

	PositionArray start_bitboards = {
		0x800F700,  	// white pawns
		0x81, 		// white rooks
		0x40040, 		// white knights
		0x24, 		// white bishops
		0x10, 		// white queens
		0x8, 		// white king
		0xDF002000000000, 	// black pawns
		0x8100000000000000, // black rooks
		0x4200000000000000, // black knights
		0x2400000000000000, // black bishops
		0x1000000000000000, 	// black queens
		0x800000000000000 	// black king
	};

	CastlingArray correct_rights {
		true,	// white kingside
		true,	// white queenside
		true,	// black kingside
		true	// black queenside
	};

	Square correct_en_passant {no_square};
	int correct_fifty_move {1};
	int correct_num_moves {2};

	EXPECT_EQ(start_board.piece_bitboards, start_bitboards);
	EXPECT_EQ(start_board.side_to_move, Color::black);
	EXPECT_EQ(start_board.castling_rights, correct_rights);
	EXPECT_EQ(start_board.en_passant, correct_en_passant);
	EXPECT_EQ(start_board.fifty_move, correct_fifty_move);
	EXPECT_EQ(start_board.num_moves, correct_num_moves);
}

TEST (BoardConstructor, SicilianWith2Ke2) {
	string pos_fen {"rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPPKPPP/RNBQ1BNR b kq - 1 2"};

	Board start_board(pos_fen);

	PositionArray start_bitboards = {
		0x800F700,  	// white pawns
		0x81, 		// white rooks
		0x42, 		// white knights
		0x24, 		// white bishops
		0x10, 		// white queens
		0x800, 		// white king
		0xDF002000000000, 	// black pawns
		0x8100000000000000, // black rooks
		0x4200000000000000, // black knights
		0x2400000000000000, // black bishops
		0x1000000000000000, 	// black queens
		0x800000000000000 	// black king
	};

	CastlingArray correct_rights {
		false,	// white kingside
		false,	// white queenside
		true,	// black kingside
		true	// black queenside
	};

	Square correct_en_passant {no_square};
	int correct_fifty_move {1};
	int correct_num_moves {2};

	EXPECT_EQ(start_board.piece_bitboards, start_bitboards);
	EXPECT_EQ(start_board.side_to_move, Color::black);
	EXPECT_EQ(start_board.castling_rights, correct_rights);
	EXPECT_EQ(start_board.en_passant, correct_en_passant);
	EXPECT_EQ(start_board.fifty_move, correct_fifty_move);
	EXPECT_EQ(start_board.num_moves, correct_num_moves);
}

// TODO: Put some middle games in here as test cases. 

TEST (BoardConstructor, KingVsKing) {
	string KvK_fen {"4k3/8/8/8/8/8/8/4K3 w - - 0 55"};

	Board start_board(KvK_fen);

	PositionArray start_bitboards = {
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

	CastlingArray correct_rights {
		false,	// white kingside
		false,	// white queenside
		false,	// black kingside
		false	// black queenside
	};
	Square correct_en_passant {no_square};
	int correct_fifty_move {0};
	int correct_num_moves {55};
	uint64_t correct_pos_hash {549353003}; // = table[5][3] ^ table[11][59]

	EXPECT_EQ(start_board.piece_bitboards, start_bitboards);
	EXPECT_EQ(start_board.side_to_move, Color::white);
	EXPECT_EQ(start_board.castling_rights, correct_rights);
	EXPECT_EQ(start_board.en_passant, correct_en_passant);
	EXPECT_EQ(start_board.fifty_move, correct_fifty_move);
	EXPECT_EQ(start_board.num_moves, correct_num_moves);
	EXPECT_EQ(start_board.pos_hash, correct_pos_hash);
}

TEST (BoardConstructor, LucenaPosition) {
	string lp {"1K1k4/1P6/8/8/8/8/r7/2R5 w - - 0 60"};

	Board start_board(lp);

	PositionArray start_bitboards = {
		0x40000000000000,  	// white pawns
		0x20, 		// white rooks
		0x0, 		// white knights
		0x0, 		// white bishops
		0x0, 		// white queens
		0x4000000000000000, 		// white king
		0x0, 	// black pawns
		0x8000, // black rooks
		0x0, // black knights
		0x0, // black bishops
		0x0, 	// black queens
		0x1000000000000000 	// black king
	};

	CastlingArray correct_rights {
		false,	// white kingside
		false,	// white queenside
		false,	// black kingside
		false	// black queenside
	};

	Square correct_en_passant {no_square};
	int correct_fifty_move {0};
	int correct_num_moves {60};

	EXPECT_EQ(start_board.piece_bitboards, start_bitboards);
	EXPECT_EQ(start_board.side_to_move, Color::white);
	EXPECT_EQ(start_board.castling_rights, correct_rights);
	EXPECT_EQ(start_board.en_passant, correct_en_passant);
	EXPECT_EQ(start_board.fifty_move, correct_fifty_move);
	EXPECT_EQ(start_board.num_moves, correct_num_moves);
}


