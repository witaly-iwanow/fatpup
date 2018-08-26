#include <cassert>

#include "position.h"


Position::State Position::getState() const
{
    Position new_pos = *this;
    new_pos.toggleTurn();

    const bool king_attacked = !new_pos.isKingSafe();
    const bool moves_present = legalMovesPresent();
    return king_attacked ? (moves_present ? Position::State::Check : Position::State::Checkmate) :
                           (moves_present ? Position::State::Normal : Position::State::Stalemate);
}


bool Position::isKingSafe() const
{
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
                               legal = appendPossibleWhitePawnMoves(NULL, s_idx);
                           else
                               legal = appendPossibleBlackPawnMoves(NULL, s_idx);
                    break;

                case Knight: legal = appendPossibleKnightMoves(NULL, s_idx); break;
                case Bishop: legal = appendPossibleBishopMoves(NULL, s_idx); break;
                case Rook: legal = appendPossibleRookMoves(NULL, s_idx); break;
                case Queen: legal = appendPossibleQueenMoves(NULL, s_idx); break;
                default: legal = appendPossibleKingMoves(NULL, s_idx);
                }

                if (!legal)
                    return false;
            }
        }
    }

    return true;
}


bool Position::legalMovesPresent() const
{
    const unsigned char white_turn = (m_board[0].state() & WhiteTurn) ? White : 0;
    std::vector<Move> moves;

    for (int s_idx = 0; s_idx < BOARD_SIZE * BOARD_SIZE; ++s_idx)
    {
        const Square square = m_board[s_idx];

        const unsigned char piece = square.piece();

        if (piece != Empty)
        {
            const unsigned char white = square.isWhite();

            if (white_turn == white)
            {
                switch (piece)
                {
                    case Pawn: if (white)
                        appendPossibleWhitePawnMoves(&moves, s_idx);
                    else
                        appendPossibleBlackPawnMoves(&moves, s_idx);
                        break;

                    case Knight: appendPossibleKnightMoves(&moves, s_idx); break;
                    case Bishop: appendPossibleBishopMoves(&moves, s_idx); break;
                    case Rook: appendPossibleRookMoves(&moves, s_idx); break;
                    case Queen: appendPossibleQueenMoves(&moves, s_idx); break;
                    default: appendPossibleKingMoves(&moves, s_idx);
                }

                if (!moves.empty())
                    return true;
            }
        }
    }

    return false;
}


bool Position::isMoveLegal(Move move) const
{
    Position new_pos(*this, move);
    return new_pos.isKingSafe();
}


bool Position::appendPossibleWhitePawnMoves(std::vector<Move>* moves, int square_idx) const
{
    const Square square = m_board[square_idx];
    assert(square.piece() == Pawn);
    assert(square.isWhite());

    const int row_idx = square_idx / BOARD_SIZE;
    const int col_idx = square_idx - row_idx * BOARD_SIZE;

    assert(row_idx > 0 && row_idx < (BOARD_SIZE - 1));

    Move move;
    move.fields.src_row = row_idx;
    move.fields.src_col = col_idx;

    // move 1 or two squares forward
    if (moves)
    {
        if (m_board[square_idx + BOARD_SIZE].piece() == Empty)
        {
            move.fields.dst_row = row_idx + 1;
            move.fields.dst_col = col_idx;

            if (isMoveLegal(move))
            {
                moves->push_back(move);

                if (row_idx == (BOARD_SIZE - 2))
                {
                    moves->back().fields.promoted_to = Knight;

                    move.fields.promoted_to = Bishop;
                    moves->push_back(move);

                    move.fields.promoted_to = Rook;
                    moves->push_back(move);

                    move.fields.promoted_to = Queen;
                    moves->push_back(move);

                    move.fields.promoted_to = 0;
                }
            }

            if (row_idx == 1 && m_board[square_idx + BOARD_SIZE * 2].piece() == Empty)
            {
                move.fields.dst_row = row_idx + 2;
                move.fields.dst_col = col_idx;
                if (isMoveLegal(move))
                    moves->push_back(move);
            }
        }
    }

    // left-hand side capture
    if (col_idx > 0)
    {
        const Square left_square = m_board[square_idx + BOARD_SIZE - 1];
        if ((left_square.piece() != Empty && !left_square.isWhite()) || (row_idx == (BOARD_SIZE - 4) && (left_square.state() & EnPassant)))
        {
            if (left_square.piece() == King)
            {
                assert(!moves);
                return false;
            }

            if (moves)
            {
                move.fields.dst_row = row_idx + 1;
                move.fields.dst_col = col_idx - 1;

                if (isMoveLegal(move))
                {
                    moves->push_back(move);

                    if (row_idx == (BOARD_SIZE - 2))
                    {
                        moves->back().fields.promoted_to = Knight;

                        move.fields.promoted_to = Bishop;
                        moves->push_back(move);

                        move.fields.promoted_to = Rook;
                        moves->push_back(move);

                        move.fields.promoted_to = Queen;
                        moves->push_back(move);

                        move.fields.promoted_to = 0;
                    }
                }
            }
        }
    }

    // right-hand side capture
    if (col_idx < (BOARD_SIZE - 1))
    {
        const Square right_square = m_board[square_idx + BOARD_SIZE + 1];
        if ((right_square.piece() != Empty && !right_square.isWhite()) || (row_idx == (BOARD_SIZE - 4) && (right_square.state() & EnPassant)))
        {
            if (right_square.piece() == King)
            {
                assert(!moves);
                return false;
            }

            if (moves)
            {
                move.fields.dst_row = row_idx + 1;
                move.fields.dst_col = col_idx + 1;

                if (isMoveLegal(move))
                {
                    moves->push_back(move);

                    if (row_idx == (BOARD_SIZE - 2))
                    {
                        moves->back().fields.promoted_to = Knight;

                        move.fields.promoted_to = Bishop;
                        moves->push_back(move);

                        move.fields.promoted_to = Rook;
                        moves->push_back(move);

                        move.fields.promoted_to = Queen;
                        moves->push_back(move);

                        move.fields.promoted_to = 0;
                    }
                }
            }
        }
    }

    return true;
}


bool Position::appendPossibleBlackPawnMoves(std::vector<Move>* moves, int square_idx) const
{
    const Square square = m_board[square_idx];
    assert(square.piece() == Pawn);
    assert(!square.isWhite());

    const int row_idx = square_idx / BOARD_SIZE;
    const int col_idx = square_idx - row_idx * BOARD_SIZE;

    assert(row_idx > 0 && row_idx < (BOARD_SIZE - 1));

    Move move;
    move.fields.src_row = row_idx;
    move.fields.src_col = col_idx;

    // move 1 or two fields forward
    if (moves)
    {
        if (m_board[square_idx - BOARD_SIZE].piece() == Empty)
        {
            move.fields.dst_row = row_idx - 1;
            move.fields.dst_col = col_idx;

            if (isMoveLegal(move))
            {
                moves->push_back(move);

                if (row_idx == 1)
                {
                    moves->back().fields.promoted_to = Knight;

                    move.fields.promoted_to = Bishop;
                    moves->push_back(move);

                    move.fields.promoted_to = Rook;
                    moves->push_back(move);

                    move.fields.promoted_to = Queen;
                    moves->push_back(move);

                    move.fields.promoted_to = 0;
                }
            }

            if (row_idx == (BOARD_SIZE - 2) && m_board[square_idx - BOARD_SIZE * 2].piece() == Empty)
            {
                move.fields.dst_row = row_idx - 2;
                move.fields.dst_col = col_idx;
                if (isMoveLegal(move))
                    moves->push_back(move);
            }
        }
    }

    // right-hand side capture
    if (col_idx > 0)
    {
        const Square right_square = m_board[square_idx - BOARD_SIZE - 1];
        if ((right_square.piece() != Empty && right_square.isWhite()) || (row_idx == 3 && (right_square.state() & EnPassant)))
        {
            if (right_square.piece() == King)
            {
                assert(!moves);
                return false;
            }

            if (moves)
            {
                move.fields.dst_row = row_idx - 1;
                move.fields.dst_col = col_idx - 1;

                if (isMoveLegal(move))
                {
                    moves->push_back(move);

                    if (row_idx == 1)
                    {
                        moves->back().fields.promoted_to = Knight;

                        move.fields.promoted_to = Bishop;
                        moves->push_back(move);

                        move.fields.promoted_to = Rook;
                        moves->push_back(move);

                        move.fields.promoted_to = Queen;
                        moves->push_back(move);

                        move.fields.promoted_to = 0;
                    }
                }
            }
        }
    }

    // left-hand side capture
    if (col_idx < (BOARD_SIZE - 1))
    {
        const Square left_square = m_board[square_idx - BOARD_SIZE + 1];
        if ((left_square.piece() != Empty && left_square.isWhite()) || (row_idx == 3 && (left_square.state() & EnPassant)))
        {
            if (left_square.piece() == King)
            {
                assert(!moves);
                return false;
            }

            if (moves)
            {
                move.fields.dst_row = row_idx - 1;
                move.fields.dst_col = col_idx + 1;

                if (isMoveLegal(move))
                {
                    moves->push_back(move);

                    if (row_idx == 1)
                    {
                        moves->back().fields.promoted_to = Knight;

                        move.fields.promoted_to = Bishop;
                        moves->push_back(move);

                        move.fields.promoted_to = Rook;
                        moves->push_back(move);

                        move.fields.promoted_to = Queen;
                        moves->push_back(move);

                        move.fields.promoted_to = 0;
                    }
                }
            }
        }
    }

    return true;
}


bool Position::appendPossibleKnightMoves(std::vector<Move>* moves, int square_idx) const
{
    const Square square = m_board[square_idx];
    assert(square.piece() == Knight);

    const int row_idx = square_idx / BOARD_SIZE;
    const int col_idx = square_idx - row_idx * BOARD_SIZE;

    static const int knight_move_deltas[2 * 8] =
    {
        -2, -1,
        -2,  1,
        -1, -2,
        -1,  2,
         1, -2,
         1,  2,
         2, -1,
         2,  1
    };

    Move move;
    move.fields.src_row = row_idx;
    move.fields.src_col = col_idx;

    for (int pos_move_idx = 0; pos_move_idx < 8; ++pos_move_idx)
    {
        const int new_row_idx = row_idx + knight_move_deltas[pos_move_idx * 2];
        if (new_row_idx >= 0 && new_row_idx <= (BOARD_SIZE - 1))
        {
            const int new_col_idx = col_idx + knight_move_deltas[pos_move_idx * 2 + 1];
            if (new_col_idx >= 0 && new_col_idx <= (BOARD_SIZE - 1))
            {
                // ok, we're inside the board, check if the square's empty or contains an enemy piece
                const Square new_square = m_board[new_row_idx * BOARD_SIZE + new_col_idx];
                if (new_square.piece() == Empty || new_square.isWhite() != square.isWhite())
                {
                    if (new_square.piece() == King)
                    {
                        assert(!moves);
                        return false;
                    }

                    if (moves)
                    {
                        move.fields.dst_row = new_row_idx;
                        move.fields.dst_col = new_col_idx;
                        if (isMoveLegal(move))
                            moves->push_back(move);
                    }
                }
            }
        }
    }

    return true;
}


bool Position::appendPossibleBishopMoves(std::vector<Move>* moves, int square_idx) const
{
    const Square square = m_board[square_idx];
    assert(square.piece() == Bishop || square.piece() == Queen);

    const int row_idx = square_idx / BOARD_SIZE;
    const int col_idx = square_idx - row_idx * BOARD_SIZE;

    Move move;
    move.fields.src_row = row_idx;
    move.fields.src_col = col_idx;

    int new_row_idx = row_idx - 1;
    int new_col_idx = col_idx - 1;
    while (new_row_idx >= 0 && new_col_idx >= 0)
    {
        const Square new_square = m_board[new_row_idx * BOARD_SIZE + new_col_idx];
        if (new_square.piece() == Empty || new_square.isWhite() != square.isWhite())
        {
            if (new_square.piece() == King)
            {
                assert(!moves);
                return false;
            }

            if (moves)
            {
                move.fields.dst_row = new_row_idx;
                move.fields.dst_col = new_col_idx;
                if (isMoveLegal(move))
                    moves->push_back(move);
            }
        }

        if (new_square.piece() != Empty)
            break;

        --new_row_idx;
        --new_col_idx;
    }

    new_row_idx = row_idx - 1;
    new_col_idx = col_idx + 1;
    while (new_row_idx >= 0 && new_col_idx <= (BOARD_SIZE - 1))
    {
        const Square new_square = m_board[new_row_idx * BOARD_SIZE + new_col_idx];
        if (new_square.piece() == Empty || new_square.isWhite() != square.isWhite())
        {
            if (new_square.piece() == King)
            {
                assert(!moves);
                return false;
            }

            if (moves)
            {
                move.fields.dst_row = new_row_idx;
                move.fields.dst_col = new_col_idx;
                if (isMoveLegal(move))
                    moves->push_back(move);
            }
        }

        if (new_square.piece() != Empty)
            break;

        --new_row_idx;
        ++new_col_idx;
    }

    new_row_idx = row_idx + 1;
    new_col_idx = col_idx - 1;
    while (new_row_idx <= (BOARD_SIZE - 1) && new_col_idx >= 0)
    {
        const Square new_square = m_board[new_row_idx * BOARD_SIZE + new_col_idx];
        if (new_square.piece() == Empty || new_square.isWhite() != square.isWhite())
        {
            if (new_square.piece() == King)
            {
                assert(!moves);
                return false;
            }

            if (moves)
            {
                move.fields.dst_row = new_row_idx;
                move.fields.dst_col = new_col_idx;
                if (isMoveLegal(move))
                    moves->push_back(move);
            }
        }

        if (new_square.piece() != Empty)
            break;

        ++new_row_idx;
        --new_col_idx;
    }

    new_row_idx = row_idx + 1;
    new_col_idx = col_idx + 1;
    while (new_row_idx <= (BOARD_SIZE - 1) && new_col_idx <= (BOARD_SIZE - 1))
    {
        const Square new_square = m_board[new_row_idx * BOARD_SIZE + new_col_idx];
        if (new_square.piece() == Empty || new_square.isWhite() != square.isWhite())
        {
            if (new_square.piece() == King)
            {
                assert(!moves);
                return false;
            }

            if (moves)
            {
                move.fields.dst_row = new_row_idx;
                move.fields.dst_col = new_col_idx;
                if (isMoveLegal(move))
                    moves->push_back(move);
            }
        }

        if (new_square.piece() != Empty)
            break;

        ++new_row_idx;
        ++new_col_idx;
    }

    return true;
}


bool Position::appendPossibleRookMoves(std::vector<Move>* moves, int square_idx) const
{
    const Square square = m_board[square_idx];
    assert(square.piece() == Rook || square.piece() == Queen);

    const int row_idx = square_idx / BOARD_SIZE;
    const int col_idx = square_idx - row_idx * BOARD_SIZE;

    Move move;
    move.fields.src_row = row_idx;
    move.fields.src_col = col_idx;

    int new_row_idx = row_idx - 1;
    int new_col_idx = col_idx;
    while (new_row_idx >= 0)
    {
        const Square new_square = m_board[new_row_idx * BOARD_SIZE + new_col_idx];
        if (new_square.piece() == Empty || new_square.isWhite() != square.isWhite())
        {
            if (new_square.piece() == King)
            {
                assert(!moves);
                return false;
            }

            if (moves)
            {
                move.fields.dst_row = new_row_idx;
                move.fields.dst_col = new_col_idx;
                if (isMoveLegal(move))
                    moves->push_back(move);
            }
        }

        if (new_square.piece() != Empty)
            break;

        --new_row_idx;
    }

    new_row_idx = row_idx;
    new_col_idx = col_idx - 1;
    while (new_col_idx >= 0)
    {
        const Square new_square = m_board[new_row_idx * BOARD_SIZE + new_col_idx];
        if (new_square.piece() == Empty || new_square.isWhite() != square.isWhite())
        {
            if (new_square.piece() == King)
            {
                assert(!moves);
                return false;
            }

            if (moves)
            {
                move.fields.dst_row = new_row_idx;
                move.fields.dst_col = new_col_idx;
                if (isMoveLegal(move))
                    moves->push_back(move);
            }
        }

        if (new_square.piece() != Empty)
            break;

        --new_col_idx;
    }

    new_col_idx = col_idx + 1;
    while (new_col_idx <= (BOARD_SIZE - 1))
    {
        const Square new_square = m_board[new_row_idx * BOARD_SIZE + new_col_idx];
        if (new_square.piece() == Empty || new_square.isWhite() != square.isWhite())
        {
            if (new_square.piece() == King)
            {
                assert(!moves);
                return false;
            }

            if (moves)
            {
                move.fields.dst_row = new_row_idx;
                move.fields.dst_col = new_col_idx;
                if (isMoveLegal(move))
                    moves->push_back(move);
            }
        }

        if (new_square.piece() != Empty)
            break;

        ++new_col_idx;
    }

    new_row_idx = row_idx + 1;
    new_col_idx = col_idx;
    while (new_row_idx <= (BOARD_SIZE - 1))
    {
        const Square new_square = m_board[new_row_idx * BOARD_SIZE + new_col_idx];
        if (new_square.piece() == Empty || new_square.isWhite() != square.isWhite())
        {
            if (new_square.piece() == King)
            {
                assert(!moves);
                return false;
            }

            if (moves)
            {
                move.fields.dst_row = new_row_idx;
                move.fields.dst_col = new_col_idx;
                if (isMoveLegal(move))
                    moves->push_back(move);
            }
        }

        if (new_square.piece() != Empty)
            break;

        ++new_row_idx;
    }

    return true;
}


bool Position::appendPossibleQueenMoves(std::vector<Move>* moves, int square_idx) const
{
    if (!appendPossibleBishopMoves(moves, square_idx))
        return false;
    if (!appendPossibleRookMoves(moves, square_idx))
        return false;

    return true;
}


bool Position::appendPossibleKingMoves(std::vector<Move>* moves, int square_idx) const
{
    const Square square = m_board[square_idx];
    assert(square.piece() == King);

    const int row_idx = square_idx / BOARD_SIZE;
    const int col_idx = square_idx - row_idx * BOARD_SIZE;

    Move move;
    move.fields.src_row = row_idx;
    move.fields.src_col = col_idx;

    for (int new_row_idx = (row_idx - 1); new_row_idx <= (row_idx + 1); ++new_row_idx)
    {
        if (new_row_idx >= 0 && new_row_idx <= (BOARD_SIZE - 1))
        {
            for (int new_col_idx = (col_idx - 1); new_col_idx <= (col_idx + 1); ++new_col_idx)
            {
                if (new_col_idx >= 0 && new_col_idx <= (BOARD_SIZE - 1) &&
                   (new_row_idx != row_idx || new_col_idx != col_idx))
                {
                    // ok, we're inside the board and it's not the original location, check if the square's empty or contains an enemy piece
                    const Square new_square = m_board[new_row_idx * BOARD_SIZE + new_col_idx];
                    if (new_square.piece() == Empty || new_square.isWhite() != square.isWhite())
                    {
                        if (new_square.piece() == King)
                        {
                            assert(!moves);
                            return false;
                        }

                        if (moves)
                        {
                            move.fields.dst_row = new_row_idx;
                            move.fields.dst_col = new_col_idx;
                            if (isMoveLegal(move))
                                moves->push_back(move);
                        }
                    }
                }
            }
        }
    }

    return true;
}


bool Position::appendPossibleCastlings(std::vector<Move>* moves, int square_idx) const
{
    assert(moves);  // should never be used for checking

    const Square square = m_board[square_idx];
    assert(square.piece() == King);

    const int row_idx = square_idx / BOARD_SIZE;
    const int col_idx = square_idx - row_idx * BOARD_SIZE;

    Move move;
    move.fields.src_row = row_idx;
    move.fields.src_col = col_idx;

    if (square.state() & CanCastle)
    {
        assert(col_idx == 4);
        if (square.isWhite())
            assert(row_idx == 0);
        else
            assert(row_idx == (BOARD_SIZE - 1));

        if (m_board[square_idx + 1].piece() == Empty && m_board[square_idx + 2].piece() == Empty && (m_board[square_idx + 3].state() & CanCastle))
        {
            // short castling
            assert(m_board[square_idx + 3].isWhite() == square.isWhite());
            assert(m_board[square_idx + 3].piece() == Rook);

            if (isMoveLegal(Move(row_idx, col_idx, row_idx, col_idx)) &&
                isMoveLegal(Move(row_idx, col_idx, row_idx, col_idx + 1)) &&
                isMoveLegal(Move(row_idx, col_idx, row_idx, col_idx + 2)))
            {
                move.fields.dst_row = row_idx;
                move.fields.dst_col = col_idx + 2;
                move.fields.rook_src_col = col_idx + 3;
                move.fields.rook_dst_col = col_idx + 1;

                moves->push_back(move);
            }
        }

        if (m_board[square_idx - 1].piece() == Empty && m_board[square_idx - 2].piece() == Empty && m_board[square_idx - 3].piece() == Empty && (m_board[square_idx - 4].state() & CanCastle))
        {
            // long castling
            assert(m_board[square_idx - 4].isWhite() == square.isWhite());
            assert(m_board[square_idx - 4].piece() == Rook);

            if (isMoveLegal(Move(row_idx, col_idx, row_idx, col_idx)) &&
                isMoveLegal(Move(row_idx, col_idx, row_idx, col_idx - 1)) &&
                isMoveLegal(Move(row_idx, col_idx, row_idx, col_idx - 2)))
            {
                move.fields.dst_row = row_idx;
                move.fields.dst_col = col_idx - 2;
                move.fields.rook_src_col = col_idx - 4;
                move.fields.rook_dst_col = col_idx - 1;

                moves->push_back(move);
            }
        }
    }

    return true;
}
