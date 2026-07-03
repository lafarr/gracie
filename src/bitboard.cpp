#include "include/bitboard.hpp"

namespace gracie
{
    Bit_board::Bit_board()  = default;

    auto Bit_board::instance() -> Bit_board
    {
        static Bit_board board{};
        return board;
    }
} // namespace gracie
