#ifndef MOVE_H
#define MOVE_H

#include <string>


union Move
{
    Move() : raw_block(0) {}

    Move(unsigned int src_square_row, unsigned int src_square_col, unsigned int dst_square_row, unsigned int dst_square_col) :
        raw_block(0)
    {
        fields.src_row = src_square_row;
        fields.src_col = src_square_col;
        fields.dst_row = dst_square_row;
        fields.dst_col = dst_square_col;
    }

    // for debugging/testing purposes, so that you can set a position up like this: "pos += Move("e2e4");"
    // castling, en passant capturings and promotions are not currently supported
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

#endif // MOVE_H
