#pragma once

namespace gracie
{
    constexpr auto squares_on_board = 64;

    inline constexpr auto a_file_idx = 0;
    inline constexpr auto b_file_idx = 1;
    inline constexpr auto c_file_idx = 2;
    inline constexpr auto d_file_idx = 3;
    inline constexpr auto e_file_idx = 4;
    inline constexpr auto f_file_idx = 5;
    inline constexpr auto g_file_idx = 6;
    inline constexpr auto h_file_idx = 7;

    inline constexpr auto first_file_idx = a_file_idx;
    inline constexpr auto last_file_idx = h_file_idx;

    inline constexpr auto first_rank_idx = 0;
    inline constexpr auto second_rank_idx = 1;
    inline constexpr auto third_rank_idx = 2;
    inline constexpr auto fourth_rank_idx = 3;
    inline constexpr auto fifth_rank_idx = 4;
    inline constexpr auto sixth_rank_idx = 5;
    inline constexpr auto seventh_rank_idx = 6;
    inline constexpr auto eighth_rank_idx = 7;

    inline constexpr auto back_rank = eighth_rank_idx;
}
