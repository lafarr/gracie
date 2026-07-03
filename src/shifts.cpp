#include "include/shifts.hpp"
#include "include/masks.hpp"

#include <bitset>

namespace gracie
{
    auto shift_north(std::bitset<64> pieces) -> std::bitset<64>
    {
        return pieces >> 8;
    }

	auto shift_northeast(std::bitset<64> pieces) -> std::bitset<64>
	{
		return (pieces << 9) & gracie::not_a_file;
	}

	auto shift_northwest(std::bitset<64> pieces) -> std::bitset<64>
	{
		return pieces << 7;
	}
} // namespace gracie
