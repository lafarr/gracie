#include "include/move_gen/bishop.hpp"

#include "include/utils/bithacks.hpp"
#include "include/utils/constants.hpp"
#include "include/utils/move.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace
{
    auto compute_ray_moves(std::size_t idx, int file_delta, int rank_delta, std::uint64_t self_occupied,
                           std::uint64_t enemy_occupied) -> std::uint64_t
    {
        std::uint64_t attacks = 0;

        for (int file = gracie::file(idx) + file_delta, rank = gracie::rank(idx) + rank_delta;
             file >= gracie::first_file_idx && file <= gracie::last_file_idx &&
             rank >= gracie::first_rank_idx && rank <= gracie::back_rank;
             file += file_delta, rank += rank_delta)
        {
            const auto to     = static_cast<std::size_t>((rank * 8) + file);
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

    auto compute_northeast_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return compute_ray_moves(idx, 1, 1, self_occupied, enemy_occupied);
    }

    auto compute_northwest_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return compute_ray_moves(idx, -1, 1, self_occupied, enemy_occupied);
    }

    auto compute_southeast_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return compute_ray_moves(idx, 1, -1, self_occupied, enemy_occupied);
    }

    auto compute_southwest_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return compute_ray_moves(idx, -1, -1, self_occupied, enemy_occupied);
    }
} // namespace

namespace gracie
{
    auto gen_bishop_attacks(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> std::uint64_t
    {
        return compute_northeast_moves(idx, self_occupied, enemy_occupied) |
               compute_northwest_moves(idx, self_occupied, enemy_occupied) |
               compute_southeast_moves(idx, self_occupied, enemy_occupied) |
               compute_southwest_moves(idx, self_occupied, enemy_occupied);
    }

    auto gen_bishop_moves(std::uint64_t bishops, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::vector<gracie::Move>
    {
        std::vector<gracie::Move> generated_moves;
        while (bishops > 0)
        {
            const auto idx = pop_lsb(bishops);
            auto attacks = gen_bishop_attacks(idx, self_occupied, enemy_occupied);
            while (attacks > 0)
            {
                const auto to = pop_lsb(attacks);
                generated_moves.push_back({ .from = static_cast<int>(idx), .to = static_cast<int>(to) });
            }
        }

        return generated_moves;
    }
} // namespace gracie
