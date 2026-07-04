#define BOOST_TEST_MODULE move_gen_tests
#include <boost/test/included/unit_test.hpp>

#include "include/movegen/bishop.hpp"
#include "include/movegen/king.hpp"
#include "include/movegen/knight.hpp"
#include "include/movegen/move_making.hpp"
#include "include/movegen/pawn.hpp"
#include "include/movegen/queen.hpp"
#include "include/movegen/rook.hpp"
#include "include/types/color.hpp"
#include "include/types/piece.hpp"
#include "include/types/position.hpp"
#include "include/utils/bithacks.hpp"
#include "include/utils/constants.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <ranges>
#include <utility>
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
        constexpr auto bishop_rays = std::array{
            std::array{ 1, 1 },
            std::array{ -1, 1 },
            std::array{ 1, -1 },
            std::array{ -1, -1 },
        };

        std::uint64_t attacks = 0;
        for (const auto& ray : bishop_rays)
        {
            for (int file = static_cast<int>(file_of(idx)) + ray[0], rank = static_cast<int>(rank_of(idx)) + ray[1];
                 file >= gracie::first_file_idx && file <= gracie::last_file_idx &&
                 rank >= gracie::first_rank_idx && rank <= gracie::back_rank;
                 file += ray[0], rank += ray[1])
            {
                const auto square = square_idx(static_cast<std::size_t>(file), static_cast<std::size_t>(rank));
                const auto square_bit = gracie::bit(square);
                if ((self_occupied & square_bit) != 0)
                {
                    break;
                }

                attacks |= square_bit;
                if ((enemy_occupied & square_bit) != 0)
                {
                    break;
                }
            }
        }

        return attacks;
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
        constexpr auto rook_rays = std::array{
            std::array{ 0, 1 },
            std::array{ 0, -1 },
            std::array{ 1, 0 },
            std::array{ -1, 0 },
        };

        std::uint64_t attacks = 0;
        for (const auto& ray : rook_rays)
        {
            for (int file = static_cast<int>(file_of(idx)) + ray[0], rank = static_cast<int>(rank_of(idx)) + ray[1];
                 file >= gracie::first_file_idx && file <= gracie::last_file_idx &&
                 rank >= gracie::first_rank_idx && rank <= gracie::back_rank;
                 file += ray[0], rank += ray[1])
            {
                const auto square = square_idx(static_cast<std::size_t>(file), static_cast<std::size_t>(rank));
                const auto square_bit = gracie::bit(square);
                if ((self_occupied & square_bit) != 0)
                {
                    break;
                }

                attacks |= square_bit;
                if ((enemy_occupied & square_bit) != 0)
                {
                    break;
                }
            }
        }

        return attacks;
    }

    [[nodiscard]] constexpr auto expected_queen_moves(std::size_t idx,
                                                      std::uint64_t self_occupied,
                                                      std::uint64_t enemy_occupied) -> std::uint64_t
    {
        return expected_rook_moves(idx, self_occupied, enemy_occupied) |
               expected_bishop_moves(idx, self_occupied, enemy_occupied);
    }

    [[nodiscard]] auto moves_to_bitboard(const std::vector<gracie::Move>& moves, std::size_t from) -> std::uint64_t
    {
        std::uint64_t move_board = 0;
        for (const auto& move : moves)
        {
            if (move.from != from)
            {
                continue;
            }

            move_board |= gracie::bit(move.to);
        }

        return move_board;
    }

    [[nodiscard]] constexpr auto encode_move(std::size_t from, std::size_t to) -> int
    {
        return static_cast<int>((from * 64) + to);
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

    [[nodiscard]] auto encode_attacks(std::size_t from, std::uint64_t attacks) -> std::vector<int>
    {
        std::vector<int> encoded;
        while (attacks > 0)
        {
            encoded.push_back(encode_move(from, gracie::pop_lsb(attacks)));
        }
        return encoded;
    }

    [[nodiscard]] auto ray_squares(std::size_t idx, int file_delta, int rank_delta) -> std::vector<std::size_t>
    {
        std::vector<std::size_t> squares_on_ray;
        for (int file = static_cast<int>(file_of(idx)) + file_delta, rank = static_cast<int>(rank_of(idx)) + rank_delta;
             file >= gracie::first_file_idx && file <= gracie::last_file_idx &&
             rank >= gracie::first_rank_idx && rank <= gracie::back_rank;
             file += file_delta, rank += rank_delta)
        {
            squares_on_ray.push_back(square_idx(static_cast<std::size_t>(file), static_cast<std::size_t>(rank)));
        }

        return squares_on_ray;
    }

    template <std::size_t NumRays, typename Callback>
    auto for_each_nearest_blocker_state(std::size_t idx,
                                        const std::array<std::array<int, 2>, NumRays>& rays,
                                        Callback&& callback) -> void
    {
        struct Ray_state
        {
            std::uint64_t self_bits;
            std::uint64_t enemy_bits;
        };

        std::array<std::vector<Ray_state>, NumRays> ray_states{};
        for (std::size_t ray_idx = 0; ray_idx < NumRays; ++ray_idx)
        {
            ray_states.at(ray_idx).push_back({ .self_bits = 0, .enemy_bits = 0 });
            for (const auto square : ray_squares(idx, rays.at(ray_idx).at(0), rays.at(ray_idx).at(1)))
            {
                const auto square_bit = gracie::bit(square);
                ray_states.at(ray_idx).push_back({ .self_bits = square_bit, .enemy_bits = 0 });
                ray_states.at(ray_idx).push_back({ .self_bits = 0, .enemy_bits = square_bit });
            }
        }

        std::array<std::size_t, NumRays> state_indices{};
        while (true)
        {
            std::uint64_t self_occupied = gracie::bit(idx);
            std::uint64_t enemy_occupied = 0;
            for (std::size_t ray_idx = 0; ray_idx < NumRays; ++ray_idx)
            {
                const auto& state = ray_states.at(ray_idx).at(state_indices.at(ray_idx));
                self_occupied |= state.self_bits;
                enemy_occupied |= state.enemy_bits;
            }

            std::forward<Callback>(callback)(self_occupied, enemy_occupied);

            auto advanced = false;
            for (std::size_t ray_idx = 0; ray_idx < NumRays; ++ray_idx)
            {
                ++state_indices.at(ray_idx);
                if (state_indices.at(ray_idx) < ray_states.at(ray_idx).size())
                {
                    advanced = true;
                    break;
                }

                state_indices.at(ray_idx) = 0;
            }

            if (!advanced)
            {
                break;
            }
        }
    }

    struct Piece_on_square
    {
        gracie::Color color;
        gracie::Piece piece;
        std::size_t square;
    };

    [[nodiscard]] auto make_position(std::initializer_list<Piece_on_square> pieces) -> gracie::Position
    {
        gracie::Position pos{
            .white_occupied = 0,
            .black_occupied = 0,
            .all_occupied = 0,
            .empty_squares = ~std::uint64_t{ 0 },
            .piece_positions = {},
            .white_in_check = false,
            .black_in_check = false,
        };

        for (const auto color : {gracie::Color::white, gracie::Color::black})
        {
            for (const auto piece : {gracie::Piece::king, gracie::Piece::queen, gracie::Piece::rook,
                                     gracie::Piece::bishop, gracie::Piece::knight, gracie::Piece::pawn})
            {
                pos.piece_positions[{ color, piece }] = 0;
            }
        }

        for (const auto& piece : pieces)
        {
            const auto square_bit = gracie::bit(piece.square);
            pos.piece_positions.at({ piece.color, piece.piece }) |= square_bit;
            if (piece.color == gracie::Color::white)
            {
                pos.white_occupied |= square_bit;
            }
            else
            {
                pos.black_occupied |= square_bit;
            }
        }

        pos.all_occupied = pos.white_occupied | pos.black_occupied;
        pos.empty_squares = ~pos.all_occupied;
        return pos;
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

BOOST_AUTO_TEST_CASE(knight_move_generation_matches_reference_for_all_squares_and_self_occupancy_subsets)
{
    for (std::size_t idx = 0; idx < 64; ++idx)
    {
        const auto knight = gracie::bit(idx);
        const auto attack_mask = expected_knight_moves(idx);

        std::array<std::size_t, 8> attack_squares{};
        std::size_t attack_count = 0;
        auto remaining = attack_mask;
        while (remaining > 0)
        {
            attack_squares.at(attack_count++) = gracie::pop_lsb(remaining);
        }

        const auto subset_count = std::size_t{ 1 } << attack_count;
        for (std::size_t subset = 0; subset < subset_count; ++subset)
        {
            std::uint64_t self_occupied = knight;
            for (std::size_t bit_idx = 0; bit_idx < attack_count; ++bit_idx)
            {
                if (((subset >> bit_idx) & std::size_t{ 1 }) != 0)
                {
                    self_occupied |= gracie::bit(attack_squares.at(bit_idx));
                }
            }

            const auto board_pos = gracie::Position{
                .white_occupied = self_occupied,
                .black_occupied = 0,
                .all_occupied = self_occupied,
                .empty_squares = ~self_occupied,
                .piece_positions = {
                    {{ gracie::Color::white, gracie::Piece::knight }, knight},
                },
                .white_in_check = false,
                .black_in_check = false,
            };

            const auto actual = moves_to_bitboard(gracie::gen_knight_moves(board_pos, gracie::Color::white), idx);
            const auto expected = attack_mask & ~self_occupied;

            BOOST_TEST_CONTEXT("square=" << idx << " subset=" << subset)
            {
                BOOST_TEST(actual == expected);
            }
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
        BOOST_TEST_CONTEXT("square=" << idx)
        {
            BOOST_TEST(gracie::gen_bishop_attacks(idx, gracie::bit(idx), 0) == expected_bishop_moves(idx, gracie::bit(idx), 0));
        }
    }
}

BOOST_AUTO_TEST_CASE(bishop_move_generation_matches_blocker_and_multi_bishop_scenarios)
{
    constexpr auto bishop_rays = std::array{
        std::array{ 1, 1 },
        std::array{ -1, 1 },
        std::array{ 1, -1 },
        std::array{ -1, -1 },
    };

    for (std::size_t idx = 0; idx < 64; ++idx)
    {
        for_each_nearest_blocker_state(idx, bishop_rays, [idx](std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> void {
            const auto expected_attacks = expected_bishop_moves(idx, self_occupied, enemy_occupied);
            const auto actual_attacks = gracie::gen_bishop_attacks(idx, self_occupied, enemy_occupied);
            auto expected_moves = encode_attacks(idx, expected_attacks);
            auto actual_moves = encode_moves(gracie::gen_bishop_moves(gracie::bit(idx), self_occupied, enemy_occupied));

            std::ranges::sort(expected_moves);
            std::ranges::sort(actual_moves);

            BOOST_TEST_CONTEXT("square=" << idx
                               << " self=" << self_occupied
                               << " enemy=" << enemy_occupied)
            {
                BOOST_TEST(actual_attacks == expected_attacks);
                BOOST_TEST(actual_moves.size() == expected_moves.size());
                BOOST_TEST(actual_moves == expected_moves, boost::test_tools::per_element());
            }
        });
    }
}

BOOST_AUTO_TEST_CASE(rook_attack_generation_matches_reference_on_empty_board_for_all_squares)
{
    for (std::size_t idx = 0; idx < 64; ++idx)
    {
        BOOST_TEST_CONTEXT("square=" << idx)
        {
            BOOST_TEST(gracie::gen_rook_attacks(idx, gracie::bit(idx), 0) == expected_rook_moves(idx, gracie::bit(idx), 0));
        }
    }
}

BOOST_AUTO_TEST_CASE(rook_move_generation_matches_blocker_and_multi_rook_scenarios)
{
    constexpr auto rook_rays = std::array{
        std::array{ 0, 1 },
        std::array{ 0, -1 },
        std::array{ 1, 0 },
        std::array{ -1, 0 },
    };

    for (std::size_t idx = 0; idx < 64; ++idx)
    {
        for_each_nearest_blocker_state(idx, rook_rays, [idx](std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> void {
            const auto expected_attacks = expected_rook_moves(idx, self_occupied, enemy_occupied);
            const auto actual_attacks = gracie::gen_rook_attacks(idx, self_occupied, enemy_occupied);
            auto expected_moves = encode_attacks(idx, expected_attacks);
            auto actual_moves = encode_moves(gracie::gen_rook_moves(gracie::bit(idx), self_occupied, enemy_occupied));

            std::ranges::sort(expected_moves);
            std::ranges::sort(actual_moves);

            BOOST_TEST_CONTEXT("square=" << idx
                               << " self=" << self_occupied
                               << " enemy=" << enemy_occupied)
            {
                BOOST_TEST(actual_attacks == expected_attacks);
                BOOST_TEST(actual_moves.size() == expected_moves.size());
                BOOST_TEST(actual_moves == expected_moves, boost::test_tools::per_element());
            }
        });
    }
}

BOOST_AUTO_TEST_CASE(queen_attack_generation_matches_reference_on_empty_board_for_all_squares)
{
    for (std::size_t idx = 0; idx < 64; ++idx)
    {
        BOOST_TEST_CONTEXT("square=" << idx)
        {
            BOOST_TEST(gracie::gen_queen_attacks(idx, gracie::bit(idx), 0) == expected_queen_moves(idx, gracie::bit(idx), 0));
        }
    }
}

BOOST_AUTO_TEST_CASE(queen_move_generation_matches_blocker_and_multi_queen_scenarios)
{
    constexpr auto rook_rays = std::array{
        std::array{ 0, 1 },
        std::array{ 0, -1 },
        std::array{ 1, 0 },
        std::array{ -1, 0 },
    };
    constexpr auto bishop_rays = std::array{
        std::array{ 1, 1 },
        std::array{ -1, 1 },
        std::array{ 1, -1 },
        std::array{ -1, -1 },
    };
    struct Queen_test_case
    {
        std::uint64_t queens;
        std::uint64_t self_occupied;
        std::uint64_t enemy_occupied;
    };
    const auto mixed_queen_cases = std::array<Queen_test_case, 3>{{
        {
            .queens = gracie::bit(27),
            .self_occupied = squares({27, 18, 29}),
            .enemy_occupied = squares({13, 25, 36, 43}),
        },
        {
            .queens = gracie::bit(27),
            .self_occupied = squares({27, 20, 35}),
            .enemy_occupied = squares({24, 31, 45, 51}),
        },
        {
            .queens = squares({3, 60}),
            .self_occupied = squares({3, 60}),
            .enemy_occupied = squares({11, 19, 24, 39, 51, 52}),
        },
    }};

    for (std::size_t idx = 0; idx < 64; ++idx)
    {
        for_each_nearest_blocker_state(idx, rook_rays, [idx](std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> void {
                const auto expected_attacks = expected_queen_moves(idx, self_occupied, enemy_occupied);
                const auto actual_attacks = gracie::gen_queen_attacks(idx, self_occupied, enemy_occupied);
                auto expected_moves = encode_attacks(idx, expected_attacks);
                auto actual_moves = encode_moves(gracie::gen_queen_moves(gracie::bit(idx), self_occupied, enemy_occupied));

                std::ranges::sort(expected_moves);
                std::ranges::sort(actual_moves);

                BOOST_TEST_CONTEXT("orth_square=" << idx
                                   << " self=" << self_occupied
                                   << " enemy=" << enemy_occupied)
                {
                    BOOST_TEST(actual_attacks == expected_attacks);
                    BOOST_TEST(actual_moves.size() == expected_moves.size());
                    BOOST_TEST(actual_moves == expected_moves, boost::test_tools::per_element());
                }
        });

        for_each_nearest_blocker_state(idx, bishop_rays, [idx](std::uint64_t self_occupied, std::uint64_t enemy_occupied) -> void {
                const auto expected_attacks = expected_queen_moves(idx, self_occupied, enemy_occupied);
                const auto actual_attacks = gracie::gen_queen_attacks(idx, self_occupied, enemy_occupied);
                auto expected_moves = encode_attacks(idx, expected_attacks);
                auto actual_moves = encode_moves(gracie::gen_queen_moves(gracie::bit(idx), self_occupied, enemy_occupied));

                std::ranges::sort(expected_moves);
                std::ranges::sort(actual_moves);

                BOOST_TEST_CONTEXT("diag_square=" << idx
                                   << " self=" << self_occupied
                                   << " enemy=" << enemy_occupied)
                {
                    BOOST_TEST(actual_attacks == expected_attacks);
                    BOOST_TEST(actual_moves.size() == expected_moves.size());
                    BOOST_TEST(actual_moves == expected_moves, boost::test_tools::per_element());
                }
        });
    }

    for (std::size_t case_idx = 0; case_idx < mixed_queen_cases.size(); ++case_idx)
    {
        const auto& test_case = mixed_queen_cases.at(case_idx);
        auto expected_moves = std::vector<int>{};
        auto queens = test_case.queens;
        while (queens > 0)
        {
            const auto from = gracie::pop_lsb(queens);
            auto attacks = expected_queen_moves(from, test_case.self_occupied, test_case.enemy_occupied);
            auto encoded = encode_attacks(from, attacks);
            expected_moves.insert(expected_moves.end(), encoded.begin(), encoded.end());
        }
        auto actual_moves = encode_moves(gracie::gen_queen_moves(test_case.queens,
                                                                 test_case.self_occupied,
                                                                 test_case.enemy_occupied));
        std::ranges::sort(expected_moves);
        std::ranges::sort(actual_moves);

        BOOST_TEST_CONTEXT("mixed_queen_case=" << case_idx)
        {
            BOOST_TEST(actual_moves.size() == expected_moves.size());
            BOOST_TEST(actual_moves == expected_moves, boost::test_tools::per_element());
        }
    }
}

BOOST_AUTO_TEST_CASE(move_and_unmove_round_trip_for_quiet_moves)
{
    const auto initial = make_position({
        { gracie::Color::white, gracie::Piece::king, 4 },
        { gracie::Color::white, gracie::Piece::rook, 0 },
        { gracie::Color::black, gracie::Piece::king, 60 },
        { gracie::Color::black, gracie::Piece::bishop, 58 },
    });

    for (const auto piece : {gracie::Piece::king, gracie::Piece::rook, gracie::Piece::bishop})
    {
        const auto color = piece == gracie::Piece::bishop ? gracie::Color::black : gracie::Color::white;
        const auto from = piece == gracie::Piece::king ? 4U : (piece == gracie::Piece::rook ? 0U : 58U);
        const auto to = piece == gracie::Piece::king ? 12U : (piece == gracie::Piece::rook ? 24U : 44U);
        const gracie::Move move{
            .color = color,
            .piece = piece,
            .from = static_cast<u8>(from),
            .to = static_cast<u8>(to),
            .captured_piece = std::nullopt,
        };

        const auto after_move = gracie::move(initial, move);
        const auto after_unmove = gracie::unmove(after_move, move);

        BOOST_TEST_CONTEXT("piece=" << static_cast<int>(piece))
        {
            BOOST_TEST(after_unmove.white_occupied == initial.white_occupied);
            BOOST_TEST(after_unmove.black_occupied == initial.black_occupied);
            BOOST_TEST(after_unmove.all_occupied == initial.all_occupied);
            BOOST_TEST(after_unmove.empty_squares == initial.empty_squares);
            BOOST_TEST(after_unmove.piece_positions == initial.piece_positions);
        }
    }
}

BOOST_AUTO_TEST_CASE(move_and_unmove_round_trip_for_captures)
{
    struct Capture_test_case
    {
        gracie::Move move;
        gracie::Position position;
    };

    const auto capture_test_cases = std::array<Capture_test_case, 3>{{
        {
            .move = {
                .color = gracie::Color::white,
                .piece = gracie::Piece::rook,
                .from = static_cast<u8>(0),
                .to = static_cast<u8>(24),
                .captured_piece = gracie::Piece::knight,
            },
            .position = make_position({
                { gracie::Color::white, gracie::Piece::king, 4 },
                { gracie::Color::white, gracie::Piece::rook, 0 },
                { gracie::Color::black, gracie::Piece::king, 60 },
                { gracie::Color::black, gracie::Piece::knight, 24 },
            }),
        },
        {
            .move = {
                .color = gracie::Color::white,
                .piece = gracie::Piece::bishop,
                .from = static_cast<u8>(18),
                .to = static_cast<u8>(45),
                .captured_piece = gracie::Piece::rook,
            },
            .position = make_position({
                { gracie::Color::white, gracie::Piece::king, 4 },
                { gracie::Color::white, gracie::Piece::bishop, 18 },
                { gracie::Color::black, gracie::Piece::king, 60 },
                { gracie::Color::black, gracie::Piece::rook, 45 },
            }),
        },
        {
            .move = {
                .color = gracie::Color::black,
                .piece = gracie::Piece::queen,
                .from = static_cast<u8>(59),
                .to = static_cast<u8>(11),
                .captured_piece = gracie::Piece::pawn,
            },
            .position = make_position({
                { gracie::Color::white, gracie::Piece::king, 4 },
                { gracie::Color::white, gracie::Piece::pawn, 11 },
                { gracie::Color::black, gracie::Piece::king, 60 },
                { gracie::Color::black, gracie::Piece::queen, 59 },
            }),
        },
    }};

    for (std::size_t idx = 0; idx < capture_test_cases.size(); ++idx)
    {
        const auto& test_case = capture_test_cases.at(idx);
        const auto after_move = gracie::move(test_case.position, test_case.move);
        const auto after_unmove = gracie::unmove(after_move, test_case.move);

        BOOST_TEST_CONTEXT("capture_case=" << idx)
        {
            BOOST_TEST(after_unmove.white_occupied == test_case.position.white_occupied);
            BOOST_TEST(after_unmove.black_occupied == test_case.position.black_occupied);
            BOOST_TEST(after_unmove.all_occupied == test_case.position.all_occupied);
            BOOST_TEST(after_unmove.empty_squares == test_case.position.empty_squares);
            BOOST_TEST(after_unmove.piece_positions == test_case.position.piece_positions);
        }
    }
}
