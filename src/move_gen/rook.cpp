#include "include/move_gen/rook.hpp"

#include "include/utils/bithacks.hpp"
#include "include/utils/constants.hpp"
#include "include/utils/masks.hpp"
#include "include/utils/move.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace
{
    auto compute_ray_moves(std::size_t idx, int file_delta, int rank_delta, std::uint64_t self_occupied,
                           std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        std::uint64_t attacks = 0;

        for (int file = gracie::file(idx) + file_delta, rank = gracie::rank(idx) + rank_delta;
             file >= gracie::first_file_idx && file <= gracie::last_file_idx &&
             rank >= gracie::first_rank_idx && rank <= gracie::back_rank;
             file += file_delta, rank += rank_delta)
        {
            const auto to = static_cast<std::size_t>((rank * 8) + file);
            const auto to_bit = gracie::bit(to);

            if ((enemy_occupied & to_bit) != 0)
            {
                attacks |= to_bit;
                break;
            }

            if ((self_occupied & to_bit) != 0)
            {
                break;
            }

            attacks |= to_bit;
        }

        return attacks;
    }

    auto compute_north_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return compute_ray_moves(idx, 0, 1, self_occupied, enemy_occupied);
    }

    auto compute_south_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return compute_ray_moves(idx, 0, -1, self_occupied, enemy_occupied);
    }

    auto compute_east_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return compute_ray_moves(idx, 1, 0, self_occupied, enemy_occupied);
    }

    auto compute_west_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return compute_ray_moves(idx, -1, 0, self_occupied, enemy_occupied);
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
            auto attacks = gen_rook_attacks(idx, self_occupied, enemy_occupied);
            while (attacks > 0)
            {
                const auto to = pop_lsb(attacks);
                generated_moves.push_back({ .from = static_cast<int>(idx), .to = static_cast<int>(to) });
            }
        }

        return generated_moves;
    }
} // namespace gracie
