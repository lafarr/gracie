#include "include/move_gen/rook.hpp"

#include "include/utils/move.hpp"
#include "include/utils/bithacks.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace
{
    auto find_rook_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> std::vector<gracie::Move>
    {
		
    }
} // namespace

namespace gracie
{
    auto gen_rook_moves(std::uint64_t rooks, std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> std::vector<Move>
    {
		std::vector<Move> generated_moves;
		while (rooks > 0)
		{
			auto idx = pop_lsb(rooks);
			auto moves = find_rook_moves(static_cast<std::size_t>(idx), self_occupied, enemy_occupied);
			generated_moves.insert(generated_moves.end(), moves.begin(), moves.end());	
		}

		return generated_moves;
    }
} // namespace gracie
