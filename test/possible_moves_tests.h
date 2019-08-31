#ifndef FATPUP_POSSIBLE_MOVES_TESTS_H
#define FATPUP_POSSIBLE_MOVES_TESTS_H

namespace fatpup
{
    bool runPossibleMovesTests(bool verbose = false);

    bool runPossiblePawnMovesTests(bool verbose = false);
    bool runPossibleKnightMovesTests(bool verbose = false);
    bool runPossibleBishopMovesTests(bool verbose = false);
    bool runPossibleRookMovesTests(bool verbose = false);
    bool runPossibleQueenMovesTests(bool verbose = false);
    bool runPossibleKingMovesTests(bool verbose = false);
    bool runPossibleCastlingMovesTests(bool verbose = false);

    bool runCheckTests(bool verbose = false);
    bool runCheckmateTests(bool verbose = false);
    bool runStalemateTests(bool verbose = false);
}   // namespace fatpup

#endif  // FATPUP_POSSIBLE_MOVES_TESTS_H
