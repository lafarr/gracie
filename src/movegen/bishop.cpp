#include "include/movegen/bishop.hpp"

#include "include/movegen/helpers.hpp"
#include "include/types/move.hpp"
#include "include/types/piece.hpp"
#include "include/utils/bithacks.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace
{
    auto compute_northeast_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return gracie::compute_ray_moves(idx, 1, 1, self_occupied, enemy_occupied);
    }

    auto compute_northwest_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return gracie::compute_ray_moves(idx, -1, 1, self_occupied, enemy_occupied);
    }

    auto compute_southeast_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return gracie::compute_ray_moves(idx, 1, -1, self_occupied, enemy_occupied);
    }

    auto compute_southwest_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::uint64_t
    {
        return gracie::compute_ray_moves(idx, -1, -1, self_occupied, enemy_occupied);
    }

    auto compute_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> std::uint64_t
    {
        return compute_northeast_moves(idx, self_occupied, enemy_occupied) |
               compute_northwest_moves(idx, self_occupied, enemy_occupied) |
               compute_southeast_moves(idx, self_occupied, enemy_occupied) |
               compute_southwest_moves(idx, self_occupied, enemy_occupied);
    }
} // namespace

auto gracie::gen_bishop_attacks(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> std::uint64_t
{
    return compute_moves(idx, self_occupied, enemy_occupied);
}

auto gracie::gen_bishop_moves(std::uint64_t bishops, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
    -> std::vector<gracie::Move>
{
    std::vector<gracie::Move> generated_moves;
    while (bishops > 0)
    {
        const auto idx = pop_lsb(bishops);
        auto moves = gen_bishop_attacks(idx, self_occupied, enemy_occupied);

        while (moves > 0)
        {
            const auto to = pop_lsb(moves);
            generated_moves.push_back({ .piece = gracie::Piece::bishop,
                                        .from = static_cast<std::uint8_t>(idx),
                                        .to = static_cast<std::uint8_t>(to),
                                        .captured_piece = std::nullopt });
        }
    }

    return generated_moves;
}
