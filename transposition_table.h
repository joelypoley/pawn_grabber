#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include <array>
#include <unordered_map>

#include "board.h"

using namespace std;

typedef array<array<Hash, board_size * board_size>, PieceType::num_piece_types > ZobristTable;

extern unordered_map<Hash, int> transposition_table;

ZobristTable init_zobrist_table();

const ZobristTable zobrist_table {init_zobrist_table()};

Hash zobrist(PositionArray piece_bitboards);




#endif