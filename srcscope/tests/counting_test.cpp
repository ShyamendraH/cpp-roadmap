// Unit tests for count_lines() (srcscope/src/counting.hpp).
//
// This is a plain C++ program, no test framework: one check-function
// per behavior, every check reports its file/line/expression, and
// failures are counted. Exit code 0 = all green (CTest: Passed),
// nonzero = at least one failure (CTest: Failed).
//
// Why a check() function instead of <cassert>? assert() disappears in
// Release builds (NDEBUG), so a Release test would silently verify
// nothing. check() works in every build type and reports all failures
// of one run, not just the first. See guides/testing.md.

#include <cstdlib>
#include <iostream>

#include "counting.hpp"

int failures = 0;

// Reports one expectation. Pass when the condition holds; record file,
// line, and the expression's source text when it does not.
void check(bool condition, const char* expression, const char* file, int line) {
    if (condition) {
        std::cout << "  ok       " << expression << '\n';
    } else {
        std::cout << "  FAILED   " << file << ':' << line << ":  "
                  << expression << '\n';
        ++failures;
    }
}

// CHECK(count_lines("x") == 1)
//  → check(count_lines("x") == 1, "count_lines(\"x\") == 1", file, line)
// The '#' turns the condition into readable text for the failure line.
#define CHECK(condition) check((condition), #condition, __FILE__, __LINE__)

// One check-function per behavior — the whole organizing idea. A
// failure must announce what broke through its name alone.

void test_empty_text_has_zero_lines() {
    CHECK(count_lines("") == 0);
    CHECK(count_lines("") == 0);  // same answer on repeated calls
}

void test_single_completed_line_counts_once() {
    CHECK(count_lines("srcscope\n") == 1);
}

void test_last_line_without_newline_still_counts() {
    CHECK(count_lines("srcscope") == 1);
    CHECK(count_lines("srcscope\nsrcscope") == 2);
}

void test_every_newline_starts_a_new_line() {
    CHECK(count_lines("a\nb\nc\n") == 3);
}

void test_blank_lines_count_too() {
    CHECK(count_lines("a\n\nb\n") == 3);  // "a", empty line, "b"
    CHECK(count_lines("\n") == 1);        // exactly one empty, ended line
}

int main() {
    test_empty_text_has_zero_lines();
    test_single_completed_line_counts_once();
    test_last_line_without_newline_still_counts();
    test_every_newline_starts_a_new_line();
    test_blank_lines_count_too();

    if (failures > 0) {
        std::cout << failures << " check(s) failed\n";
        return 1;  // CTest reads this as Failed
    }
    std::cout << "all checks passed\n";
    return 0;  // CTest reads this as Passed
}
