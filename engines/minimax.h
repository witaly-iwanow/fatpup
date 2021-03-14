#ifndef FATPUP_MINIMAX_H
#define FATPUP_MINIMAX_H

#include "fatpup/engine.h"

namespace fatpup
{

class MinimaxEngine: public Engine
{
public:
    MinimaxEngine() {}

    int SetPosition(const Position& pos) override;

    //void Start() override {}
    //void Stop() override {}

    Move GetBestMove() override;
    void MoveDone(Move move) override;

private:
    static Move FindBestMove(const Position& position, int& afterMoveEval, int currentDepth);

    Position _pos;
    Move _bestMove;
};

}   // namespace fatpup

#endif // FATPUP_MINIMAX_H
