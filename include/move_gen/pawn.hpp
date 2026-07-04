#pragma once

#include <cstdint>

namespace gracie
{
	auto gen_white_pawn_moves(std::uint64_t white_pawns, std::uint64_t empty_squares, std::uint64_t black_occupied) -> std::uint64_t;
}
