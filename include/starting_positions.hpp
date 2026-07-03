#pragma once

// 63-56
namespace gracie
{
    // squares 1 and 8 (from the right) on the first rank
    inline constexpr unsigned long long white_rook_starting_pos = 1 | (1 << 7);

    // squares 2 and 7 (from the right) on the first rank
    inline constexpr unsigned long long white_knight_starting_pos =
        2 | (1 << 6);

    // squares 3 and 6 (from the right) on the first rank
    inline constexpr unsigned long long white_bishop_starting_pos =
        4 | (1 << 5);

    // square 5 (from the right) on the first rank
    inline constexpr unsigned long long white_queen_starting_pos = 1 << 4;

    // squares 2 and 7 (from the right) on the first rank
    inline constexpr unsigned long long white_king_starting_pos = 1 << 3;

    // entire second rank
    inline constexpr unsigned long long white_pawn_starting_pos = 0xFF << 8;

    // squares 1 and 8 (from the right) on the last rank
	inline constexpr unsigned long long black_rook_starting_pos = (1ULL << 63) | (1ULL << 56);

    // squares 2 and 7 (from the right) on the last rank
	inline constexpr unsigned long long black_knight_starting_pos = (1ULL << 62) | (1ULL << 57);

    // squares 3 and 6 (from the right) on the last rank
	inline constexpr unsigned long long black_bishop_starting_pos = (1ULL << 61) | (1ULL << 58);

	// square 4 (from the right) on the last rank
	inline constexpr unsigned long long black_king_starting_pos = (1ULL << 60);

	// square 5 (from the right) on the last rank
	inline constexpr unsigned long long black_queen_starting_pos = (1ULL << 59);

	// entire second-to-last rank
	inline constexpr unsigned long long black_pawn_starting_pos = (1ULL << 63) >> 8;
} // namespace gracie
