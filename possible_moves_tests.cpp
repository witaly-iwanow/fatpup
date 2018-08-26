#include <iostream>

#include "position.h"
#include "color_scheme.h"

#include "possible_moves_tests.h"


static const auto testTitleColor = rang::fg::yellow;
static const auto errorMsgColor = rang::fg::red;


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

    return true;
}


bool runPossiblePawnMovesTests(bool verbose)
{
    bool success = true;

    Position pos;
    pos.setEmpty();

    // advancing only, no capturing, no promotion
    pos.square("a2") = Pawn | White;
    pos.square("b3") = Pawn | White;
    pos.square("c2") = Pawn | White;
    pos.square("c4") = Pawn | White;
    pos.square("e2") = Pawn | White;
    pos.square("e6") = Pawn | White;
    pos.square("h6") = Pawn | White;
    pos.square("h5") = Pawn | White;

    pos.square("a6") = Pawn | Black;
    pos.square("b7") = Pawn | Black;
    pos.square("c7") = Pawn | Black;
    pos.square("e5") = Pawn | Black;
    pos.square("e3") = Pawn | Black;
    pos.square("g4") = Pawn | Black;
    pos.square("g3") = Pawn | Black;

    std::cout << testTitleColor << "Pawn Test #1 (advancing)" << rang::fg::reset << std::endl;
    if (verbose)
        pos.print();

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        std::vector<Move> moves = pos.possibleMoves();

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

    pos.square("a2") = Pawn | White;
    pos.square("c2") = Pawn | White;
    pos.square("e5") = Pawn | White;
    pos.square("e6") = Pawn | White;
    pos.square("g4") = Pawn | White;
    pos.square("g5") = Pawn | White;

    pos.square("a3") = Pawn | Black;
    pos.square("b4") = Pawn | Black;
    pos.square("d7") = Pawn | Black;
    pos.square("d6") = Pawn | Black;
    pos.square("d4") = Pawn | Black;
    pos.square("e7") = Pawn | Black;
    pos.square("f7") = Pawn | Black;
    pos.square("f4") = Pawn | Black;
    pos.square("h5") = Pawn | Black;

    if (verbose)
        pos.print();

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        std::vector<Move> moves = pos.possibleMoves();

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
    pos += Move("c2c4");

    if (verbose)
        pos.print();

    {
        std::vector<Move> moves = pos.possibleMoves();
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
    pos += Move("f7f5");

    if (verbose)
        pos.print();

    {
        std::vector<Move> moves = pos.possibleMoves();

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
    pos += Move("e6d7");

    if (verbose)
        pos.print();

    {
        std::vector<Move> moves = pos.possibleMoves();

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

    pos.square("a6") = Pawn | White;
    pos.square("b7") = Pawn | White;
    pos.square("e7") = Pawn | White;
    pos.square("f7") = Pawn | White;
    pos.square("h7") = Pawn | White;
    pos.square("b1") = Queen | White;
    pos.square("f1") = Rook | White;
    pos.square("g1") = Rook | White;

    pos.square("a2") = Pawn | Black;
    pos.square("b2") = Pawn | Black;
    pos.square("c2") = Pawn | Black;
    pos.square("d2") = Pawn | Black;
    pos.square("f2") = Pawn | Black;
    pos.square("g2") = Pawn | Black;
    pos.square("d8") = Bishop | Black;
    pos.square("e8") = Bishop | Black;
    pos.square("f8") = Knight | Black;

    if (verbose)
        pos.print();

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        std::vector<Move> moves = pos.possibleMoves();
        std::vector<Move> pawnMoves;
        for (size_t i = 0; i < moves.size(); i++)
        {
            if (pos.square(moves[i].fields.src_row, moves[i].fields.src_col).piece() == Pawn)
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

    Position pos;
    pos.setEmpty();

    pos.square("a1") = Knight | White;
    pos.square("a8") = Knight | White;
    pos.square("b3") = Knight | White;
    pos.square("c2") = Knight | White;
    pos.square("d4") = Knight | White;
    pos.square("g2") = Knight | White;
    pos.square("g7") = Knight | White;
    pos.square("h1") = Knight | White;
    pos.square("h8") = Knight | White;

    pos.square("b4") = Knight | Black;
    pos.square("b6") = Knight | Black;
    pos.square("c3") = Knight | Black;
    pos.square("c7") = Knight | Black;
    pos.square("d5") = Knight | Black;
    pos.square("e3") = Knight | Black;
    pos.square("e7") = Knight | Black;
    pos.square("f4") = Knight | Black;
    pos.square("f6") = Knight | Black;

    std::cout << testTitleColor << "Knight Test" << rang::fg::reset << std::endl;
    if (verbose)
        pos.print();

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        std::vector<Move> moves = pos.possibleMoves();

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

    Position pos;
    pos.setEmpty();

    pos.square("a1") = Bishop | White;
    pos.square("c6") = Bishop | White;
    pos.square("d2") = Bishop | White;
    pos.square("d4") = Bishop | White;
    pos.square("e4") = Bishop | White;
    pos.square("f7") = Bishop | White;

    pos.square("c3") = Bishop | Black;
    pos.square("c7") = Bishop | Black;
    pos.square("e5") = Bishop | Black;
    pos.square("h1") = Bishop | Black;
    pos.square("h3") = Bishop | Black;
    pos.square("h8") = Bishop | Black;

    std::cout << testTitleColor << "Bishop Test" << rang::fg::reset << std::endl;
    if (verbose)
        pos.print();

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        std::vector<Move> moves = pos.possibleMoves();

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

    Position pos;
    pos.setEmpty();

    pos.square("a1") = Rook | White;
    pos.square("e2") = Rook | White;
    pos.square("f2") = Rook | White;
    pos.square("g7") = Rook | White;
    pos.square("h8") = Rook | White;

    pos.square("a2") = Rook | Black;
    pos.square("b1") = Rook | Black;
    pos.square("b2") = Rook | Black;
    pos.square("d5") = Rook | Black;
    pos.square("h1") = Rook | Black;

    std::cout << testTitleColor << "Rook Test" << rang::fg::reset << std::endl;
    if (verbose)
        pos.print();

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        std::vector<Move> moves = pos.possibleMoves();

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

    Position pos;
    pos.setEmpty();

    pos.square("d5") = Queen | White;

    pos.square("g1") = Queen | Black;

    std::cout << testTitleColor << "Queen Test #1" << rang::fg::reset << std::endl;
    if (verbose)
        pos.print();

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        std::vector<Move> moves = pos.possibleMoves();

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

    pos.square("d3") = Queen | White;
    pos.square("d5") = Queen | White;
    pos.square("f3") = Queen | White;
    pos.square("f5") = Queen | White;

    pos.square("a1") = Queen | Black;
    pos.square("e4") = Queen | Black;

    if (verbose)
        pos.print();

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        std::vector<Move> moves = pos.possibleMoves();

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

    Position pos;
    pos.setEmpty();

    pos.square("a1") = King | White;
    pos.square("a8") = King | White;
    pos.square("h1") = King | White;
    pos.square("h8") = King | White;
    pos.square("c6") = Pawn | White;

    pos.square("b6") = King | Black;
    pos.square("c7") = King | Black;
    pos.square("d4") = King | Black;
    pos.square("f3") = King | Black;

    std::cout << testTitleColor << "King Test" << rang::fg::reset << std::endl;
    if (verbose)
        pos.print();

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        std::vector<Move> moves = pos.possibleMoves();

        if (verbose)
        {
            std::cout << "Number of moves for " << (side == 0 ? "white" : "black") << ": " << moves.size() << std::endl;
            for (size_t i = 0; i < moves.size(); i++)
                std::cout << "  " << (i + 1) << "/" << moves.size() << " " << pos.moveToString(moves[i]) << std::endl;

            std::cout << std::endl;
        }

        if (moves.size() != (side ? 24 : 8))
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
    Position pos;

    std::cout << testTitleColor << "Castling Test #1" << rang::fg::reset << std::endl;
    pos.setEmpty();

    pos.square("a1") = Rook | White | CanCastle;
    pos.square("a2") = Pawn | White;
    pos.square("e1") = King | White | CanCastle;
    pos.square("h1") = Rook | White | CanCastle;
    pos.square("h2") = Pawn | White;

    pos.square("a8") = Rook | Black | CanCastle;
    pos.square("a7") = Pawn | Black;
    pos.square("e8") = King | Black | CanCastle;
    pos.square("h8") = Rook | Black | CanCastle;
    pos.square("h7") = Pawn | Black;

    if (verbose)
        pos.print();

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        std::vector<Move> moves = pos.possibleMoves();

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
    pos += Move("h1f1");

    if (verbose)
        pos.print();

    {
        std::vector<Move> moves = pos.possibleMoves();

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
    pos += Move("h8f8");
    pos += Move("f1f8");

    if (verbose)
        pos.print();

    {
        std::vector<Move> moves = pos.possibleMoves();

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

    pos.square("a1") = Rook | White | CanCastle;
    pos.square("a2") = Pawn | White;
    pos.square("e1") = King | White | CanCastle;
    pos.square("h1") = Rook | White | CanCastle;
    pos.square("h2") = Pawn | White;
    pos.square("f1") = Bishop | White;

    pos.square("a8") = Rook | Black | CanCastle;
    pos.square("a7") = Pawn | Black;
    pos.square("e8") = King | Black | CanCastle;
    pos.square("h8") = Rook | Black | CanCastle;
    pos.square("h7") = Pawn | Black;
    pos.square("g7") = Bishop | Black;
    pos.square("g6") = Bishop | Black;

    pos.setWhiteTurn(true);
    if (verbose)
        pos.print();

    {
        std::vector<Move> moves = pos.possibleMoves();

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
    pos += Move("f1b5");
    pos += Move("e8e7");
    pos += Move("b5e8");
    pos += Move("e7e8");
    pos += Move("h2h4");
    pos += Move("g7h6");

    if (verbose)
        pos.print();

    for (int side = 0; side < 2; side++)
    {
        pos.setWhiteTurn(side == 0);
        std::vector<Move> moves = pos.possibleMoves();

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
    Position pos;

    std::cout << testTitleColor << "Check Test #1" << rang::fg::reset << std::endl;

    pos.setEmpty();

    pos.square("a5") = Knight | White;
    pos.square("e8") = King | White;
    pos.square("b3") = Knight | White;
    pos.square("h2") = Bishop | White;
    pos.square("h7") = Rook | White;

    pos.square("c7") = King | Black;
    pos.square("d7") = Pawn | Black;

    if (verbose)
        pos.print();

    {
        pos.setWhiteTurn(false);

        if (pos.getState() != Position::State::Check)
            success = false;

        std::vector<Move> moves = pos.possibleMoves();

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
    pos.square("e4") = Knight | Black;
    if (verbose)
        pos.print();

    {
        pos.setWhiteTurn(false);

        if (pos.getState() != Position::State::Check)
            success = false;

        std::vector<Move> moves = pos.possibleMoves();

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
    pos += Move("e4d6");
    if (verbose)
        pos.print();

    {
        if (pos.getState() != Position::State::Check)
            success = false;

        std::vector<Move> moves = pos.possibleMoves();

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

    pos.square("a4") = Rook | White;
    pos.square("c4") = Pawn | White;
    pos.square("c3") = EnPassant;
    pos.square("d2") = King | White;

    pos.square("b4") = Pawn | Black;
    pos.square("c5") = Knight | Black;
    pos.square("d4") = King | Black;
    pos.square("d5") = Queen | Black;
    pos.square("d6") = Bishop | Black;
    pos.square("e2") = Pawn | Black;
    pos.square("e4") = Pawn | Black;
    pos.square("f2") = Pawn | Black;

    if (verbose)
        pos.print();

    {
        pos.setWhiteTurn(false);

        {
            const Position::State state = pos.getState();
            if (state == Position::State::Check)
                success = false;
            else if (state == Position::State::Checkmate)
                success = false;
            else if (state == Position::State::Stalemate)
                success = false;
        }

        std::vector<Move> moves = pos.possibleMoves();

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
            const Position::State state = (pos + moves[i]).getState();
            if (state == Position::State::Check)
                numChecks++;
            else if (state == Position::State::Checkmate)
                success = false;
            else if (state == Position::State::Stalemate)
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

    pos.square("a1") = Rook | CanCastle | White;
    pos.square("e1") = King | CanCastle | White;
    pos.square("e4") = Knight | White;
    pos.square("f2") = Queen | White;
    pos.square("f3") = Bishop | White;
    pos.square("g2") = Pawn | White;

    pos.square("f4") = Knight | Black;
    pos.square("h1") = King | Black;
    pos.square("h3") = Pawn | Black;
    pos.square("h4") = Bishop | Black;

    if (verbose)
        pos.print();

    {
        pos.setWhiteTurn(true);

        {
            const Position::State state = pos.getState();
            if (state == Position::State::Check)
                success = false;
            else if (state == Position::State::Checkmate)
                success = false;
            else if (state == Position::State::Stalemate)
                success = false;
        }

        std::vector<Move> moves = pos.possibleMoves();

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
            const Position::State state = (pos + moves[i]).getState();
            if (state == Position::State::Check)
                numChecks++;
            else if (state == Position::State::Checkmate)
                success = false;
            else if (state == Position::State::Stalemate)
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
    Position pos;

    std::cout << testTitleColor << "Checkmate Test #1" << rang::fg::reset << std::endl;

    pos.setEmpty();

    pos.square("a2") = Bishop | White;
    pos.square("e7") = King | White;
    pos.square("g5") = Pawn | White;
    pos.square("h5") = Queen | White;

    pos.square("f5") = Pawn | Black;
    pos.square("f6") = EnPassant;
    pos.square("g7") = King | Black;

    if (verbose)
        pos.print();

    {
        pos.setWhiteTurn(true);

        {
            const Position::State state = pos.getState();
            if (state == Position::State::Check)
                success = false;
            else if (state == Position::State::Checkmate)
                success = false;
            else if (state == Position::State::Stalemate)
                success = false;
        }

        std::vector<Move> moves = pos.possibleMoves();

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
            const Position::State state = (pos + moves[i]).getState();
            if (state == Position::State::Checkmate)
              numCheckmates++;
            else if (state == Position::State::Stalemate)
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

    pos.square("c6") = Queen | White;
    pos.square("d3") = Knight | White;
    pos.square("f3") = King | White;
    pos.square("f4") = Bishop | White;
    pos.square("g4") = Rook | White;
    pos.square("h7") = Pawn | White;

    pos.square("a5") = Bishop | Black;
    pos.square("a7") = Rook | Black;
    pos.square("e5") = Pawn | Black;
    pos.square("f2") = Pawn | Black;
    pos.square("h1") = King | Black;

    if (verbose)
        pos.print();

    {
        pos.setWhiteTurn(true);

        {
            const Position::State state = pos.getState();
            if (state == Position::State::Check)
                success = false;
            else if (state == Position::State::Checkmate)
                success = false;
            else if (state == Position::State::Stalemate)
                success = false;
        }

        std::vector<Move> moves = pos.possibleMoves();

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
            const Position::State state = (pos + moves[i]).getState();
            if (state == Position::State::Checkmate)
                numCheckmates++;
            else if (state == Position::State::Stalemate)
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
    Position pos;

    std::cout << testTitleColor << "Stalemate Test #1" << rang::fg::reset << std::endl;

    pos.setEmpty();

    pos.square("c7") = King | White;
    pos.square("d1") = Bishop | White;
    pos.square("f2") = Queen | White;
    pos.square("g1") = Queen | White;

    pos.square("a8") = King | Black;
    pos.square("b7") = Pawn | Black;

    if (verbose)
        pos.print();

    {
        pos.setWhiteTurn(true);

        {
            const Position::State state = pos.getState();
            if (state == Position::State::Check)
                success = false;
            else if (state == Position::State::Checkmate)
                success = false;
            else if (state == Position::State::Stalemate)
                success = false;
        }

        std::vector<Move> moves = pos.possibleMoves();

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
            const Position::State state = (pos + moves[i]).getState();
            if (state == Position::State::Stalemate)
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

    pos.square("a1") = Knight | White;
    pos.square("b3") = Pawn | White;
    pos.square("c2") = Pawn | White;
    pos.square("e2") = King | White;
    pos.square("f2") = Pawn | White;

    pos.square("b4") = Pawn | Black;
    pos.square("c3") = Rook | Black;
    pos.square("d2") = Pawn | Black;
    pos.square("e4") = King | Black;
    pos.square("f6") = Queen | Black;
    pos.square("h1") = Rook | Black;
    pos.square("h2") = Rook | Black;
    pos.square("h4") = Knight | Black;

    if (verbose)
        pos.print();

    {
        pos.setWhiteTurn(false);

        {
            const Position::State state = pos.getState();
            if (state == Position::State::Check)
                success = false;
            else if (state == Position::State::Checkmate)
                success = false;
            else if (state == Position::State::Stalemate)
                success = false;
        }

        std::vector<Move> moves = pos.possibleMoves();

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
            const Position::State state = (pos + moves[i]).getState();
            if (state == Position::State::Stalemate)
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
