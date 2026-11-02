# Smoke test: the CLI must build and exit cleanly.
add_test(NAME srcscope_runs COMMAND srcscope)

# Unit tests for the shared logic (srcscope_core).
add_executable(srcscope_tests
    counting_test.cpp)

# The checks call count_lines(); linking the library also delivers its
# public include path, so counting_test.cpp can #include "counting.hpp".
target_link_libraries(srcscope_tests PRIVATE srcscope_core)

add_test(NAME srcscope_counting COMMAND srcscope_tests)
