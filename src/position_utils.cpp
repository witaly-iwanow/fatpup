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
            assert(0);  // to do
        }

        return result;
    }
}   // namespace fatpup
