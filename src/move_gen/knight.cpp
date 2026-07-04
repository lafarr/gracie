#include "include/move_gen/knight.hpp"

#include "include/utils/constants.hpp"
#include "include/utils/shifts.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace
{
    consteval auto compute_possible_knight_moves() -> std::array<std::uint64_t, gracie::squares_on_board>
    {
        std::array<std::uint64_t, gracie::squares_on_board> possible_moves{ 0 };

        for (std::size_t i = 0; i < possible_moves.size(); ++i)
        {
            auto curr_pos        = std::uint64_t{ i };
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
	constexpr auto possible_knight_moves = compute_possible_knight_moves();

    auto gen_possible_knight_moves(std::size_t idx, std::uint64_t self_occupied) -> std::uint64_t
    {
        return possible_knight_moves.at(idx) & (~self_occupied);
    }
} // namespace gracie
