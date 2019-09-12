#ifndef FATPUP_CLI_SOLVER_H
#define FATPUP_CLI_SOLVER_H

#include <string>

#include "fatpup/position.h"

class Solver
{
public:
    static Solver* create(const std::string& solver_name, const fatpup::Position& pos);

    virtual ~Solver() {}

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual fatpup::Move getBestMove() = 0;
    virtual void moveDone(fatpup::Move move) = 0;

    // to-do: analysis/kibitzing mode API (return best lines with evaluations)
};

#endif // FATPUP_CLI_SOLVER_H
