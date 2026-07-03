#include "include/move_gen/pawn_move_gen.hpp"

#include "include/utils/masks.hpp"
#include "include/utils/shifts.hpp"

#include <bitset>

namespace
{
    auto constexpr white_pawn_single_push_moves(std::bitset<64> white_pawns, std::bitset<64> empty_squares)
        -> std::bitset<64>
    {
        return gracie::shift_north(white_pawns) & empty_squares;
    }

    auto constexpr white_pawn_double_push_moves(std::bitset<64> white_pawns, std::bitset<64> empty_squares)
        -> std::bitset<64>
    {
        auto single_push_moves = white_pawn_single_push_moves(white_pawns, empty_squares);

        // rank_4 is here becauase a double push from a pawn will always be to rank 4
        return gracie::shift_north(single_push_moves) & empty_squares & gracie::rank_4;
    }

    auto constexpr white_pawn_captures(std::bitset<64> white_pawns, std::bitset<64> black_occupied) -> std::bitset<64>
    {
        auto west_captures = gracie::shift_northeast(white_pawns) & black_occupied;
        auto east_captures = gracie::shift_northwest(white_pawns) & black_occupied;

        return west_captures | east_captures;
    }
} // namespace

namespace gracie
{
    auto constexpr gen_white_pawn_moves(std::bitset<64> white_pawns, std::bitset<64> empty_squares) -> std::bitset<64>
    {
        return white_pawn_single_push_moves(white_pawns, empty_squares) |
               white_pawn_double_push_moves(white_pawns, empty_squares) |
               white_pawn_captures(white_pawns, empty_squares);
    }
} // namespace gracie
