#ifndef POSITION_H
#define POSITION_H

#include <vector>

#include "square.h"
#include "move.h"

const int BOARD_SIZE = 8;

inline int rowColToIdx(int row_idx, int col_idx) { return row_idx * BOARD_SIZE + col_idx; }

class Position
{
public:
    Position() {}
    Position(const Position& prev_pos, Move move);

    void                setInitial();
    void                setEmpty();
    void                print() const;

    float               evaluate() const;

    std::vector<Move>   possibleMoves() const;

    void                moveDone(Move move);

    bool                isWhiteTurn() const { return m_board[0].isFlagSet(WhiteTurn); }
    void                setWhiteTurn(bool white) { m_board[0].setFlag(WhiteTurn, white); }
    void                toggleTurn() { m_board[0].toggleFlag(WhiteTurn); }

    Position&           operator += (const Move& move) { moveDone(move); return *this; }
    Position            operator + (const Move& move) const { Position new_pos(*this, move); return new_pos; }

    // row_idx == 0 is 1, row_idx == 1 is 2, etc. col_idx == 0 is A, col_idx == 1 is B
    Square&             square(int row_idx, int col_idx) { return m_board[row_idx * BOARD_SIZE + col_idx]; }
    const Square&       square(int row_idx, int col_idx) const { return m_board[row_idx * BOARD_SIZE + col_idx]; }

    // for debugging/testing purposes, so that you can set a position up like this: "pos.square("a1") = Rook | White;"
    Square&             square(const std::string& square_name);
    const Square&       square(const std::string& square_name) const;

    enum class State { Normal, Check, Checkmate, Stalemate, Illegal };
    State               getState() const;

    bool                isMoveLegal(Move move) const;
    std::string         moveToString(Move move) const;
    std::string         moveToStringPGN(Move move) const;

    // to do:
    // bool isLegal() - two kings of diff colours, less than 8 pawns of each colour, no pawns on first/last rows, etc.
    // void exportToFEN() / importFromFEN()

protected:
    bool                isKingSafe() const;
    bool                legalMovesPresent() const;
    int                 evaluateMaterial() const;

    // these return false is the position is invalid (the opponent's king can be captured)
    bool                appendPossibleWhitePawnMoves(std::vector<Move>* moves, int square_idx) const;
    bool                appendPossibleBlackPawnMoves(std::vector<Move>* moves, int square_idx) const;
    bool                appendPossibleKnightMoves(std::vector<Move>* moves, int square_idx) const;
    bool                appendPossibleBishopMoves(std::vector<Move>* moves, int square_idx) const;
    bool                appendPossibleRookMoves(std::vector<Move>* moves, int square_idx) const;
    bool                appendPossibleQueenMoves(std::vector<Move>* moves, int square_idx) const;
    bool                appendPossibleKingMoves(std::vector<Move>* moves, int square_idx) const;
    bool                appendPossibleCastlings(std::vector<Move>* moves, int square_idx) const;

    Square              m_board[BOARD_SIZE * BOARD_SIZE];
};

#endif // POSITION_H
