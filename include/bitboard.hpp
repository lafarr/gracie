#pragma once

#include "include/starting_positions.hpp"

#include <cstdint>

namespace gracie
{
	class Bit_board
	{
		public:
			static auto instance() -> Bit_board&;
			[[nodiscard]] auto white_occupied() const -> std::uint64_t;
			[[nodiscard]] auto black_occupied() const -> std::uint64_t;
			[[nodiscard]] auto all_occupied() const -> std::uint64_t;
		private:
			Bit_board();

			std::uint64_t white_king_{ white_king_starting_pos };
          	std::uint64_t white_queen_{ white_queen_starting_pos };
          	std::uint64_t white_rook_{ white_rook_starting_pos };
          	std::uint64_t white_bishop_{ white_bishop_starting_pos };
          	std::uint64_t white_knight_{ white_knight_starting_pos };
          	std::uint64_t white_pawn_{ white_pawn_starting_pos };
          	std::uint64_t black_king_{ black_king_starting_pos };
          	std::uint64_t black_queen_{ black_queen_starting_pos };
          	std::uint64_t black_rook_{ black_rook_starting_pos };
          	std::uint64_t black_bishop_{ black_bishop_starting_pos };
          	std::uint64_t black_knight_{ black_knight_starting_pos };
          	std::uint64_t black_pawn_{ black_pawn_starting_pos };
	};
}
