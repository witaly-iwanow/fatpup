#ifndef FATPUP_UCI_FP_EXTENSIONS_H
#define FATPUP_UCI_FP_EXTENSIONS_H

#include <cctype>
#include <ostream>
#include <string>
#include <vector>

#include "fatpup/engine.h"

namespace fatpup
{
namespace uci_fp
{

inline bool isSquare(const std::string& square)
{
    if (square.length() != 2)
        return false;

    const char file = (char)std::tolower((unsigned char)square[0]);
    return (file >= 'a' && file <= 'h' && square[1] >= '1' && square[1] <= '8');
}

inline bool startsWithSquare(const std::string& text)
{
    if (text.length() < 2)
        return false;

    return isSquare(text.substr(0, 2));
}

inline int promotionPiece(char symbol)
{
    switch (std::tolower((unsigned char)symbol))
    {
    case 'n': return fatpup::Knight;
    case 'b': return fatpup::Bishop;
    case 'r': return fatpup::Rook;
    case 'q': return fatpup::Queen;
    default: return fatpup::Empty;
    }
}

inline bool parseUciMove(const fatpup::Position& pos, const std::string& uciMove, fatpup::Move* move)
{
    if (uciMove.length() != 4 && uciMove.length() != 5)
        return false;

    const std::string srcSquare = uciMove.substr(0, 2);
    const std::string dstSquare = uciMove.substr(2, 2);
    if (!isSquare(srcSquare) || !isSquare(dstSquare))
        return false;

    const int srcRow = fatpup::symbolToRowIdx(srcSquare[1]);
    const int srcCol = fatpup::symbolToColumnIdx(srcSquare[0]);
    const int dstRow = fatpup::symbolToRowIdx(dstSquare[1]);
    const int dstCol = fatpup::symbolToColumnIdx(dstSquare[0]);

    const auto legalMoves = pos.possibleMoves(srcRow, srcCol, dstRow, dstCol);
    if (legalMoves.empty())
        return false;

    const int promotedTo = (uciMove.length() == 5) ? promotionPiece(uciMove[4]) : fatpup::Empty;
    if (uciMove.length() == 5 && promotedTo == fatpup::Empty)
        return false;

    for (const auto& legalMove : legalMoves)
    {
        if (legalMove.fields.promoted_to == promotedTo)
        {
            *move = legalMove;
            return true;
        }
    }

    return false;
}

inline std::string moveToUci(const fatpup::Move move)
{
    if (move.isEmpty())
        return "0000";

    std::string result;
    result.reserve(5);
    result += (char)((int)'a' + move.fields.src_col);
    result += (char)((int)'1' + move.fields.src_row);
    result += (char)((int)'a' + move.fields.dst_col);
    result += (char)((int)'1' + move.fields.dst_row);

    if (move.fields.promoted_to != fatpup::Empty)
    {
        switch (move.fields.promoted_to)
        {
        case fatpup::Knight: result += 'n'; break;
        case fatpup::Bishop: result += 'b'; break;
        case fatpup::Rook: result += 'r'; break;
        case fatpup::Queen: result += 'q'; break;
        default: break;
        }
    }

    return result;
}

inline const char* gameOverLabel(fatpup::Position::State state)
{
    if (state == fatpup::Position::State::Checkmate)
        return "checkmate";
    if (state == fatpup::Position::State::Stalemate)
        return "stalemate";
    return nullptr;
}

inline bool emitGameOverIfAny(const fatpup::Position& pos, std::ostream& out)
{
    const char* label = gameOverLabel(pos.getState());
    if (!label)
        return false;

    out << "info string gameover " << label << "\n";
    return true;
}

inline std::string emitBestMove(fatpup::Position* pos, fatpup::Engine* engine, std::ostream& out)
{
    if (emitGameOverIfAny(*pos, out))
    {
        out << "bestmove 0000\n";
        return "0000";
    }

    const fatpup::Move bestMove = engine->GetBestMove();
    if (!bestMove.isEmpty())
        *pos += bestMove;

    const std::string bestMoveUci = moveToUci(bestMove);
    out << "bestmove " << bestMoveUci << "\n";
    emitGameOverIfAny(*pos, out);
    return bestMoveUci;
}

inline char pieceToFenChar(const fatpup::Square& square)
{
    const int piece = square.piece();
    if (piece == fatpup::Empty)
        return '\0';

    char symbol = '\0';
    switch (piece)
    {
    case fatpup::Pawn: symbol = 'p'; break;
    case fatpup::Knight: symbol = 'n'; break;
    case fatpup::Bishop: symbol = 'b'; break;
    case fatpup::Rook: symbol = 'r'; break;
    case fatpup::Queen: symbol = 'q'; break;
    case fatpup::King: symbol = 'k'; break;
    default: return '\0';
    }

    if (square.isWhite())
        symbol = (char)std::toupper((unsigned char)symbol);

    return symbol;
}

inline std::string positionToFen(const fatpup::Position& pos)
{
    std::string fen;

    for (int row = fatpup::ROW8; row >= fatpup::ROW1; --row)
    {
        int emptyCount = 0;
        for (int col = fatpup::COLA; col <= fatpup::COLH; ++col)
        {
            const auto& square = pos.square(row, col);
            const char pieceSymbol = pieceToFenChar(square);

            if (pieceSymbol == '\0')
            {
                ++emptyCount;
                continue;
            }

            if (emptyCount > 0)
            {
                fen += (char)('0' + emptyCount);
                emptyCount = 0;
            }
            fen += pieceSymbol;
        }

        if (emptyCount > 0)
            fen += (char)('0' + emptyCount);

        if (row > fatpup::ROW1)
            fen += '/';
    }

    fen += pos.isWhiteTurn() ? " w " : " b ";

    std::string castling;
    const auto& e1 = pos.square(fatpup::ROW1, fatpup::COLE);
    const auto& e8 = pos.square(fatpup::ROW8, fatpup::COLE);
    if (e1.pieceWithColor() == (fatpup::King | fatpup::White))
    {
        const auto& h1 = pos.square(fatpup::ROW1, fatpup::COLH);
        const auto& a1 = pos.square(fatpup::ROW1, fatpup::COLA);
        if (h1.pieceWithColor() == (fatpup::Rook | fatpup::White) && h1.isFlagSet(fatpup::CanCastle))
            castling += 'K';
        if (a1.pieceWithColor() == (fatpup::Rook | fatpup::White) && a1.isFlagSet(fatpup::CanCastle))
            castling += 'Q';
    }
    if (e8.pieceWithColor() == (fatpup::King | fatpup::Black))
    {
        const auto& h8 = pos.square(fatpup::ROW8, fatpup::COLH);
        const auto& a8 = pos.square(fatpup::ROW8, fatpup::COLA);
        if (h8.pieceWithColor() == (fatpup::Rook | fatpup::Black) && h8.isFlagSet(fatpup::CanCastle))
            castling += 'k';
        if (a8.pieceWithColor() == (fatpup::Rook | fatpup::Black) && a8.isFlagSet(fatpup::CanCastle))
            castling += 'q';
    }
    fen += castling.empty() ? "-" : castling;
    fen += ' ';

    std::string enPassant = "-";
    for (int row = fatpup::ROW1; row <= fatpup::ROW8; ++row)
    {
        for (int col = fatpup::COLA; col <= fatpup::COLH; ++col)
        {
            const auto& square = pos.square(row, col);
            if (square.isFlagSet(fatpup::EnPassant))
            {
                enPassant.clear();
                enPassant += (char)('a' + col);
                enPassant += (char)('1' + row);
                break;
            }
        }
        if (enPassant != "-")
            break;
    }
    fen += enPassant;

    // Fatpup does not track halfmove/fullmove counters yet.
    fen += " 0 1";

    return fen;
}

struct RollbackState
{
    fatpup::Position pos;
    std::string restoredBestMove;
};

inline bool handleCommand(
    const std::vector<std::string>& tokens,
    fatpup::Position* pos,
    fatpup::Engine* engine,
    std::vector<RollbackState>* history,
    std::string* lastBestMove,
    std::ostream& out)
{
    if (tokens.empty())
        return false;

    const auto& cmd = tokens[0];
    if (cmd == "help")
    {
        out << "info string extensions:\n";
        out << "info string   help\n";
        out << "info string   game [white|black] [first_move]\n";
        out << "info string   move <uci_move> (or just <uci_move>)\n";
        out << "info string   back\n";
        out << "info string   fen\n";
        return true;
    }

    if (cmd == "fen")
    {
        out << "info string fen " << positionToFen(*pos) << "\n";
        return true;
    }

    if (cmd == "move" || startsWithSquare(cmd))
    {
        std::string moveStr;
        if (cmd == "move")
        {
            if (tokens.size() < 2)
            {
                out << "info string usage: move <uci_move>\n";
                return true;
            }
            moveStr = tokens[1];
        }
        else
            moveStr = cmd;

        fatpup::Move move;
        if (parseUciMove(*pos, moveStr, &move))
        {
            const fatpup::Position rollbackPos = *pos;
            const std::string rollbackBestMove = *lastBestMove;
            *pos += move;
            engine->MoveDone(move);
            const std::string bestMove = emitBestMove(pos, engine, out);
            history->push_back(RollbackState{rollbackPos, rollbackBestMove});
            *lastBestMove = bestMove;
        }
        else
        {
            out << "info string illegal move " << moveStr << "\n";
        }

        return true;
    }

    if (cmd == "back")
    {
        if (tokens.size() != 1)
        {
            out << "info string usage: back\n";
            return true;
        }

        if (history->empty())
        {
            out << "info string no moves to rollback\n";
            return true;
        }

        const RollbackState rollbackState = history->back();
        history->pop_back();
        const std::string rollbackMove = rollbackState.restoredBestMove.empty() ? "0000" : rollbackState.restoredBestMove;
        out << "info string rolling back to " << rollbackMove << "\n";
        *pos = rollbackState.pos;
        *lastBestMove = rollbackState.restoredBestMove;
        engine->SetPosition(*pos);
        return true;
    }

    if (cmd == "game")
    {
        std::string side = "white";
        std::string firstMove;

        if (tokens.size() >= 2)
        {
            std::string arg1 = tokens[1];
            for (char& c : arg1)
                c = (char)std::tolower((unsigned char)c);

            if (arg1 == "white" || arg1 == "black")
            {
                side = arg1;
                if (tokens.size() >= 3)
                    firstMove = tokens[2];
                if (tokens.size() > 3)
                {
                    out << "info string usage: game [white|black] [first_move]\n";
                    return true;
                }
            }
            else
            {
                firstMove = tokens[1];
                if (tokens.size() > 2)
                {
                    out << "info string usage: game [white|black] [first_move]\n";
                    return true;
                }
            }
        }

        for (char& c : side)
            c = (char)std::tolower((unsigned char)c);

        if (side != "white" && side != "black")
        {
            out << "info string usage: game [white|black] [first_move]\n";
            return true;
        }

        if (!firstMove.empty() && side != "white")
        {
            out << "info string usage: game black\n";
            return true;
        }

        pos->setInitial();
        engine->SetPosition(*pos);
        history->clear();
        lastBestMove->clear();

        if (!firstMove.empty())
        {
            fatpup::Move move;
            if (!parseUciMove(*pos, firstMove, &move))
            {
                out << "info string illegal move " << firstMove << "\n";
                return true;
            }

            const fatpup::Position rollbackPos = *pos;
            const std::string rollbackBestMove = *lastBestMove;
            *pos += move;
            engine->MoveDone(move);
            const std::string bestMove = emitBestMove(pos, engine, out);
            history->push_back(RollbackState{rollbackPos, rollbackBestMove});
            *lastBestMove = bestMove;
            return true;
        }

        if (side == "black")
            *lastBestMove = emitBestMove(pos, engine, out);

        return true;
    }

    return false;
}

}   // namespace uci_fp
}   // namespace fatpup

#endif // FATPUP_UCI_FP_EXTENSIONS_H
