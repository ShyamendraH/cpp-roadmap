#ifndef SRCSCOPE_COUNTING_HPP
#define SRCSCOPE_COUNTING_HPP

#include <string>

// Counts lines in text: a line is what ends in '\n'; a final line
// without a trailing newline still counts as one line.
// An empty text has zero lines.
//
// This is the testable seed of Milestone 02 (File Counter) and the
// worked example of guides/testing.md — every check in
// srcscope/tests/counting_test.cpp pins one rule listed here.
int count_lines(const std::string& text);

#endif  // SRCSCOPE_COUNTING_HPP
