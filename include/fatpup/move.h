#ifndef FATPUP_MOVE_H
#define FATPUP_MOVE_H

#include <string>

namespace fatpup
{
    union Move
    {
        Move(): raw_block(0) {}

        // works only for "regular" moves, i.e. castling won't be represented correctly with
        // just Move(ROW1, COLE, ROW1, COLG) as rook_src_col/rook_dst_col won't be set. The right
        // way to handle it in UI is:
        // Position pos;
        // pos.setInitial/setFEN/etc.
        // ...
        // Move move;
        // // pos.possibleMoves() returns a vector of up to 4 moves (a7a8Q, a7a8R, etc.), but in most cases it's
        // // only one move. Most importantly, possibleMoves() will set all the fields correctly
        // const auto moves = pos.possibleMoves(src_square_row, src_square_col, dst_square_row, dst_square_col);
        // if (!moves.empty())
        //     move = moves[0];
        // else
        //     error

        Move(unsigned int src_square_row, unsigned int src_square_col, unsigned int dst_square_row, unsigned int dst_square_col):
            raw_block(0)
        {
            fields.src_row = src_square_row;
            fields.src_col = src_square_col;
            fields.dst_row = dst_square_row;
            fields.dst_col = dst_square_col;
        }

        bool isEmpty() const { return !raw_block; }
        void setEmpty() { raw_block = 0; }

        // for debugging/testing purposes, so that you can set a position up like this: "pos += Move("e2e4");"
        // note that it will work only for "regular" moves, i.e. castling won't be represented correctly with
        // just Move("e1g1") as rook_src_col/rook_dst_col won't be set
        Move(const std::string& move_string);

        struct
        {
            unsigned int    src_row : 3;
            unsigned int    src_col : 3;
            unsigned int    dst_row : 3;
            unsigned int    dst_col : 3;

            // castling case
            unsigned int    rook_src_col : 3;
            unsigned int    rook_dst_col : 3;

            unsigned int    promoted_to : 3;
        }                   fields;

        unsigned int        raw_block;
    };
}   // namespace fatpup

#endif // FATPUP_MOVE_H
