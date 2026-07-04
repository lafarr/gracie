#include "include/movegen/helpers.hpp"

#include "include/utils/bithacks.hpp"
#include "include/utils/constants.hpp"

#include <cstdint>

namespace gracie
{
    auto compute_ray_moves(std::size_t idx, int file_delta, int rank_delta, std::uint64_t self_occupied,
                           std::uint64_t enemy_occupied) -> std::uint64_t
    {
        std::uint64_t attacks = 0;

        for (int file = gracie::file(idx) + file_delta, rank = gracie::rank(idx) + rank_delta;
             file >= gracie::first_file_idx && file <= gracie::last_file_idx && rank >= gracie::first_rank_idx &&
             rank <= gracie::back_rank;
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
} // namespace gracie
