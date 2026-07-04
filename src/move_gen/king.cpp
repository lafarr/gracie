#include "include/move_gen/king.hpp"

#include "include/utils/constants.hpp"
#include "include/utils/shifts.hpp"

#include <array>
#include <cstdint>

namespace
{
    consteval auto compute_possible_king_moves() -> std::array<std::uint64_t, gracie::squares_on_board>
    {
        std::array<std::uint64_t, gracie::squares_on_board> possible_moves{ 0 };

        for (std::size_t i = 0; i < possible_moves.size(); ++i)
        {
			std::uint64_t curr_pos = i;
            std::uint64_t attacks = gracie::shift_east(curr_pos) | gracie::shift_west(curr_pos);
            curr_pos |= attacks;
            attacks |= gracie::shift_north(curr_pos) | gracie::shift_south(curr_pos);
			possible_moves.at(i) = attacks;
        }

		return possible_moves;
    }
} // namespace

namespace gracie
{
	constexpr auto possible_moves = compute_possible_king_moves();

    auto gen_possible_king_moves(std::uint64_t king_pos) -> std::uint64_t
    {
		return possible_moves.at(king_pos);
    }
} // namespace gracie
