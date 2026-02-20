#include <cassert>

#include "fatpup/position.h"

namespace fatpup
{
    static constexpr char pieceSymbols[] = { ' ', ' ', 'N', 'B', 'R', 'Q', 'K' };

    std::string Position::moveToString(Move move) const
    {
        std::string result;

        if (move.fields.rook_src_col != move.fields.rook_dst_col)
        {
            // castling
            if (move.fields.rook_src_col == 0)
                result += "0-0-0";
            else
                result += "0-0";
        }
        else
        {
            const Square& src_square = square(move.fields.src_row, move.fields.src_col);
            const Square& dst_square = square(move.fields.dst_row, move.fields.dst_col);

            if (src_square.piece() != Pawn)
                result =+ pieceSymbols[src_square.piece()];

            result += (char)((int)('a') + move.fields.src_col);
            result += (char)((int)('1') + move.fields.src_row);

            // NB: in the case of en passant capturing the pawn moves to an empty square
            if (dst_square.piece() != Empty || (src_square.piece() == Pawn && move.fields.src_col != move.fields.dst_col))
                result += "x";
            else
                result += "-";

            result += (char)((int)('a') + move.fields.dst_col);
            result += (char)((int)('1') + move.fields.dst_row);

            if (move.fields.promoted_to > Pawn)
                result += pieceSymbols[move.fields.promoted_to];
        }

        const Position new_pos(*this, move);
        Position::State state = new_pos.getState();
        if (state == Position::State::Checkmate)
            result += "#";
        else if (state == Position::State::Check)
            result += "+";
        else if (state == Position::State::Stalemate)
            result += "@";  // suprisingly there's no designated sign for stalemate

        return result;
    }

    bool Position::isMoveCapture(Move move) const
    {
        const Square& src_square = square(move.fields.src_row, move.fields.src_col);
        const Square& dst_square = square(move.fields.dst_row, move.fields.dst_col);

        // NB: in the case of en passant capturing the pawn moves to an empty square
        if (dst_square.piece() != Empty || (src_square.piece() == Pawn && move.fields.src_col != move.fields.dst_col))
            return true;

        return false;
    }


    std::string Position::moveToStringPGN(Move move) const
    {
        std::string result;

        if (move.fields.rook_src_col != move.fields.rook_dst_col)
        {
            // castling
            if (move.fields.rook_src_col == 0)
                result += "O-O-O";
            else
                result += "O-O";
        }
        else
        {
            const Square& src_square = square(move.fields.src_row, move.fields.src_col);
            const unsigned char piece = src_square.piece();
            const bool capture = isMoveCapture(move);

            if (piece != Pawn)
            {
                result += pieceSymbols[piece];

                bool ambiguous = false;
                bool same_file = false;
                bool same_rank = false;

                const std::vector<Move> all_moves = possibleMoves();
                for (const auto& candidate : all_moves)
                {
                    if (candidate.fields.src_row == move.fields.src_row && candidate.fields.src_col == move.fields.src_col)
                        continue;
                    if (candidate.fields.dst_row != move.fields.dst_row || candidate.fields.dst_col != move.fields.dst_col)
                        continue;

                    const Square& candidate_src_square = square(candidate.fields.src_row, candidate.fields.src_col);
                    if (candidate_src_square.piece() != piece)
                        continue;

                    ambiguous = true;
                    if (candidate.fields.src_col == move.fields.src_col)
                        same_file = true;
                    if (candidate.fields.src_row == move.fields.src_row)
                        same_rank = true;
                }

                if (ambiguous)
                {
                    if (!same_file)
                        result += (char)((int)('a') + move.fields.src_col);
                    else if (!same_rank)
                        result += (char)((int)('1') + move.fields.src_row);
                    else
                    {
                        result += (char)((int)('a') + move.fields.src_col);
                        result += (char)((int)('1') + move.fields.src_row);
                    }
                }
            }
            else if (capture)
                result += (char)((int)('a') + move.fields.src_col);

            if (capture)
                result += "x";

            result += (char)((int)('a') + move.fields.dst_col);
            result += (char)((int)('1') + move.fields.dst_row);

            if (move.fields.promoted_to > Pawn)
            {
                result += "=";
                result += pieceSymbols[move.fields.promoted_to];
            }
        }

        const Position new_pos(*this, move);
        Position::State state = new_pos.getState();
        if (state == Position::State::Checkmate)
            result += "#";
        else if (state == Position::State::Check)
            result += "+";

        return result;
    }
}   // namespace fatpup
