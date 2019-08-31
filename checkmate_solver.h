#ifndef FATPUP_CHECKMATE_SOLVER_H
#define FATPUP_CHECKMATE_SOLVER_H

#include "solver.h"

namespace fatpup
{
    class CheckmateSolver: public Solver
    {
    public:
        CheckmateSolver(const Position& pos);

        void start() override {}
        void stop() override {}

        Move getBestMove() override { return _bestMove; }
        void moveDone(Move move) override;

    private:
        void findBestMove();

        Position _pos;
        Move _bestMove;
    };
}   // namespace fatpup

#endif // FATPUP_CHECKMATE_SOLVER_H
