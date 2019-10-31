#include <iostream>

#include "fatpup/engine.h"

#include "minimax.h"

namespace fatpup
{

Engine* Engine::Create(const std::string& engineName)
{
    if (engineName == "minimax")
        return new MinimaxEngine();

    std::cerr << "Unknown engine (" << engineName << ")\n";

    return nullptr;
}

}   // namespace fatpup
