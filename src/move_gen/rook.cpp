#include "include/move_gen/rook.hpp"

#include "include/utils/bithacks.hpp"
#include "include/utils/move.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace
{
    auto find_rook_moves(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::vector<gracie::Move>
    {
        // for each direction:
        // move one square in direction, if off board stop, if enemy piece or friendly piece,
        // add the move to the vector
        //
        // north -> << 8
        // south -> >> 8
        // east -> << 1
        // west -> >> 1

        std::vector<gracie::Move> moves;

		// north
        auto curr_pos = gracie::bit(idx) << 8;

        while (curr_pos != 0)
        {
			auto bitset_idx = gracie::square_of(curr_pos);

            if (auto is_enemy_square = (enemy_occupied & curr_pos) != 0; is_enemy_square)
            {
                moves.push_back({ .from = static_cast<int>(idx), .to = static_cast<int>(bitset_idx) });
                break;
            }

            if (auto is_friendly_square = (self_occupied & curr_pos) != 0; is_friendly_square)
            {
                break;
            }

            moves.push_back({ .from = static_cast<int>(idx), .to = static_cast<int>(bitset_idx) });
            curr_pos <<= 8;
        }

		// south
		curr_pos = gracie::bit(idx) >> 8;
		
		while (curr_pos != 0)
		{
			auto bitset_idx = gracie::square_of(curr_pos);

            if (auto is_enemy_square = (enemy_occupied & curr_pos) != 0; is_enemy_square)
            {
                moves.push_back({ .from = static_cast<int>(idx), .to = static_cast<int>(bitset_idx) });
                break;
            }

            if (auto is_friendly_square = (self_occupied & curr_pos) != 0; is_friendly_square)
            {
                break;
            }

            moves.push_back({ .from = static_cast<int>(idx), .to = static_cast<int>(bitset_idx) });
            curr_pos >>= 8;
		}

		// east
		curr_pos = gracie::bit(idx) << 1;
		const auto start_rank = idx / 8;

		while ((curr_pos != 0) && ((gracie::square_of(curr_pos) / 8) == start_rank))
		{
			auto bitset_idx = gracie::square_of(curr_pos);

            if (auto is_enemy_square = (enemy_occupied & curr_pos) != 0; is_enemy_square)
            {
                moves.push_back({ .from = static_cast<int>(idx), .to = static_cast<int>(bitset_idx) });
                break;
            }

            if (auto is_friendly_square = (self_occupied & curr_pos) != 0; is_friendly_square)
            {
                break;
            }

            moves.push_back({ .from = static_cast<int>(idx), .to = static_cast<int>(bitset_idx) });
            curr_pos <<= 1;
		}

		// west
		curr_pos = gracie::bit(idx) >> 1;

		while ((curr_pos != 0) && ((gracie::square_of(curr_pos) / 8) == start_rank))
		{
			auto bitset_idx = gracie::square_of(curr_pos);

            if (auto is_enemy_square = (enemy_occupied & curr_pos) != 0; is_enemy_square)
            {
                moves.push_back({ .from = static_cast<int>(idx), .to = static_cast<int>(bitset_idx) });
                break;
            }

            if (auto is_friendly_square = (self_occupied & curr_pos) != 0; is_friendly_square)
            {
                break;
            }

            moves.push_back({ .from = static_cast<int>(idx), .to = static_cast<int>(bitset_idx) });
            curr_pos >>= 1;
		}

        return moves;
    }
} // namespace

namespace gracie
{
    auto gen_rook_moves(std::uint64_t rooks, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::vector<Move>
    {
        std::vector<Move> generated_moves;
        while (rooks > 0)
        {
            auto idx   = pop_lsb(rooks);
            auto moves = find_rook_moves(idx, self_occupied, enemy_occupied);
            generated_moves.insert(generated_moves.end(), moves.begin(), moves.end());
        }

        return generated_moves;
    }
} // namespace gracie
