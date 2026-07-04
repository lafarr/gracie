#include "include/bitboard.hpp"

namespace gracie
{
    Bit_board::Bit_board()  = default;

    auto Bit_board::instance() -> Bit_board&
    {
        static Bit_board board{};
        return board;
    }

    auto Bit_board::white_occupied() const -> std::uint64_t
    {
        return white_king_ | white_queen_ | white_rook_ | white_bishop_ | white_knight_ | white_pawn_;
    }

    auto Bit_board::black_occupied() const -> std::uint64_t
    {
        return black_king_ | black_queen_ | black_rook_ | black_bishop_ | black_knight_ | black_pawn_;
    }

    auto Bit_board::all_occupied() const -> std::uint64_t
    {
        return white_occupied() | black_occupied();
    }
} // namespace gracie
