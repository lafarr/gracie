#pragma once

#include <cstdint>

namespace gracie
{
    auto compute_ray_moves(std::size_t idx, int file_delta, int rank_delta, std::uint64_t self_occupied,
                           std::uint64_t enemy_occupied) -> std::uint64_t;
}
