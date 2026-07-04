#include "include/movegen/rook.hpp"

#include "include/movegen/helpers.hpp"
#include "include/types/piece.hpp"
#include "include/utils/bithacks.hpp"
#include "include/types/move.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace
{
	// TODO: add magic bitboards (https://www.chessprogramming.org/Magic_Bitboards)

    auto compute_north_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return gracie::compute_ray_moves(idx, 0, 1, self_occupied, enemy_occupied);
    }

    auto compute_south_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return gracie::compute_ray_moves(idx, 0, -1, self_occupied, enemy_occupied);
    }

    auto compute_east_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> std::uint64_t
    {
        return gracie::compute_ray_moves(idx, 1, 0, self_occupied, enemy_occupied);
    }

    auto compute_west_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> std::uint64_t
    {
        return gracie::compute_ray_moves(idx, -1, 0, self_occupied, enemy_occupied);
    }
} // namespace

namespace gracie
{
    auto gen_rook_attacks(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> std::uint64_t
    {
        return compute_north_moves(idx, self_occupied, enemy_occupied) |
               compute_south_moves(idx, self_occupied, enemy_occupied) |
               compute_east_moves(idx, self_occupied, enemy_occupied) |
               compute_west_moves(idx, self_occupied, enemy_occupied);
    }

    auto gen_rook_moves(std::uint64_t rooks, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::vector<Move>
    {
        std::vector<Move> generated_moves;
        while (rooks > 0)
        {
            const auto idx = pop_lsb(rooks);
            auto attacks   = gen_rook_attacks(idx, self_occupied, enemy_occupied);
            while (attacks > 0)
            {
                const auto to = pop_lsb(attacks);
                generated_moves.push_back({ .piece = gracie::Piece::rook,
                                            .from = static_cast<std::uint8_t>(idx),
                                            .to = static_cast<std::uint8_t>(to),
                                            .captured_piece = std::nullopt });
            }
        }

        return generated_moves;
    }
} // namespace gracie
