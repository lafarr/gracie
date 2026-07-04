#include "include/move_gen/pawn.hpp"

#include "include/color.hpp"
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

        // fourth_rank is here because a double push from a white pawn always lands on rank 4.
        return gracie::shift_north(single_push_moves) & empty_squares & gracie::fourth_rank;
    }

    auto white_pawn_captures(std::uint64_t white_pawns, std::uint64_t black_occupied) -> std::uint64_t
    {
        auto west_captures = gracie::shift_northeast(white_pawns);
        auto east_captures = gracie::shift_northwest(white_pawns);

        return (west_captures | east_captures) & black_occupied;
    }

    auto constexpr black_pawn_single_push_moves(std::uint64_t black_pawns, std::uint64_t empty_squares) -> std::uint64_t
    {
        return gracie::shift_south(black_pawns) & empty_squares;
    }

    auto constexpr black_pawn_double_push_moves(std::uint64_t black_pawns, std::uint64_t empty_squares) -> std::uint64_t
    {
        auto single_push_moves = black_pawn_single_push_moves(black_pawns, empty_squares);

        return gracie::shift_south(single_push_moves) & empty_squares & gracie::fifth_rank;
    }

    auto black_pawn_captures(std::uint64_t black_pawns, std::uint64_t white_occupied) -> std::uint64_t
    {
        auto west_captures = gracie::shift_southwest(black_pawns);
        auto east_captures = gracie::shift_southeast(black_pawns);

        return (west_captures | east_captures) & white_occupied;
    }
} // namespace

namespace gracie
{
	// TODO: need to figure out how to do en-passant
    auto gen_pawn_moves(Color color, std::uint64_t pawns, std::uint64_t empty_squares, std::uint64_t enemy_occupied) -> std::uint64_t
    {
        if (color == Color::white)
        {
            return white_pawn_single_push_moves(pawns, empty_squares) |
                   white_pawn_double_push_moves(pawns, empty_squares) |
                   white_pawn_captures(pawns, enemy_occupied);
        }

        return black_pawn_single_push_moves(pawns, empty_squares) |
               black_pawn_double_push_moves(pawns, empty_squares) |
               black_pawn_captures(pawns, enemy_occupied);
    }
} // namespace gracie
