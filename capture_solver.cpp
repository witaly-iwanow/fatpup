#include <limits>

#include "capture_solver.h"

namespace fatpup
{
    class CaptureSolverPosition: public Position
    {
    public:
        CaptureSolverPosition(const Position& prev_pos, Move move):
            Position(prev_pos)
        {
            moveDone(move);
        }

        int evaluateMaterial() const
        {
            int eval = 0;
            for (int s_idx = 0; s_idx < BOARD_SIZE * BOARD_SIZE; ++s_idx)
                eval += m_board[s_idx].value();

            return eval;
        }
    };

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
}   // namespace fatpup
