#include <limits>

#include "capture_solver.h"

class CaptureSolverPosition: public fatpup::Position
{
public:
    CaptureSolverPosition(const fatpup::Position& prev_pos, fatpup::Move move):
        Position(prev_pos)
    {
        moveDone(move);
    }

    int evaluateMaterial() const
    {
        int eval = 0;
        for (int s_idx = 0; s_idx < fatpup::BOARD_SIZE * fatpup::BOARD_SIZE; ++s_idx)
            eval += m_board[s_idx].value();

        return eval;
    }
};

CaptureSolver::CaptureSolver(const fatpup::Position& pos):
    _pos(pos)
{
    findBestMove();
}

void CaptureSolver::moveDone(fatpup::Move move)
{
    _pos += move;
    findBestMove();
}

void CaptureSolver::findBestMove()
{
    const auto moves = _pos.possibleMoves();
    fatpup::Move bestMove;
    int bestMoveEval = std::numeric_limits<int>::lowest();
    const bool reverseEval = !_pos.isWhiteTurn();

    for (auto move: moves)
    {
        const CaptureSolverPosition pos(_pos, move);
        int eval = pos.evaluateMaterial();
        if (reverseEval)
            eval = -eval;

        if (eval > bestMoveEval)
        {
            bestMoveEval = eval;
            bestMove = move;
        }
    }

    _bestMove = bestMove;
}
