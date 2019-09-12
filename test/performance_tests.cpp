#include <iostream>
#include <chrono>

#include "fatpup/position.h"
#include "solver.h"

void runEvaluationPerformanceTests()
{
/*
    static constexpr int numPositions = 2;
    fatpup::Position positions[numPositions];

    positions[0].setEmpty();
    positions[0].square("a5") = fatpup::Knight | fatpup::White;
    positions[0].square("e8") = fatpup::King | fatpup::White;
    positions[0].square("b3") = fatpup::Knight | fatpup::White;
    positions[0].square("h2") = fatpup::Bishop | fatpup::White;
    positions[0].square("h7") = fatpup::Rook | fatpup::White;

    positions[0].square("c7") = fatpup::King | fatpup::Black;
    positions[0].square("d7") = fatpup::Pawn | fatpup::Black;

    positions[0].setWhiteTurn(false);


    positions[1].setEmpty();
    positions[1].square("a4") = fatpup::Rook | fatpup::White;
    positions[1].square("c4") = fatpup::Pawn | fatpup::White;
    positions[1].square("c3") = fatpup::EnPassant;
    positions[1].square("d2") = fatpup::King | fatpup::White;

    positions[1].square("b4") = fatpup::Pawn | fatpup::Black;
    positions[1].square("c5") = fatpup::Knight | fatpup::Black;
    positions[1].square("d4") = fatpup::King | fatpup::Black;
    positions[1].square("d5") = fatpup::Queen | fatpup::Black;
    positions[1].square("d6") = fatpup::Bishop | fatpup::Black;
    positions[1].square("e2") = fatpup::Pawn | fatpup::Black;
    positions[1].square("e4") = fatpup::Pawn | fatpup::Black;
    positions[1].square("f2") = fatpup::Pawn | fatpup::Black;

    positions[1].setWhiteTurn(false);

    static constexpr int numLoops = 8 * 1000 * 1000;
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
    static constexpr int numPositions = 2;
    fatpup::Position positions[numPositions];

    positions[0].setEmpty();
    positions[0].square("a5") = fatpup::Knight | fatpup::White;
    positions[0].square("e8") = fatpup::King | fatpup::White;
    positions[0].square("b3") = fatpup::Knight | fatpup::White;
    positions[0].square("h2") = fatpup::Bishop | fatpup::White;
    positions[0].square("h7") = fatpup::Rook | fatpup::White;

    positions[0].square("c7") = fatpup::King | fatpup::Black;
    positions[0].square("d7") = fatpup::Pawn | fatpup::Black;

    positions[0].setWhiteTurn(false);


    positions[1].setEmpty();
    positions[1].square("a4") = fatpup::Rook | fatpup::White;
    positions[1].square("c4") = fatpup::Pawn | fatpup::White;
    positions[1].square("c3") = fatpup::EnPassant;
    positions[1].square("d2") = fatpup::King | fatpup::White;

    positions[1].square("b4") = fatpup::Pawn | fatpup::Black;
    positions[1].square("c5") = fatpup::Knight | fatpup::Black;
    positions[1].square("d4") = fatpup::King | fatpup::Black;
    positions[1].square("d5") = fatpup::Queen | fatpup::Black;
    positions[1].square("d6") = fatpup::Bishop | fatpup::Black;
    positions[1].square("e2") = fatpup::Pawn | fatpup::Black;
    positions[1].square("e4") = fatpup::Pawn | fatpup::Black;
    positions[1].square("f2") = fatpup::Pawn | fatpup::Black;

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
    fatpup::Position pos;
    pos.setEmpty();

    pos.square("c6") = fatpup::King | fatpup::White;
    pos.square("c8") = fatpup::Knight | fatpup::White;
    pos.square("b6") = fatpup::Pawn | fatpup::White;
    pos.square("c7") = fatpup::Pawn | fatpup::White;

    pos.square("a8") = fatpup::King | fatpup::Black;
    pos.square("d8") = fatpup::Rook | fatpup::Black;
    pos.square("e7") = fatpup::Bishop | fatpup::Black;
    pos.square("b5") = fatpup::Knight | fatpup::Black;

    std::string solverNames[] = { "capture", "checkmate" };
    for (const auto& solverName: solverNames)
    {
        for (int wb = 0; wb < 2; ++wb)
        {
            pos.setWhiteTurn(wb == 0);

            Solver* solver = Solver::create(solverName, pos);
            if (solver)
            {
                const fatpup::Move bestMove = solver->getBestMove();
                std::cout << "[" << solverName << " solver] best move (" << (wb == 0 ? "white" : "black") << "): " << pos.moveToString(bestMove) << std::endl;

                delete solver;
            }
        }
    }
}
