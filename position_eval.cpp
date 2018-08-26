#include <cassert>

#include "position.h"


float Position::evaluate() const
{
    return float(evaluateMaterial());
}


int Position::evaluateMaterial() const
{
    int eval = 0;
    for (int s_idx = 0; s_idx < BOARD_SIZE * BOARD_SIZE; ++s_idx)
        eval += m_board[s_idx].value();

    return eval;
}

