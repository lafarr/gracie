#define BOOST_TEST_MODULE move_gen_tests
#include <boost/test/included/unit_test.hpp>

#include "include/move_gen/king.hpp"
#include "include/move_gen/knight.hpp"
#include "include/move_gen/pawn.hpp"
#include "include/utils/bithacks.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>

namespace
{
    struct Pawn_test_case
    {
        std::uint64_t white_pawns;
        std::uint64_t empty_squares;
        std::uint64_t black_occupied;
        std::uint64_t expected_moves;
    };

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
                if ((next_file < 0) || (next_file >= 8) || (next_rank < 0) || (next_rank >= 8))
                {
                    continue;
                }

                moves |= gracie::bit(square_idx(static_cast<std::size_t>(next_file), static_cast<std::size_t>(next_rank)));
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
            if ((next_file < 0) || (next_file >= 8) || (next_rank < 0) || (next_rank >= 8))
            {
                continue;
            }

            moves |= gracie::bit(square_idx(static_cast<std::size_t>(next_file), static_cast<std::size_t>(next_rank)));
        }

        return moves;
    }

    constexpr auto white_pawn_test_cases = std::array<Pawn_test_case, 8>{{
        {
            .white_pawns = gracie::bit(12),
            .empty_squares = ~(gracie::bit(12)),
            .black_occupied = 0,
            .expected_moves = squares({20, 28}),
        },
        {
            .white_pawns = gracie::bit(12),
            .empty_squares = ~(gracie::bit(12) | gracie::bit(20)),
            .black_occupied = 0,
            .expected_moves = 0,
        },
        {
            .white_pawns = gracie::bit(12),
            .empty_squares = ~(gracie::bit(12) | gracie::bit(28)),
            .black_occupied = 0,
            .expected_moves = squares({20}),
        },
        {
            .white_pawns = gracie::bit(12),
            .empty_squares = ~(gracie::bit(12) | gracie::bit(20) | gracie::bit(28)),
            .black_occupied = 0,
            .expected_moves = 0,
        },
        {
            .white_pawns = gracie::bit(28),
            .empty_squares = ~(gracie::bit(28)),
            .black_occupied = 0,
            .expected_moves = squares({36}),
        },
        {
            .white_pawns = gracie::bit(28),
            .empty_squares = ~(gracie::bit(28) | squares({35, 37})),
            .black_occupied = squares({35, 37}),
            .expected_moves = squares({35, 36, 37}),
        },
        {
            .white_pawns = gracie::bit(8),
            .empty_squares = ~(gracie::bit(8) | gracie::bit(17)),
            .black_occupied = gracie::bit(17),
            .expected_moves = squares({16, 24, 17}),
        },
        {
            .white_pawns = squares({8, 12}),
            .empty_squares = ~(squares({8, 12, 16, 20, 24, 28, 17, 21})),
            .black_occupied = squares({17, 21}),
            .expected_moves = squares({16, 17, 20, 21, 24, 28}),
        },
    }};
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

BOOST_AUTO_TEST_CASE(white_pawn_move_generation_matches_scenario_matrix)
{
    for (std::size_t idx = 0; idx < white_pawn_test_cases.size(); ++idx)
    {
        const auto& test_case = white_pawn_test_cases.at(idx);
        BOOST_TEST_CONTEXT("pawn_case=" << idx)
        {
            BOOST_TEST(gracie::gen_white_pawn_moves(test_case.white_pawns,
                                                    test_case.empty_squares,
                                                    test_case.black_occupied) == test_case.expected_moves);
        }
    }
}
