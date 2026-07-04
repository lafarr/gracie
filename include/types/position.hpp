#pragma once

#include "include/types/color.hpp"
#include "include/types/piece.hpp"
#include "include/utils/types.hpp"

#include <boost/container_hash/hash.hpp>

#include <unordered_map>
#include <utility>

namespace gracie
{
    struct Position
    {
            u64 white_occupied;
            u64 black_occupied;
            u64 all_occupied;
            u64 empty_squares;

            std::unordered_map<std::pair<Color, Piece>, u64, boost::hash<std::pair<Color, Piece>>> piece_positions;

            bool white_in_check;
            bool black_in_check;
    };

	inline auto update_white_occupied(Position pos, u64 white_occ) -> Position
	{
		pos.white_occupied = white_occ;
		pos.all_occupied = pos.white_occupied | pos.black_occupied;
		pos.empty_squares = ~pos.all_occupied;
		return pos;
	}

	inline auto update_black_occupied(Position pos, u64 black_occ) -> Position
	{
		pos.black_occupied = black_occ;
		pos.all_occupied = pos.black_occupied | pos.white_occupied;
		pos.empty_squares = ~pos.all_occupied;
		return pos;
	}
} // namespace gracie
