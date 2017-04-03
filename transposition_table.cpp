#include "transposition_table.h"
using namespace std;

array<Bitboard, board_size * board_size> init_zobrist() {
    array<Bitboard, board_size * board_size> zobrist_table;
    return zobrist_table;
}

uint64_t zobrist(array<Bitboard, PieceType::num_piece_types> piece_bitboards) {
    // TODO: implement zobrist hash
    return 0;
}