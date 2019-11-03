#include <iostream>
#include <limits>

#include "minimax.h"

namespace fatpup
{

class MinimaxPosition:
    public Position
{
public:
    MinimaxPosition(const Position& prevPos, Move move):
        Position(prevPos)
    {
        moveDone(move);
    }

    int EvaluateMaterial() const
    {
        int eval = 0;
        for (int s_idx = 0; s_idx < BOARD_SIZE * BOARD_SIZE; ++s_idx)
            eval += m_board[s_idx].value();

        return eval;
    }
};


int MinimaxEngine::SetPosition(const Position& pos)
{
    _pos = pos;
    return 0;
}

void MinimaxEngine::MoveDone(Move move)
{
    _pos += move;
}

Move MinimaxEngine::GetBestMove()
{
    int eval = 0;

    _bestMove = FindBestMove(_pos, eval, 1, 3);
    if (!_bestMove.isEmpty())
        _pos += _bestMove;

    return _bestMove;
}

Move MinimaxEngine::FindBestMove(const Position& position, int& afterMoveEval, int currentDepth, int maxDepth)
{
    const auto moves = position.possibleMoves();
    Move bestMove;
    int bestMoveEval = std::numeric_limits<int>::min();
    const bool reverseEval = !position.isWhiteTurn();

    for (auto move: moves)
    {
        const MinimaxPosition afterMovePos(position, move);
        const auto state = afterMovePos.getState();
        if (state == Position::State::Checkmate)
        {
            afterMoveEval = std::numeric_limits<int>::max();
            return move;
        }

        int eval = 0;
        if (currentDepth < maxDepth)
        {
            FindBestMove(afterMovePos, eval, currentDepth + 1, maxDepth);
            eval = -eval;
        }
        else
        {
            eval = afterMovePos.EvaluateMaterial() * 128;
            if (state != Position::State::Stalemate)
            {
                if (reverseEval)
                    eval = -eval;

                // we like checks
                if (state == Position::State::Check)
                    eval += 32;

                // these mobility checks are really expensive

                // and mobility
                eval -= afterMovePos.possibleMoves().size();
                if (state == Position::State::Normal)
                {
                    // it doesn't make sense to toggle a check position for example,
                    // do this for normal states only
                    Position mobilityTestPos(afterMovePos);
                    mobilityTestPos.toggleTurn();
                    eval += mobilityTestPos.possibleMoves().size();
                }
            }
            else
                eval = 0;
        }

        if (eval > bestMoveEval)
        {
            bestMoveEval = eval;
            bestMove = move;
        }
    }

    afterMoveEval = bestMoveEval;
    return bestMove;
}

}   // namespace fatpup
