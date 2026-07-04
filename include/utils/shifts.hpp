#pragma once

#include "include/utils/masks.hpp"

#include <cstdint>

namespace gracie
{
    constexpr auto shift_north(std::uint64_t pieces) -> std::uint64_t
    {
        return pieces << 8;
    }

    constexpr auto shift_northeast(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces << 9) & gracie::not_a_file;
    }

    constexpr auto shift_northwest(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces << 7) & gracie::not_h_file;
    }

    constexpr auto shift_southeast(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces >> 7) & gracie::not_a_file;
    }

    constexpr auto shift_southwest(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces >> 9) & gracie::not_h_file;
    }

    constexpr auto shift_north_north_east(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces << 17) & gracie::not_a_file;
    }

    constexpr auto shift_north_east_east(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces << 10) & gracie::not_a_or_b_file;
    }

    constexpr auto shift_south_east_east(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces >> 6) & gracie::not_a_or_b_file;
    }

    constexpr auto shift_south_south_east(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces >> 15) & gracie::not_a_file;
    }

    constexpr auto shift_north_north_west(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces << 15) & gracie::not_h_file;
    }

    constexpr auto shift_north_west_west(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces << 6) & not_g_or_h_file;
    }

    constexpr auto shift_south_west_west(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces >> 10) & not_g_or_h_file;
    }

    constexpr auto shift_south_south_west(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces >> 17) & gracie::not_h_file;
    }

    constexpr auto shift_east(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces & not_h_file) << 1;
    }

    constexpr auto shift_west(std::uint64_t pieces) -> std::uint64_t
    {
        return (pieces & not_a_file) >> 1;
    }

    constexpr auto shift_south(std::uint64_t pieces) -> std::uint64_t
    {
        return pieces >> 8;
    }
} // namespace gracie
