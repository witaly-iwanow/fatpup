#if !defined(BUILD_TESTS)
    #error Wrong build configuration: BUILD_TESTS not defined, but the file is included in build
#endif

#include <iostream>

#include "minimax.h"

namespace fatpup
{

int MinimaxEngine::RunTests()
{
    Position pos;
    if (!pos.setFEN("1r4k1/5Npp/4Q3/8/8/8/6K1/8 w - -"))
    {
        std::cerr << "Pos #1 load failed\n";
        return -1;
    }

    MinimaxEngine engine;
    engine.SetPosition(pos);
    int eval = 0;
    auto bestMove = FindBestMove(_pos, eval, 1, 3);
    if (bestMove != Move("f7d8"))
    {
        std::cerr << "Minimax test 1 failed\n";
        return -1;
    }

    return 0;
}

}   // namespace fatpup
