#include <cassert>

#include "position.h"
#include "possible_moves_tests.h"
#include "performance_tests.h"


int main(int argc, char *argv[])
{
    //runPossibleMovesTests(true);

    //runEvaluationPerformanceTests();
    //runPossibleMovesPerformanceTests();

    runFindBestMoveTests();

    return 0;
}
