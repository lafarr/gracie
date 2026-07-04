#include "include/movegen/queen.hpp"

#include "include/movegen/helpers.hpp"
#include "include/types/piece.hpp"
#include "include/utils/bithacks.hpp"
#include "include/types/move.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace gracie
{
    auto gen_queen_attacks(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> std::uint64_t
    {
        constexpr auto directions = std::array{
            std::array{ 0, 1 }, std::array{ 0, -1 }, std::array{ 1, 0 },  std::array{ -1, 0 },
            std::array{ 1, 1 }, std::array{ -1, 1 }, std::array{ 1, -1 }, std::array{ -1, -1 },
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
            auto attacks   = gen_queen_attacks(idx, self_occupied, enemy_occupied);
            while (attacks > 0)
            {
                const auto to = pop_lsb(attacks);
                generated_moves.push_back({ .piece = gracie::Piece::queen,
                                            .from = static_cast<std::uint8_t>(idx),
                                            .to = static_cast<std::uint8_t>(to),
                                            .captured_piece = std::nullopt });
            }
        }

        return generated_moves;
    }
} // namespace gracie
