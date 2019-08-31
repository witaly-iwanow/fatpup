#include "checkmate_solver.h"

CheckmateSolver::CheckmateSolver(const fatpup::Position& pos):
    _pos(pos)
{
    findBestMove();
}

void CheckmateSolver::moveDone(fatpup::Move move)
{
    _pos += move;
    findBestMove();
}

void CheckmateSolver::findBestMove()
{
    const auto moves = _pos.possibleMoves();
    if (!moves.empty())
        _bestMove = moves[0];   // just in case there's no checkmate

    for (auto move: moves)
    {
        const fatpup::Position pos(_pos, move);
        if (pos.getState() == fatpup::Position::State::Checkmate)
        {
            _bestMove = move;
            break;
        }
    }
}
