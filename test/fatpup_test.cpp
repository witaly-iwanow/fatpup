#include <iostream>

#include "fatpup/position.h"
#include "possible_moves_tests.h"
#include "performance_tests.h"
#include "fen_tests.h"

int main(int argc, char *argv[])
{
    runPossibleMovesTests(false);

    //runEvaluationPerformanceTests();
    //runEvaluationPerformanceTests();

    //runFindBestMoveTests();

    runFenTests();

    return 0;
}
