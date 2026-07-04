#define BOOST_TEST_MODULE move_gen_tests
#include <boost/test/included/unit_test.hpp>

#include "include/move_gen/bishop.hpp"
#include "include/move_gen/king.hpp"
#include "include/move_gen/knight.hpp"
#include "include/move_gen/pawn.hpp"
#include "include/move_gen/rook.hpp"
#include "include/color.hpp"
#include "include/utils/bithacks.hpp"
#include "include/utils/constants.hpp"
#include "include/utils/masks.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <vector>

namespace
{
    [[nodiscard]] constexpr auto squares(std::initializer_list<std::size_t> idxs) -> std::uint64_t
    {
        std::uint64_t board = 0;
        for (const auto idx : idxs)
        {
            board |= gracie::bit(idx);
        }

        return board;
    }

    [[nodiscard]] constexpr auto file_of(std::size_t idx) -> std::size_t
    {
        return idx % 8;
    }

    [[nodiscard]] constexpr auto rank_of(std::size_t idx) -> std::size_t
    {
        return idx / 8;
    }

    [[nodiscard]] constexpr auto square_idx(std::size_t file, std::size_t rank) -> std::size_t
    {
        return (rank * 8) + file;
    }

    [[nodiscard]] constexpr auto expected_king_moves(std::size_t idx) -> std::uint64_t
    {
        std::uint64_t moves = 0;
        const auto file = file_of(idx);
        const auto rank = rank_of(idx);

        for (int rank_delta = -1; rank_delta <= 1; ++rank_delta)
        {
            for (int file_delta = -1; file_delta <= 1; ++file_delta)
            {
                if ((rank_delta == 0) && (file_delta == 0))
                {
                    continue;
                }

                const auto next_file = static_cast<int>(file) + file_delta;
                const auto next_rank = static_cast<int>(rank) + rank_delta;
                if ((next_file < gracie::first_file_idx) || (next_file > gracie::last_file_idx) ||
                    (next_rank < gracie::first_rank_idx) || (next_rank > gracie::back_rank))
                {
                    continue;
                }

                moves |= gracie::bit(square_idx(static_cast<std::size_t>(next_file), static_cast<std::size_t>(next_rank)));
            }
        }

        return moves;
    }

    [[nodiscard]] constexpr auto expected_bishop_moves(std::size_t idx,
                                                       std::uint64_t self_occupied,
                                                       std::uint64_t enemy_occupied) -> std::uint64_t
    {
        std::uint64_t moves = 0;
        const auto file = file_of(idx);
        const auto rank = rank_of(idx);

        for (std::size_t next_file = file + 1, next_rank = rank + 1; next_file < 8 && next_rank < 8;
             ++next_file, ++next_rank)
        {
            const auto next_square = square_idx(next_file, next_rank);
            const auto next_bit = gracie::bit(next_square);
            if ((self_occupied & next_bit) != 0)
            {
                break;
            }

            moves |= next_bit;
            if ((enemy_occupied & next_bit) != 0)
            {
                break;
            }
        }

        for (std::size_t next_file = file, next_rank = rank; next_file-- > 0 && ++next_rank < 8;)
        {
            const auto next_square = square_idx(next_file, next_rank);
            const auto next_bit = gracie::bit(next_square);
            if ((self_occupied & next_bit) != 0)
            {
                break;
            }

            moves |= next_bit;
            if ((enemy_occupied & next_bit) != 0)
            {
                break;
            }
        }

        for (std::size_t next_file = file + 1, next_rank = rank; next_file < 8 && next_rank-- > 0; ++next_file)
        {
            const auto next_square = square_idx(next_file, next_rank);
            const auto next_bit = gracie::bit(next_square);
            if ((self_occupied & next_bit) != 0)
            {
                break;
            }

            moves |= next_bit;
            if ((enemy_occupied & next_bit) != 0)
            {
                break;
            }
        }

        for (std::size_t next_file = file, next_rank = rank; next_file-- > 0 && next_rank-- > 0;)
        {
            const auto next_square = square_idx(next_file, next_rank);
            const auto next_bit = gracie::bit(next_square);
            if ((self_occupied & next_bit) != 0)
            {
                break;
            }

            moves |= next_bit;
            if ((enemy_occupied & next_bit) != 0)
            {
                break;
            }
        }

        return moves;
    }

    [[nodiscard]] constexpr auto expected_knight_moves(std::size_t idx) -> std::uint64_t
    {
        constexpr std::array<std::array<int, 2>, 8> deltas{{
            {{1, 2}},
            {{2, 1}},
            {{2, -1}},
            {{1, -2}},
            {{-1, -2}},
            {{-2, -1}},
            {{-2, 1}},
            {{-1, 2}},
        }};

        std::uint64_t moves = 0;
        const auto file = file_of(idx);
        const auto rank = rank_of(idx);

        for (const auto& delta : deltas)
        {
            const auto next_file = static_cast<int>(file) + delta[0];
            const auto next_rank = static_cast<int>(rank) + delta[1];
            if ((next_file < gracie::first_file_idx) || (next_file > gracie::last_file_idx) ||
                (next_rank < gracie::first_rank_idx) || (next_rank > gracie::back_rank))
            {
                continue;
            }

            moves |= gracie::bit(square_idx(static_cast<std::size_t>(next_file), static_cast<std::size_t>(next_rank)));
        }

        return moves;
    }

    [[nodiscard]] constexpr auto expected_rook_moves(std::size_t idx,
                                                     std::uint64_t self_occupied,
                                                     std::uint64_t enemy_occupied) -> std::uint64_t
    {
        std::uint64_t moves = 0;
        const auto file = file_of(idx);
        const auto rank = rank_of(idx);

        for (std::size_t next_rank = rank + 1; next_rank < 8; ++next_rank)
        {
            const auto next_square = square_idx(file, next_rank);
            const auto next_bit = gracie::bit(next_square);
            if ((self_occupied & next_bit) != 0)
            {
                break;
            }

            moves |= next_bit;
            if ((enemy_occupied & next_bit) != 0)
            {
                break;
            }
        }

        for (std::size_t next_rank = rank; next_rank-- > 0;)
        {
            const auto next_square = square_idx(file, next_rank);
            const auto next_bit = gracie::bit(next_square);
            if ((self_occupied & next_bit) != 0)
            {
                break;
            }

            moves |= next_bit;
            if ((enemy_occupied & next_bit) != 0)
            {
                break;
            }
        }

        for (std::size_t next_file = file + 1; next_file < 8; ++next_file)
        {
            const auto next_square = square_idx(next_file, rank);
            const auto next_bit = gracie::bit(next_square);
            if ((self_occupied & next_bit) != 0)
            {
                break;
            }

            moves |= next_bit;
            if ((enemy_occupied & next_bit) != 0)
            {
                break;
            }
        }

        for (std::size_t next_file = file; next_file-- > 0;)
        {
            const auto next_square = square_idx(next_file, rank);
            const auto next_bit = gracie::bit(next_square);
            if ((self_occupied & next_bit) != 0)
            {
                break;
            }

            moves |= next_bit;
            if ((enemy_occupied & next_bit) != 0)
            {
                break;
            }
        }

        return moves;
    }

    [[nodiscard]] auto moves_to_bitboard(const std::vector<gracie::Move>& moves, std::size_t from) -> std::uint64_t
    {
        std::uint64_t move_board = 0;
        for (const auto& move : moves)
        {
            if (move.from != static_cast<int>(from))
            {
                continue;
            }

            move_board |= gracie::bit(static_cast<std::size_t>(move.to));
        }

        return move_board;
    }

    [[nodiscard]] constexpr auto encode_move(int from, int to) -> int
    {
        return (from * 64) + to;
    }

    [[nodiscard]] auto encode_moves(const std::vector<gracie::Move>& moves) -> std::vector<int>
    {
        std::vector<int> encoded_moves;
        encoded_moves.reserve(moves.size());
        for (const auto& move : moves)
        {
            encoded_moves.push_back(encode_move(move.from, move.to));
        }

        return encoded_moves;
    }
} // namespace

BOOST_AUTO_TEST_CASE(king_attack_table_matches_reference_for_all_squares)
{
    for (std::size_t idx = 0; idx < 64; ++idx)
    {
        BOOST_TEST_CONTEXT("square=" << idx)
        {
            BOOST_TEST(gracie::gen_possible_king_moves(idx) == expected_king_moves(idx));
        }
    }
}

BOOST_AUTO_TEST_CASE(knight_attack_table_matches_reference_for_all_squares)
{
    for (std::size_t idx = 0; idx < 64; ++idx)
    {
        BOOST_TEST_CONTEXT("square=" << idx)
        {
            BOOST_TEST(gracie::gen_possible_knight_moves(idx) == expected_knight_moves(idx));
        }
    }
}

BOOST_AUTO_TEST_CASE(pawn_move_generation_matches_exhaustive_single_pawn_local_patterns)
{
    for (const auto color : {gracie::Color::white, gracie::Color::black})
    {
        for (std::size_t idx = 0; idx < 64; ++idx)
        {
            const auto pawn = gracie::bit(idx);
            const auto file = file_of(idx);
            const auto rank = rank_of(idx);

            const auto one_step_valid = (color == gracie::Color::white) ? (rank < 7) : (rank > 0);
            const auto two_step_valid =
                (color == gracie::Color::white) ? (rank == 1) : (rank == 6);
            const auto left_capture_valid =
                (color == gracie::Color::white) ? (rank < 7 && file > 0) : (rank > 0 && file > 0);
            const auto right_capture_valid =
                (color == gracie::Color::white) ? (rank < 7 && file < 7) : (rank > 0 && file < 7);

            const auto one_step_square = (color == gracie::Color::white) ? idx + 8 : idx - 8;
            const auto two_step_square = (color == gracie::Color::white) ? idx + 16 : idx - 16;
            const auto left_capture_square = (color == gracie::Color::white) ? idx + 7 : idx - 9;
            const auto right_capture_square = (color == gracie::Color::white) ? idx + 9 : idx - 7;

            for (int occupancy_pattern = 0; occupancy_pattern < 16; ++occupancy_pattern)
            {
                std::uint64_t empty_squares = ~pawn;
                std::uint64_t enemy_occupied = 0;

                const auto one_step_empty = ((occupancy_pattern & 0b0001) == 0);
                const auto two_step_empty = ((occupancy_pattern & 0b0010) == 0);
                const auto left_enemy = (occupancy_pattern & 0b0100) != 0;
                const auto right_enemy = (occupancy_pattern & 0b1000) != 0;

                if (one_step_valid && !one_step_empty)
                {
                    empty_squares &= ~gracie::bit(one_step_square);
                }

                if (two_step_valid && !two_step_empty)
                {
                    empty_squares &= ~gracie::bit(two_step_square);
                }

                if (left_capture_valid && left_enemy)
                {
                    empty_squares &= ~gracie::bit(left_capture_square);
                    enemy_occupied |= gracie::bit(left_capture_square);
                }

                if (right_capture_valid && right_enemy)
                {
                    empty_squares &= ~gracie::bit(right_capture_square);
                    enemy_occupied |= gracie::bit(right_capture_square);
                }

                std::uint64_t expected_moves = 0;
                if (one_step_valid && one_step_empty)
                {
                    expected_moves |= gracie::bit(one_step_square);
                    if (two_step_valid && two_step_empty)
                    {
                        expected_moves |= gracie::bit(two_step_square);
                    }
                }

                if (left_capture_valid && left_enemy)
                {
                    expected_moves |= gracie::bit(left_capture_square);
                }

                if (right_capture_valid && right_enemy)
                {
                    expected_moves |= gracie::bit(right_capture_square);
                }

                BOOST_TEST_CONTEXT("color=" << static_cast<int>(color)
                                   << " square=" << idx
                                   << " pattern=" << occupancy_pattern)
                {
                    BOOST_TEST(gracie::gen_pawn_moves(color, pawn, empty_squares, enemy_occupied) == expected_moves);
                }
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(bishop_attack_generation_matches_reference_on_empty_board_for_all_squares)
{
    for (std::size_t idx = 0; idx < 64; ++idx)
    {
        const auto bishop = gracie::bit(idx);
        const auto moves = gracie::gen_bishop_moves(bishop, bishop, 0);
        BOOST_TEST_CONTEXT("square=" << idx)
        {
            BOOST_TEST(moves_to_bitboard(moves, idx) == expected_bishop_moves(idx, bishop, 0));
        }
    }
}

BOOST_AUTO_TEST_CASE(bishop_move_generation_matches_blocker_and_multi_bishop_scenarios)
{
    struct Bishop_test_case
    {
        std::uint64_t bishops;
        std::uint64_t self_occupied;
        std::uint64_t enemy_occupied;
        std::vector<int> expected_moves;
    };

    const auto bishop_test_cases = std::array<Bishop_test_case, 3>{{
        {
            .bishops = gracie::bit(27),
            .self_occupied = gracie::bit(27),
            .enemy_occupied = 0,
            .expected_moves = {
                encode_move(27, 0),  encode_move(27, 6),  encode_move(27, 9),  encode_move(27, 13),
                encode_move(27, 18), encode_move(27, 20), encode_move(27, 34), encode_move(27, 36),
                encode_move(27, 41), encode_move(27, 45), encode_move(27, 48), encode_move(27, 54),
                encode_move(27, 63),
            },
        },
        {
            .bishops = gracie::bit(27),
            .self_occupied = squares({27, 18, 45}),
            .enemy_occupied = squares({13, 36}),
            .expected_moves = {
                encode_move(27, 13), encode_move(27, 20), encode_move(27, 34), encode_move(27, 36),
                encode_move(27, 41), encode_move(27, 48),
            },
        },
        {
            .bishops = squares({2, 61}),
            .self_occupied = squares({2, 61}),
            .enemy_occupied = squares({16, 54}),
            .expected_moves = {
                encode_move(2, 9),  encode_move(2, 11), encode_move(2, 16), encode_move(2, 20),
                encode_move(2, 29), encode_move(2, 38), encode_move(2, 47),
                encode_move(61, 16), encode_move(61, 25), encode_move(61, 34), encode_move(61, 43),
                encode_move(61, 52), encode_move(61, 54),
            },
        },
    }};

    for (std::size_t idx = 0; idx < bishop_test_cases.size(); ++idx)
    {
        auto expected = bishop_test_cases.at(idx).expected_moves;
        auto actual = encode_moves(gracie::gen_bishop_moves(bishop_test_cases.at(idx).bishops,
                                                            bishop_test_cases.at(idx).self_occupied,
                                                            bishop_test_cases.at(idx).enemy_occupied));

        std::ranges::sort(actual);
        std::ranges::sort(expected);

        BOOST_TEST_CONTEXT("bishop_case=" << idx)
        {
            BOOST_TEST(actual.size() == expected.size());
            BOOST_TEST(actual == expected, boost::test_tools::per_element());
        }
    }
}

BOOST_AUTO_TEST_CASE(rook_attack_generation_matches_reference_on_empty_board_for_all_squares)
{
    for (std::size_t idx = 0; idx < 64; ++idx)
    {
        const auto rook = gracie::bit(idx);
        const auto moves = gracie::gen_rook_moves(rook, rook, 0);
        BOOST_TEST_CONTEXT("square=" << idx)
        {
            BOOST_TEST(moves_to_bitboard(moves, idx) == expected_rook_moves(idx, rook, 0));
        }
    }
}

BOOST_AUTO_TEST_CASE(rook_move_generation_matches_blocker_and_multi_rook_scenarios)
{
    struct Rook_test_case
    {
        std::uint64_t rooks;
        std::uint64_t self_occupied;
        std::uint64_t enemy_occupied;
        std::vector<int> expected_moves;
    };

    const auto rook_test_cases = std::array<Rook_test_case, 3>{{
        {
            .rooks = gracie::bit(27),
            .self_occupied = gracie::bit(27),
            .enemy_occupied = 0,
            .expected_moves = {
                encode_move(27, 3),  encode_move(27, 11), encode_move(27, 19), encode_move(27, 24),
                encode_move(27, 25), encode_move(27, 26), encode_move(27, 28), encode_move(27, 29),
                encode_move(27, 30), encode_move(27, 31), encode_move(27, 35), encode_move(27, 43),
                encode_move(27, 51), encode_move(27, 59),
            },
        },
        {
            .rooks = gracie::bit(27),
            .self_occupied = squares({27, 19, 29}),
            .enemy_occupied = squares({25, 43}),
            .expected_moves = {
                encode_move(27, 25), encode_move(27, 26), encode_move(27, 28), encode_move(27, 35), encode_move(27, 43),
            },
        },
        {
            .rooks = squares({0, 63}),
            .self_occupied = squares({0, 63}),
            .enemy_occupied = squares({7, 56}),
            .expected_moves = {
                encode_move(0, 1),   encode_move(0, 2),   encode_move(0, 3),   encode_move(0, 4),   encode_move(0, 5),
                encode_move(0, 6),   encode_move(0, 7),   encode_move(0, 8),   encode_move(0, 16),  encode_move(0, 24),
                encode_move(0, 32),  encode_move(0, 40),  encode_move(0, 48),  encode_move(0, 56),  encode_move(63, 7),
                encode_move(63, 15), encode_move(63, 23), encode_move(63, 31), encode_move(63, 39), encode_move(63, 47),
                encode_move(63, 55), encode_move(63, 56), encode_move(63, 57), encode_move(63, 58), encode_move(63, 59),
                encode_move(63, 60), encode_move(63, 61), encode_move(63, 62),
            },
        },
    }};

    for (std::size_t idx = 0; idx < rook_test_cases.size(); ++idx)
    {
        const auto& test_case = rook_test_cases.at(idx);
        auto actual = encode_moves(gracie::gen_rook_moves(test_case.rooks,
                                                          test_case.self_occupied,
                                                          test_case.enemy_occupied));
        auto expected = test_case.expected_moves;

        std::ranges::sort(actual);
        std::ranges::sort(expected);

        BOOST_TEST_CONTEXT("rook_case=" << idx)
        {
            BOOST_TEST(actual.size() == expected.size());
            BOOST_TEST(actual == expected, boost::test_tools::per_element());
        }
    }
}
