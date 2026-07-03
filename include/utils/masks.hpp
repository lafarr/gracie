#include <bitset>

namespace gracie
{
    inline constexpr std::bitset<64> not_a_file = 0xFEFEFEFEFEFEFEFE;
    inline constexpr std::bitset<64> not_h_file = 0x7f7f7f7f7f7f7f7f;
    inline constexpr std::bitset<64> rank_4     = 0xFF'00'00'00ULL;
} // namespace gracie
