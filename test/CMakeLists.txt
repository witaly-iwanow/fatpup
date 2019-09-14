set(FATPUP_CLI_HEADERS capture_solver.h checkmate_solver.h color_scheme.h fen_tests.h performance_tests.h possible_moves_tests.h rang.h solver.h utils.h)
set(FATPUP_CLI_SOURCES capture_solver.cpp checkmate_solver.cpp fen_tests.cpp performance_tests.cpp possible_moves_tests.cpp solver.cpp utils.cpp)

add_executable(fatpup-test ${FATPUP_CLI_HEADERS} ${FATPUP_CLI_SOURCES} fatpup_test.cpp)
target_link_libraries(fatpup-test fatpup)
