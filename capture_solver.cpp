#include <limits>

#include "capture_solver.h"

CaptureSolver::CaptureSolver(const Position& pos):
    _pos(pos)
{
    findBestMove();
}

void CaptureSolver::moveDone(Move move)
{
    _pos += move;
    findBestMove();
}

void CaptureSolver::findBestMove()
{
    const std::vector<Move> moves = _pos.possibleMoves();
    Move bestMove;
    float bestMoveEval = std::numeric_limits<float>::lowest();
    const bool reverseEval = !_pos.isWhiteTurn();

    for (auto move: moves)
    {
        const Position pos(_pos, move);
        float eval = pos.evaluate();
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
