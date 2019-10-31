#ifndef FATPUP_ENGINE_H
#define FATPUP_ENGINE_H

#include <string>

#include "fatpup/position.h"

namespace fatpup
{

class Engine
{
public:
    // only "minimax" is available at the moment
    static Engine* Create(const std::string& engineName);
    virtual ~Engine() {}
    virtual int SetPosition(const Position& pos) = 0;
    //virtual void Start() = 0;
    //virtual void Stop() = 0;
    virtual Move GetBestMove() = 0;
    virtual void MoveDone(Move move) = 0;
};

}   // namespace fatpup

#endif // FATPUP_ENGINE_H
