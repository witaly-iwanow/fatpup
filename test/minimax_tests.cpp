#if !defined(BUILD_TESTS)
    #error Wrong build configuration: BUILD_TESTS not defined, but the file is included in build
#endif

#include <iostream>

#include "../engines/minimax.h"

#include "color_scheme.h"

bool runMinimaxTests(bool verbose)
{
    struct Test
    {
        std::string fen;
        std::string bestMove;
        int searchDepth;
    };

    static const Test tests[] =
    {
        { "1r4k1/5Npp/4Q3/8/8/8/6K1/8 w - -",                       "f7d8", 3  },
        { "8/8/2kN4/8/8/8/3r2r1/2K5 b - -",                         "d2f2", 3  },
        { "8/8/5q2/8/4N3/2r1k3/8/4K3 w - -",                        "e4c3", 3  },
        { "3n4/ppB5/1P6/8/K1k5/P7/1r6/8 b - -",                     "b2b6", 5  },
        { "8/8/pp2r3/1kprpP2/3p4/1KPP4/8/2B5 w - -",                "c3c4", 5  },
        { "k7/1pK5/1P1PP3/8/8/8/8/8 w - -",                         "c7d7", 4  },
        { "3k4/8/4K3/3P4/8/8/8/8 w - - 0 1",                        "e6d6", 9  },
        { "1k1r4/pp1b4/3q4/8/8/8/1PP2B2/2K5 b - -",                 "d6d1", 5  }
    };
    static const int numTests = sizeof(tests) / sizeof(tests[0]);

    fatpup::Position pos;
    int eval = 0;
    fatpup::Move bestMove;
    for (int t = 0; t < numTests; ++t)
    {
        if (!pos.setFEN(tests[t].fen))
        {
            std::cout << errorMsgColor << "Minimax position " << (t + 1) << " load failed" << rang::fg::reset << std::endl;
            return false;
        }
        bestMove = fatpup::MinimaxEngine::FindBestMove(pos, eval, 1, tests[t].searchDepth);
        if (bestMove != fatpup::Move(tests[t].bestMove))
        {
            std::cout << errorMsgColor << "Minimax test " << (t + 1) << " failed, expected " << pos.moveToString(tests[t].bestMove) <<
                ", got " << pos.moveToString(bestMove) << rang::fg::reset << std::endl;
            return false;
        }
        if (verbose)
            std::cout << successMsgColor << "Minimax test " << (t + 1) << "/" << numTests << " passed" << rang::fg::reset << std::endl;
    }

    std::cout << successMsgColor << "  Success, all Minimax tests passed!" << rang::fg::reset << std::endl;

    return true;
}
