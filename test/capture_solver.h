#ifndef FATPUP_CLI_CAPTURE_SOLVER_H
#define FATPUP_CLI_CAPTURE_SOLVER_H

#include "solver.h"

class CaptureSolver: public Solver
{
public:
    CaptureSolver(const fatpup::Position& pos);

    void start() override {}
    void stop() override {}

    fatpup::Move getBestMove() override { return _bestMove; }
    void moveDone(fatpup::Move move) override;

private:
    void findBestMove();

    fatpup::Position _pos;
    fatpup::Move _bestMove;
};

#endif // FATPUP_CLI_CAPTURE_SOLVER_H
