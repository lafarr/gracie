#pragma once

#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <concepts>

namespace gracie
{
    [[nodiscard]] constexpr auto bit(std::size_t idx) -> std::uint64_t
    {
        return 1ULL << idx;
    }

    [[nodiscard]] constexpr auto has_single_bit(std::uint64_t val) -> bool
    {
        return std::has_single_bit(val);
    }

    [[nodiscard]] constexpr auto square_of(std::uint64_t val) -> std::size_t
    {
        assert(has_single_bit(val));
        return static_cast<std::size_t>(std::countr_zero(val));
    }

    template <typename T>
    constexpr auto pop_lsb(T& val) -> std::size_t
    {
        assert(val != 0);
        const auto lsb_index = static_cast<std::size_t>(std::countr_zero(val));
        val &= (val - 1);
        return lsb_index;
    }

	constexpr auto file(std::integral auto val) -> int
	{
		return val % 8;
	}
	
	constexpr auto rank(std::integral auto val) -> int
	{
		return static_cast<int>(val) / 8;
	}
} // namespace gracie
