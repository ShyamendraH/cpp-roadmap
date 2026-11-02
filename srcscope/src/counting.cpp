#include "counting.hpp"

int count_lines(const std::string& text) {
    int lines = 0;

    // Every '\n' completes one line.
    for (char c : text) {
        if (c == '\n') {
            ++lines;
        }
    }

    // A final line without a trailing newline still counts.
    if (!text.empty() && text.back() != '\n') {
        ++lines;
    }

    return lines;
}
