#include "board.h"
#include <string>
#include <sstream>
//#include <iostream>

using namespace std;

PieceType ascii_to_piece_type(const char& c) {
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

Square algebraic_to_square(const string& algebraic_square) {
    File file {algebraic_square[0] - 'a'};
    Rank rank {algebraic_square[1] - 1};

    Square square {lsb_bitboard << (rank * board_size + board_size - file - 1)};

    return square;

}

Board::Board(string fen) {
    istringstream fen_stream {fen};

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
    } // TODO: move to init pieces
	Rank rank = board_size - 1;
	File file = 0;

	for (auto c : pieces_fen) {
		Square square_mask {lsb_bitboard << (rank * board_size + board_size - file - 1)};
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
    for (auto& x: castling_rights) {
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
