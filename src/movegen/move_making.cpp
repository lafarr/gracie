#include "include/movegen/move_making.hpp"
#include "include/types/position.hpp"
#include "include/utils/bithacks.hpp"

namespace
{
    auto opponent_of(gracie::Color color) -> gracie::Color
    {
        return color == gracie::Color::white ? gracie::Color::black : gracie::Color::white;
    }

    auto recompute_occupied(const gracie::Position& pos, gracie::Color color) -> u64
    {
        u64 occupied = 0;
        for (const auto piece : {gracie::Piece::king, gracie::Piece::queen, gracie::Piece::rook,
                                 gracie::Piece::bishop, gracie::Piece::knight, gracie::Piece::pawn})
        {
            if (const auto it = pos.piece_positions.find({ color, piece }); it != pos.piece_positions.end())
            {
                occupied |= it->second;
            }
        }

        return occupied;
    }

    auto recompute_position(gracie::Position pos) -> gracie::Position
    {
        pos.white_occupied = recompute_occupied(pos, gracie::Color::white);
        pos.black_occupied = recompute_occupied(pos, gracie::Color::black);
        pos.all_occupied = pos.white_occupied | pos.black_occupied;
        pos.empty_squares = ~pos.all_occupied;
        return pos;
    }

    auto infer_captured_piece(const gracie::Position& pos, gracie::Color victim_color, u64 square_bit)
        -> std::optional<gracie::Piece>
    {
        for (const auto piece : {gracie::Piece::king, gracie::Piece::queen, gracie::Piece::rook,
                                 gracie::Piece::bishop, gracie::Piece::knight, gracie::Piece::pawn})
        {
            if (const auto it = pos.piece_positions.find({ victim_color, piece });
                it != pos.piece_positions.end() && ((it->second & square_bit) != 0))
            {
                return piece;
            }
        }

        return std::nullopt;
    }
}

auto gracie::move(const gracie::Position& board_pos, const gracie::Move& move) -> gracie::Position
{
    gracie::Position new_pos = board_pos;
    const auto from_bit = gracie::bit(move.from);
    const auto to_bit = gracie::bit(move.to);

    auto& piece_pos = new_pos.piece_positions.at({ move.color, move.piece });
    piece_pos = (piece_pos & ~from_bit) | to_bit;

    const auto victim_color = opponent_of(move.color);
    const auto captured_piece = move.captured_piece.has_value() ? move.captured_piece
                                                                : infer_captured_piece(board_pos, victim_color, to_bit);
    if (captured_piece.has_value())
    {
        auto& victim_piece_pos = new_pos.piece_positions.at({ victim_color, *captured_piece });
        victim_piece_pos &= ~to_bit;
    }

    return recompute_position(new_pos);
}

auto gracie::unmove(const gracie::Position& board_pos, const gracie::Move& move) -> gracie::Position
{
    auto new_board = board_pos;
    const auto from_bit = gracie::bit(move.from);
    const auto to_bit = gracie::bit(move.to);
    auto& piece_pos = new_board.piece_positions.at({ move.color, move.piece });
    piece_pos = (piece_pos & ~to_bit) | from_bit;

    if (move.captured_piece.has_value())
    {
        auto& victim_piece_pos = new_board.piece_positions[{ opponent_of(move.color), *move.captured_piece }];
        victim_piece_pos |= to_bit;
    }

    return recompute_position(new_board);
}
