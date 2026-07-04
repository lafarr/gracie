#pragma once

#include "include/color.hpp"

#include <cstdint>

namespace gracie
{
	auto gen_pawn_moves(Color color, std::uint64_t pawns, std::uint64_t empty_squares, std::uint64_t enemy_occupied) -> std::uint64_t;
}
