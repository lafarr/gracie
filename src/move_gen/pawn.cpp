#include "include/move_gen/pawn.hpp"

#include "include/utils/masks.hpp"
#include "include/utils/shifts.hpp"

#include <cstdint>

namespace
{
    auto constexpr white_pawn_single_push_moves(std::uint64_t white_pawns, std::uint64_t empty_squares) -> std::uint64_t
    {
        return gracie::shift_north(white_pawns) & empty_squares;
    }

    auto constexpr white_pawn_double_push_moves(std::uint64_t white_pawns, std::uint64_t empty_squares) -> std::uint64_t
    {
        auto single_push_moves = white_pawn_single_push_moves(white_pawns, empty_squares);

        // rank_4 is here becauase a double push from a pawn will always be to rank 4
        return gracie::shift_north(single_push_moves) & empty_squares & gracie::rank_4;
    }

    auto white_pawn_captures(std::uint64_t white_pawns, std::uint64_t black_occupied) -> std::uint64_t
    {
        auto west_captures = gracie::shift_northeast(white_pawns);
        auto east_captures = gracie::shift_northwest(white_pawns);

        return (west_captures | east_captures) & black_occupied;
    }
} // namespace

namespace gracie
{
	// TODO: need to figure out how to do en-passant
    auto constexpr gen_white_pawn_moves(std::uint64_t white_pawns, std::uint64_t empty_squares, std::uint64_t black_occupied) -> std::uint64_t
    {
        return white_pawn_single_push_moves(white_pawns, empty_squares) |
               white_pawn_double_push_moves(white_pawns, empty_squares) |
               white_pawn_captures(white_pawns, black_occupied);
    }
} // namespace gracie
