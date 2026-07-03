#pragma once

#include <cstdint>

namespace gracie
{
	enum class Piece : std::uint8_t
	{
		King,
		Queen,
		Rook, 
		Bishop,
		Knight,
		Pawn
	};
}
