#include "board.h"
#include <string>
#include <sstream>
//#include <iostream>

using namespace std;

PieceType ascii_to_piece_type(char c) {
    switch (c) {
        case 'P': return PieceType::white_pawn;
        case 'R': return PieceType::white_rook;
        case 'N': return PieceType::white_knight;
        case 'B': return PieceType::white_bishop;
        case 'Q': return PieceType::white_queen;
        case 'K': return PieceType::white_king;
        case 'p': return PieceType::black_pawn;
        case 'r': return PieceType::black_rook;
        case 'n': return PieceType::black_knight;
        case 'b': return PieceType::black_bishop; 
        case 'q': return PieceType::black_queen;
        case 'k': return PieceType::black_king;
        default: throw exception();
	}
}

void init_pieces(string pieces_fen, array<Bitboard, PieceType::num_piece_types>& piece_bitboards) {
	int rank = Rank::rank_8;
	int file = File::a_file;

	for (auto c : pieces_fen) {
		Bitboard square_mask = {lsb_bitboard << (rank * board_size + board_size - file - 1)};
		switch (c) {
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':	file += (c - '0');
						break;
			case '/':	rank -= 1;
						file = 0;
						break;
			default:	PieceType piece_type = ascii_to_piece_type(c);
			 			piece_bitboards[piece_type] |= square_mask;
			 			file += 1;
			 			break;
		}
	}
}

Board::Board(string fen) {
	istringstream fen_stream {fen};

	string pieces_fen {fen_stream.str()};
	fen_stream >> pieces_fen;
	for (auto& bb : piece_bitboards) {
		bb = 0;
	}
	init_pieces(pieces_fen, piece_bitboards);



}

