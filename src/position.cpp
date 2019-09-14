#include <cstdio>
#include <cassert>
#include <cstring>
#include <map>

#include "fatpup/position.h"

namespace fatpup
{
    Position::Position(const Position& prev_pos, Move move)
    {
        std::memcpy(m_board, prev_pos.m_board, sizeof(m_board));
        moveDone(move);
    }

    bool Position::operator == (const Position& rhs) const
    {
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i)
        {
            if (m_board[i] != rhs.m_board[i])
                return false;
        }
        return true;
    }


    void Position::setInitial()
    {
        assert(BOARD_SIZE == 8);

        m_board[A1] = Rook | White | CanCastle | WhiteTurn;
        m_board[B1] = Knight | White;
        m_board[C1] = Bishop | White;
        m_board[D1] = Queen | White;
        m_board[E1] = King | White;
        m_board[F1] = Bishop | White;
        m_board[G1] = Knight | White;
        m_board[H1] = Rook | White | CanCastle;

        int s_idx = A2;
        for (; s_idx <= H2; ++s_idx)
            m_board[s_idx] = Pawn | White;

        for (; s_idx < A7; ++s_idx)
            m_board[s_idx] = Empty;

        for (; s_idx <= H7; ++s_idx)
            m_board[s_idx] = Pawn;

        m_board[A8] = Rook | CanCastle;
        m_board[B8] = Knight;
        m_board[C8] = Bishop;
        m_board[D8] = Queen;
        m_board[E8] = King;
        m_board[F8] = Bishop;
        m_board[G8] = Knight;
        m_board[H8] = Rook | CanCastle;
    }

    void Position::setEmpty()
    {
        memset(m_board, 0, sizeof(m_board));
        assert(m_board[A1].state() == 0);
        assert(m_board[H8].state() == 0);

        m_board[A1].setFlagToOne(WhiteTurn);
    }

    bool Position::setFEN(const std::string& FEN)
    {
        static const std::map<char, Square> charToPiece
        {
            { 'p', Pawn | Black }, { 'n' , Knight | Black }, { 'b', Bishop | Black }, { 'r', Rook | Black }, { 'q', Queen | Black }, { 'k', King | Black },
            { 'P', Pawn | White }, { 'N' , Knight | White }, { 'B', Bishop | White }, { 'R', Rook | White }, { 'Q', Queen | White }, { 'K', King | White }
        };
        Position result;
        result.setEmpty();

        // overrun protection without constant FEN.length() checks
        constexpr char fenGuard{'#'};
        const std::string guardedFen = FEN + std::string(8, fenGuard);

        int sym_idx = 0;
        int row_idx = 0, col_idx = 0;
        while (row_idx < BOARD_SIZE)
        {
            const char sym = guardedFen[sym_idx++];

            if (sym >= '1' && sym <= '8')
            {
                col_idx += (int)sym - (int)'0';
                if (col_idx > BOARD_SIZE)
                    return false;
            }
            else
            {
                const auto& piece = charToPiece.find(sym);
                if (piece == charToPiece.end())
                    return false;

                result.square(BOARD_SIZE - 1 - row_idx, col_idx) = piece->second;
                ++col_idx;
            }

            if (col_idx == BOARD_SIZE)
            {
                ++row_idx;
                col_idx = 0;

                const char next_sym = guardedFen[sym_idx++];
                if (next_sym != (row_idx < BOARD_SIZE ? '/' : ' '))
                    return false;
            }
        }

        if (row_idx != BOARD_SIZE || col_idx != 0)
            return false;

        const char turn_sym = guardedFen[sym_idx++];
        if (turn_sym != 'w' && turn_sym != 'b')
            return false;
        result.setWhiteTurn(turn_sym == 'w');

        if (guardedFen[sym_idx++] != ' ')
            return false;

        // castling availability
        if (guardedFen[sym_idx] == '-')
            ++sym_idx;
        else
        {
            char sym = guardedFen[sym_idx++];
            while (sym != ' ')
            {
                if (sym == 'k')
                {
                    Square& square = result.square("h8");
                    if (square.pieceWithColor() != (Rook | Black))
                        return false;
                    square.setFlagToOne(CanCastle);
                }
                else if (sym == 'q')
                {
                    Square& square = result.square("a8");
                    if (square.pieceWithColor() != (Rook | Black))
                        return false;
                    square.setFlagToOne(CanCastle);
                }
                else if (sym == 'K')
                {
                    Square& square = result.square("h1");
                    if (square.pieceWithColor() != (Rook | White))
                        return false;
                    square.setFlagToOne(CanCastle);
                }
                else if (sym == 'Q')
                {
                    Square& square = result.square("a1");
                    if (square.pieceWithColor() != (Rook | White))
                        return false;
                    square.setFlagToOne(CanCastle);
                }
                else
                    return false;

                sym = guardedFen[sym_idx++];
            }
        }

        // en passant
        if (guardedFen[sym_idx] == '-')
            ++sym_idx;
        else
        {
            const auto col_sym = guardedFen[sym_idx++];
            const auto row_sym = guardedFen[sym_idx++];
            if (col_sym < 'a' || col_sym > 'h' || (row_sym != (turn_sym == 'w' ? '6' : '3')))
                return false;

            Square& square = result.square(std::string{col_sym, row_sym});
            if (square.piece() != Empty)
                return false;
            square.setFlagToOne(EnPassant);
        }

        // Halfmove clock / Fullmove number are skipped for now

        *this = result;
        return true;
    }

    Square& Position::square(const std::string& square_name)
    {
        assert(square_name.length() == 2);
        return square(symbolToRowIdx(square_name[1]), symbolToColumnIdx(square_name[0]));
    }

    const Square& Position::square(const std::string& square_name) const
    {
        assert(square_name.length() == 2);
        return square(symbolToRowIdx(square_name[1]), symbolToColumnIdx(square_name[0]));
    }

    std::vector<Move> Position::possibleMoves() const
    {
        std::vector<Move> all_moves;
        all_moves.reserve(64);
        const unsigned char white_turn = (m_board[A1].state() & WhiteTurn) ? White : 0;

        for (int s_idx = 0; s_idx < BOARD_SIZE * BOARD_SIZE; ++s_idx)
        {
            const Square square = m_board[s_idx];
            const unsigned char piece = square.piece();

            if (piece != Empty)
            {
                const unsigned char white = square.isWhite();

                if (white_turn == white)
                {
                    bool legal = true;
                    switch (piece)
                    {
                    case Pawn: if (white)
                                   legal = appendPossibleWhitePawnMoves(&all_moves, s_idx);
                               else
                                   legal = appendPossibleBlackPawnMoves(&all_moves, s_idx);
                               break;

                    case Knight: legal = appendPossibleKnightMoves(&all_moves, s_idx); break;
                    case Bishop: legal = appendPossibleBishopMoves(&all_moves, s_idx); break;
                    case Rook: legal = appendPossibleRookMoves(&all_moves, s_idx); break;
                    case Queen: legal = appendPossibleQueenMoves(&all_moves, s_idx); break;
                    default: legal = appendPossibleKingMoves(&all_moves, s_idx);
                             appendPossibleCastlings(&all_moves, s_idx);
                    }
                    assert(legal);
                }
            }
        }

        return all_moves;
    }

    std::vector<Move> Position::possibleMoves(int src_row, int src_col, int dst_row, int dst_col) const
    {
        // we cannot just create a move with (src_row, src_col, dst_row, dst_col)
        // and return it to the caller if it's legal. There are castlings which
        // require Move::fields::rook_src/dst_col set correctly and pawn promotions

        // we'll have 4 moves in the case of promotion - white pawn move a7a8 can be
        // a7a8 -> queen, a7a8 -> rook, a7a8 -> bishop, a7a8 -> knight. But in most
        // cases it will be only one move, or even zero if the move is illegal or the
        // source square is empty
        std::vector<Move> moves;
        moves.reserve(4);

        // possible moves of the piece at (src_row, dst_row)
        std::vector<Move> src_possible_moves;
        src_possible_moves.reserve(32);
        const unsigned char white_turn = (m_board[A1].state() & WhiteTurn) ? White : 0;

        const int s_idx = src_row * BOARD_SIZE + src_col;
        const Square square = m_board[s_idx];
        const unsigned char piece = square.piece();

        if (piece != Empty)
        {
            const unsigned char white = square.isWhite();

            if (white_turn == white)
            {
                bool legal = true;
                switch (piece)
                {
                case Pawn: if (white)
                                legal = appendPossibleWhitePawnMoves(&src_possible_moves, s_idx);
                            else
                                legal = appendPossibleBlackPawnMoves(&src_possible_moves, s_idx);
                            break;

                case Knight: legal = appendPossibleKnightMoves(&src_possible_moves, s_idx); break;
                case Bishop: legal = appendPossibleBishopMoves(&src_possible_moves, s_idx); break;
                case Rook: legal = appendPossibleRookMoves(&src_possible_moves, s_idx); break;
                case Queen: legal = appendPossibleQueenMoves(&src_possible_moves, s_idx); break;
                default: legal = appendPossibleKingMoves(&src_possible_moves, s_idx);
                            appendPossibleCastlings(&src_possible_moves, s_idx);
                }
                assert(legal);

                for (const auto m: src_possible_moves)
                {
                    if (dst_row == m.fields.dst_row && dst_col == m.fields.dst_col)
                        moves.push_back(m);
                }
            }
        }

        return moves;
    }

    void Position::moveDone(const Move move)
    {
        const unsigned char white_turn = (m_board[A1].state() & WhiteTurn);

        if (move.fields.src_row != move.fields.dst_row || move.fields.src_col != move.fields.dst_col)
        {
            // check for the empty move - a special case used for castling availability check (if the king is
            // under attack, it cannot castle)
            const Square src_square = m_board[move.fields.src_row * BOARD_SIZE + move.fields.src_col];
            const Square dst_square = m_board[move.fields.dst_row * BOARD_SIZE + move.fields.dst_col];

            m_board[move.fields.src_row * BOARD_SIZE + move.fields.src_col] = Empty;

            if (move.fields.promoted_to == 0)
            {
                Square& resulting_dst_square = m_board[move.fields.dst_row * BOARD_SIZE + move.fields.dst_col];
                resulting_dst_square = src_square.pieceWithColor();

                if (src_square.piece() == Pawn)
                {
                    if (dst_square.state() & EnPassant)
                    {
                        assert(dst_square.piece() == Empty);
                        Square& captured_pawn_square = square(move.fields.src_row, move.fields.dst_col);

                        assert(captured_pawn_square.piece() == Pawn);
                        captured_pawn_square = Empty;
                    }
                    else if (move.fields.src_col == move.fields.dst_col)
                    {
                        if (move.fields.src_row == ROW2 && move.fields.dst_row == ROW4)
                            m_board[ROW3 * BOARD_SIZE + move.fields.src_col] = EnPassant;
                        else if (move.fields.src_row == ROW7 && move.fields.dst_row == ROW5)
                            m_board[ROW6 * BOARD_SIZE + move.fields.src_col] = EnPassant;
                    }
                }
                else if (src_square.piece() == King && move.fields.rook_src_col != move.fields.rook_dst_col)
                {
                    assert(move.fields.src_row == ROW1 || move.fields.dst_row == ROW8);
                    assert(move.fields.src_col == COLE);
                    assert(move.fields.rook_src_col == COLA || move.fields.rook_src_col == COLH);
                    assert(move.fields.src_row == move.fields.dst_row);

                    // move the rook too
                    Square& rook_src_square = m_board[move.fields.src_row * BOARD_SIZE + move.fields.rook_src_col];
                    Square& rook_dst_square = m_board[move.fields.dst_row * BOARD_SIZE + move.fields.rook_dst_col];
                    assert(rook_src_square.piece() == Rook);
                    assert(rook_src_square.state() & CanCastle);
                    rook_dst_square = rook_src_square.pieceWithColor();
                    rook_src_square = Empty;

                    // set CanCastle flags of the rook to zero
                    rook_dst_square.setFlagToZero(CanCastle);
                }
                else if (src_square.piece() == King && move.fields.src_col == COLE && move.fields.src_row == (white_turn ? ROW1 : ROW8))
                {
                    // the king moved from its original position, invalidate castling on both rooks
                    Square& king_rook_square = m_board[white_turn ? H1 : H8];
                    Square& queen_rook_square = m_board[white_turn ? A1 : A8];
                    king_rook_square.setFlagToZero(CanCastle);
                    queen_rook_square.setFlagToZero(CanCastle);
                }
            }
            else
                m_board[move.fields.dst_row * BOARD_SIZE + move.fields.dst_col] = move.fields.promoted_to | src_square.isWhite();

            // erase en passant marks from the previous move
            const int en_passant_row_to_clear = white_turn ? ROW6 : ROW3;
            Square* row_ptr = m_board + en_passant_row_to_clear * BOARD_SIZE;
            for (int s_idx = 0; s_idx < BOARD_SIZE; ++s_idx)
                row_ptr[s_idx].setFlagToZero(EnPassant);
        }

        if (white_turn)
            m_board[A1].setFlagToZero(WhiteTurn);
        else
            m_board[A1].setFlagToOne(WhiteTurn);
    }
}   // namespace fatpup
