#include <cstdio>
#include <cassert>

#include "position.h"
#include "color_scheme.h"

namespace fatpup
{
    Position::Position(const Position& prev_pos, Move move)
    {
        memcpy(m_board, prev_pos.m_board, sizeof(m_board));
        moveDone(move);
    }

    void Position::setInitial()
    {
        assert(BOARD_SIZE == 8);

        m_board[0] = Rook | White | CanCastle;
        m_board[1] = Knight | White;
        m_board[2] = Bishop | White;
        m_board[3] = Queen | White;
        m_board[4] = King | White | CanCastle;
        m_board[5] = Bishop | White;
        m_board[6] = Knight | White;
        m_board[7] = Rook | White | CanCastle;

        int s_idx = 8;
        for (; s_idx < 16; ++s_idx)
            m_board[s_idx] = Pawn | White;

        for (; s_idx < 48; ++s_idx)
            m_board[s_idx] = Empty;

        for (; s_idx < 56; ++s_idx)
            m_board[s_idx] = Pawn;

        m_board[56] = Rook | CanCastle;
        m_board[57] = Knight;
        m_board[58] = Bishop;
        m_board[59] = Queen;
        m_board[60] = King | CanCastle;
        m_board[61] = Bishop;
        m_board[62] = Knight;
        m_board[63] = Rook | CanCastle;

        m_board[0].setFlagToOne(WhiteTurn);
    }

    void Position::setEmpty()
    {
        memset(m_board, 0, sizeof(m_board));
        assert(m_board[0].state() == 0);
        assert(m_board[BOARD_SIZE * BOARD_SIZE - 1].state() == 0);

        m_board[0].setFlagToOne(WhiteTurn);
    }

    void Position::print() const
    {
        for (int row_idx = BOARD_SIZE - 1; row_idx >= 0; --row_idx)
        {
            std::cout << (row_idx + 1) << " ";
            for (int col_idx = 0; col_idx < BOARD_SIZE; ++col_idx)
            {
                const Square square = m_board[row_idx * BOARD_SIZE + col_idx];
                const unsigned char piece = square.piece();

                if (piece == Empty)
                {
                    if (square.isFlagSet(EnPassant))
                        std::cout << "*";
                    else
                        std::cout << (((row_idx ^ col_idx ^ lightForegroundScheme) & 1) ? emptySquare : " ");
                }
                else
                {
                    std::cout << (square.isWhite() ? whitePieceColor : blackPieceColor);

                    switch (piece)
                    {
                    case Pawn: std::cout << "p"; break;
                    case Knight: std::cout << "N"; break;
                    case Bishop: std::cout << "B"; break;
                    case Rook: std::cout << "R"; break;
                    case Queen: std::cout << "Q"; break;
                    default: std::cout << "K";
                    }

                    std::cout << rang::fg::reset;
                }
            }

            std::cout << "\n";
        }

        std::cout << "  ABCDEFGH" << std::endl;
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
        const unsigned char white_turn = (m_board[0].state() & WhiteTurn) ? White : 0;

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

    #if 0
        printf("Num of pos moves: %d\n", all_moves.size());
        for (int m_idx = 0; m_idx < all_moves.size(); ++m_idx)
        {
            printf("%d.%d-%d.%d\n", all_moves[m_idx].fields.src_row, all_moves[m_idx].fields.src_col, all_moves[m_idx].fields.dst_row, all_moves[m_idx].fields.dst_col);
        }
    #endif

        return all_moves;
    }

    void Position::moveDone(Move move)
    {
        const unsigned char white_turn = (m_board[0].state() & WhiteTurn);

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
                resulting_dst_square = src_square;
                resulting_dst_square.setFlagToZero(CanCastle);

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
                        if (move.fields.src_row == 1 && move.fields.dst_row == 3)
                            m_board[2 * BOARD_SIZE + move.fields.src_col] = EnPassant;
                        else if (move.fields.src_row == (BOARD_SIZE - 2) && move.fields.dst_row == (BOARD_SIZE - 4))
                            m_board[(BOARD_SIZE - 3) * BOARD_SIZE + move.fields.src_col] = EnPassant;
                    }
                }
                else if (src_square.piece() == King && move.fields.rook_src_col != move.fields.rook_dst_col)
                {
                    assert(src_square.state() & CanCastle);
                    assert(move.fields.src_row == 0 || move.fields.dst_row == (BOARD_SIZE - 1));
                    assert(move.fields.src_col == 4);
                    assert(move.fields.rook_src_col == 0 || move.fields.rook_src_col == (BOARD_SIZE - 1));
                    assert(move.fields.src_row == move.fields.dst_row);

                    // move the rook too
                    Square& rook_src_square = m_board[move.fields.src_row * BOARD_SIZE + move.fields.rook_src_col];
                    Square& rook_dst_square = m_board[move.fields.dst_row * BOARD_SIZE + move.fields.rook_dst_col];
                    assert(rook_src_square.piece() == Rook);
                    assert(rook_src_square.state() & CanCastle);
                    rook_dst_square = rook_src_square;
                    rook_src_square = Empty;

                    // set CanCastle flags of the rook to zero
                    rook_dst_square.setFlagToZero(CanCastle);
                }
            }
            else
                m_board[move.fields.dst_row * BOARD_SIZE + move.fields.dst_col] = move.fields.promoted_to | src_square.isWhite();

            // erase en passant marks from the previous move
            const int en_passant_row_to_clear = white_turn ? (BOARD_SIZE - 3) : 2;
            Square * row_ptr = m_board + en_passant_row_to_clear * BOARD_SIZE;
            for (int s_idx = 0; s_idx < BOARD_SIZE; ++ s_idx)
                row_ptr[s_idx].setFlagToZero(EnPassant);
        }

        if (white_turn)
            m_board[0].setFlagToZero(WhiteTurn);
        else
            m_board[0].setFlagToOne(WhiteTurn);
    }
}   // namespace fatpup
