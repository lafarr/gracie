#pragma once

#include "include/types/position.hpp"
#include "include/types/move.hpp"

namespace gracie
{
	auto move(const Position& board_pos, const Move& move) -> Position;

	auto unmove(const Position& board_pos, const Move& move) -> Position;
}
