#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "fatpup/engine.h"
#include "uci_fp_extensions.h"

namespace
{

std::vector<std::string> split(const std::string& text)
{
    std::istringstream stream(text);
    std::vector<std::string> tokens;

    std::string token;
    while (stream >> token)
        tokens.push_back(token);

    return tokens;
}

bool setPosition(const std::vector<std::string>& tokens, fatpup::Position* pos)
{
    if (tokens.size() < 2)
        return false;

    size_t nextToken = 1;
    if (tokens[nextToken] == "startpos")
    {
        pos->setInitial();
        ++nextToken;
    }
    else if (tokens[nextToken] == "fen")
    {
        ++nextToken;
        std::string fen;

        while (nextToken < tokens.size() && tokens[nextToken] != "moves")
        {
            if (!fen.empty())
                fen += " ";
            fen += tokens[nextToken++];
        }

        if (fen.empty())
            return false;

        if (!pos->setFEN(fen))
            return false;
    }
    else
    {
        return false;
    }

    if (nextToken == tokens.size())
        return true;

    if (tokens[nextToken] != "moves")
        return false;

    ++nextToken;
    while (nextToken < tokens.size())
    {
        fatpup::Move move;
        if (!fatpup::uci_fp::parseUciMove(*pos, tokens[nextToken++], &move))
            return false;
        *pos += move;
    }

    return true;
}

}   // namespace

int main()
{
    std::unique_ptr<fatpup::Engine> engine(fatpup::Engine::Create("minimax"));
    if (!engine)
        return 1;

    fatpup::Position pos;
    pos.setInitial();
    engine->SetPosition(pos);
    std::vector<fatpup::uci_fp::RollbackState> history;
    std::string lastBestMove;

    std::string line;
    while (std::getline(std::cin, line))
    {
        const auto tokens = split(line);
        if (tokens.empty())
            continue;

        const auto& cmd = tokens[0];
        if (cmd == "uci")
        {
            std::cout << "id name fatpup minimax\n";
            std::cout << "id author fatpup\n";
            std::cout << "uciok\n";
        }
        else if (cmd == "isready")
        {
            std::cout << "readyok\n";
        }
        else if (cmd == "ucinewgame")
        {
            pos.setInitial();
            engine->SetPosition(pos);
            history.clear();
            lastBestMove.clear();
        }
        else if (cmd == "position")
        {
            if (setPosition(tokens, &pos))
            {
                engine->SetPosition(pos);
                history.clear();
                lastBestMove.clear();
            }
        }
        else if (cmd == "go")
        {
            lastBestMove = fatpup::uci_fp::emitBestMove(&pos, engine.get(), std::cout);
        }
        else if (fatpup::uci_fp::handleCommand(tokens, &pos, engine.get(), &history, &lastBestMove, std::cout))
        {
            // extension handled
        }
        else if (cmd == "stop")
        {
            // no asynchronous search at the moment
        }
        else if (cmd == "quit")
        {
            break;
        }
        else
        {
            std::cout << "info string unknown command\n";
        }
    }

    return 0;
}
