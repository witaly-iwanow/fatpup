#include <iostream>

#include "fatpup/position.h"
#include "color_scheme.h"

static bool applyAndCheckFen(const std::string& FEN, const fatpup::Position& reference_pos)
{
    fatpup::Position test_pos;
    if (!test_pos.setFEN(FEN))
    {
        std::cout << "Error! Position::setFEN failed!\n";
        return false;
    }
    if (test_pos != reference_pos)
    {
        std::cout << "Error! Position after setFEN doesn't match the reference!\n";
        return false;
    }
    return true;
}

static bool fenValid(const std::string& FEN)
{
    fatpup::Position test_pos;
    return test_pos.setFEN(FEN);
}

bool runFenTests()
{
    std::cout << testTitleColor << "Valid FEN Test" << rang::fg::reset << std::endl;

    fatpup::Position pos;
    pos.setInitial();
    if (!applyAndCheckFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", pos))
    {
        std::cout << "Failed, terminating..." << std::endl;
        return false;
    }

    pos += fatpup::Move("e2e4");
    pos += fatpup::Move("d7d5");
    pos += fatpup::Move("e4e5");
    pos += fatpup::Move("f7f5");
    if (!applyAndCheckFen("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3", pos))
    {
        std::cout << "Failed, terminating..." << std::endl;
        return false;
    }

    pos += fatpup::Move("e1e2");
    pos += fatpup::Move("a7a5");
    pos += fatpup::Move("e2e1");
    pos += fatpup::Move("a8a6");
    pos += fatpup::Move("g1f3");
    pos += fatpup::Move("a6a8");
    if (!applyAndCheckFen("rnbqkbnr/1pp1p1pp/8/p2pPp2/8/5N2/PPPP1PPP/RNBQKB1R w k - 4 6", pos))
    {
        std::cout << "Failed, terminating..." << std::endl;
        return false;
    }


    std::cout << testTitleColor << "FEN Parsing Robustness Test" << rang::fg::reset << std::endl;

    if (fenValid("rnbq1kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"))
    {
        std::cout << "Failed, terminating..." << std::endl;
        return false;
    }

    if (fenValid("rnbqkbnr/pppppppp/8/9/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"))
    {
        std::cout << "Failed, terminating..." << std::endl;
        return false;
    }

    if (fenValid("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"))
    {
        std::cout << "Failed, terminating..." << std::endl;
        return false;
    }

    if (fenValid("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQKBNR - KQkq - 0 1"))
    {
        std::cout << "Failed, terminating..." << std::endl;
        return false;
    }

    if (fenValid("rnbqkbnr/pppppppp/8/8/8/PPPP0PPP/RNBQKBNR b KQkq - 0 1"))
    {
        std::cout << "Failed, terminating..." << std::endl;
        return false;
    }

    if (fenValid("8/8/8/8/8/8/8/8 w kq c4 0 1"))
    {
        std::cout << "Failed, terminating..." << std::endl;
        return false;
    }

    std::cout << successMsgColor << "  Success, all FEN tests passed!" << rang::fg::reset << std::endl;

    return true;
}
