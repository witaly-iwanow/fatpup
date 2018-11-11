#include <iostream>

#include "position.h"
#include "solver.h"

void runEvaluationPerformanceTests()
{
/*
    static const int numPositions = 2;
    Position positions[numPositions];

    positions[0].setEmpty();
    positions[0].square("a5") = Knight | White;
    positions[0].square("e8") = King | White;
    positions[0].square("b3") = Knight | White;
    positions[0].square("h2") = Bishop | White;
    positions[0].square("h7") = Rook | White;

    positions[0].square("c7") = King | Black;
    positions[0].square("d7") = Pawn | Black;

    positions[0].setWhiteTurn(false);


    positions[1].setEmpty();
    positions[1].square("a4") = Rook | White;
    positions[1].square("c4") = Pawn | White;
    positions[1].square("c3") = EnPassant;
    positions[1].square("d2") = King | White;

    positions[1].square("b4") = Pawn | Black;
    positions[1].square("c5") = Knight | Black;
    positions[1].square("d4") = King | Black;
    positions[1].square("d5") = Queen | Black;
    positions[1].square("d6") = Bishop | Black;
    positions[1].square("e2") = Pawn | Black;
    positions[1].square("e4") = Pawn | Black;
    positions[1].square("f2") = Pawn | Black;

    positions[1].setWhiteTurn(false);

    static const int numLoops = 8 * 1000 * 1000;
    float evalAcc = 0;
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < numLoops; ++i)
    {
        evalAcc += positions[0].evaluate();
        evalAcc -= positions[1].evaluate();
        evalAcc -= positions[0].evaluate();
        evalAcc += positions[1].evaluate();
    }
    auto finish = std::chrono::system_clock::now();
    auto executedIn = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

    std::cout << "Total evaluation time " << executedIn << " ms, check result: " << evalAcc <<
    ", keps: " << (numLoops * 4 / executedIn) << std::endl;
*/
}

void runPossibleMovesPerformanceTests()
{
    static const int numPositions = 2;
    Position positions[numPositions];

    positions[0].setEmpty();
    positions[0].square("a5") = Knight | White;
    positions[0].square("e8") = King | White;
    positions[0].square("b3") = Knight | White;
    positions[0].square("h2") = Bishop | White;
    positions[0].square("h7") = Rook | White;

    positions[0].square("c7") = King | Black;
    positions[0].square("d7") = Pawn | Black;

    positions[0].setWhiteTurn(false);


    positions[1].setEmpty();
    positions[1].square("a4") = Rook | White;
    positions[1].square("c4") = Pawn | White;
    positions[1].square("c3") = EnPassant;
    positions[1].square("d2") = King | White;

    positions[1].square("b4") = Pawn | Black;
    positions[1].square("c5") = Knight | Black;
    positions[1].square("d4") = King | Black;
    positions[1].square("d5") = Queen | Black;
    positions[1].square("d6") = Bishop | Black;
    positions[1].square("e2") = Pawn | Black;
    positions[1].square("e4") = Pawn | Black;
    positions[1].square("f2") = Pawn | Black;

    positions[1].setWhiteTurn(false);

    static const int numLoops = 200 * 1000;
    int evalAcc = 0;
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < numLoops; ++i)
    {
        auto moves = positions[0].possibleMoves();
        evalAcc += moves.size();
        moves = positions[1].possibleMoves();
        evalAcc -= moves.size();
        moves = positions[0].possibleMoves();
        evalAcc -= moves.size();
        moves = positions[1].possibleMoves();
        evalAcc += moves.size();
    }
    auto finish = std::chrono::system_clock::now();
    auto executedIn = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

    std::cout << "Total possible move calculation time " << executedIn << " ms, check result: " << evalAcc <<
    ", kops: " << (numLoops * 4 / executedIn) << std::endl;
}

void runFindBestMoveTests()
{
    Position pos;
    pos.setEmpty();

    pos.square("c6") = King | White;
    pos.square("c8") = Knight | White;
    pos.square("b6") = Pawn | White;
    pos.square("c7") = Pawn | White;

    pos.square("a8") = King | Black;
    pos.square("d8") = Rook | Black;
    pos.square("e7") = Bishop | Black;
    pos.square("b5") = Knight | Black;

    std::string solverNames[] = { "capture", "checkmate" };
    for (const auto& solverName: solverNames)
    {
        for (int wb = 0; wb < 2; ++wb)
        {
            pos.setWhiteTurn(wb == 0);

            Solver* solver = Solver::create(solverName, pos);
            if (solver)
            {
                const Move bestMove = solver->getBestMove();
                std::cout << "[" << solverName << " solver] best move (" << (wb == 0 ? "white" : "black") << "): " << pos.moveToString(bestMove) << std::endl;

                delete solver;
            }
        }
    }
}
