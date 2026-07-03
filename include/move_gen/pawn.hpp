#pragma once

#include <cstdint>

namespace gracie
{
	auto constexpr gen_white_pawn_moves(std::uint64_t white_pawns, std::uint64_t empty_squares, std::uint64_t black_occupied) -> std::uint64_t;

    auto white_pawns_able_to_capture_east(std::uint64_t white_pawns, std::uint64_t black_pieces) -> std::uint64_t;

    auto white_pawns_able_to_capture_west(std::uint64_t white_pawns, std::uint64_t black_pieces) -> std::uint64_t;

    auto white_pawns_able_to_capture(std::uint64_t white_pawns, std::uint64_t black_pieces) -> std::uint64_t;

    auto white_pawn_safe_squares(std::uint64_t wpawns, std::uint64_t bpawns) -> std::uint64_t;
}
