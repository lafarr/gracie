#pragma once

#include "include/types/move.hpp"
#include "include/types/position.hpp"
#include "include/types/color.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace gracie
{
    auto gen_possible_knight_moves(std::size_t idx) -> std::uint64_t;

	/// @brief generates pseudo-legal knight moves. does not consider if the move puts the 
	/// moving piece's king in check
	/// @returns possible knight moves
    auto gen_knight_moves(const Position& board_pos, Color to_move) -> std::vector<Move>;
}
