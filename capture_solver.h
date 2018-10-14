#ifndef CAPTURE_SOLVER_H
#define CAPTURE_SOLVER_H

#include "solver.h"

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

#endif // CAPTURE_SOLVER_H
