#include <cstdlib>
//for debugging
//#include <iostream>

#include "transposition_table.h"
#include "board.h"

using namespace std;

ZobristTable init_zobrist_table() {
    ZobristTable table;
    srand(16121992);
    for (int i = 0; i < PieceType::num_piece_types; ++i) {
        for (size_t j = 0; j < board_size * board_size; ++j) {
            table[i][j] = rand();
        }
    }
    return table;
}

int zobrist(PositionArray piece_bitboards) {
    int key {0};

    for (size_t i = 0; i < piece_bitboards.size(); ++i) {
        Bitboard curr_bitboard {piece_bitboards[i]};
        for (int j = 0; j < board_size * board_size; ++j) {
            if (((curr_bitboard >> j) & lsb_bitboard) != 0) {
                key ^= zobrist_table[i][j];
            }
        }
    }
    return key;
}
