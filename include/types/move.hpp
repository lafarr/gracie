#pragma once

#include "include/types/color.hpp"
#include "include/types/piece.hpp"
#include "include/utils/types.hpp"

#include <optional>

namespace gracie
{
    struct Move
    {
            Color color{ Color::white };
            Piece piece{ Piece::pawn };
            u8 from{ 0 };
            u8 to{ 0 };
            std::optional<Piece> captured_piece;
    };
} // namespace gracie
