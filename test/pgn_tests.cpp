#include <iostream>

#include "fatpup/position.h"
#include "color_scheme.h"

static bool expectPgn(
    const std::string& fen,
    int src_row,
    int src_col,
    int dst_row,
    int dst_col,
    const std::string& expected_pgn,
    unsigned int promoted_to = 0)
{
    fatpup::Position pos;
    if (!pos.setFEN(fen))
    {
        std::cout << "Error! Position::setFEN failed for FEN: " << fen << std::endl;
        return false;
    }

    const std::vector<fatpup::Move> moves = pos.possibleMoves(src_row, src_col, dst_row, dst_col);
    if (moves.empty())
    {
        std::cout << "Error! Move not found in possibleMoves: " << src_row << src_col << dst_row << dst_col << std::endl;
        return false;
    }

    fatpup::Move move = moves[0];
    if (promoted_to)
    {
        bool promotion_found = false;
        for (const auto& candidate : moves)
        {
            if (candidate.fields.promoted_to == promoted_to)
            {
                move = candidate;
                promotion_found = true;
                break;
            }
        }
        if (!promotion_found)
        {
            std::cout << "Error! Promotion move not found for target piece: " << promoted_to << std::endl;
            return false;
        }
    }

    const std::string pgn = pos.moveToStringPGN(move);
    if (pgn != expected_pgn)
    {
        std::cout << "Error! moveToStringPGN mismatch. Expected '" << expected_pgn << "', got '" << pgn << "'" << std::endl;
        return false;
    }

    return true;
}

bool runPgnTests()
{
    std::cout << testTitleColor << "PGN SAN Tests" << rang::fg::reset << std::endl;

    if (!expectPgn("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", fatpup::ROW2, fatpup::COLE, fatpup::ROW4, fatpup::COLE, "e4"))
        return false;

    if (!expectPgn("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", fatpup::ROW1, fatpup::COLG, fatpup::ROW3, fatpup::COLF, "Nf3"))
        return false;

    if (!expectPgn("4k3/8/8/3p4/4P3/8/8/4K3 w - - 0 1", fatpup::ROW4, fatpup::COLE, fatpup::ROW5, fatpup::COLD, "exd5"))
        return false;

    {
        fatpup::Position pos;
        if (!pos.setFEN("rnbqkb1r/1ppp2pp/8/p3p3/4p3/6P1/PPP2P1P/RNBQKB1R w KQkq - 0 1"))
            return false;

        const std::vector<fatpup::Move> white_moves = pos.possibleMoves(fatpup::ROW2, fatpup::COLF, fatpup::ROW4, fatpup::COLF);
        if (white_moves.empty())
            return false;
        pos += white_moves[0];

        const std::vector<fatpup::Move> black_moves = pos.possibleMoves(fatpup::ROW4, fatpup::COLE, fatpup::ROW3, fatpup::COLF);
        if (black_moves.empty())
            return false;
        if (pos.moveToStringPGN(black_moves[0]) != "exf3")
        {
            std::cout << "Error! moveToStringPGN mismatch. Expected 'exf3', got '" << pos.moveToStringPGN(black_moves[0]) << "'" << std::endl;
            return false;
        }
    }

    if (!expectPgn("4k3/8/8/8/8/8/8/4K2R w K - 0 1", fatpup::ROW1, fatpup::COLE, fatpup::ROW1, fatpup::COLG, "O-O"))
        return false;

    if (!expectPgn("k7/4P3/8/8/8/8/8/4K3 w - - 0 1", fatpup::ROW7, fatpup::COLE, fatpup::ROW8, fatpup::COLE, "e8=Q+", fatpup::Queen))
        return false;

    if (!expectPgn("4k3/8/8/8/8/2N1N3/8/4K3 w - - 0 1", fatpup::ROW3, fatpup::COLC, fatpup::ROW5, fatpup::COLD, "Ncd5"))
        return false;

    if (!expectPgn("4k3/8/8/8/8/R7/8/R3K3 w - - 0 1", fatpup::ROW1, fatpup::COLA, fatpup::ROW2, fatpup::COLA, "R1a2"))
        return false;

    if (!expectPgn("4k3/8/8/8/8/1N6/8/1N3N1K w - - 0 1", fatpup::ROW1, fatpup::COLB, fatpup::ROW2, fatpup::COLD, "Nb1d2"))
        return false;

    if (!expectPgn("1r4k1/5Npp/4Q3/8/8/8/6K1/8 w - - 0 1", fatpup::ROW7, fatpup::COLF, fatpup::ROW8, fatpup::COLD, "Nd8+"))
        return false;

    if (!expectPgn("8/r6P/2Q5/b3p3/5BR1/3N1K2/5p2/7k w - - 0 1", fatpup::ROW3, fatpup::COLD, fatpup::ROW2, fatpup::COLF, "Nxf2#"))
        return false;

    std::cout << successMsgColor << "  Success, all PGN SAN tests passed!" << rang::fg::reset << std::endl;
    return true;
}
