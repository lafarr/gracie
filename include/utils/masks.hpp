#pragma once

#include <cstdint>

namespace gracie
{
    inline constexpr auto a_file     = std::uint64_t{ 0x01'01'01'01'01'01'01'01 };
    inline constexpr auto not_a_file = ~a_file;

    inline constexpr auto b_file     = std::uint64_t{ 0x02'02'02'02'02'02'02'02 };
    inline constexpr auto not_b_file = ~b_file;

    inline constexpr auto c_file     = std::uint64_t{ 0x40'40'40'40'40'40'40'40 };
    inline constexpr auto not_c_file = ~c_file;

    inline constexpr auto d_file     = std::uint64_t{ 0x80'80'80'80'80'80'80'80 };
    inline constexpr auto not_d_file = ~d_file;

    inline constexpr auto e_file     = std::uint64_t{ 0x10'10'10'10'10'10'10'10 };
    inline constexpr auto not_e_file = ~e_file;

    inline constexpr auto f_file     = std::uint64_t{ 0x20'20'20'20'20'20'20'20 };
    inline constexpr auto not_f_file = ~f_file;

    inline constexpr auto g_file     = std::uint64_t{ 0x40'40'40'40'40'40'40'40 };
    inline constexpr auto not_g_file = ~g_file;

    inline constexpr auto h_file     = std::uint64_t{ 0x80'80'80'80'80'80'80'80 };
    inline constexpr auto not_h_file = ~h_file;

    inline constexpr auto not_a_or_b_file = ~(a_file | b_file);
    inline constexpr auto not_a_or_f_file = ~(a_file | f_file);
    inline constexpr auto not_h_or_f_file = ~(h_file | f_file);
    inline constexpr auto not_g_or_h_file = ~(g_file | h_file);

    inline constexpr auto rank_4 = std::uint64_t{ 0xFF'00'00'00 };
} // namespace gracie
