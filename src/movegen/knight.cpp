#include "include/movegen/knight.hpp"

#include "include/types/color.hpp"
#include "include/types/piece.hpp"
#include "include/types/position.hpp"
#include "include/utils/bithacks.hpp"
#include "include/utils/constants.hpp"
#include "include/utils/shifts.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace
{
    consteval auto precompute_moves() -> std::array<std::uint64_t, gracie::squares_on_board>
    {
        std::array<std::uint64_t, gracie::squares_on_board> possible_moves{ 0 };

        for (std::size_t i = 0; i < possible_moves.size(); ++i)
        {
            auto curr_pos        = gracie::bit(i);
            possible_moves.at(i) = gracie::shift_north_north_east(curr_pos) | gracie::shift_north_east_east(curr_pos) |
                                   gracie::shift_south_east_east(curr_pos) | gracie::shift_south_south_east(curr_pos) |
                                   gracie::shift_north_north_west(curr_pos) | gracie::shift_north_west_west(curr_pos) |
                                   gracie::shift_south_west_west(curr_pos) | gracie::shift_south_south_west(curr_pos);
        }

        return possible_moves;
    }
} // namespace

namespace gracie
{
    constexpr auto precomputed_moves = precompute_moves();
} // namespace gracie

auto gracie::gen_possible_knight_moves(std::size_t idx) -> std::uint64_t
{
    return gracie::precomputed_moves.at(idx);
}

auto gracie::gen_knight_moves(const gracie::Position& board_pos, gracie::Color to_move) -> std::vector<gracie::Move>
{

    std::vector<gracie::Move> generated_moves;
    auto knights = board_pos.piece_positions.at({ to_move, gracie::Piece::knight });

    while (knights > 0)
    {
        const auto self_occupied =
            to_move == gracie::Color::white ? board_pos.white_occupied : board_pos.black_occupied;
        const auto idx = gracie::pop_lsb(knights);
        auto moves     = gracie::precomputed_moves.at(idx) & (~self_occupied);

        while (moves > 0)
        {
            const auto to = gracie::pop_lsb(moves);
            generated_moves.push_back({ .color = to_move,
                                        .piece = gracie::Piece::knight,
                                        .from = static_cast<std::uint8_t>(idx),
                                        .to = static_cast<std::uint8_t>(to),
                                        .captured_piece = std::nullopt });
        }
    }

    return generated_moves;
}
