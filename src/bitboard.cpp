#include "include/bitboard.hpp"

#include "include/starting_positions.hpp"

namespace gracie
{
    Bit_board::Bit_board()
        : white_king_{ white_king_starting_pos },
          white_queen_{ white_queen_starting_pos },
          white_rook_{ white_rook_starting_pos },
          white_bishop_{ white_bishop_starting_pos },
          white_knight_{ white_knight_starting_pos },
          white_pawn_{ white_pawn_starting_pos },
          black_king_{ black_king_starting_pos },
          black_queen_{ black_queen_starting_pos },
          black_rook_{ black_rook_starting_pos },
          black_bishop_{ black_bishop_starting_pos },
          black_knight_{ black_knight_starting_pos },
          black_pawn_{ black_pawn_starting_pos }
    {
    }

	auto Bit_board::instance() -> Bit_board
	{
		static Bit_board board{};
		return board;
	}
} // namespace gracie
