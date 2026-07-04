#pragma once

#include "include/types/move.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace gracie
{
    auto gen_queen_attacks(std::size_t idx, std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> std::uint64_t;
    auto gen_queen_moves(std::uint64_t queens, std::uint64_t self_occupied, std::uint64_t enemy_occupied)
        -> std::vector<Move>;
}
