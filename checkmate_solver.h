#ifndef CHECKMATE_SOLVER_H
#define CHECKMATE_SOLVER_H

#include "solver.h"

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

#endif // CHECKMATE_SOLVER_H
