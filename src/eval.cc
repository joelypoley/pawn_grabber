#include "board.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char** argv) {
	Board board("r1b1qbkr/ppp1p1pp/1nnpP3/6N1/8/5Q2/PPPP1PPP/RNB1K2R w KQ - 3 9");
	int depth = std::atoi(argv[1]);
	std::cout << "Depth: " << depth << '\n';
	std::vector<Move> pv;
	std::cout << "Evaluation: " << evaluation(board, depth, &pv) << '\n';
	std::cout << "Principal variation: " << '\n';
	std::cout << "pv.size() == " << pv.size() << '\n';
	for (Move move : pv) {
		std::cout << '\t' << move.to_pretty_str() << '\n';
	}
}