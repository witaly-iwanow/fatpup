#include <iostream>

#include "fatpup/position.h"
#include "color_scheme.h"
#include "utils.h"

#include "possible_moves_tests.h"

bool runPossibleMovesTests(bool verbose)
{
    if (!runPossiblePawnMovesTests(verbose))
        return false;

    if (!runPossibleKnightMovesTests(verbose))
        return false;

    if (!runPossibleBishopMovesTests(verbose))
        return false;

    if (!runPossibleRookMovesTests(verbose))
        return false;

    if (!runPossibleQueenMovesTests(verbose))
        return false;

    if (!runPossibleKingMovesTests(verbose))
        return false;

    if (!runPossibleCastlingMovesTests(verbose))
        return false;


    if (!runCheckTests(verbose))
        return false;

    if (!runCheckmateTests(verbose))
        return false;

    if (!runStalemateTests(verbose))
        return false;

    std::cout << successMsgColor << "  Success, all move tests passed!" << rang::fg::reset << std::endl;

    return true;
}

bool runPossiblePawnMovesTests(bool verbose)
{
    bool success = true;

    fatpup::Position pos;
    pos.setEmpty();

    // advancing only, no capturing, no promotion
    pos.square("a2") = fatpup::Pawn | fatpup::White;
    pos.square("b3") = fatpup::Pawn | fatpup::White;
    pos.square("c2") = fatpup::Pawn | fatpup::White;
    pos.square("c4") = fatpup::Pawn | fatpup::White;
    pos.square("e2") = fatpup::Pawn | fatpup::White;
    pos.square("e6") = fatpup::Pawn | fatpup::White;
    pos.square("h6") = fatpup::Pawn | fatpup::White;
    pos.square("h5") = fatpup::Pawn | fatpup::White;

    pos.square("a6") = fatpup::Pawn | fatpup::Black;
    pos.square("b7") = fatpup::Pawn | fatpup::Black;
    pos.square("c7") = fatpup::Pawn | fatpup::Black;
    pos.square("e5") = fatpup::Pawn | fatpup::Black;
    pos.square("e3") = fatpup::Pawn | fatpup::Black;
    pos.square("g4") = fatpup::Pawn | fatpup::Black;
    pos.square("g3") = fatpup::Pawn | fatpup::Black;

    std::cout << testTitleColor << "Pawn Test #1 (advancing)" << rang::fg::reset << std::endl;
    if (verbose)
        PrintPosition(pos);

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != 7)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Pawn Test #2 (capturing)" << rang::fg::reset << std::endl;
    pos.setEmpty();

    pos.square("a2") = fatpup::Pawn | fatpup::White;
    pos.square("c2") = fatpup::Pawn | fatpup::White;
    pos.square("e5") = fatpup::Pawn | fatpup::White;
    pos.square("e6") = fatpup::Pawn | fatpup::White;
    pos.square("g4") = fatpup::Pawn | fatpup::White;
    pos.square("g5") = fatpup::Pawn | fatpup::White;

    pos.square("a3") = fatpup::Pawn | fatpup::Black;
    pos.square("b4") = fatpup::Pawn | fatpup::Black;
    pos.square("d7") = fatpup::Pawn | fatpup::Black;
    pos.square("d6") = fatpup::Pawn | fatpup::Black;
    pos.square("d4") = fatpup::Pawn | fatpup::Black;
    pos.square("e7") = fatpup::Pawn | fatpup::Black;
    pos.square("f7") = fatpup::Pawn | fatpup::Black;
    pos.square("f4") = fatpup::Pawn | fatpup::Black;
    pos.square("h5") = fatpup::Pawn | fatpup::Black;

    if (verbose)
        PrintPosition(pos);

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != (side ? 11 : 7))
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Pawn Test #3A (en passant capturing)" << rang::fg::reset << std::endl;
    pos.setWhiteTurn(true);
    pos += fatpup::Move("c2c4");

    if (verbose)
        PrintPosition(pos);

    {
        const auto moves = pos.possibleMoves();
        if (verbose)
        {
            std::cout << "Number of moves for black: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != 13)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Pawn Test #3B (en passant capturing)" << rang::fg::reset << std::endl;
    pos += fatpup::Move("f7f5");

    if (verbose)
        PrintPosition(pos);

    {
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for white: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != 8)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Pawn Test #3C (en passant capturing)" << rang::fg::reset << std::endl;
    pos.setWhiteTurn(true);
    pos += fatpup::Move("e6d7");

    if (verbose)
        PrintPosition(pos);

    {
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for black: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != 9)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Pawn Test #4 (promotion)" << rang::fg::reset << std::endl;
    pos.setEmpty();

    pos.square("a6") = fatpup::Pawn | fatpup::White;
    pos.square("b7") = fatpup::Pawn | fatpup::White;
    pos.square("e7") = fatpup::Pawn | fatpup::White;
    pos.square("f7") = fatpup::Pawn | fatpup::White;
    pos.square("h7") = fatpup::Pawn | fatpup::White;
    pos.square("b1") = fatpup::Queen | fatpup::White;
    pos.square("f1") = fatpup::Rook | fatpup::White;
    pos.square("g1") = fatpup::Rook | fatpup::White;

    pos.square("a2") = fatpup::Pawn | fatpup::Black;
    pos.square("b2") = fatpup::Pawn | fatpup::Black;
    pos.square("c2") = fatpup::Pawn | fatpup::Black;
    pos.square("d2") = fatpup::Pawn | fatpup::Black;
    pos.square("f2") = fatpup::Pawn | fatpup::Black;
    pos.square("g2") = fatpup::Pawn | fatpup::Black;
    pos.square("d8") = fatpup::Bishop | fatpup::Black;
    pos.square("e8") = fatpup::Bishop | fatpup::Black;
    pos.square("f8") = fatpup::Knight | fatpup::Black;

    if (verbose)
        PrintPosition(pos);

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        const auto moves = pos.possibleMoves();
        std::vector<fatpup::Move> pawnMoves;
        for (size_t i = 0; i < moves.size(); i++)
        {
            if (pos.square(moves[i].fields.src_row, moves[i].fields.src_col).piece() == fatpup::Pawn)
                pawnMoves.push_back(moves[i]);
        }

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << pawnMoves.size() << std::endl;
            for (size_t i = 0; i < pawnMoves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << pawnMoves.size() << " " << pos.moveToString(pawnMoves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (pawnMoves.size() != (side ? 28 : 21))
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }

    return true;
}


bool runPossibleKnightMovesTests(bool verbose)
{
    bool success = true;

    fatpup::Position pos;
    pos.setEmpty();

    pos.square("a1") = fatpup::Knight | fatpup::White;
    pos.square("a8") = fatpup::Knight | fatpup::White;
    pos.square("b3") = fatpup::Knight | fatpup::White;
    pos.square("c2") = fatpup::Knight | fatpup::White;
    pos.square("d4") = fatpup::Knight | fatpup::White;
    pos.square("g2") = fatpup::Knight | fatpup::White;
    pos.square("g7") = fatpup::Knight | fatpup::White;
    pos.square("h1") = fatpup::Knight | fatpup::White;
    pos.square("h8") = fatpup::Knight | fatpup::White;

    pos.square("b4") = fatpup::Knight | fatpup::Black;
    pos.square("b6") = fatpup::Knight | fatpup::Black;
    pos.square("c3") = fatpup::Knight | fatpup::Black;
    pos.square("c7") = fatpup::Knight | fatpup::Black;
    pos.square("d5") = fatpup::Knight | fatpup::Black;
    pos.square("e3") = fatpup::Knight | fatpup::Black;
    pos.square("e7") = fatpup::Knight | fatpup::Black;
    pos.square("f4") = fatpup::Knight | fatpup::Black;
    pos.square("f6") = fatpup::Knight | fatpup::Black;

    std::cout << testTitleColor << "Knight Test" << rang::fg::reset << std::endl;
    if (verbose)
        PrintPosition(pos);

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != (side ? 48 : 28))
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }

    return true;
}


bool runPossibleBishopMovesTests(bool verbose)
{
    bool success = true;

    fatpup::Position pos;
    pos.setEmpty();

    pos.square("a1") = fatpup::Bishop | fatpup::White;
    pos.square("c6") = fatpup::Bishop | fatpup::White;
    pos.square("d2") = fatpup::Bishop | fatpup::White;
    pos.square("d4") = fatpup::Bishop | fatpup::White;
    pos.square("e4") = fatpup::Bishop | fatpup::White;
    pos.square("f7") = fatpup::Bishop | fatpup::White;

    pos.square("c3") = fatpup::Bishop | fatpup::Black;
    pos.square("c7") = fatpup::Bishop | fatpup::Black;
    pos.square("e5") = fatpup::Bishop | fatpup::Black;
    pos.square("h1") = fatpup::Bishop | fatpup::Black;
    pos.square("h3") = fatpup::Bishop | fatpup::Black;
    pos.square("h8") = fatpup::Bishop | fatpup::Black;

    std::cout << testTitleColor << "Bishop Test" << rang::fg::reset << std::endl;
    if (verbose)
        PrintPosition(pos);

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != (side ? 30 : 43))
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }

    return true;
}


bool runPossibleRookMovesTests(bool verbose)
{
    bool success = true;

    fatpup::Position pos;
    pos.setEmpty();

    pos.square("a1") = fatpup::Rook | fatpup::White;
    pos.square("e2") = fatpup::Rook | fatpup::White;
    pos.square("f2") = fatpup::Rook | fatpup::White;
    pos.square("g7") = fatpup::Rook | fatpup::White;
    pos.square("h8") = fatpup::Rook | fatpup::White;

    pos.square("a2") = fatpup::Rook | fatpup::Black;
    pos.square("b1") = fatpup::Rook | fatpup::Black;
    pos.square("b2") = fatpup::Rook | fatpup::Black;
    pos.square("d5") = fatpup::Rook | fatpup::Black;
    pos.square("h1") = fatpup::Rook | fatpup::Black;

    std::cout << testTitleColor << "Rook Test" << rang::fg::reset << std::endl;
    if (verbose)
        PrintPosition(pos);

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != (side ? 48 : 49))
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }

    return true;
}


bool runPossibleQueenMovesTests(bool verbose)
{
    bool success = true;

    fatpup::Position pos;
    pos.setEmpty();

    pos.square("d5") = fatpup::Queen | fatpup::White;

    pos.square("g1") = fatpup::Queen | fatpup::Black;

    std::cout << testTitleColor << "Queen Test #1" << rang::fg::reset << std::endl;
    if (verbose)
        PrintPosition(pos);

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != (side ? 21 : 27))
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Queen Test #2" << rang::fg::reset << std::endl;
    pos.setEmpty();

    pos.square("d3") = fatpup::Queen | fatpup::White;
    pos.square("d5") = fatpup::Queen | fatpup::White;
    pos.square("f3") = fatpup::Queen | fatpup::White;
    pos.square("f5") = fatpup::Queen | fatpup::White;

    pos.square("a1") = fatpup::Queen | fatpup::Black;
    pos.square("e4") = fatpup::Queen | fatpup::Black;

    if (verbose)
        PrintPosition(pos);

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != (side ? 39 : 61))
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }

    return true;
}


bool runPossibleKingMovesTests(bool verbose)
{
    bool success = true;

    fatpup::Position pos;
    pos.setEmpty();

    pos.square("a8") = fatpup::King | fatpup::White;
    pos.square("c6") = fatpup::Pawn | fatpup::White;

    pos.square("c7") = fatpup::King | fatpup::Black;

    std::cout << testTitleColor << "King Test #1" << rang::fg::reset << std::endl;
    if (verbose)
        PrintPosition(pos);

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != (side ? 5 : 1))
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "King Test #2" << rang::fg::reset << std::endl;
    pos.setEmpty();

    pos.square("h5") = fatpup::King | fatpup::White;
    pos.square("h4") = fatpup::Knight | fatpup::White;
    pos.square("h3") = fatpup::Knight | fatpup::White;

    pos.square("h1") = fatpup::King | fatpup::Black;
    pos.square("g1") = fatpup::Queen | fatpup::Black;

    if (verbose)
        PrintPosition(pos);

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != (side ? 21 : 9))
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "King Test #3" << rang::fg::reset << std::endl;
    pos.setEmpty();

    pos.square("d1") = fatpup::King | fatpup::White;
    pos.square("h3") = fatpup::Rook | fatpup::White;
    pos.square("a6") = fatpup::Bishop | fatpup::White;
    pos.square("a2") = fatpup::Pawn | fatpup::White;

    pos.square("c3") = fatpup::King | fatpup::Black;
    pos.square("e8") = fatpup::Rook | fatpup::Black;
    pos.square("e3") = fatpup::Pawn | fatpup::Black;
    pos.square("f3") = fatpup::Pawn | fatpup::Black;

    if (verbose)
        PrintPosition(pos);

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != (side ? 16 : 20))
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }

    return true;
}


bool runPossibleCastlingMovesTests(bool verbose)
{
    bool success = true;
    fatpup::Position pos;

    std::cout << testTitleColor << "Castling Test #1" << rang::fg::reset << std::endl;
    pos.setEmpty();

    pos.square("a1") = fatpup::Rook | fatpup::White | fatpup::CanCastle;
    pos.square("a2") = fatpup::Pawn | fatpup::White;
    pos.square("e1") = fatpup::King | fatpup::White;
    pos.square("h1") = fatpup::Rook | fatpup::White | fatpup::CanCastle;
    pos.square("h2") = fatpup::Pawn | fatpup::White;

    pos.square("a8") = fatpup::Rook | fatpup::Black | fatpup::CanCastle;
    pos.square("a7") = fatpup::Pawn | fatpup::Black;
    pos.square("e8") = fatpup::King | fatpup::Black;
    pos.square("h8") = fatpup::Rook | fatpup::Black | fatpup::CanCastle;
    pos.square("h7") = fatpup::Pawn | fatpup::Black;

    if (verbose)
        PrintPosition(pos);

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != 16)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Castling Test #2" << rang::fg::reset << std::endl;
    pos.setWhiteTurn(true);
    pos += fatpup::Move("h1f1");

    if (verbose)
        PrintPosition(pos);

    {
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for black: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != 13)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Castling Test #3" << rang::fg::reset << std::endl;
    pos += fatpup::Move("h8f8");
    pos += fatpup::Move("f1f8");

    if (verbose)
        PrintPosition(pos);

    {
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for black: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != 3)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Castling Test #4" << rang::fg::reset << std::endl;
    pos.setEmpty();

    pos.square("a1") = fatpup::Rook | fatpup::White | fatpup::CanCastle;
    pos.square("a2") = fatpup::Pawn | fatpup::White;
    pos.square("e1") = fatpup::King | fatpup::White;
    pos.square("h1") = fatpup::Rook | fatpup::White | fatpup::CanCastle;
    pos.square("h2") = fatpup::Pawn | fatpup::White;
    pos.square("f1") = fatpup::Bishop | fatpup::White;

    pos.square("a8") = fatpup::Rook | fatpup::Black | fatpup::CanCastle;
    pos.square("a7") = fatpup::Pawn | fatpup::Black;
    pos.square("e8") = fatpup::King | fatpup::Black;
    pos.square("h8") = fatpup::Rook | fatpup::Black | fatpup::CanCastle;
    pos.square("h7") = fatpup::Pawn | fatpup::Black;
    pos.square("g7") = fatpup::Bishop | fatpup::Black;
    pos.square("g6") = fatpup::Bishop | fatpup::Black;

    pos.setWhiteTurn(true);
    if (verbose)
        PrintPosition(pos);

    {
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for white: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != 20)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Castling Test #5" << rang::fg::reset << std::endl;
    pos += fatpup::Move("f1b5");
    pos += fatpup::Move("e8e7");
    pos += fatpup::Move("b5e8");
    pos += fatpup::Move("e7e8");
    pos += fatpup::Move("h2h4");
    pos += fatpup::Move("g7h6");

    if (verbose)
        PrintPosition(pos);

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != (side ? 26 : 15))
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }

    return true;
}


bool runCheckTests(bool verbose)
{
    bool success = true;
    fatpup::Position pos;

    std::cout << testTitleColor << "Check Test #1" << rang::fg::reset << std::endl;

    pos.setEmpty();

    pos.square("a5") = fatpup::Knight | fatpup::White;
    pos.square("e8") = fatpup::King | fatpup::White;
    pos.square("b3") = fatpup::Knight | fatpup::White;
    pos.square("h2") = fatpup::Bishop | fatpup::White;
    pos.square("h7") = fatpup::Rook | fatpup::White;

    pos.square("c7") = fatpup::King | fatpup::Black;
    pos.square("d7") = fatpup::Pawn | fatpup::Black;

    if (verbose)
        PrintPosition(pos);

    {
        pos.setWhiteTurn(false);

        if (pos.getState() != fatpup::Position::State::Check)
            success = false;

        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for black: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != 2)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Check Test #2" << rang::fg::reset << std::endl;
    pos.square("e4") = fatpup::Knight | fatpup::Black;
    if (verbose)
        PrintPosition(pos);

    {
        pos.setWhiteTurn(false);

        if (pos.getState() != fatpup::Position::State::Check)
            success = false;

        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for black: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != 4)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Check Test #3" << rang::fg::reset << std::endl;
    pos += fatpup::Move("e4d6");
    if (verbose)
        PrintPosition(pos);

    {
        if (pos.getState() != fatpup::Position::State::Check)
            success = false;

        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for white: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != 3)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Check Test #4" << rang::fg::reset << std::endl;

    pos.setEmpty();

    pos.square("a4") = fatpup::Rook | fatpup::White;
    pos.square("c4") = fatpup::Pawn | fatpup::White;
    pos.square("c3") = fatpup::EnPassant;
    pos.square("d2") = fatpup::King | fatpup::White;

    pos.square("b4") = fatpup::Pawn | fatpup::Black;
    pos.square("c5") = fatpup::Knight | fatpup::Black;
    pos.square("d4") = fatpup::King | fatpup::Black;
    pos.square("d5") = fatpup::Queen | fatpup::Black;
    pos.square("d6") = fatpup::Bishop | fatpup::Black;
    pos.square("e2") = fatpup::Pawn | fatpup::Black;
    pos.square("e4") = fatpup::Pawn | fatpup::Black;
    pos.square("f2") = fatpup::Pawn | fatpup::Black;

    if (verbose)
        PrintPosition(pos);

    {
        pos.setWhiteTurn(false);

        {
            const fatpup::Position::State state = pos.getState();
            if (state == fatpup::Position::State::Check)
                success = false;
            else if (state == fatpup::Position::State::Checkmate)
                success = false;
            else if (state == fatpup::Position::State::Stalemate)
                success = false;
        }

        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for black: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        size_t numChecks = 0;
        for (size_t i = 0; i < moves.size(); i++)
        {
            const fatpup::Position::State state = (pos + moves[i]).getState();
            if (state == fatpup::Position::State::Check)
                numChecks++;
            else if (state == fatpup::Position::State::Checkmate)
                success = false;
            else if (state == fatpup::Position::State::Stalemate)
                success = false;
        }

        if (numChecks != 9)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Check Test #5" << rang::fg::reset << std::endl;

    pos.setEmpty();

    pos.square("a1") = fatpup::Rook | fatpup::White | fatpup::CanCastle;
    pos.square("e1") = fatpup::King | fatpup::White;
    pos.square("e4") = fatpup::Knight | fatpup::White;
    pos.square("f2") = fatpup::Queen | fatpup::White;
    pos.square("f3") = fatpup::Bishop | fatpup::White;
    pos.square("g2") = fatpup::Pawn | fatpup::White;

    pos.square("f4") = fatpup::Knight | fatpup::Black;
    pos.square("h1") = fatpup::King | fatpup::Black;
    pos.square("h3") = fatpup::Pawn | fatpup::Black;
    pos.square("h4") = fatpup::Bishop | fatpup::Black;

    if (verbose)
        PrintPosition(pos);

    {
        pos.setWhiteTurn(true);

        {
            const fatpup::Position::State state = pos.getState();
            if (state == fatpup::Position::State::Check)
                success = false;
            else if (state == fatpup::Position::State::Checkmate)
                success = false;
            else if (state == fatpup::Position::State::Stalemate)
                success = false;
        }

        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for white: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        size_t numChecks = 0;
        for (size_t i = 0; i < moves.size(); i++)
        {
            const fatpup::Position::State state = (pos + moves[i]).getState();
            if (state == fatpup::Position::State::Check)
                numChecks++;
            else if (state == fatpup::Position::State::Checkmate)
                success = false;
            else if (state == fatpup::Position::State::Stalemate)
                success = false;
        }

        if (numChecks != 6)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }

    return true;
}


bool runCheckmateTests(bool verbose)
{
    bool success = true;
    fatpup::Position pos;

    std::cout << testTitleColor << "Checkmate Test #1" << rang::fg::reset << std::endl;

    pos.setEmpty();

    pos.square("a2") = fatpup::Bishop | fatpup::White;
    pos.square("e7") = fatpup::King | fatpup::White;
    pos.square("g5") = fatpup::Pawn | fatpup::White;
    pos.square("h5") = fatpup::Queen | fatpup::White;

    pos.square("f5") = fatpup::Pawn | fatpup::Black;
    pos.square("f6") = fatpup::EnPassant;
    pos.square("g7") = fatpup::King | fatpup::Black;

    if (verbose)
        PrintPosition(pos);

    {
        pos.setWhiteTurn(true);

        {
            const fatpup::Position::State state = pos.getState();
            if (state == fatpup::Position::State::Check)
                success = false;
            else if (state == fatpup::Position::State::Checkmate)
                success = false;
            else if (state == fatpup::Position::State::Stalemate)
                success = false;
        }

        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for white: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        size_t numCheckmates = 0;
        for (size_t i = 0; i < moves.size(); i++)
        {
            const fatpup::Position::State state = (pos + moves[i]).getState();
            if (state == fatpup::Position::State::Checkmate)
                numCheckmates++;
            else if (state == fatpup::Position::State::Stalemate)
                success = false;
        }

        if (numCheckmates != 2)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Checkmate Test #2" << rang::fg::reset << std::endl;

    pos.setEmpty();

    pos.square("c6") = fatpup::Queen | fatpup::White;
    pos.square("d3") = fatpup::Knight | fatpup::White;
    pos.square("f3") = fatpup::King | fatpup::White;
    pos.square("f4") = fatpup::Bishop | fatpup::White;
    pos.square("g4") = fatpup::Rook | fatpup::White;
    pos.square("h7") = fatpup::Pawn | fatpup::White;

    pos.square("a5") = fatpup::Bishop | fatpup::Black;
    pos.square("a7") = fatpup::Rook | fatpup::Black;
    pos.square("e5") = fatpup::Pawn | fatpup::Black;
    pos.square("f2") = fatpup::Pawn | fatpup::Black;
    pos.square("h1") = fatpup::King | fatpup::Black;

    if (verbose)
        PrintPosition(pos);

    {
        pos.setWhiteTurn(true);

        {
            const fatpup::Position::State state = pos.getState();
            if (state == fatpup::Position::State::Check)
                success = false;
            else if (state == fatpup::Position::State::Checkmate)
                success = false;
            else if (state == fatpup::Position::State::Stalemate)
                success = false;
        }

        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for white: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        size_t numCheckmates = 0;
        for (size_t i = 0; i < moves.size(); i++)
        {
            const fatpup::Position::State state = (pos + moves[i]).getState();
            if (state == fatpup::Position::State::Checkmate)
                numCheckmates++;
            else if (state == fatpup::Position::State::Stalemate)
                success = false;
        }

        if (numCheckmates != 2)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }

    return true;
}




bool runStalemateTests(bool verbose)
{
    bool success = true;
    fatpup::Position pos;

    std::cout << testTitleColor << "Stalemate Test #1" << rang::fg::reset << std::endl;

    pos.setEmpty();

    pos.square("c7") = fatpup::King | fatpup::White;
    pos.square("d1") = fatpup::Bishop | fatpup::White;
    pos.square("f2") = fatpup::Queen | fatpup::White;
    pos.square("g1") = fatpup::Queen | fatpup::White;

    pos.square("a8") = fatpup::King | fatpup::Black;
    pos.square("b7") = fatpup::Pawn | fatpup::Black;

    if (verbose)
        PrintPosition(pos);

    {
        pos.setWhiteTurn(true);

        {
            const fatpup::Position::State state = pos.getState();
            if (state == fatpup::Position::State::Check)
                success = false;
            else if (state == fatpup::Position::State::Checkmate)
                success = false;
            else if (state == fatpup::Position::State::Stalemate)
                success = false;
        }

        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for white: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        size_t numStalemates = 0;
        for (size_t i = 0; i < moves.size(); i++)
        {
            const fatpup::Position::State state = (pos + moves[i]).getState();
            if (state == fatpup::Position::State::Stalemate)
                numStalemates++;
        }

        if (numStalemates != 6)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }


    std::cout << testTitleColor << "Stalemate Test #2" << rang::fg::reset << std::endl;

    pos.setEmpty();

    pos.square("a1") = fatpup::Knight | fatpup::White;
    pos.square("b3") = fatpup::Pawn | fatpup::White;
    pos.square("c2") = fatpup::Pawn | fatpup::White;
    pos.square("e2") = fatpup::King | fatpup::White;
    pos.square("f2") = fatpup::Pawn | fatpup::White;

    pos.square("b4") = fatpup::Pawn | fatpup::Black;
    pos.square("c3") = fatpup::Rook | fatpup::Black;
    pos.square("d2") = fatpup::Pawn | fatpup::Black;
    pos.square("e4") = fatpup::King | fatpup::Black;
    pos.square("f6") = fatpup::Queen | fatpup::Black;
    pos.square("h1") = fatpup::Rook | fatpup::Black;
    pos.square("h2") = fatpup::Rook | fatpup::Black;
    pos.square("h4") = fatpup::Knight | fatpup::Black;

    if (verbose)
        PrintPosition(pos);

    {
        pos.setWhiteTurn(false);

        {
            const fatpup::Position::State state = pos.getState();
            if (state == fatpup::Position::State::Check)
                success = false;
            else if (state == fatpup::Position::State::Checkmate)
                success = false;
            else if (state == fatpup::Position::State::Stalemate)
                success = false;
        }

        const auto moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for black: " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        size_t numStalemates = 0;
        for (size_t i = 0; i < moves.size(); i++)
        {
            const fatpup::Position::State state = (pos + moves[i]).getState();
            if (state == fatpup::Position::State::Stalemate)
                numStalemates++;
        }

        if (numStalemates != 8)
            success = false;    // to do: check if all the moves are correct
    }

    if (!success)
    {
        std::cout << errorMsgColor << "Failed, terminating..." << rang::fg::reset << std::endl;
        return false;
    }

    return true;
}
