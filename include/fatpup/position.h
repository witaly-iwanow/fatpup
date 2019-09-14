#ifndef FATPUP_POSITION_H
#define FATPUP_POSITION_H

#include <vector>
#include <cstring>

#include "fatpup/square.h"
#include "fatpup/move.h"

namespace fatpup
{
    constexpr int BOARD_SIZE = 8;
    enum { ROW1 = 0, ROW2 = 1, ROW3 = 2, ROW4 = 3, ROW5 = 4, ROW6 = 5, ROW7 = 6, ROW8 = 7 };
    enum { COLA = 0, COLB = 1, COLC = 2, COLD = 3, COLE = 4, COLF = 5, COLG = 6, COLH = 7 };
    enum
    {
        A1 =  0, B1 =  1, C1 =  2, D1 =  3, E1 =  4, F1 =  5, G1 =  6, H1 =  7,
        A2 =  8, B2 =  9, C2 = 10, D2 = 11, E2 = 12, F2 = 13, G2 = 14, H2 = 15,
        A3 = 16, B3 = 17, C3 = 18, D3 = 19, E3 = 20, F3 = 21, G3 = 22, H3 = 23,
        A4 = 24, B4 = 25, C4 = 26, D4 = 27, E4 = 28, F4 = 29, G4 = 30, H4 = 31,
        A5 = 32, B5 = 33, C5 = 34, D5 = 35, E5 = 36, F5 = 37, G5 = 38, H5 = 39,
        A6 = 40, B6 = 41, C6 = 42, D6 = 43, E6 = 44, F6 = 45, G6 = 46, H6 = 47,
        A7 = 48, B7 = 49, C7 = 50, D7 = 51, E7 = 52, F7 = 53, G7 = 54, H7 = 55,
        A8 = 56, B8 = 57, C8 = 58, D8 = 59, E8 = 60, F8 = 61, G8 = 62, H8 = 63
    };

    inline int rowColToIdx(int row_idx, int col_idx) { return row_idx * BOARD_SIZE + col_idx; }

    class Position
    {
    public:
        // this is left blank intentionally for performance reasons, can be
        // useful for engine development or something like that
        Position() {}

        Position(const Position& prev_pos, Move move);

        void                setInitial();
        void                setEmpty();
        // returns false if FEN parsing failed
        bool                setFEN(const std::string& fen);

        std::vector<Move>   possibleMoves() const;
        std::vector<Move>   possibleMoves(int src_row, int src_col, int dst_row, int dst_col) const;

        void                moveDone(Move move);

        bool                isWhiteTurn() const { return m_board[A1].isFlagSet(WhiteTurn); }
        void                setWhiteTurn(bool white) { m_board[A1].setFlag(WhiteTurn, white); }
        void                toggleTurn() { m_board[A1].toggleFlag(WhiteTurn); }

        bool                operator == (const Position& rhs) const;
        bool                operator != (const Position& rhs) const { return !(*this == rhs); }

        Position&           operator += (const Move& move) { moveDone(move); return *this; }
        Position            operator + (const Move& move) const { Position new_pos(*this, move); return new_pos; }

        // row_idx == 0 is 1, row_idx == 1 is 2, etc. col_idx == 0 is A, col_idx == 1 is B
        Square&             square(int row_idx, int col_idx) { return m_board[row_idx * BOARD_SIZE + col_idx]; }
        const Square&       square(int row_idx, int col_idx) const { return m_board[row_idx * BOARD_SIZE + col_idx]; }

        // for debugging/testing purposes, so that you can set a position up like this: "pos.square("a1") = Rook | White;"
        Square&             square(const std::string& square_name);
        const Square&       square(const std::string& square_name) const;

        // only handles check, checkmate and stalemate cases at the moment. "Illegal" is
        // never actually returned from getState() and it should probably stay this way as
        // legality check is expensive and rarely needed. There will be two separate methods
        // in the future, I guess, getStatLite() and getStateFull().
        // to do: DrawByInsufficientMaterial, DrawByRepetition, DrawBy50Moves
        enum class State { Normal, Check, Checkmate, Stalemate, Illegal };
        State               getState() const;

        std::string         moveToString(Move move) const;
        std::string         moveToStringPGN(Move move) const;

        // to do:
        // bool isLegal() - two kings of diff colors, less than 8 pawns of each color, no pawns on first/last rows, etc.

    protected:
        bool                isMoveLegal(Move move) const;
        bool                isKingSafe() const;
        bool                legalMovesPresent() const;

        // these return false if the position is invalid (the opponent's king can be captured)
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

}   // namespace fatpup

#endif // FATPUP_POSITION_H
