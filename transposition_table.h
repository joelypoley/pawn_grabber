#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include <array>

#include "board.h"

using namespace std;

array<Bitboard, board_size * board_size> init_zobrist();

const array<uint64_t, board_size * board_size> zobrist_table {init_zobrist()};

uint64_t zobrist(array<Bitboard, PieceType::num_piece_types> piece_bitboards);



#endif