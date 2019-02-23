#include <iostream>

#include "board.h"

int main() {

    Board board = Board();
    std::cout << "Start position.\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 0) << ". Correct result: " <<  1 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 1) << ". Correct result: " <<  20 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 2) << ". Correct result: " <<  400 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 3) << ". Correct result: " <<  8902 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 4) << ". Correct result: " <<  197281 << ".\n";

    board= Board(
                "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    std::cout << "Kiwipete.\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 0) << ". Correct result: " <<  1 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 1) << ". Correct result: " <<  48 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 2) << ". Correct result: " <<  2039 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 3) << ". Correct result: " <<  97862 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 4) << ". Correct result: " <<  4085603 << ".\n";

    board= Board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    std::cout << "Position 3.";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 0) << ". Correct result: " <<  1 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 1) << ". Correct result: " <<  14 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 2) << ". Correct result: " <<  191 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 3) << ". Correct result: " <<  2812 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 4) << ". Correct result: " <<  43238 << ".\n";

    board= Board("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    std::cout << "Position 4.";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 0) << ". Correct result: " <<  1 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 1) << ". Correct result: " <<  6 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 2) << ". Correct result: " <<  264 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 3) << ". Correct result: " <<  9467 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 4) << ". Correct result: " <<  422333 << ".\n";

    board= Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    std::cout << "Position 5.";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 0) << ". Correct result: " <<  1 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 1) << ". Correct result: " <<  44 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 2) << ". Correct result: " <<  1486 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 3) << ". Correct result: " <<  62379 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 4) << ". Correct result: " <<  2103487 << ".\n";

    board= Board(
                "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 "
                "10");
    std::cout << "Position 6.";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 0) << ". Correct result: " <<  1 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 1) << ". Correct result: " <<  46 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 2) << ". Correct result: " <<  2079 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 3) << ". Correct result: " <<  89890 << ".\n";
    std::cout << "Depth: 0. Computed result: " << number_of_moves(board, 4) << ". Correct result: " <<  3894594 << ".\n";
}
