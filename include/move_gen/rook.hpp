#pragma once

#include "include/utils/move.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace gracie
{
    auto gen_rook_attacks(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> std::uint64_t;
    auto gen_rook_moves(std::uint64_t rooks, std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> std::vector<Move>;
}
