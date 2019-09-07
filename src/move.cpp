#include <cassert>

#include "fatpup/move.h"
#include "fatpup/position.h"

namespace fatpup
{
    Move::Move(const std::string& move_string):
        raw_block(0)
    {
        assert(move_string.length() == 4);

        const std::string src_square = move_string.substr(0, 2);
        const std::string dst_square = move_string.substr(2, 2);

        fields.src_row = symbolToRowIdx(src_square[1]);
        fields.src_col = symbolToColumnIdx(src_square[0]);
        fields.dst_row = symbolToRowIdx(dst_square[1]);
        fields.dst_col = symbolToColumnIdx(dst_square[0]);
    }
}   // namespace fatpup
