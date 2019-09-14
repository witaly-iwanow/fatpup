#ifndef FATPUP_SQUARE_H
#define FATPUP_SQUARE_H

#include <cassert>

namespace fatpup
{
    inline int symbolToRowIdx(const char sym)
    {
        if (sym >= '1' && sym <= '8')
            return ((int)sym - (int)'1');

        assert(0);
        return 0;
    }

    inline int symbolToColumnIdx(const char sym)
    {
        if (sym >= 'a' && sym <= 'h')
            return ((int)sym - (int)'a');
        else if (sym >= 'A' && sym <= 'H')
            return ((int)sym - (int)'A');

        assert(0);
        return 0;
    }

    enum
    {
        Empty = 0,
        Pawn = 1,
        Knight = 2,
        Bishop = 3,
        Rook = 4,
        Queen = 5,
        King = 6,
        PieceMask = 7
    };

    enum
    {
        Black = 0,          // just a stub for consistency (square1 = Pawn | White; square2 = Pawn | Black;)
        White = 8,          // indicates a white piece
        ColorMask = 8,
        CanCastle = 16,
        EnPassant = 32,
        WhiteTurn = 64      // only set on A1 square (m_board[0])
    };

    class Square
    {
    public:
        Square(): m_state(0) {}
        Square(unsigned char state): m_state(state) {}

        unsigned char           state() const { return m_state; }
        void                    set(unsigned char state) { m_state = state; }
        Square&                 operator = (unsigned char state) { m_state = state; return *this; }
        Square&                 operator |= (unsigned char state) { m_state |= state; return *this; }
        bool                    operator == (const Square& rhs) const { return (m_state == rhs.m_state); }
        bool                    operator != (const Square& rhs) const { return (m_state != rhs.m_state); }

        void                    setFlag(unsigned char flag, bool state) { if (state) m_state |= flag; else m_state &= ~flag; }
        bool                    isFlagSet(unsigned char flag) const { return (m_state & flag) ? true : false; }
        void                    setFlagToZero(unsigned char flag) { m_state &= ~flag; }
        void                    setFlagToOne(unsigned char flag) { m_state |= flag; }
        void                    toggleFlag(unsigned char flag) { m_state ^= flag; }

        unsigned char           piece() const { return (m_state & PieceMask); }
        unsigned char           pieceWithColor() const { return (m_state & (PieceMask | ColorMask)); }
        unsigned char           isWhite() const { return (m_state & White); }

        int value() const
        {
            const int piece = (m_state & PieceMask);

            if (piece == Empty)
                return 0;

            int val;
            switch (piece)
            {
            case Pawn: val = 1; break;
            case Knight: val = 3; break;
            case Bishop: val = 3; break;
            case Rook: val = 5; break;
            case Queen: val = 9; break;
            default: val = 1000;
            }

            return (m_state & White) ? val : -val;
        }

    protected:
        unsigned char       m_state;
    };
}   // namespace fatpup

#endif // FATPUP_SQUARE_H
