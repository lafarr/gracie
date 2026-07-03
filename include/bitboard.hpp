#pragma once

#include <bitset>
#include "color.hpp"
#include "piece.hpp"

namespace gracie
{
	class Bit_board
	{
		public:
			auto instance() -> Bit_board;

			auto compute_moves(Color color, Piece piece);
		private:
			Bit_board();

			std::bitset<64> white_king_;
			std::bitset<64> white_queen_;
			std::bitset<64> white_rook_;
			std::bitset<64> white_bishop_;
			std::bitset<64> white_knight_;
			std::bitset<64> white_pawn_;

			std::bitset<64> black_king_;
			std::bitset<64> black_queen_;
			std::bitset<64> black_rook_;
			std::bitset<64> black_bishop_;
			std::bitset<64> black_knight_;
			std::bitset<64> black_pawn_;
	};
}
