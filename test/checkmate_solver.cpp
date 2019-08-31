#include "checkmate_solver.h"

namespace fatpup
{
    CheckmateSolver::CheckmateSolver(const Position& pos):
        _pos(pos)
    {
        findBestMove();
    }

    void CheckmateSolver::moveDone(Move move)
    {
        _pos += move;
        findBestMove();
    }

    void CheckmateSolver::findBestMove()
    {
        const std::vector<Move> moves = _pos.possibleMoves();
        if (!moves.empty())
            _bestMove = moves[0];   // just in case there's no checkmate

        for (auto move: moves)
        {
            const Position pos(_pos, move);
            if (pos.getState() == Position::State::Checkmate)
            {
                _bestMove = move;
                break;
            }
        }
    }
}   // namespace fatpup
