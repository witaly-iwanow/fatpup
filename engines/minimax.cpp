#include <iostream>
#include <limits>

#include "minimax.h"

namespace fatpup
{

// we use / 2 here in order to avoid int overflows when flipping the sign
static constexpr int maxEvaluation = std::numeric_limits<int>::max() / 2;
static constexpr int minEvaluation = -maxEvaluation;

class MinimaxPosition:
    public Position
{
public:
    MinimaxPosition(const Position& prevPos, Move move):
        Position(prevPos)
    {
        moveDone(move);
    }

    MinimaxPosition() : Position() {}

    int Evaluate() const;

protected:
    static constexpr int materialWeight = 32;
    static constexpr int protectWeight = 1;
    static constexpr int mobilityWeight = 2;
    static constexpr int attackWeight = 8;

    int EvaluateWhitePawnAttackSquare(const int s_idx) const;
    int EvaluateBlackPawnAttackSquare(const int s_idx) const;
    int EvaluatePawn(const int color, const int s_idx) const;

    int EvaluateKnight(const int color, const int s_idx) const;
    int EvaluateBishop(const int color, const int s_idx) const;
    int EvaluateRook(const int color, const int s_idx) const;
    int EvaluateQueen(const int color, const int s_idx) const;
    int EvaluateKing(const int color, const int s_idx) const;
};

int MinimaxPosition::Evaluate() const
{
    int eval = 0;
    for (int s_idx = 0; s_idx < BOARD_SIZE * BOARD_SIZE; ++s_idx)
    {
        const auto piece = m_board[s_idx].pieceWithColor();
        if (piece != Empty)
        {
            const auto color = piece & ColorMask;
            switch (piece & PieceMask)
            {
                case Pawn: eval += EvaluatePawn(color, s_idx); break;
                case Knight: eval += EvaluateKnight(color, s_idx); break;
                case Bishop: eval += EvaluateBishop(color, s_idx); break;
                case Rook: eval += EvaluateRook(color, s_idx); break;
                case Queen: eval += EvaluateQueen(color, s_idx); break;
                default: eval += EvaluateKing(color, s_idx); break;
            }
        }
    }

    return eval;
}

int MinimaxPosition::EvaluateWhitePawnAttackSquare(const int s_idx) const
{
    const auto square = m_board[s_idx];
    const auto piece = square.pieceWithColor();

    if (piece == (Pawn | White))
        return protectWeight * PawnValue;
    else if ((piece & Black) && piece != (Pawn | Black))
        return -square.value() * attackWeight;

    return 0;
}

int MinimaxPosition::EvaluateBlackPawnAttackSquare(const int s_idx) const
{
    const auto square = m_board[s_idx];
    const auto piece = square.pieceWithColor();

    if (piece == (Pawn | Black))
        return -protectWeight * PawnValue;
    else if ((piece & White) && piece != (Pawn | White))
        return -square.value() * attackWeight;

    return 0;
}

int MinimaxPosition::EvaluatePawn(const int color, const int s_idx) const
{
    const auto rc = idxToRowCol(s_idx);
    if (color == Black)
    {
        int value = -materialWeight * PawnValue;
        value -= ((ROW7 - rc.row) + 1) * (std::min(rc.col - COLA, COLH - rc.col) + 1);
        if (rc.col > COLA)
            value += EvaluateBlackPawnAttackSquare(rowColToIdx(rc.row - 1, rc.col - 1));
        if (rc.col < COLH)
            value += EvaluateBlackPawnAttackSquare(rowColToIdx(rc.row - 1, rc.col + 1));

        //std::cerr << "black pawn at " << (char)((int)('a') + rc.col) << (char)((int)('1') + rc.row) << ", value: " << value << "\n";
        return value;
    }

    int value = materialWeight * PawnValue;
    value += ((rc.row - ROW2) + 1) * (std::min(rc.col - COLA, COLH - rc.col) + 1);

    if (rc.col > COLA)
        value += EvaluateWhitePawnAttackSquare(rowColToIdx(rc.row + 1, rc.col - 1));
    if (rc.col < COLH)
        value += EvaluateWhitePawnAttackSquare(rowColToIdx(rc.row + 1, rc.col + 1));

    return value;
}

int MinimaxPosition::EvaluateKnight(const int color, const int s_idx) const
{
    const auto rc = idxToRowCol(s_idx);
    auto value = materialWeight * KnightValue;
    value += mobilityWeight * (std::min(rc.row - ROW1, ROW8 - rc.row) + std::min(rc.col - COLA, COLH - rc.col));
    return (color == Black) ? -value : value;
}

int MinimaxPosition::EvaluateBishop(const int color, const int s_idx) const
{
    const auto rc = idxToRowCol(s_idx);
    auto value = materialWeight * BishopValue;
    value += mobilityWeight * (std::min(rc.row - ROW1, ROW8 - rc.row) + std::min(rc.col - COLA, COLH - rc.col));
    return (color == Black) ? -value : value;
}

int MinimaxPosition::EvaluateRook(const int color, const int s_idx) const
{
    return (color == Black) ? -materialWeight * RookValue : materialWeight * RookValue;
}

int MinimaxPosition::EvaluateQueen(const int color, const int s_idx) const
{
    return (color == Black) ? -materialWeight * QueenValue : materialWeight * QueenValue;
}

int MinimaxPosition::EvaluateKing(const int color, const int s_idx) const
{
    return (color == Black) ? -materialWeight * KingValue : materialWeight * KingValue;
}

// end of MinimaxPosition methods


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

    _bestMove = FindBestMove(_pos, eval, 1);
    if (!_bestMove.isEmpty())
        _pos += _bestMove;

    return _bestMove;
}

Move MinimaxEngine::FindBestMove(const Position& position, int& afterMoveEval, int currentDepth)
{
    const auto moves = position.possibleMoves();
    const bool searchForMin = !position.isWhiteTurn();

    Move bestMove;
    int bestMoveEval = searchForMin ? maxEvaluation : minEvaluation;

    // check moves in this order, otherwise it will postpone capturing
    // or promotion forever because promoting a pawn right away gives
    // you exactly the same evaluation as promoting in three moves as
    // long as it's inevitable
    enum MoveType { Capture = 0, Promotion, Other };

    for (int moveType = Capture; moveType <= Other; ++moveType)
    {
        for (auto move: moves)
        {
            const bool isMoveCapture = position.isMoveCapture(move);
            const bool isPromotion = (move.fields.promoted_to > Pawn);
            if (moveType == Capture)
            {
                if (!isMoveCapture)
                    continue;
            }
            else if (moveType == Promotion)
            {
                // captures are already checked in the first run, skip
                if (isMoveCapture || !isPromotion)
                    continue;
            }
            else
            {
                // captures and promotions are already checked, skip
                if (isMoveCapture || isPromotion)
                    continue;
            }

            const MinimaxPosition afterMovePos(position, move);
            const auto state = afterMovePos.getState();

            int eval = 0;
            if (state == Position::State::Checkmate)
            {
                afterMoveEval = searchForMin ? minEvaluation + 1 : maxEvaluation - 1;
                return move;
            }

            if (state != Position::State::Stalemate)
            {
                auto depthLimit = isMoveCapture ? 5 : (state == Position::State::Check ? 4 : 3);
                if (currentDepth < depthLimit)
                    FindBestMove(afterMovePos, eval, currentDepth + 1);
                else
                    eval = afterMovePos.Evaluate();
            }

            /*
            {
                auto moveStr = position.moveToString(move);
                std::cerr << currentDepth << " " << moveStr << " " << eval << "\n";
            */

            if ((searchForMin && eval < bestMoveEval) || (!searchForMin && eval > bestMoveEval))
            {
                bestMoveEval = eval;
                bestMove = move;
            }
        }
    }

    afterMoveEval = bestMoveEval;
    return bestMove;
}

}   // namespace fatpup
