#define BOOST_TEST_MODULE move_gen_tests
#include <boost/test/included/unit_test.hpp>

#include "include/move_gen/king.hpp"
#include "include/move_gen/knight.hpp"
#include "include/move_gen/pawn.hpp"
#include "include/move_gen/rook.hpp"
#include "include/color.hpp"
#include "include/utils/bithacks.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <vector>

namespace
{
    struct Pawn_test_case
    {
        gracie::Color color;
        std::uint64_t pawns;
        std::uint64_t empty_squares;
        std::uint64_t enemy_occupied;
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

    constexpr auto pawn_test_cases = std::array<Pawn_test_case, 13>{{
        {
            .color = gracie::Color::white,
            .pawns = gracie::bit(12),
            .empty_squares = ~(gracie::bit(12)),
            .enemy_occupied = 0,
            .expected_moves = squares({20, 28}),
        },
        {
            .color = gracie::Color::white,
            .pawns = gracie::bit(12),
            .empty_squares = ~(gracie::bit(12) | gracie::bit(20)),
            .enemy_occupied = 0,
            .expected_moves = 0,
        },
        {
            .color = gracie::Color::white,
            .pawns = gracie::bit(12),
            .empty_squares = ~(gracie::bit(12) | gracie::bit(28)),
            .enemy_occupied = 0,
            .expected_moves = squares({20}),
        },
        {
            .color = gracie::Color::white,
            .pawns = gracie::bit(12),
            .empty_squares = ~(gracie::bit(12) | gracie::bit(20) | gracie::bit(28)),
            .enemy_occupied = 0,
            .expected_moves = 0,
        },
        {
            .color = gracie::Color::white,
            .pawns = gracie::bit(28),
            .empty_squares = ~(gracie::bit(28)),
            .enemy_occupied = 0,
            .expected_moves = squares({36}),
        },
        {
            .color = gracie::Color::white,
            .pawns = gracie::bit(28),
            .empty_squares = ~(gracie::bit(28) | squares({35, 37})),
            .enemy_occupied = squares({35, 37}),
            .expected_moves = squares({35, 36, 37}),
        },
        {
            .color = gracie::Color::white,
            .pawns = gracie::bit(8),
            .empty_squares = ~(gracie::bit(8) | gracie::bit(17)),
            .enemy_occupied = gracie::bit(17),
            .expected_moves = squares({16, 24, 17}),
        },
        {
            .color = gracie::Color::white,
            .pawns = squares({8, 12}),
            .empty_squares = ~(squares({8, 12, 17, 21})),
            .enemy_occupied = squares({17, 21}),
            .expected_moves = squares({16, 17, 20, 21, 24, 28}),
        },
        {
            .color = gracie::Color::black,
            .pawns = gracie::bit(52),
            .empty_squares = ~(gracie::bit(52)),
            .enemy_occupied = 0,
            .expected_moves = squares({44, 36}),
        },
        {
            .color = gracie::Color::black,
            .pawns = gracie::bit(52),
            .empty_squares = ~(gracie::bit(52) | gracie::bit(44)),
            .enemy_occupied = 0,
            .expected_moves = 0,
        },
        {
            .color = gracie::Color::black,
            .pawns = gracie::bit(52),
            .empty_squares = ~(gracie::bit(52) | gracie::bit(36)),
            .enemy_occupied = 0,
            .expected_moves = squares({44}),
        },
        {
            .color = gracie::Color::black,
            .pawns = gracie::bit(35),
            .empty_squares = ~(gracie::bit(35) | squares({26, 28})),
            .enemy_occupied = squares({26, 28}),
            .expected_moves = squares({27, 26, 28}),
        },
        {
            .color = gracie::Color::black,
            .pawns = squares({52, 55}),
            .empty_squares = ~(squares({52, 55, 45, 46})),
            .enemy_occupied = squares({45, 46}),
            .expected_moves = squares({44, 45, 46, 47, 36, 39}),
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

BOOST_AUTO_TEST_CASE(pawn_move_generation_matches_scenario_matrix)
{
    for (std::size_t idx = 0; idx < pawn_test_cases.size(); ++idx)
    {
        const auto& test_case = pawn_test_cases.at(idx);
        BOOST_TEST_CONTEXT("pawn_case=" << idx)
        {
            BOOST_TEST(gracie::gen_pawn_moves(test_case.color,
                                              test_case.pawns,
                                              test_case.empty_squares,
                                              test_case.enemy_occupied) == test_case.expected_moves);
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
