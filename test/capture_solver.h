#ifndef FATPUP_CAPTURE_SOLVER_H
#define FATPUP_CAPTURE_SOLVER_H

#include "solver.h"

namespace fatpup
{
    class CaptureSolver: public Solver
    {
    public:
        CaptureSolver(const Position& pos);

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

#endif // FATPUP_CAPTURE_SOLVER_H
