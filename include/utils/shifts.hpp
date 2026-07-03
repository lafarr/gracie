#pragma once

#include <bitset>

namespace gracie
{
	auto shift_north(std::bitset<64> pieces) -> std::bitset<64>;

	auto shift_northeast(std::bitset<64> pieces) -> std::bitset<64>;

	auto shift_northwest(std::bitset<64> pieces) -> std::bitset<64>;
}
