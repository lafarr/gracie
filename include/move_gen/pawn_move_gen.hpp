#pragma once

#include <bitset>

namespace gracie
{
	auto constexpr gen_white_pawn_moves(std::bitset<64> white_pawns, std::bitset<64> empty_squares) -> std::bitset<64>;
}
