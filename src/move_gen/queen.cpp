#include "include/move_gen/queen.hpp"

#include "include/utils/bithacks.hpp"
#include "include/utils/constants.hpp"
#include "include/utils/masks.hpp"
#include "include/utils/move.hpp"

#include <array>
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
} // namespace

namespace gracie
{
    auto gen_queen_attacks(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> std::uint64_t
    {
        constexpr auto directions = std::array{
            std::array{0, 1},  std::array{0, -1}, std::array{1, 0},  std::array{-1, 0},
            std::array{1, 1},  std::array{-1, 1}, std::array{1, -1}, std::array{-1, -1},
        };

        std::uint64_t attacks = 0;
        for (const auto& direction : directions)
        {
            attacks |= compute_ray_moves(idx, direction[0], direction[1], self_occupied, enemy_occupied);
        }

        return attacks;
    }

    auto gen_queen_moves(std::uint64_t queens, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::vector<Move>
    {
        std::vector<Move> generated_moves;
        while (queens > 0)
        {
            const auto idx = pop_lsb(queens);
            auto attacks = gen_queen_attacks(idx, self_occupied, enemy_occupied);
            while (attacks > 0)
            {
                const auto to = pop_lsb(attacks);
                generated_moves.push_back({ .from = static_cast<int>(idx), .to = static_cast<int>(to) });
            }
        }

        return generated_moves;
    }
}
