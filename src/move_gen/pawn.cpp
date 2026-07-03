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
        auto west_captures = gracie::shift_northeast(white_pawns) & black_occupied;
        auto east_captures = gracie::shift_northwest(white_pawns) & black_occupied;

        return west_captures | east_captures;
    }
} // namespace

namespace gracie
{
    auto white_pawns_able_to_capture_east(std::uint64_t white_pawns, std::uint64_t black_pieces) -> std::uint64_t
    {
        return white_pawns & shift_northwest(black_pieces);
    }

    auto white_pawns_able_to_capture_west(std::uint64_t white_pawns, std::uint64_t black_pieces) -> std::uint64_t
    {
        return white_pawns & shift_northeast(black_pieces);
    }

    auto white_pawns_able_to_capture(std::uint64_t white_pawns, std::uint64_t black_pieces) -> std::uint64_t
    {
        return white_pawns & (shift_northeast(black_pieces) | shift_northwest(black_pieces));
    }

    auto white_pawn_safe_squares(std::uint64_t wpawns, std::uint64_t bpawns) -> std::uint64_t
    {
        std::uint64_t white_pawn_east_attacks   = shift_northeast(wpawns);
        std::uint64_t white_pawn_west_attacks   = shift_northwest(wpawns);
        std::uint64_t black_pawn_east_attacks   = shift_northeast(bpawns);
        std::uint64_t black_pawn_west_attacks   = shift_northwest(bpawns);
        std::uint64_t white_pawn_double_attacks = white_pawn_east_attacks & white_pawn_west_attacks;
        std::uint64_t white_pawn_odd_attacks    = white_pawn_east_attacks ^ white_pawn_west_attacks;
        std::uint64_t black_pawn_double_attacks = black_pawn_east_attacks & black_pawn_west_attacks;
        std::uint64_t black_pawn_any_attacks    = black_pawn_east_attacks | black_pawn_west_attacks;
        return white_pawn_double_attacks | ~black_pawn_any_attacks |
               (white_pawn_odd_attacks & ~black_pawn_double_attacks);
    }

    auto constexpr gen_white_pawn_moves(std::uint64_t white_pawns, std::uint64_t empty_squares,
                                        std::uint64_t black_occupied) -> std::uint64_t
    {
        return white_pawn_single_push_moves(white_pawns, empty_squares) |
               white_pawn_double_push_moves(white_pawns, empty_squares) |
               white_pawn_captures(white_pawns, black_occupied);
    }
} // namespace gracie
