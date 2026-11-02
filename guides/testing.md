# Testing, complete — from your first test to reliable, professional tests

Read this the first time you want to *understand* a test, and before Milestone 04 (Week 4), when the roadmap asks you to make the tests real. Day 1 ([START.md](../START.md)) has you run one test without explaining it; this guide explains all of it, step by step, from zero.

"You are here": [README](../README.md) → [START](../START.md) (day one) → [build](build.md) → **testing**

**This file's job:** everything about tests — what testing is, how a C++ test actually comes into existence (compile → run → verdict), the repository's exact CMake and CTest wiring, how to write and run your first test, how to think about *what* to test, edge cases, memory and concurrency, debugging, checklists, and a troubleshooting encyclopedia. Build commands and the build directory live in [build.md](build.md); what each repo file is for lives in [tour.md](tour.md).

---

## How to read this guide (you do not read it in one sitting)

The parts build on each other, in three labeled bands:

| Label | Parts | Read it |
|:------|:------|:--------|
| **Beginner** | A–E, H, S | When you write your first test. A–E take you from "I have never heard of a unit test" to "I can write, run, break, and fix a test in this repo". H is the command reference. S is the checklist. |
| **Intermediate** | F, G, I, J, M, N | Once your first test is green and before Milestone 04's "real tests". What to test, how to organize tests as the repo grows. |
| **Advanced** | K, L, O, P, Q, R (late exercises) | Phase II onward, when a milestone deals with memory, threads, files, or CI. Also the reference you return to the first time a test misbehaves. |

Suggested learning order: **pass 1 today** — A, B, E, H (about one focused session). **Pass 2 before Milestone 04** — C, D, F, G, I. **Pass 3 as your roadmap phase requires** — the rest, plus [S](#part-s--checklists-and-quick-reference) every milestone.

Two facts to keep ready from START.md: a test is *a program the test runner launches*, and **exit code 0 = Passed, anything else = Failed**. Everything in this guide either builds on those two facts or explains how to keep them true.

---

<a id="part-a--understanding-testing-from-zero"></a>
## Part A — Understanding testing from zero *(Beginner)*

### What software testing means

Software testing means running your code in a controlled way and comparing what happens against what should happen. The "what should happen" comes from the specification — for you, the acceptance criteria in [Roadmap.md](../Roadmap.md).

That sentence contains three roles:

- The **code under test** — the function, class, or program being examined.
- The **check** — the comparison of expected against actual.
- The **verdict** — pass or fail.

Example: "when given 0 arguments, `srcscope` must still exit cleanly." Run `./build/bin/srcscope`, note its exit code, compare with 0. Done — that is a test.

### Why you write tests

Every manual action you take to verify something costs minutes and evaporates afterwards. A written test costs minutes *once* and then:

- **Runs forever.** You re-run it every build, in every future session, with zero willpower spent.
- **Documents the behavior.** A test named `test_last_line_without_newline_still_counts` is a precise, executable statement of a rule.
- **Protects you from yourself.** When Tuesday-you deletes a rule that Monday-you wrote tests for, the machine notices. You don't have to remember.

### Why running the program manually is not enough

Manual running checks one path, one input, *this* time — and silently forgets everything else. It also has a nasty timing problem: you check features while they're fresh, but bugs get *introduced* on days when the author "totally didn't touch that part". A test suite checks every recorded behavior every single time, on days nobody feels like it.

### Bug, regression, test case, expected, actual

- A **bug** (defect) is behavior that contradicts what the software should do. `count_lines("a")` returning `0` is a bug: "a" is a line of a text file.
- A **regression** is a specific kind of bug: something that *used to work*, and a later change broke. Regressions are the most expensive bugs to tolerate because users remember the working version.
- A **test case** is one behavior under one input: "with the input `"srcscope"`, `count_lines` must return 1".
- The **expected result** is what the specification says must happen (here: 1 — the spec counts a final line without a trailing newline).
- The **actual result** is what the code really produced when you ran it.
- **Pass** = expected == actual. **Fail** = otherwise.

### The regression story

Milestone 02 gives `srcscope` a file counter. Early version:

```cpp
// COUNT OF LINES = every '\n', plus a final line without '\n'
int count_lines(const std::string& text) { /* ... */ }
```

Tests pin that behavior and everything passes. A week later the implementation is refactored for speed — and someone (you, in three weeks, at 23:00) rewrote the loop and dropped the final-line-without-newline rule. Every file whose last line has no trailing newline now counts one short. Nobody notices for days: test files they hand-run all end in `\n`. When the bug appears later, two realities have changed and one stays the same: the *implementation* changed, the *spec* did not, and the test was the only thing in the repo that still remembered the original rule. It fails instantly, points at the violated behavior by name, and the bug costs minutes instead of days. That chain — change → old behavior silently broken → saved test catches it — is what "regression testing" means.

### Automated vs manual, and what "unit" means

**Automated testing** means the computer performs the check, records the verdict, and reports — every run, same way. The alternative is you, a terminal window, and your memory (unreliable).

Sizes of automated tests, smallest to largest:

| Level | Tests | In this repo, that looks like |
|:------|:------|:------------------------------|
| **Unit** | One function/class in isolation, fast, no I/O | srcscope/src/counting.hpp's `count_lines("srcscope") == 1` |
| **Integration** | A group of components working through the real interfaces | Running the built `srcscope` binary and checking it exits 0 |
| **System** | The whole program end-to-end, like a real user would | Run `srcscope` on a real file, count its bytes/lines, compare |
| **End-to-end** | The full workflow across programs and environment | Later phases: `forge` building a real project; `httpkit` serving a real request |

One clarification beginners need: "unit" doesn't mean a physically small piece of code. It means the test is about *one behavior* and needs *nothing from the outside world* (no files, no network, no clock). That's why a pure-ish function like `count_lines` — same input, same answer, no printing, no randomness — is a joy to test, and why the roadmap has you extract counting logic into such functions.

### What testing cannot guarantee

Be honest with yourself about the limits, because false confidence is worse than no test:

- A passing suite proves nothing about inputs or behaviors you didn't think to test. A suite with a hole looks exactly like a suite without one, from the outside.
- A passing suite says nothing about code paths that are *unreachable at test time* — a failure path triggered only by a full disk still goes untested.
- Tests check *correlations* ("this input produced this output on my machine today"), not absolute truth. Two passing tests can still leave opposite bugs in two features, because each test only sees its own case.

The safe mental model: tests are a *net* that catches mistakes the author already imagined. You write more tests to widen the net, and — when you meet one — a bug that slipped through teaches you what the net was missing, and you add that now.

---

<a id="part-b--how-c-testing-works"></a>
## Part B — How C++ testing works *(Beginner)*

### From source to verdict — the ten steps

This is the whole lifecycle. Bookmark it; Part E does it for real.

1. **Write production code** — the real program behavior (`srcscope/src/counting.cpp`).
2. **Write a test** — a second small C++ program whose only job is to call the production code and compare results for you (`srcscope/tests/counting_test.cpp`).
3. **CMake learns the recipe** — the *configure* step reads your recipe files (`CMakeLists.txt`) and writes the exact build commands. Nothing compiled yet.
4. **Compile** — each `.cpp` file becomes one object file (`.o`). An error here means your text doesn't parse ("syntax error", "unknown name").
5. **Link** — objects are combined into an executable. Missing pieces ("undefined reference") are discovered here, not earlier, because up to this point nobody checks that *all* functions actually exist somewhere.
6. **Run the test executable** — it's just a program: `./build/bin/srcscope_tests`.
7. **Evaluate the checks** — each check compares expected with actual, records every mismatch.
8. **Report each failure** on the spot (file, line, the failing expression).
9. **Exit code** — `0` if nothing failed, nonzero otherwise.
10. **CTest reads the exit code** and prints the verdict you see.

Where each tool stands (recap with the roles as separate nouns):

```text
   you write                  configure                    build                          test
┌────────────────────────┐ ┌────────────────────────┐ ┌────────────────────────────┐ ┌──────────────────────────┐
│ srcscope/src/*.cpp     │ │ CMakeLists.txt recipes │ │ g++ compiles each *.cpp    │ │ ctest launches each      │
│ srcscope/tests/*.cpp   │─► generate build files  │─► g++ links the pieces into ► registered test binary,     │
│ (sources + test source)│ │ → build/CTestTestfile  │ │   build/bin/<targets>      │ │ judges by exit code      │
└────────────────────────┘ └────────────────────────┘ └────────────────────────────┘ └──────────────────────────┘
```

### Who is who — six roles that beginners confuse

| Role | What it is | Where it lives here |
|:-----|:-----------|:--------------------|
| **Compiler** (`g++`/`clang++`) | Translates one source file to machine code (one `.o` each). Catches *syntax + semantics* problems file-by-file. | Runs when you `cmake --build build` |
| **Linker** (`ld`, usually run for you) | Stitches `.o` files into one runnable program; resolves "function X is defined *somewhere*" promises. Catches *missing definitions*, not syntax. | Runs at the end of `cmake --build build` |
| **Test framework** | The *pattern* your test file follows — how checks are declared, how the verdict is decided (our hand-rolled check-based suite today; a library can take the same role later; see Part F). | `srcscope/tests/counting_test.cpp` today |
| **Test executable** | One compiled program that contains checks and exits 0/nonzero. | `build/bin/srcscope_tests` |
| **CTest** | The *runner*: knows the list of tests (from CMake), launches each, judges by exit code, prints the summary. The tool that says Passed/Failed. | `ctest --test-dir build ...` |
| **CMake** | The *recipe system*: configures the project (compiler flags, binaries, test registrations into `build/CTestTestfile.cmake`). | root + per-folder `CMakeLists.txt` |

The relationship beginners trip over, said plainly:

- CMake **writes** instructions. It never compiles or runs tests itself.
- The compiler/linker (via `cmake --build`) **produces** the programs.
- Test executables **run themselves** when launched.
- CTest **launches and judges**. It is a separate program from the compiler, the linker, and your test binary. It reads only two things: which tests exist (the registry `build/CTestTestfile.cmake`, written during configure) and what exit code each got.
- So `cmake -S . -B build` (configure) refreshes ctest's knowledge of tests, and `cmake --build build` refreshes ctest's knowledge of *whether the tests are even up to date*. A green ctest can still be green on **stale binaries** — the one green that lies (real case study in Part E, step 8, and Part O #17).

```text
        writes the recipe                      builds per the recipe
you ─────────────────────────► CMake ─────────────────────────► compiler + linker ──► build/bin/srcscope_tests
                        └─ add_test(...) ──►  build/CTestTestfile.cmake ─┐
                                                                         │ reads names + commands
ctest ──────────────────────────────────────────────────────────────────-┘ → runs each, judges the exit code
```

*The block above shows the roles; the actual artifacts in `build/` are listed in [build.md §3](build.md).*

### Compile problems vs test problems — telling the two apart

These failures look similar (both are red text) but come from *completely different tools* and mean different fixes:

| What you see | Whose complaint | Means | Fix at |
|:-------------|:----------------|:------|:-------|
| `error: expected ';'` · `error: 'string' does not name a type` · `fatal error: file.hpp: No such file` | **Compiler** (during build) | Your text doesn't parse / a name is unknown / a header can't be found | Source or include path ([Part G](#part-g--cmake-integration-in-detail), Part O) |
| `undefined reference to 'fn()'` · `cannot find -llib` | **Linker** (during build) | Something is *promised* but *not built or not passed in* | Target list or `target_link_libraries` |
| `Failed` lines in ctest summary | **The test (run)** | A *checked expectation* was violated while the program ran — that's the mechanism working | Production code, the test's expected value, or the environment |

*Nearly all compile/link complaints show up in `cmake --build` output. A suspiciously fast "all green" usually means nothing compiled — the build never got there. Rule: fix the build first (Part H), then tests, never both at once.*

---

<a id="part-c--preparing-the-environment"></a>
## Part C — Preparing the environment *(Beginner)*

You need four tools before anything in this guide runs. None are installed by this repo; check what you have first, install only what's missing.

### 1. What you need, and why

| Tool | Why testing needs it | Minimum here |
|:-----|:---------------------|:-------------|
| **CMake** | Reads the repo's `CMakeLists.txt` recipes, generates the real build commands, writes the test registry for CTest | 3.20+ |
| **GCC (`g++`)** or **Clang (`clang++`)** | Compiles every `.cpp` — both production code and tests — to object files, then links them | GCC 11+ or Clang 14+ (C++20 support) |
| **Make** (or **Ninja**) | Executes the generated compile/link commands quickly, incrementally | any recent version |
| **Git** | The undo safety net for every experiment this guide asks you to try | any recent version |

Ninja is optional. This repo's documented flow uses whatever generator CMake picks by default on Linux — **Unix Makefiles** (when Make is installed). Ninja gives slightly faster incremental builds; Part H shows how to opt into it with `-G Ninja`.

### 2. Install (Debian/Ubuntu, Fedora, Arch)

```bash
# Debian / Ubuntu — what each package is:
sudo apt update && sudo apt install -y cmake build-essential git
#   cmake            → the recipe system + leaves ctest behind (same package ships the runner)
#   build-essential  → a metapackage: g++ (compiler), make (executor), libc dev headers (system library interfaces)
#   git              → version control / the undo system
sudo apt install -y ninja-build        # optional: the fast alternative executor
```

```bash
# Fedora — the compiler and make come via gcc-c++ + make (no metapackage)
sudo dnf install -y cmake gcc-c++ make git
sudo dnf install -y ninja-build       # optional
```

```bash
# Arch (as in START.md)
sudo pacman -S --needed cmake gcc make git ninja
```

macOS note (from START.md): Phase I works via `xcode-select --install` and `brew install cmake`, but this repo is Linux/POSIX-first — expect later milestones to assume Linux behavior.

### 3. Verify each one, one command at a time

Run each check; each prints one line. The *digits* don't matter — what matters is that the command exists and the version satisfies the minimum.

```bash
cmake --version
# cmake version 4.4.1            ← yours could say 3.28.x; 3.20+ is what counts
g++ --version
# g++ (GCC) 16.1.1               ← need 11+ for full C++20
clang++ --version                 # optional second compiler
make --version | head -1
# GNU Make 4.4.1
git --version
# git version 2.55.0
ninja --version                   # optional
# 1.13.2
```

A stricter check: does the compiler really accept C++20? Have it compile a one-liner:

```bash
echo 'int main() {}' > /tmp/ok.cpp && g++ -std=c++20 -Wall -Wextra -Wpedantic /tmp/ok.cpp -o /tmp/ok
# prints nothing, exit code 0 → the exact flag set this repo uses works
./tmp/ok 2>/dev/null || /tmp/ok    # either way: no output, exit 0 → done
```

### 4. Common installation failures, decoded

| Symptom | Meaning | Action |
|:--------|:--------|:-------|
| `cmake: command not found` (after install) | The binary exists but the shell PATH doesn't include it, or the install silently failed | Re-run install; then `which cmake`. Still missing → open a new terminal (PATH refreshes per session) |
| `CMake 3.20 or higher is required` | The distro repo shipped an ancient CMake | Upgrade: Debian/Ubuntu may need `cmake` from cmake.org's .deb or apt from backports; Fedora usually recent; the fix is tooling, **not** editing the repo's `cmake_minimum_required` line (that line is a compatibility contract, not a suggestion — [tour.md §3](tour.md)) |
| `g++: command not found` but `gcc` works | C++ builder missing — `gcc` alone doesn't ship the C++ standard library glue | Debian/Ubuntu: `build-essential`; Fedora: `gcc-c++`; Arch: `gcc` (includes it) |
| `clang++: command not found` | Clang just isn't installed | Only install it if you want the second compiler; GCC alone satisfies the repo |
| `sudo: password prompt appears` | Normal | Type your password; the packages are installed system-wide |

❑ This corresponds to [checklist.md](../checklist.md) §1 "CMake 3.20+ and a C++20 compiler installed" — tick it with the version numbers you saw.

---

<a id="part-d--know-the-project-before-testing-it"></a>
## Part D — Know the project before testing it *(Beginner)*

The project this guide teaches against is `srcscope/` — the Phase I component. Its current job is tiny by design: print two informative lines and exit 0. You will now see exactly which files exist, which you write next, and why the shape is what it is.

### 1. What actually exists today

```text
Cpp-practice/
├── CMakeLists.txt                      ← master recipe: C++20, warnings, Debug, sanitizers switch, include(CTest), add_subdirectory(srcscope)
└── srcscope/
    ├── CMakeLists.txt                  ← component recipe: the logic library, the CLI, the tests guard
    ├── src/
    │   ├── main.cpp                    ← the CLI program (prints 2 lines, returns 0)
    │   ├── counting.hpp                ← public interface of the testable logic: count_lines(...)
    │   └── counting.cpp                ← its implementation
    └── tests/
        ├── CMakeLists.txt              ← registers: srcscope_runs, srcscope_counting
        └── counting_test.cpp           ← 9 checks across 5 behaviors (Part E walks through it)
```

Everything above is already in your checkout. `build/` appears when you configure and is always disposable (gitignored, regenerable — [build.md §3](build.md)).

Why these files? A quick tour; the line-by-line version is [tour.md](tour.md):

- `main.cpp` has the **uses** of the program: talk to a human. It contains no logic you will ever want to assert.
- `counting.hpp` / `counting.cpp` hold the program's **computations** ("print at the edges, compute in testable functions"). `count_lines` is the testable seed of Milestone 02's File Counter and the worked example of this guide.
- `tests/` is where the *expectations* about that computation live — a separate program that links the logic but never contains the program.

### 2. Why production code and test code are separated

Three reasons, all practical:

1. **Different goals.** Production code is optimized for real usage; test code is optimized for *clarity about behavior*. Mixing them makes both worse.
2. **Different shipping.** You never ship test binaries to users, but you must compile them in every development session — the `if(BUILD_TESTING)` guard flips tests off cleanly when packaging.
3. **Different failure modes.** A production bug stops the CTest suit; a *test* bug (wrong expectation) must be traceable to the test file — impossible if they're tangled together.

The concrete shape in this repo does one more thing worth copying: testable logic lives in a **library target** (`srcscope_core`) rather than one giant executable. The CLI (`srcscope`) links the library and keeps `main()` thin; the tests (`srcscope_tests`) link the *same* library. One source of truth, three consumers.

### 3. The contract you will test

A test is only as good as the rule it checks, so read `count_lines`'s contract in `srcscope/src/counting.hpp` first. Verbatim:

- every `'\n'` completes one line;
- a final line *without* a trailing newline still counts as one line;
- empty text ⇒ zero lines.

Rules → checks: each rule in the contract gets its own test function. That mapping is the core habit of all unit testing: **spec → tests**, in that order.

### 4. The build/run/test loop for this project

All commands from the repo root (the folder containing this README — the repo is small, but `-S . -B build` interprets relative paths from *where you stand*):

```bash
cmake -S . -B build         # configure: recipes → build files + test registry
cmake --build build         # build: compiler + linker make all targets (see the [100%] lines)
ctest --test-dir build --output-on-failure   # test: run every registered test
./build/bin/srcscope        # run: launch the CLI the way any user would
./build/bin/srcscope_tests  # run (optional): launch the unit-test binary directly
```

Expect — full real output, captured from a clean configure on the reference machine (yours will differ only in paths and version digits):

```text
cmake -S . -B build  →
-- The CXX compiler identification is GNU 16.1.1
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features - done
-- Configuring done (0.2s)
-- Generating done (0.0s)
-- Build files have been written to: /home/.../Cpp-practice/build
```

```text
cmake --build build  →
[ 16%] Building CXX object srcscope/CMakeFiles/srcscope_core.dir/src/counting.cpp.o
[ 33%] Linking CXX static library libsrcscope_core.a
[ 33%] Built target srcscope_core
[ 50%] Building CXX object srcscope/CMakeFiles/srcscope.dir/src/main.cpp.o
[ 66%] Linking CXX executable ../bin/srcscope
[ 66%] Built target srcscope
[ 83%] Building CXX object srcscope/tests/CMakeFiles/srcscope_tests.dir/counting_test.cpp.o
[100%] Linking CXX executable ../../bin/srcscope_tests
[100%] Built target srcscope_tests
```

```text
ctest --test-dir build --output-on-failure  →
Test project /home/.../Cpp-practice/build
    Start 1: srcscope_runs
1/2 Test #1: srcscope_runs ....................   Passed    0.00 sec
    Start 2: srcscope_counting
2/2 Test #2: srcscope_counting ................   Passed    0.00 sec

100% tests passed out of 2

Total Test time (real) =   0.00 sec
```

Read that last block slowly once. Line by line:

- `Test project /home/.../build` — the directory ctest is reading its registry from. If this is a path you don't expect, everything after it is about the wrong project (classic "I typed the command from the wrong place" tell).
- `1/2 Test #1: ... Passed 0.00 sec` — test #1 of a total 2, named by its registry entry, verdict, wall time. CTest renames nothing.
- `100% tests passed out of 2` — the aggregate. This line is what a milestone's "tests pass" claim refers to.
- `Total Test time` — total wall time of the whole run.

---

<a id="part-e--your-first-test-from-absolute-zero"></a>
## Part E — Your first test, from absolute zero *(Beginner — the most important part)*

First, a paragraph on what you're about to build and why it's shaped like that.

**The code under test.** The repo needs a tiny real function with a clear contract, so that the *testing* story stays about testing. (A toy like an `add(a, b)` serves teaching but adds nothing to the roadmap — this one is Milestone 02's seed instead, so everything you create here is used later.) `count_lines(text)` takes a string and returns how many lines it contains, per the three rules in [Part D §3](#part-d--know-the-project-before-testing-it). It's a **pure function**: same input in → same answer out; no files, no clock, no printing. Pure functions are the best first test subjects, because nothing can go wrong *between* the check and the answer of the function itself.

**The test file.** A second small program. No test framework — the repo deliberately starts you with the bare mechanism (one compiled program whose checks decide its exit code; a framework is a later chapter, and honestly not much magic on top of this; see Part F). You will:

```text
5. write a header          →  declaration: what the function promises
6. write an implementation →  definition: how it keeps the promise
7. teach CMake the pieces  →  a library + a test executable + registrations
8. run it                  →  compile, link, execute, verdict
9. break it, fix it        →  experience a fail, then green again
```

*These files already exist in your checkout (see Part D §1). Learn by rebuilding them blind: open yours side by side and type each file from the steps below, or use `git diff` to compare after you've re-created them. If you prefer to delete-and-recreate as you go: `rm` the files, follow along, and `git status` at the end must show the same file set again. Nothing about the repo changes either way — both are practice.*

### Step 1 — the header (the promise)

`srcscope/src/counting.hpp`:

```cpp
#ifndef SRCSCOPE_COUNTING_HPP
#define SRCSCOPE_COUNTING_HPP

#include <string>

// Counts lines in text: a line is what ends in '\n'; a final line
// without a trailing newline still counts as one line.
// An empty text has zero lines.
int count_lines(const std::string& text);

#endif  // SRCSCOPE_COUNTING_HPP
```

Line by line:

- `#ifndef ... #define ... #endif` — the **header guard**: guards the whole file so that if this file is included twice (a .cpp can legitimately do that), the compiler doesn't compile the declarations twice with an error. Standard boilerplate of any new header; the names just have to be unique.
- `#include <string>` — you promise a type that lives in the `<string>` header. Why this line matters at *all* to compile — write a `counting.cpp` *without* it and the compiler explains in one diagnostic:

  ```text
  counting.cpp:1:28: error: 'string' in namespace 'std' does not name a type
      1 | int count_lines(const std::string& text) {
      |                            ^~~~~~
  counting.cpp:1:28: note: 'std::string' is defined in header '<string>';
                           this is probably fixable by adding '#include <string>'
  ```

  (Captured on the reference machine — the "fixable by adding #include" note is modern GCC being kind. Read the *first error*, fix, rebuild.)
- The `//` comment block — a rule you will **test**: it must match exactly. Comments lie sometimes; tests don't.
- `int count_lines(const std::string& text);` — a *declaration* (the promise), not a *definition* (the code). `const ... &` means "I read this string, I never change it". The trailing `;` makes it a promise instead of a function body. The definitions live in the `.cpp`.

**Separation of labor:** the header is the page others read when deciding *how to call* this; only the `.cpp` says how it works. This distinction is used in every later milestone.

### Step 2 — the implementation (keeping the promise)

`srcscope/src/counting.cpp`:

```cpp
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
```

Line by line, with the bugs this shape helps avoid:

- `#include "counting.hpp"` — quotes denote *your project's* headers (angle brackets mean system headers). The file you made in step 1.
- `for (char c : text)` — range-based loop: look at every character one by one. If `text` is empty, the loop body simply never runs (`lines` stays 0 — already correct for the empty case!). No special case. It's `char` because `std::string` is a line of characters.
- `if (c == '\n') ++lines;` — rule 1.
- `if (!text.empty() && text.back() != '\n')` — rule 2, guarded (`.back()` on an empty string is undefined behavior — the guard is not optional).
- `return lines;`

Ten lines, one rule each, and nothing else to trip over. This is what "testable" looks like: a small, named, deterministic promise with a tiny surface area.

### Step 3 — teach CMake the pieces

Three declarations, all in files CMake re-reads on the next configure. First the *component* recipe `srcscope/CMakeLists.txt`:

```cmake
# Testable logic lives in a library: main.cpp stays thin, and the
# tests link the same code without inheriting main().
add_library(srcscope_core
    src/counting.cpp)

# Headers live in src/ beside the sources. PUBLIC means the include
# path reaches everything that links srcscope_core.
target_include_directories(srcscope_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(srcscope
    src/main.cpp)

target_link_libraries(srcscope PRIVATE srcscope_core)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
```

Every line, uniquely explained — part 1 of the CMake table:

| Line | Meaning |
|:-----|:--------|
| `add_library(srcscope_core src/counting.cpp)` | "build a chunk of compiled code (a library), named `srcscope_core`, from this source file." Libraries are the shared-logic noun of CMake: not themselves runnable, but linkable by any executable that needs them. |
| `target_include_directories(... PUBLIC .../src)` | "directories where `#include "counting.hpp"` can be resolved from." **PUBLIC** = propagate: any target that *links* `srcscope_core` also gets this include path automatically. That sustains the including of "counting.hpp" in BOTH main.cpp and counting_test.cpp with zero duplication. |
| `add_executable(srcscope src/main.cpp)` | "a runnable program named `srcscope` from these sources." The file you already know. |
| `target_link_libraries(srcscope PRIVATE srcscope_core)` | "CONSUME the library": main.cpp's binary gets the compiled count_lines code bylined. PRIVATE = "for me, not my customers": srcscope's own users would not inherit a dependency it doesn't expose in its own interface. (As a rule of thumb: use PRIVATE for internal consumers inside the same folder, PUBLIC when a header leaks the library's types.) |
| `if(BUILD_TESTING) ... endif()` | "when tests are enabled, also read `srcscope/tests/CMakeLists.txt`." `BUILD_TESTING` is a switch `include(CTest)` in the root defines, default **ON**. Packaging builds can flip it OFF and get zero test code compiled. |

Then the test-recipe file `srcscope/tests/CMakeLists.txt` — the same two lines involve *test-specific* actions:

```cmake
# Smoke test: the CLI must build and exit cleanly.
add_test(NAME srcscope_runs COMMAND srcscope)

# Unit tests for the shared logic (srcscope_core).
add_executable(srcscope_tests
    counting_test.cpp)

# The checks call count_lines(); linking the library also delivers its
# public include path, so counting_test.cpp can #include "counting.hpp".
target_link_libraries(srcscope_tests PRIVATE srcscope_core)

add_test(NAME srcscope_counting COMMAND srcscope_tests)
```

| Line | Meaning |
|:-----|:--------|
| `add_test(NAME srcscope_runs COMMAND srcscope)` | (day-1 smoke test) "there is a test called `srcscope_runs`; running it means launching the `srcscope` target." Registered things go into build/CTestTestfile.cmake for ctest to find. |
| `add_executable(srcscope_tests counting_test.cpp)` | compile your test file into its own program, named `srcscope_tests`, placed in build/bin/ like every executable. |
| `target_link_libraries(srcscope_tests PRIVATE srcscope_core)` | "the test uses the library too." The include path is part of what "uses" means here. |
| `add_test(NAME srcscope_counting COMMAND srcscope_tests)` | "there is a test called `srcscope_counting`; running it launches the *test program*, not the CLI." |

The **two distinct registrations** beginners ignore: `add_executable` makes CMake build something; `add_test` makes CTest *know about* something. Omit the second → your program builds, `ctest -N` lists nothing, and there's no error anywhere. Omit the first → ctest finds a test you registered for a target that doesn't exist and errors. [Part O](#part-o--troubleshooting-encyclopedia) has both diagnosis recipes.

### Step 4 — the test program

`srcscope/tests/counting_test.cpp`:

```cpp
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

// CHECK(count_lines("") == 0)
//  → check(count_lines("") == 0, "count_lines(\"\") == 0", file, line)
// The '#' turns the condition into readable text for the failure line.
#define CHECK(condition) check((condition), #condition, __FILE__, __LINE__)

// One check-function per behavior — the whole organizing idea.

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
```

Reading it as three layers:

1. **The check mechanism** (`check`, `CHECK`, `failures`). `#define CHECK(cond) check(cond, #cond, __FILE__, __LINE__)` is the one macro. `#cond` ("stringification") turns the *source text* of the condition `count_lines("") == 0` into a string literal, so the failure message quotes what you wrote rather than just true/false. `__FILE__`/`__LINE__` are pre-known, compiler-filled coordinates. So one failing check prints e.g.:

   ```text
     FAILED   .../srcscope/tests/counting_test.cpp:50:  count_lines("srcscope") == 1
   ```

   — file, line, and the violated expectation, in one line. (Why not plain `assert()` from `<cassert>`? `assert` vanishes when `NDEBUG` is defined — i.e. in every **Release** build — silently turning a test suite into a no-op that "passes" everywhere. And `assert` aborts at the *first* failure, hiding later ones from the same run. This little helper does neither. If you ever hit the "tests passed but did nothing in Release" trap, this [Part O #14](#part-o--troubleshooting-encyclopedia) will look familiar.)
2. **The test functions.** One behavior per function, real-worded names that *state the rule*, and `CHECK` lines recording it. Naming is a superpower: when the suite fails, the failing *name* usually tells you what's broken before you even open the file.
3. **`main` calls every test, then returns `failures > 0 ? 1 : 0`.** The **exit code is the verdict**, because CTest judges tests by it — nothing more.

*Note on registration granularity:* CTest's registered unit is **the executable**, not each check. This design keeps all checks in one program (one add_test), because the check-functions make the *name* of each failure appear on the very line that failed. When you want finer `ctest -R` filtering per behavior, a real framework auto-generates separate registrations inside one binary — that's one of its actual advantages (Part F).

### Step 5 — configure, build, run, verdict

From the repo root:

```bash
cmake -S . -B build        # first configure (or auto-re-configured by the next line)
cmake --build build        # build everything: srcscope_core, srcscope, srcscope_tests
ctest --test-dir build --output-on-failure
```

*Expected (captured from a clean build directory):*

```text
Test project /home/.../build
    Start 1: srcscope_runs
1/2 Test #1: srcscope_runs ....................   Passed    0.00 sec
    Start 2: srcscope_counting
2/2 Test #2: srcscope_counting ................   Passed    0.00 sec

100% tests passed out of 2

Total Test time (real) =   0.00 sec
```

**That is a passing test.** Two registered tests, both exit 0, all 9 checks inside the second one green.

Want to check the *direct* detail without ctest? Run the test program itself and look at its stdout, one line per check:

```bash
./build/bin/srcscope_tests
#   ok       count_lines("") == 0
#   ok       count_lines("") == 0
#   ok       count_lines("srcscope\n") == 1
#   ok       count_lines("srcscope") == 1
#   ok       count_lines("srcscope\nsrcscope") == 2
#   ok       count_lines("a\nb\nc\n") == 3
#   ok       count_lines("a\n\nb\n") == 3
#   ok       count_lines("\n") == 1
# all checks passed
echo $?   # → 0
```

Notice what ctest *added* and what the direct run *adds back*:

| | ctest run | direct run |
|:--|:----------|:-----------|
| Runs the **program** | yes | yes |
| Prints each **check** line | no (summarises) | yes |
| Verdict in the **suite's language** (Passed/Failed, totals) | yes | no |
| Exit code available for chaining (`&&`, scripts, CI) | yes | yes |
| Registry / name / timeout enforcement | yes | no |

Rule of thumb: **direct run to debug one binary, ctest run to claim "everything is green".** Which is also why the [checklist](../checklist.md) "tests pass" always refers to the ctest run.

### Step 6 — make it fail, on purpose *(the part you must not skip)*

A test that has only ever passed is unverified — you have no evidence it *can* fail. Break it exactly once, deliberately, both ways.

**Direction 1 — the expected value is wrong** (the test lied). Open `srcscope/tests/counting_test.cpp` and change one expectation to a wrong value — e.g. declare that a final line without a newline counts as **2** (which is nonsense per the contract):

```cpp
    CHECK(count_lines("srcscope") == 2);   // lie introduced: contract says 1
```

Rebuild + run:

```bash
cmake --build build 2>&1 | tail -1        # expect: [100%] Built target srcscope_tests
ctest --test-dir build --output-on-failure
```

*Expected, exactly as captured:*

```text
Errors while running CTest
1 check(s) failed

50% tests passed, 1 tests failed out of 2

Total Test time (real) =   0.00 sec

The following tests FAILED:
	  2 - srcscope_counting (Failed)
```

where the test's own stdout (before the summary) shows the one failed line:

```text
  ok       count_lines("") == 0
  ok       count_lines("") == 0
  ok       count_lines("srcscope\n") == 1
  FAILED   .../srcscope/tests/counting_test.cpp:50:  count_lines("srcscope") == 2
  ok       count_lines("srcscope\nsrcscope") == 2
  ok       count_lines("a\nb\nc\n") == 3
  ok       count_lines("a\n\nb\n") == 3
  ok       count_lines("\n") == 1
1 check(s) failed
```

Learn these three reads:

1. The **FAILED line is the test's own file, own line, own expression.** That's the `__FILE__`/`__LINE__`/`#cond` machinery already working. You never invert logic hunting "which check failed".
2. **`srcscope_runs` still passes.** The CLI is fine — only the unit suite broke. This is your first taste of *failure localization*: names and registration granularity tell you *which* behavior broke without opening anything.
3. The **summary line is `50% tests passed, 1 tests failed out of 2`** and ctest **exits nonzero** — for CI, means failure.

Undo the lie (restore the `== 1`). Re-verify green.

**Direction 2 — the implementation is wrong** (the code lied). Now break the *production* code instead: open `srcscope/src/counting.cpp` and delete rule 2's whole `if` block (leaving only the newline count):

```cpp
    // (temporarily) forgot: a final line without a trailing newline still counts
    // if (!text.empty() && text.back() != '\n') {
    //     ++lines;
    // }
```

Build + run:

```text
cmake --build build ...   ← recompiles counting.cpp, and relinks everything that used it:
[ 16%] Building CXX object srcscope/CMakeFiles/srcscope_core.dir/src/counting.cpp.o
[ 33%] Linking CXX static library libsrcscope_core.a
[ 33%] Built target srcscope_core
[ 50%] Linking CXX executable ../bin/srcscope
[ 66%] Built target srcscope
[ 83%] Linking CXX executable ../../bin/srcscope_tests
[100%] Built target srcscope_tests
```

```text
ctest --test-dir build --output-on-failure  →
    Start 1: srcscope_runs
1/2 Test #1: srcscope_runs ....................   Passed    0.00 sec
    Start 2: srcscope_counting
2/2 Test #2: srcscope_counting ................***Failed    0.00 sec
  ok       count_lines("") == 0
  ok       count_lines("") == 0
  ok       count_lines("srcscope\n") == 1
  FAILED   .../srcscope/tests/counting_test.cpp:50:  count_lines("srcscope") == 1
  FAILED   .../srcscope/tests/counting_test.cpp:51:  count_lines("srcscope\nsrcscope") == 2
  ok       count_lines("a\nb\nc\n") == 3
  ok       count_lines("a\n\nb\n") == 3
  ok       count_lines("\n") == 1
2 check(s) failed

50% tests passed, 1 tests failed out of 2
```

Study the failure set. The broken *rule* affects exactly two of the nine checks — the ones whose input had a final line without `\n` (`"srcscope"`, and `"srcscope\nsrcscope"` where the *last* line lacks it). Every other check is untouched. A well-factored set of tests acts like a **fingerprint for a bug**: it names the violated rule, the file, and even its pair of failing cases, with zero debugging. That is the muscle every later milestone leans on.

Also note line 50's message now says `== 1` — the *same line* as Direction 1 but a different lie's direction. Face it once: the line shows the **expected** expression, not the actual value. To see the actual, you'd add a std::cout (or, later, a framework's EXPECT_EQ prints both sides; one reason frameworks exist). *Print the value and rebuild once to satisfy curiosity; that's a 30-second habit worth forming now.*

Restore the code (the comment/lines back, or `git restore srcscope/src/counting.cpp`) → **green again**:

```text
100% tests passed out of 2
```

**You have now: written production code + tests from scratch, registered them via CMake, compiled and linked, experienced each of the two failure directions, and fixed both.** These are the same motions grown-up testing frameworks automate away.

### One habit worth adopting today

Save a test when you fix a bug you found manually: the fix without a test leaves a hole exactly where someone can re-introduce it. Every regression below is this same one-liner re-run.

### Case study — the one "all green" that lies *(read once now, and remember)*

While validating this guide, I introduced a real bug — except I *forgot* the `#include "counting.hpp"` in a rebadged `counting.cpp`, so the file **failed to compile**. My command then piped the build through `grep` and chained with `&&`:

```bash
cmake --build build 2>&1 | grep -E "Building|Built" && ctest --test-dir build --output-on-failure
```

What happened: `grep` *found lines to print*, so the pipeline exited 0 regardless of make's real error, ctest ran, and reported **all tests passed** — because the moments earlier the *old* (still-good) binaries were what ctest ran. The suite was green while reporting on binaries that had already been declared no-longer-current. The suite had quietly lied by omission: nothing tested the new code — that code never even made it into a binary. Two lessons, both free:

1. Don't pipe critical builds through programs that can mask the exit code (`grep`, `tee`, `slurp` all do). Chain `x && y && z` directly. If you did that once, always re-build from scratch (`rm -rf build`) when in doubt.
2. **Green means "the binaries I ran were good"** — it does not mean "my source is good." When a test run stuns you by *passing*, rebuild clean and look again (Part O #17 has the recipe).

---

<a id="part-f--test-frameworks-the-current-choice-and-what-a-real-framework-looks-like"></a>
## Part F — Test frameworks: the current choice, and what a real framework looks like *(Intermediate)*

### What a testing framework is, and why it exists

A testing framework is a library that answers four questions for you:

1. **How are test cases declared?** (our answer today: one `void test_...()` function per behavior, manually listed in `main`)
2. **How are assertions stated?** (our answer: `CHECK(x)`-style helpers wrapping a boolean, printing file/line/expression)
3. **How are failures detected and reported?** (ours: `failures > 0`, message per failed check)
4. **How are tests discovered/registered?** (ours: written in CMake by hand, one `add_test`)

Every existing framework (GoogleTest, Catch2, doctest, …) answers the same four questions with different answers. Once you can answer them yourself for the hand-rolled version, a framework is a set of conveniences, not a mystery. The knowledge is the same; the arrangement differs.

### The framework this repository chose *today*

Read it plainly: the chosen test framework for the whole 40-week baseline is the one Part E just built — **CTest + hand-rolled check functions**, with a roadmap rule to *test every new behavior plus at least one edge or failure case* ([Roadmap.md §Engineering conventions](../Roadmap.md)), and the long-standing guidance that hand-rolled asserts first is how you *learn* what a framework is worth before adoption becomes a milestone's question. Week 4 then asks for "real tests" in *this shape* — more functions, more behaviors, same exit-code verdicts. No new dependency, no framework config, nothing to install. Nothing in Part F below changes your build today.

Why this is a sound choice (know it, so you don't just obey): it keeps C++20 + CMake + CTest as the whole dependency surface; it makes the teachable parts visible; and hand-rolling the first 100 assertions teaches what frameworks later do *for* you.

### `TEST`, `EXPECT_EQ`, `ASSERT_EQ` … — the GoogleTest vocabulary, so you recognize it the day a milestone asks

**Labeled clearly: this is not wired into this repository and must not be wired in before some milestone says so.** GoogleTest is the reference framework the repo's reference list names ([Roadmap.md §References](../Roadmap.md)). The four beginner questions, GoogleTest's answers, and the shape of your counting suite if/when translated:

```cpp
// Conceptual only — NOT part of this repository: no GoogleTest dependency exists here.
#include <gtest/gtest.h>
#include "counting.hpp"

TEST(CountLines, EmptyTextHasZeroLines) {     // grouping + human name → declaration
    EXPECT_EQ(count_lines(""), 0);            // assertion: compares, prints both sides on failure
}

TEST(CountLines, LastLineWithoutNewlineStillCounts) {
    EXPECT_EQ(count_lines("srcscope"), 1);
    EXPECT_EQ(count_lines("srcscope\nsrcscope"), 2);
}
```

The four questions' answers look like: cases declared by `TEST(Group, Name)` — *self-registered*: no list in `main`, no manual call; the framework collects them (that's "test discovery") because the macro expands to code that puts the case into a registry at program start. Assertions are a family of macros that print *both the expected and the actual* on failure (`Actual: 0`, `Expected: 1`) — a diagnosis gift after our one-sided `check`. The runner aggregates, and `gtest_discover_tests()` (a CTest helper script bundled with GoogleTest, used *in place of* plain `add_test`) turns each case into its own `ctest` entry — a finer registration than our one-binary-one-name model.

Your counting suite, GoogleTest-shaped, would compile with the file layout of the same contract — Sourcing it means a `GTest::gtest` dependency in CMake. **And until a milestone asks for it, the hand-rolled variant is correct here.** The value of reading this far: the *vocabulary* EXPECT_/ASSERT_, discovery, gtest_discover_tests — so docs and jobs stop being foreign on the day it matters.

The most important macro pair for beginners, in both directions:

- `EXPECT_*` — the **continuing** flavor: a failure is recorded and the test *goes on*, so one run can show multiple failures of the same test.
- `ASSERT_*` — the **stopping** flavor: a failure *returns from the current function immediately*. Use it when continuing would be meaningless or unsafe — usually to prove a value *exists* before something happens that handles it, e.g. `ASSERT_NE(v.size(), 0)` before you write a check about `v[0]`.

Correct real shapes, with what happens on failure annotated:

```cpp
// Conceptual GoogleTest — the EXPECT / ASSERT distinction, correctly spelled.
// Not part of this repository (no GoogleTest dependency exists).
TEST(VectorBasics, FrontOfEmptyVectorWouldBeUndefined) {
    std::vector<int> v;          // empty vector
    ASSERT_EQ(v.size(), 1);      // LIES about the invariant, so this fails →
                                 // test stops HERE. Nothing later runs — and
                                 // that is exactly what we want, because the
                                 // front() check below would read garbage anyway.
    EXPECT_EQ(v.front(), 1);     // (would only run when the ASSERT held)
}

TEST(VectorBasics, ContinueVsStop) {
    std::vector<int> v{1, 2, 3};
    EXPECT_EQ(v.front(), 1);     // if THIS failed: recorded, next line STILL runs
    ASSERT_EQ(v.back(), 3);      // if THIS failed: recording stops — the later
                                 // capacity check doesn't run in this TEST
    EXPECT_EQ(v.capacity(), 3);  // ...which is a separate observation, so arguably belongs in its own TEST
}
```

The rule to keep: **`ASSERT_` for preconditions of later checks; `EXPECT_` for independent observations.** In the hand-rolled style, the same discipline appears as "do the earlier CHECK first, and only reference state the earlier checks established sane" — same thinking, different spellings.

Other macros the beginner list usually means, one line each (true of GoogleTest):

| Macro | What it asserts |
|:------|:----------------|
| `EXPECT_EQ(a, b)` / `EXPECT_NE(a, b)` | equal / not equal (ints, strings, any printed value type) |
| `EXPECT_TRUE(x)` / `EXPECT_FALSE(x)` | a condition / its negation (also the catch-all) |
| `EXPECT_STREQ(a, b)` | C-string equality (`char*` null-terminated text; use `EXPECT_EQ` for `std::string` — a common beginner mixing of the two) |
| `EXPECT_THROW(f, Ex)` | the expression `f` throws type `Ex` (and survives doing it) |
| `EXPECT_NO_THROW(f)` | the expression runs without throwing |
| `EXPECT_FLOAT_EQ(a, b)` / `EXPECT_DOUBLE_EQ` | floating compare with sensible tolerance — **use these, never `==`, on floats** |
| `TEST_F(Fixture, Name)` | like `TEST` but reusing a fixture class — one setUp object per test (see fixtures at Part M) |

Whichever mechanism a future milestone picks, the trade-offs are these (the CMake dependency guide is the general reference: [Fetching Dependencies](https://cmake.org/cmake/help/latest/guide/fetching-dependencies/index.html)). The decision and the pins get recorded in the repo that day:

- **FetchContent** (CMake-native: a `FetchContent_MakeAvailable(googletest)` in CMake with a **pinned commit/tag** in the recipe). Costs: *needs network at configure time* — a first configure with no internet fails unless a copy is pre-populated; benefits: the pin is version-controlled and every machine builds the same thing. Offline workflow exists and is legal (pre-populate via a mirror; document it in the repo README that day).
- **System package** (`sudo apt install libgtest-dev`): cheapest to install, but versions vary across machines/CI images — breaks the "same software everywhere" reproducibility CI cares about, and older distro packages sometimes ship incomplete binaries.
- **Vendored copy** (`third_party/googletest` committed): fully offline, fully pinned, but adds a large vendored tree to review.

Whichever is chosen later: pin the exact revision, document *where* it came from, and record it in `PROGRESS.md`. Framework adoption is an engineering-convention change, not a one-line edit — worth the ceremony, not worth the guessing.

---

<a id="part-g--cmake-integration-in-detail"></a>
## Part G — CMake integration in detail *(Intermediate)*

This part explains every test-related line actually present in the repo's three CMake files — the master, `srcscope/`, and `srcscope/tests/`. No lines are shown that don't exist; the complete annotated root recipe ([tour.md §3](tour.md)) covers the rest.

### The full test-related wiring, as it exists

Root `CMakeLists.txt` (the testing-relevant lines):

```cmake
cmake_minimum_required(VERSION 3.20)
project(cpp_systems_builder ... LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)             # becomes -std=c++20 for every target below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ... build type, output dir, warnings: see tour.md §3 ...

include(CTest)                          # (A)
add_subdirectory(srcscope)              # (B) — one line per component; others follow as their weeks arrive
```

**(A)** `include(CTest)` vs `enable_testing()`: both turn on the test machinery. `include(CTest)` is a wrapper that *also* defines the user-facing option `BUILD_TESTING` (default **ON**) which components guard their tests with. `enable_testing()` is just the switch, without the option — use it in code that must not expose the toggle. This repo uses `include(CTest)`.

Two consequences to remember:

- configuration writes `build/CTestTestfile.cmake` — the file `ctest --test-dir build` reads. Open it after a configure; it's plain text and contains your registered tests.
- `BUILD_TESTING=ON` is what makes every `if(BUILD_TESTING) ... endif()` block materialize. Turn it off (`cmake -B <dir> -DBUILD_TESTING=OFF`) and no component compiles its tests at all.

**(B)** `add_subdirectory(dir)` — "read that directory's `CMakeLists.txt` as part of this one." Target names live in one global namespace, so a component's `srcscope_core` refers to the same everywhere. New components (tinycore in Week 5, …) each get their own commented placeholder line, uncommented when their week arrives.

`srcscope/CMakeLists.txt` (annotated in Part E §3 as it stands) plus `srcscope/tests/CMakeLists.txt` (also Part E) — together the wiring makes three targets: `srcscope_core` (library), `srcscope` + `srcscope_tests` (executables).

### The three commands that add tests, once and forever as a pattern

```cmake
add_executable(name file.cpp [more.cpp ...])   # the test program itself
target_link_libraries(name PRIVATE what_it_uses)
add_test(NAME <unique-name> COMMAND name [args...])
```

- `COMMAND name` in `add_test` refers to a *target* — CMake resolves the target's location at generate time and hard-writes the full path into `CTestTestfile.cmake`. This makes the same registration work on every machine; do **not** put a manually guessed absolute path in there.
- `add_test` doesn't compile anything. If you mistype the target name, configure still passes and the test errors *at run time*. Run the build; otherwise the mistyped name is a silent time bomb (recipe in Part O).
- Extra arguments after `COMMAND name` become the executable's argv — that's how e.g. `add_test(NAME srcscope_args COMMAND srcscope a b c)` exercises argument handling with no C++ at all; ctest does *not* shell-interpolate those words, so filenames with spaces work as-is (a ready-made exercise in Part R #2).

### Registration vs. linking — the distinction the whole file pivots on

- **Linking** (`target_link_libraries`) is a *build-time* question: "can this target's binary find the code it calls?" Failure shape: linker errors (`undefined reference`).
- **Registering** (`add_test`) is a *test-time* question: "does ctest know this binary exists and what to run?" Failure shape: `ctest` complaining — or worse, quietly *not listing* your work.

Both are needed and neither implies the other. The full "why is my test invisible" ladder, top to bottom, is in Part O.

### The mistakes this exact structure prevents

These all occurred as real captures during this guide's validation; each has a recipe in Part O:

| Mistake | The actual error you'd face (captured) |
|:--------|:----------------------------------------|
| Not adding `target_include_directories(srcscope_core PUBLIC src)` — or removing the link that carries it | Compilation fails: `counting.hpp: No such file or directory` — the include path traveled with the link |
| Forgetting to link `srcscope_core` into a test target that calls its functions | Build fails at *link* time: `undefined reference to 'count_lines(...)'` — named after the caller, not the library |
| `target_link_libraries(srcscope_tests PRIVATE srcscope_coer)` — typo'd target name | Configure **passes** (CMake treats the unknown name as a plain library name, not a target) and the build fails: `ld: cannot find -lsrcscope_coer: No such file or directory` |
| Two targets named the same | Error: "the same name already exists ... CMP0002" |
| Two tests with the same `NAME` (duplicate add_test NAME) | Error: `add_test given test NAME "same" which already exists in this directory.` |
| Adding tests without re-running configure | `cmake --build` re-runs it automatically at the next build, that's fine. If however the change came as a surprise, manually `cmake -S . -B build` — configure errors then surface at the right time |

### Naming — the conventions this repo already has

| Thing | Convention | Example here |
|:------|:-----------|:-------------|
| Targets (library/program) | lowercase, underscores, singular | `srcscope_core`, `srcscope`, `srcscope_tests` |
| Test names (the `add_test` NAME) | `<component>_<behavior>` | `srcscope_runs`, `srcscope_counting` |
| Test executables | `<component>_tests` | `srcscope_tests` |
| Test *functions* inside | `test_<full-behavior-phrase>` | `test_last_line_without_newline_still_counts` |
| Test files | `<area>_test.cpp` | `counting_test.cpp` |

Why both an *executable* name and a *test* name: the executable is the compiled thing (build system view), the test name is what `ctest -R` filters by (the user-facing registry view).

### Framework wiring, in case a milestone one day adds GoogleTest

Nothing below is in the repository today (Part F). If/when a milestone installs GoogleTest, the pattern (which you'll have been fully equipped to read) is:

```cmake
# Concept wiring in a future milestone — not present today:
include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.15.2)  # pinned
FetchContent_MakeAvailable(googletest)
target_link_libraries(srcscope_tests PRIVATE GTest::gtest GTest::gtest_main)
add_test(... COMMAND srcscope_tests)  # or: gtest_discover_tests(srcscope_tests)  — finer-grained ctests per TEST()
```

---

<a id="part-h--running-tests-like-a-professional"></a>
## Part H — Running tests like a professional *(Beginner)*

The muscle-memory block, one copy each for the two "identifiers" you use:

```bash
# in the repo root; the copy you should have after every session:
cmake -S . -B build                              # configure (glue: also auto-refreshes when recipes change)
cmake --build build                              # build exactly what changed
ctest --test-dir build --output-on-failure       # test all, show failed output
```

The complete command reference, each explained:

| Command | From | What it does |
|:--------|:-----|:-------------|
| `cmake -S . -B build` | repo root | (Re)generate build files + test registry into `build/`. `-S .` = sources here; `-B build` = outputs to `build/`. Safe any time. |
| `cmake --build build` | repo root | Compile + link everything CMake knows, only what changed. |
| `cmake --build build --target srcscope_tests` | repo root | Only that one target (auto-includes `srcscope_core` as its prerequisite). List available names: `--target help` |
| `ctest --test-dir build --output-on-failure` | repo root | Run all registered tests; **failure output printed only for failures** — set the standard, your terminal stays calm. |
| `ctest --test-dir build -N` | repo root | List every registered test. First response to "why isn't mine running": run this, compare with what your `add_test` wrote. |
| `ctest --test-dir build -R counting` | repo root | Run only names matching the regex `counting`. **Debug one test, don't rebuild the whole suite.** |
| `ctest --test-dir build -V` | repo root | Verbose run: prints each test's **full command and its stdout/stderr** in the summary. |
| `ctest --test-dir build --parallel N` | repo root | Run tests concurrently (each test = separate process). Safe *only if* tests are isolated — which is your job in Part M. |
| `ctest --test-dir build --timeout 10` | repo root | Per-test wall clock enforcement. Heroes left hanging (Part O hangs) become visible instead of silently burning your time. |
| `ctest --test-dir build --schedule-random` | repo root | Run in random order every time — a cheap, routine test of [independence rules](#part-m--test-design-and-organization) (order dependencies become flaky random failures and are very findable). |
| `./build/bin/srcscope_tests` | repo root | Direct executable run: the raw `ok/FAILED` lines, no ctest wrapper. Best tool while *iterating on one test*. |
| `rm -rf build` then the four-line block | repo root | The clean rebuild ritual; every milestone-complete claim requires it. Detail: [build.md §7](build.md). |

Clarifications embedded: `-V` vs `--output-on-failure` — `-V` always shows output; `--output-on-failure` only on failure ( quieter daily use). `--parallel` requires actual independence (a test that writes files with an own directory; a suite that shares a temp name  will collide under less parallelism than you'd hope).

### Ninja in this repo (optional)

By default CMake picks **Unix Makefiles** (until told otherwise). Opting into Ninja per-directory:

```bash
cmake -S . -B build-ninja -G Ninja
cmake --build build-ninja
ctest --test-dir build-ninja --output-on-failure   # everything else identical
```

Everything else in this guide works exactly the same. Ninja just runs the same compile commands with a slightly faster graph traversal. If you're on a slow machine, try it; it costs nothing and changes no commands beyond the `-G Ninja`.

### The running sequence, end to day, when a code change happens

For every tiny change, exactly the three lines above. When tests are failing and you want the *fast spot-check* style:

```bash
cmake --build build --target srcscope_tests            # rebuild just the test binary
./build/bin/srcscope_tests                             # run directly, see raw ok/FAILED lines
```

When green again: full run (`ctest ...`) so the *registration* view is also the one that shows the milestone-complete state (and [checklist](../checklist.md) accreditation ticks to the full suite, not one target).

### Release and sanitizer builds of tests (overview here; detail in build.md §4 / Part K)

The test suite exists in every *type* of build directory:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release    # different type → different directory, always
ctest ... -R counting                                     # note: asserts vanish in Release — our check()-style
                                                          # tests still work in ALL build types; changes nothing here
cmake -S . -B build-asan -DENABLE_SANITIZERS=ON           # ASan + UBSan (full walk: Part K)
ctest --test-dir build-asan --output-on-failure
```

---

<a id="part-i--what-should-you-test"></a>
## Part I — What should you test? *(Intermediate)*

### The seven beginner questions

Before writing any test, ask about the function/feature:

| # | Question | For `count_lines` |
|:--|:---------|:------------------|
| 1 | What can go **right**? (the normal case) | `"a\nb\nc\n"` → 3 |
| 2 | What can go **wrong**? (input that breaks assumptions) | no trailing newline — handled? verified |
| 3 | What is the **smallest** input? | `""` → 0 |
| 4 | What is the **largest reasonable** input? | a file-sized string (a few MB — fast? right answer?) |
| 5 | What happens **at the boundary**? | text ending exactly at `"\n"`; text of exactly one char |
| 6 | What happens when an **operation fails**? | (files: reading fails half-way; for a pure function: nothing can fail — a documented fact, itself worth knowing) |
| 7 | What if a **resource is unavailable**? | (again nothing here — but say that out loud in the test set so you know it was considered) |

Question 1 alone makes a test. Questions 2–7 make a *suite*. You don't need 100% of both for every function, but you should be able to say *why* a question doesn't apply for the one you skip.

### The standard input classes, with examples

| Class | Example input for a counting-style function | Why beginners under-test it |
|:------|:---------------------------------------------|:------------------------------|
| Normal input | `"a\nb\nc\n"` → 3 | It's the only case most people write |
| Empty input | `""` → 0 | "It's an object, of course it works" — loops, `.back()` on empty, and division-by-magic all live here |
| Zero | a numeric 0 | `0` flows through arithmetic surprisingly; `n = 0` often untested |
| Negative | a numeric -1, -100 | Underflows, sign errors, "can't happen" — until it does |
| Boundary value | the last valid index; length+1 | Off-by-one bugs are the most common bug shape in novice C++ |
| Unexpected/invalid input | garbage characters, `\r` (Windows line endings) | Behavior must be *defined*: reject, treat, or document — never accidental |
| Duplicate values | `"a\na\na\n"` | Sets vs vectors bugs: "unique" logic has never seen a duplicate |
| Large input | thousands of lines | O(n²) loops and capacity bugs (tinycore Phase II!) |
| Resource failure | file that can't be read; disk full | Unreachable in normal dev but users hit it; needs simulation or design |
| Exceptions | function documented "throws on invalid input" | Two branches exist: throws / doesn't. Test both — see snippets below |
| State changes | call the same function twice | First call success ≠ second call success (IO on closed handle, iterated-out iterators) |
| Ownership/lifetime | borrowed pointer returned after its base is destroyed | Later phases; sanitizers found these classes' bugs (Part K) |
| Concurrency | two threads on one counter | Even later; deterministic sync matters (Part L) |

Two snippets in the repo's own style — exceptions, and state:

```cpp
// Testing exceptions (a file-counting function that reports a
// missing file via a documented exception — an exercise in Part R #3):
void test_missing_file_is_reported() {
    bool threw = false;
    try {
        count_lines_of_file("/nonexistent/path/definitely.txt");
    } catch (const std::exception&) {
        threw = true;
    }
    CHECK(threw);   // the documented failure happened, not something else
}

// Testing state: same call, same answer, twice in a row:
void test_repeated_calls_same_answer() {
    const std::string text = "a\nb\n";
    CHECK(count_lines(text) == 2);
    CHECK(count_lines(text) == 2);   // second call, same text: no drift
}
```

### Behavior, not implementation

A good test survives refactoring. If your test calls the exact internal mechanics of the function, the *smallest harmless cleanup* breaks it and teaches you to distrust tests. Rule of thumb:

```cpp
// GOOD — about the contract: the input's meaning and the documented behavior:
CHECK(count_lines("srcscope") == 1);          // rule 2, no mention of how it's achieved

// BAD — about one implementation trick:
CHECK(text.internal_buffer_has_reached_end_flag_for("srcscope"));  // gone after any tidy-up
```

The "print at the edges, compute in testable functions" discipline from [tour.md §5](tour.md) exists exactly for this: if behavior can be verified through the *public* interface, it usually should be; implementation-detail tests are reserved for lifetime and invariant bugs — where sanitizers and invariant-focused checks (Part II, tinycore) are the right tool, not UI-shaped expectations.

### Assertion quality vs coverage *(advanced note; expanded in Part P)*

A test's worth is not "how many lines ran" — it's "what would fail if this behavior regressed". One `CHECK` that pins a documented rule beats a hundred empty checks that merely *execute* the code. When adding tests, ask: "if I broke this line on purpose, would this test suite catch it in one run, with a message I can read without opening the debugger?" If no — add or upgrade the check.

---

<a id="part-j--edge-cases-every-beginner-must-understand"></a>
## Part J — Edge cases every beginner must understand *(Intermediate)*

Not every case feels worth a test — until the pattern's first failure teaches you the price. This part is deliberately dense: each entry is one idea, one why-you-might-skip-it, and a small verified-shape snippet. Use them as a checklist per function you write in Phase II. Snippets show the CHECK-style wording that compiles in this repo's flag set.

### Containers

**Empty container.** Scenario: any loop/index on `std::vector`/`std::string`. Overlooked because loops "do nothing" on empty — until the code *also* says `container.back()` or `container.front()` (undefined behavior on empty!). Expected: a defined answer, no crash. Test:

```cpp
void test_empty_string_has_zero_lines_and_no_crash() {
    CHECK(count_lines("") == 0);
    // would have caught the .back()-on-empty class of bugs above the guard
}
```

A failure usually reveals the caller *assumed non-empty* and the callee didn't — fix by deciding who guards (callee here, per the documented contract).

**Single-element container.** Loops work; the "one item" case misses the *couples* bugs (first==last predicate, `size()==1` vs `size()>=1`). Test `count_lines("x") == 1` — already in the suite; keep the habit for containers in Phase II.

**Duplicates.** Any code with a notion of "the set of things" can sneak a `for`-loop that assumes uniqueness. Expected: defined (usually added twice, or rejected — *your* contract decides, then the test pins it).

### Numbers

**Integer limits.** `int` overflow is **undefined behavior** — anything can happen, and does vary. Quote-worthy demo (compiled and run on the reference machine; the program is the one quoted in full in Part K):

```text
$ g++ -w -fsanitize=undefined -fno-sanitize-recover=all ubsan_demo.cpp && ./ubsan_demo
ubsan_demo.cpp:3:36: runtime error: signed integer overflow: 1 + 2147483647 cannot be represented in type 'int'
exit=1
```

*The check:* don't test overflow *behavior* (it's UB!) — instead test your code's *documented limits* ("works for inputs up to X") and add a defensive rejection where the contract says. And compile with UBSan for these classes of bugs behind you (Part K shows the enabling commands for this repo).

**Zero, negative.** `count_lines-of-""`-style zero cases are cheap tests since so many arithmetical bugs live there. Numeric seeds (both *signed* and *unsigned* paths of a function, when both exist) belong in every function's suite; both flavors need tests for their own corner (`-1`, `0`, huge).

**Off-by-one.** LAST LINE of Part E Direction 2 gave you the exposure: a rule about *when* the loop counts the final element. Write the per-boundary checks: first element, last element, exactly-one-element, zero elements. Four `CHECK`s. Already present in our suite's shape.

**Invalid index.** `v.at(i)` is *designed to throw* on OOB (test it: `try { v.at(9999); } catch (const std::out_of_range&) { threw = true; }`); `v[i]` on out-of-range is undefined behavior — *don't test UB, prevent it*: assert the range first, or code up `.at()` where failure is expected.

### Pointers, references, lifetime

**Null pointers.** Code that dereferences without checking crashes; the C++ idiom, though, is to *not pass* null — but functions promise "may return nullptr" all the time (`find`-style APIs). Test what the API *documented*: "returns nullptr when not found" → check that. UBSan (via `-fsanitize=undefined`) catches null-pointer dereferences at runtime; ASan catches null-pointer crashes too.

**Dangling references / borrow-outlives-owner.** A documented trap in modern C++: a `std::string_view` or span borrowed from a string that then goes out of scope. The view itself doesn't copy — it points to the dead string's memory — and reading it is undefined behavior. The honest, correct framing:

```cpp
// A view from an owner that OUTLIVES the view is fine:
std::string owner = "abc";
std::string_view view = owner;      // owner of `view` value can be used while owner is alive

// A view of a TEMPORARY is a dangling view — the temporary dies at this line's ';':
std::string_view bad = std::string("temporary");   // reading `bad` afterwards is UB
```

Testing this bug class directly is unreliable (it depends on what memory reused the spot), so the remedy is threefold, in the order you'll actually use it: design review (never return/store a view into a temporary), a documented lifetime rule in the header (like the [checklist.md](../checklist.md) §C++ fluency item "string_view lifetime constraints"), and sanitizers — ASan flags *use-after-free* if it sneaks in (Part K). Testing *drives the design* here; you cannot out-test a dangle reliably.

**Moved-from objects.** What a `std::move`d-from object *is* cuts across all of Phase II: the moved-from primitive types are unspecified — for `std::string`, `std::unique_ptr`, `std::vector` — each with its own documented "valid but unspecified" clause. Write tests that pin *your chosen* moved-from states, not imaginary ones:

```cpp
void test_move_transfers_ownership() {
    auto owner = std::make_unique<int>(7);
    int* before = owner.get();
    auto receiver = std::move(owner);
    CHECK(receiver.get() == before);   // the same resource, new owner
    CHECK(owner.get() == nullptr);     // moved-from: guaranteed empty (documented for unique_ptr)
    CHECK(*receiver == 7);
}
```

Verified: this compiles clean under `-std=c++20 -Wall -Wextra -Wpedantic` and passes.

**Copy and move behavior.** What Phase II asks in [checklist.md](../checklist.md) §tinycore String/Vector — copies *independent* (change one, other unchanged); move *transfers, leaves moved-from defined*; self-assignment `x = x;` safe; assignment chains `a = b = c;` compile. Each becomes one tiny test in the owning class's suite. Larger shapes come in Part K.

### Resources: files, dirs, failure paths

The file-operations cases you will need in M02/M03, each a real expected behavior:

| Case | Expected (decide, then metric) | What a failing test reveals |
|:-----|:-------------------------------|:------------------------------|
| File not found | a reported error (not a crash), behavior documented | The file was *opened without checking* — the silent missing-file bug |
| Empty file | 0 bytes / 0 counting semantics | Off-by-one and `.back()`-on-empty bugs again |
| File with no trailing newline | counts the final line exactly once | The rule-2 rule; M02's test file |
| Large file (MBs+) | correct count (and no unbounded memory growth) | Whether the reader *streams* or hoards the whole file in memory — the difference a multi-GB file notices |
| Permission denied | reported, distinct from "empty" | Error-code mapping bugs — open failures treated as success |
| Missing directory | reported distinctly | Accidental assumptions about `std::filesystem`'s behavior |
| Invalid path (trailing garbage, embedded NUL) | an explicit rejection (where `std::filesystem` itself refuses) | Construction errors happening *before* any use |

*The M02 test runner that exercises these will create real temp files — per Part N, with a `TempDir`-style fixture (snippet there) so no test's input files are ever another test's leftovers.*

### Difficult to test — says the honest part

Some error paths are architecturally hard to test:

- **Allocation failures** — `operator new` throwing `std::bad_alloc` under memory exhaustion isn't reproducible "just by running" — you'd need a custom allocator and a plan. Roadmap resolution: **document** ("allocation failure not tested; growth capped in tests at reasonable sizes") and design so it can't corrupt invariants (exception safety) rather than pretending to test.
- **Disk full / network drop** in the middle of an operation — injectable seams (a *format* abstraction like `std::istream`&) are the honest path, not hardcoded simulators.
- **Timing-based failures** (concurrency) — Part L's whole problem space.

The genuinely useful discipline: when a failure path is untestable *as written*, factor the unit (constructor of objects — e.g. one `open()` returning a result object, testable — instead of an invisible file handle), and the untestable seam becomes a testable seam. Testing *drives design*, not the reverse.

---

<a id="part-k--testing-memory-and-resource-management"></a>
## Part K — Testing memory and resource management *(Advanced)*

The roadmap's Phase II is about ownership and lifetime. This part gives the testing habits *early* so that when `tinycore::String`/`Vector<T>`/`UniquePtr<T>` arrive at Weeks 5–8, the suite shapes are already familiar.

### Lifetime spy objects — the workhorse pattern

The primitive question of RAII testing: *how many objects exist right now, and does a rule end up with the same count as before the rule ran?*:

```cpp
// Verified pattern — compiles clean with -std=c++20 -Wall -Wextra -Wpedantic:
struct Resource {
    static int live;
    explicit Resource() { ++live; }
    ~Resource() { --live; }
};
int Resource::live = 0;

void test_scopes_reverse_exactly_once() {
    CHECK(Resource::live == 0);
    {
        Resource one, two;
        CHECK(Resource::live == 2);   // constructed exactly twice
    }   // scope ends: destructors run HERE, automatically (RAII)
    CHECK(Resource::live == 0);       // destroyed exactly twice
}
```

Every tinycore milestone (copy constructor, move constructor, push_back reallocation, UniquePtr reset) re-uses this one instrument: run the *rule*, then check `live` before and after. Rules fail cheaply — double-destroy shows negative counts, leaked objects leave positive counts. Everything about RAII is testable *because* RAII ties resources to scope, and scope is what a test can create and end.

### The smart-pointer tests you'll rewrite for `tinycore::UniquePtr` (Week 8)

```cpp
auto owned = std::make_unique<int>(42);
CHECK(owned != nullptr);

std::unique_ptr<int> moved = std::move(owned);   // transfer, not copy
CHECK(moved.get() == old_ptr);                   // same resource
CHECK(owned == nullptr);                         // moved-from = guaranteed empty

// reset: destroys what existed before, adopts the new value:
moved.reset();
CHECK(moved == nullptr);                          // deleted exactly once (LeakSanitizer verifies below)
```

Consequences worth pinning when Phase II arrives: copying a UniquePtr is *prohibited* (won't compile — a test can only confirm that the delete happened once per created object), destruction happens once, null-pointer `reset()`/`get()` are all safe and defined.

### Sanitizers — the runtime detectors this repo ships a switch for

The root recipe defines one switch, `-DENABLE_SANITIZERS=ON`, that builds every target with **AddressSanitizer (ASan)** + **UndefinedBehaviorSanitizer (UBSan)**:

```bash
cmake -S . -B build-asan -DENABLE_SANITIZERS=ON   # separate directory: sanitizer builds run slower, keep them nowhere near your daily `build/`
cmake --build build-asan
ctest --test-dir build-asan --output-on-failure   # the same two tests, now instrumented
```

Verified real reports, each from a small deliberate bug compiled with the same flags (the programs are one-liners, full sources in this part's reference block):

```text
==...==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x...
READ of size 4 at 0x... thread T0
    #0 ... in main (...+0x...)        ← ABOUT WHERE it went wrong, and what the memory did
0x... is located 8 bytes after 12-byte region [0x...,0x...)
allocated by thread T0 here:
    #0 ... operator new[](unsigned long)   ← WHERE the buffer came from
    #1 ... in main
SUMMARY: AddressSanitizer: heap-buffer-overflow ... in main      (exit code 1)
```

```text
==...==ERROR: LeakSanitizer: detected memory leaks              ← exit code 1; runs with ASan by default
SUMMARY: AddressSanitizer: 400 byte(s) leaked in 1 allocation(s).
```

```text
ubsan_demo.cpp:3:36: runtime error: signed integer overflow: 1 + 2147483647 cannot be represented in type 'int'   (exit 1 with -fno-sanitize-recover)
```

**Read them as:** type of violation → address → who read/wrote it (stack) → where the memory came from (allocation stack). That pair of stacks answers "what bug is this" nearly every time.

What each tool catches — and, more important, doesn't:

| Tool | Catches | Cannot catch |
|:-----|:--------|:-------------|
| **ASan** | out-of-bounds heap/stack/global access, use-after-free, double free, invalid frees, plus **memory leaks on exit** (LeakSanitizer) | use-before-init (a different instrument), data races, pure logic bugs in *fully in-bounds* operations |
| **UBSan** | signed integer overflow, null-pointer dereference, misaligned access, out-of-range enum value, invalid shifts — undefined behaviors defined by the standard | races, leaks, or logic bugs — and it stays off unless you opt in (speed) |
| **Both together** (this repo's switch) | the union of the two, in one build | only code paths *your tests actually execute*: a sanitizer run without coverage of the buggy path finds nothing. That is what "run where supported" in the roadmap means — their value is exactly proportional to your suite's reach |

The deliberate-bug sources behind those reports:

```cpp
// asan_demo.cpp — out-of-bounds READ:
int* raw = new int[3]{10, 20, 30};
int bad = raw[5];          // reads 2 slots past the end → heap-buffer-overflow report
(void)bad;
delete[] raw;

// leak_demo.cpp — forgotten `delete`:
int main() { int* p = new int[100]{1,2,3}; (void)p; return 0; }

// ubsan_demo.cpp — signed overflow (UB):
int add(int a, int b) { return a + b; }        // add(1, INT_MAX) is undefined
```

A failing ASan/UBSan test is a *good* failure — the tool found what the test's CHECKs alone missed. **Never** disable the sanitizer run to make a green result; fix the bug it reported (Part O).

### Valgrind — mentioned for completeness

`valgrind --leak-check=full ./build/bin/srcscope_tests` gives ASan-comparable detection *without recompiling*, at a much larger slowdown, and it must **not** be combined with sanitizer builds (they're different runtime instruments). Not installed on the reference machine; optional on your distro. Prefer the repo's own sanitizers switch for daily work.

---

<a id="part-l--testing-concurrent-c-code"></a>
## Part L — Testing concurrent C++ code *(Advanced, Phase V-ish; the concepts arrive when the roadmap says)*

Concurrency testing has a rule before it has techniques: **a concurrent test that passes once proves almost nothing.** Two threads can cooperate 999 times out of a 1000 — the bug lives in the 1000th interleaving. This part teaches the *deterministic* patterns and the detector (ThreadSanitizer) that make concurrent tests about *design*, not luck.

### The vocabulary, one line each

- **Data race** — two threads touch the *same memory*, at least one writes, with no synchronization between them. In C++ this is **undefined behavior** (not "last writer wins" — genuinely unspecified).
- **Mutex** (`std::mutex`) — an exclusive lock around a section; the standard tool for "only one writer at a time".
- **Condition variable** (`std::condition_variable`) — how a thread *waits* for "something happened" without busy-looping; always paired with a mutex and a *predicate loop* (`while (!ready) cv.wait(lock);`).
- **Deadlock** — A holds lock A, wants lock B; B holds B, wants A. Both wait forever. Symptom in tests: a hang.
- **Producer–consumer** — one side produces data, another consumes it, buffered in between (a "queue with capacity"); the classic unit for boundary + shutdown tests.
- **Atomics** (`std::atomic<int>`…) — per-variable operations that are guaranteed to happen atomically; correct *for that variable*, but choosing memory-order parameters correctly is advanced — don't reach for them casually. The C++ core checklist says it too: "Use atomics only when their memory-ordering requirements are understood."
- **Thread lifecycle/shutdown** — thread must either be joined (`std::thread::join()`) or released (`detach()`, which is rarely right) before its handle's destructor; failing is a *rule* violation (`std::terminate`). `std::jthread` (C++20) joins automatically on destruction and carries a cooperative `stop_token` — prefer it in product code.

### Why not arbitrary sleeps

`sleep_for(50ms)`-style tests encode a guess about a machine you don't control: pass at home, fail on a slow CI, or worse — "pass because the JVM caught a breath, fail because a different thread took it". The fix is not a longer sleep; it's synchronization that *is* the test's point: sleep-based tests check timing, not correctness, and timing is exactly the thing a 40-week roadmap says to keep out of the build.

### Deterministic handshake #1 — promise/future (verified snippet)

When "the thread finished a unit of work" is the *observable thing you need*, make that observable literally:

```cpp
// Verified: compiles clean with -std=c++20 -Wall -Wextra -Wpedantic; passes.
void worker(std::promise<int> answer) { answer.set_value(41); }

int main() {
    std::promise<int> answer;
    std::future<int> expected = answer.get_future();
    std::thread t(worker, std::move(answer));
    int value = expected.get();   // returns exactly when the value exists — no sleep anywhere
    CHECK(value == 41);
    t.join();
    return failures == 0 ? 0 : 1;
}
```

### Deterministic handshake #2 — C++20 `latch` (verified snippet)

`std::latch` = a one-way barrier: count down N times, wait until zero. For "both participants are *at this known point*":

```cpp
// Verified — same flags; passes.
std::latch done(2);
void participant() { done.count_down(); }

int main() {
    std::thread a(participant), b(participant);
    done.wait();               // resolves only when BOTH counted down
    CHECK(true);
    a.join(); b.join();
    return failures == 0 ? 0 : 1;
}
```

`std::barrier` (C++20) is the multi-phase cousin (reusable across rounds — synchronized loops); same idea: *wait until a known set of threads reach a point*, instead of waiting on wall-clock luck.

### Shared state — a mutex around it, then a *deterministic check* of the outcome

```cpp
// Verified — same flags; passes. The mutex, not hope, makes the counter exact:
static int shared = 0;
static std::mutex shared_mutex;
void add_ten_thousands() { for (int i = 0; i < 100000; ++i) { std::lock_guard<std::mutex> g(shared_mutex); ++shared; } }

int main() {
    std::thread a(add_ten_thousands), b(add_ten_thousands);
    a.join(); b.join();
    CHECK(shared == 2 * 100000);   // exact — a deterministic post-condition
    return failures == 0 ? 0 : 1;
}
```

Remove the lock and the check will *sometimes* fail (order of interleavings), *sometimes* not. A test that needs N reruns for trust is a bug awaiting inclusion — once a concurrent test shows even one failure under load, treat it as a real bug and fix before merging (or before moving on, in this repo's terms).

### C++20 cooperative shutdown (`std::jthread` + `std::stop_token`) — verified shape

```cpp
// Verified — same flags; passes.
std::atomic<int> work{0};
{
    std::jthread stoppable([&](std::stop_token token) {
        while (!token.stop_requested()) { ++work; std::this_thread::yield(); }
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(1));  // let the worker run *enough*
    CHECK(work.load() >= 0);
}   // scope close makes jthread request stop and join — cleanup is RAII, exactly as tests want it
CHECK(work.load() > 0);
```

Note that the *destructor* is what gives the deterministic join here — no exit-time surprises — and [Part K](#part-k--testing-memory-and-resource-management)'s live-count spy works unchanged in a threaded fixture.

### ThreadSanitizer (TSan)

TSan is the standard detector for data races. It works by instrumenting memory accesses with *shadow memory* — it catches interleavings that actually happen during execution, not the ones that "could have happened" — so positive results are strong signals.

```bash
# SEPARATE build directory: TSan must not be combined with ASan (they're conflicting instruments)
cmake -S . -B build-tsan -DCMAKE_CXX_FLAGS=-fsanitize=thread -DCMAKE_EXE_LINKER_FLAGS=-fsanitize=thread
cmake --build build-tsan
ctest --test-dir build-tsan --output-on-failure
```

(Two flags — compile *and* link — is not a typo; a shared-library build needs both, like the repo's ENABLE_SANITIZERS switch does.)

A race under TSan, verified on the reference machine, looks like:

```text
WARNING: ThreadSanitizer: data race (pid=...)
    #0 work() ...   ← both stacks, and the pair of violating accesses
exit=66
```

Things to know about TSan before you depend on it:

- it only sees what actually happened in *this* run — a race that didn't trigger is invisible. It's a *hunting* tool paired with stress-sized loops (100000 iterations is my standard — they don't guarantee either).
- a "green" TSan run is *more* confidence than a green plain run — but still not a *proof* of race-freedom. Design review (`std::mutex` around every shared touch, no after-shared-state exceptions) is the primary discipline; TSan is the second check.

### The honest version of "the test passed"

A thread test that passed alone, once, tells you the *most likely* interleaving worked. What Phase V's roadmap requires is the fuller set: multi-client, bounded-resource, and shutdown — with deterministic fixtures (latch/order) + TSan runs where supported + the design review habit. The [roadmap](../Roadmap.md) milestone M36–40 phrases these as acceptance criteria, for good reason.

---

<a id="part-m--test-design-and-organization"></a>
## Part M — Test design and organization *(Intermediate)*

### Where tests live, now and for every later phase

The roadmap's monorepo layout ([Roadmap.md §2](../Roadmap.md)) gives every component its own `tests/` directory, exactly like `srcscope/`:

```text
Cpp-practice/
├── CMakeLists.txt            ← add_subdirectory per component (each commented until its week)
├── srcscope/                 ← exists today: src/, tests/
│   └── tests/
├── tinycore/      (Week 5)  ← include/tinycore/, src/, tests/   (Recipe A in tour.md)
│   └── tests/
├── forge/         (Week 13) ← src/, tests/
├── termkit/       (Week 21) ← src/, tests/ (+ termview/ as its example app)
├── httpkit/       (Week 29) ← src/, tests/ (+ netlab/ as its experiment app)
└── examples/                 ← stand-alone demo programs (not part of the suite unless a milestone says)
```

The rule: **tests ship with their component, not in a global pile.** That keeps each product buildable and *testable independently*, and it means no test file is more than one directory from the code it exercises. (Don't create future directories now — their milestones do that; the placeholder `add_subdirectory` lines already exist, commented, in the root recipe.)

### Naming conventions (one table, use it forever)

| Kind | Convention | Example from this repo |
|:-----|:-----------|:------------------------|
| Test file | `<subject>_test.cpp` | `srcscope/tests/counting_test.cpp` |
| Test executable target | `<component>_tests` | `srcscope_tests` |
| Registered CTest name | `<component>_<behavior>` | `srcscope_runs`, `srcscope_counting` |
| Test function | `test_<full behavior sentence>` | `test_last_line_without_newline_still_counts` |
| Test fixture class | `<Thing>Fixture` (or plain — anything local) | a `TempDir` guard struct (below) |

The naming has one rule of consequence: **a failing test's name must state what broke without a single line of code read.** `test_last_line_without_newline_still_counts` failing *is* the diagnosis.

### One executable, many check-functions (the granularity question)

The default shape here: **one test executable per component**, all its behavior tests as check-functions inside it, one `add_test` per executable. Two alternatives exist, and each is a real tool at a different time:

| Shape | When it's right | Cost |
|:------|:----------------|:-----|
| One suite executable (today's shape) | Daily development; check-functions localize failures by name | One extra rebuild to relink everything after an edit |
| One executable *per behavior* / per test | When one suite takes seconds (later phases with files/processes/IO) and you want `ctest -R` to select behaviors cheaply | More CMake wiring, more binaries, slower incremental builds |
| A framework's auto-registration | When the repo adopts one (never before a milestone asks) | A dependency (Part F's trade-offs) |

### Fixtures — when setup repeats across tests

When multiple tests repeat the same setup, a **fixture** means "the setup (and teardown) several tests share". The C++-native tool is RAII — a small object whose constructor prepares, whose destructor cleans up, living exactly as long as its tests need it:

```cpp
// A temp directory fixture whose cleanup runs whenever a test ends — win or lose.
// Verified: compiles clean with -std=c++20 -Wall -Wextra -Wpedantic; passes.
class TempDir {
public:
    TempDir() : path_(fs::temp_directory_path() / ("first_test_" + std::to_string(counter_++))) {
        fs::remove_all(path_);          // sweep leftovers of an earlier crashed run
        fs::create_directories(path_);  // then make it truly empty
    }
    ~TempDir() { std::error_code ignore; fs::remove_all(path_, ignore); }
    const fs::path& path() const { return path_; }
private:
    fs::path path_;
    inline static int counter_ = 0;
};

// usage — object stays scoped to the ones that need it:
void test_file_creations_and_counts() {
    TempDir sandbox;                       // unique per test-run in a process; starts empty
    std::ofstream(sandbox.path() / "notes.txt") << "two lines\nor three";
    CHECK(count_lines(read_file_to_string(sandbox.path() / "notes.txt")) == 2);
}   // destructor removes the directory — the test's own cleanup
```

Honest note: with the check()-based suite, a *hard crash* (segfault, `std::abort`) skips destructor runs entirely and can leave a temp dir behind on disk. That's why the fixture **sweeps leftovers on construction** (first line of the constructor above), and why the temp root is `std::filesystem::temp_directory_path()` — nobody's repo dir ever holds leftovers. Framework-style per-test teardown narrows that gap further, but the sweep-at-start idiom covers the rest cheaply, and it's how you get deterministic tests without pretending `abort()` runs destructors (it doesn't — in any test style).

### Isolation rules — each test stands alone

CTest may run tests as a set or individually; each test must pass alone *and in any order* ([Part E's doctrine](#part-e--your-first-test-from-absolute-zero)). Concretely:

- **No shared mutable state.** No global counters "to speed things up"; a static that one test populated is a bug in another test's run.
- **No shared files.** Files created under a *unique* temp dir per test (TempDir pattern), never in the repo tree, never in another test's directory.
- **No cross-test CWD assumptions.** Tests run with the *build directory* of their own CMakeLists as working directory (`ctest -V` shows `.../build/srcscope/tests` — a real capture); use absolute/temp paths, always.
- **No cross-test seeding.** One test's leftover state must not be another test's starting condition.

A failing CTest orchestrates order *randomly* — run `ctest --test-dir build --schedule-random` regularly as a cheap check that no suite depends on sequence. Order dependencies *will* surface in some future phase (a server test that assumes an earlier test started it); the fix is always the same: make the setup local to that test (fixture), or make the dependency explicit. Never "it worked because they ran in order".

### Readability — tests are documentation first

- **Shape every test the same (arrange → act → verify)** so a reader learns the pattern once.
- **Write the expected value inline** — `count_lines("srcscope") == 1`, not `CHECK(count_lines(x) == expected_configured_elsewhere)`. The failure line should carry the whole expectation (and arithmetic like `34 + 5 * 3` becomes `49`, with the intent obvious).
- **No loopy expectations** computed inside the test (loops hide what is actually being checked); a table of (input → expected) rows with per-row failure messages is fine.
- **Keep bodies short.** Past a handful of CHECKs plus setup, the function is really two tests — split it, give each a name in its own right.
- **Test public interfaces.** Verify through the documented API (the header) — not private fields — so refactoring internals never breaks a *correct* test.

### Shared helpers between tests (later, when two files need them)

One suite file usually needs zero helpers. When a second test file in the same directory needs the same `check()` machinery or the same fixture, put them in a tiny local header (e.g. `test_util.hpp`) and include it everywhere — test helpers are first-class test code, and no extra CMake wiring is needed to share a header that way. Shared helpers *across components* wait until two components genuinely need them — don't build shared infrastructure prematurely.

### Mocking — what it is, and why this repo mostly postpones it

- **Fake / stub** — a hand-written minimal replacement object (for example, a small in-memory `std::streambuf` that pretends to be a file) so your test can exercise a seam without touching the real resource.
- **Mock** — a stub you also use to *verify interaction*: "was the right method called, with those arguments?"
- The discipline for later phases comes from the roadmap's own design language: `srcscope` counts real files (use real temp files — Part N); `termkit` gets a fake terminal by design (Week 28); `httpkit`'s parser is testable without sockets (Week 32). Extract an interface *for tests* only when a second real implementation exists or is concretely imminent — tests alone are not a rule for making real objects smaller.

---

<a id="part-n--integration-tests-and-real-world-testing"></a>
## Part N — Integration tests and real-world testing *(Intermediate)*

### What "integration test" means beyond the unit smoke test

| | Unit | Integration |
|:--|:-----|:------------|
| Tests | one function's contract | several pieces *through real interactions* |
| In this repo | `srcscope_counting`'s 9 checks vs `count_lines` | `srcscope_runs` launching the *real built CLI* |
| Inputs | synthetic text in a string | real argv, real files, real sockets, real processes |
| Speed | milliseconds | milliseconds to seconds |
| Failure meaning | "this function's rule broke" | "the pieces don't cooperate (or the environment changed)" |

Day 1's smoke test is *already* an integration-grade test — it proves the CLI builds and exits cleanly. The roadmap then upgrades it in small steps: Milestone 01 adds argument behavior (`add_test(NAME srcscope_args COMMAND srcscope a b c)`), Milestone 02 gives it a real file to read.

### The script for real-file tests (Milestone 02 shape)

An integration step for file-based behavior, as a *registered command* (no C++ edits needed if the CLI is already correct for it), in three acts:

1. **Arrange** — create the input deterministically: a temp dir with a known file (fixture from Part M, or a shell one-liner in `COMMAND tempfile ... && ...` — prefer the C++ fixture; nested shell logic ages badly).
2. **Act** — run the program against it: `add_test(NAME srcscope_counts_file COMMAND srcscope <path>)`.
3. **Verify** — today's mechanism: exit code + (when the behavior needs it) *the program's own output*, checked by hand once and then pinned in the milestone's Done-block proof ([log.md](log.md)); a future milestone introduces *deeper* verification (unit-grade checks that read the file and inspect the counts) rather than exit-only.

The honest limit to name: exit-code-based integration tests can't see *what got printed*. That's fine at Phase I's pace — and it's exactly the corridor that later "unit-grade" tests of *counting functions on temp files* close: you'll test **the processing** as a unit (given bytes → returns counts) and **the CLI** only as integration (run it on one known file, exit 0, expected exit codes).

### Never depend on the machine, only on your fixtures

A failing-on-one-machine test is usually a *environment* bug, not a code problem. The rules that make this repo's tests portable:

| Rule | Why |
|:-----|:----|
| Everything under `std::filesystem::temp_directory_path()/...` | The repo dir never holds test leftovers; the temp root is writable for everyone |
| Every fixture path is *derived, not guessed* (`temp_directory_path() / std::to_string(counter_++)`) | A hardcoded `/tmp/my_test` collides across concurrent runs/users |
| All *fixtures* are created and swept **inside the test or its RAII guard** | An earlier test or a crashed run must never poison a later one |
| Time, dates, hostname, `$USER` never enter *paths* or anything compared by tests | Those differ across machines by design |
| Never couple a verdict to the machine's *locale* or *timezone* | `LANG`/`TZ` differences break verdicts across machines |
| Absolute vs relative path printing: choose one rule and document it | Windows-vs-POSIX separators are a *future* portability item, not a silent accident |

### Cleanup — the three levels, in order of reach

1. **RAII guard destructor** (fixture): cleans on normal scope exit — covers both passed and failed checks.
2. **Sweep-at-construction**: the *next* run of the same suite cleans leftovers its crashed predecessor left (already in the TempDir recipe).
3. **Never register a manual "cleanup test" that must run after another test** — that's an order dependency begging for failure (Part M).

### The repo's own planned integration-test shapes (mapped to their upcoming phases)

- **`srcscope`** (Phase I): argument handling; unreadable file behavior; counting on temp files (M02/M03).
- **`tinycore`** (Phase II): *differential* tests — run the same operations on `tinycore::Vector` and `std::vector`, compare answers (a Week 12 acceptance criterion).
- **`forge`** (Phase III): invoke a *real compiler* in a temp project; assert a clean exit and the artifacts' shape; missing-compiler failure path in a safe, harmless way (M15–M16).
- **`termkit`** (Phase IV): the *fake terminal* — a renderer + in-memory surface — so renders are asserted without a real terminal (Week 28); quit/restore paths.
- **`httpkit`** (Phase V): parser tests *without sockets*, then true loopback integration tests: a server on `127.0.0.1` + short-lived port, a real client connecting locally (M33+), with timeouts so a stuck case is a *failure*, not a hang.

---

<a id="part-o--troubleshooting-encyclopedia"></a>
## Part O — Troubleshooting encyclopedia *(Advanced; find your symptom, take its recipe)*

Each entry: symptom (what you actually see) → meaning → likely causes → diagnose → fix → prevent. All error strings below are *real captures* from this repo's environment (GCC/Clang, CMake 4.x, Make/Ninja), with paths shortened to `...` for readability.

### The framing rule

When a test misbehaves, first name *which layer* failed: **configure** (cmake writing recipes), **build** (compiler/linker), **test** (your binary's checks), or **runner** (ctest's view). Every symptom below says which layer owned it.

---

**O1. `No tests were found!!!` · ctest reports zero tests.** *(runner)*

- **Meaning:** ctest read the build directory's registry and found nothing registered.
- **Likely causes:** (a) configured `build/` without any `add_test` — nothing registered anywhere; (b) a component's tests are in the guard: `BUILD_TESTING=ON` is *off* (someone passed `-DBUILD_TESTING=OFF`); (c) you're pointing at the *wrong build directory* (e.g. output printed a different `Test project <path>` than you assumed).
- **Diagnose:**
  ```bash
  ctest --test-dir build -N                                  # what does ctest see?
  grep -c add_test build/CTestTestfile.cmake                  # registry entries actually written (0 = configure didn't see them)
  grep "BUILD_TESTING" build/CMakeCache.txt                   # what state was testing left in?
  ```
- **Fix:** add/restore the `add_test` line (Part E §3's recipe), or re-configure with testing enabled (`cmake -B build -DBUILD_TESTING=ON` — configure lines, not source edits), then rebuild.
- **Prevent:** after any CMake edit that adds tests, the very next command is `ctest --test-dir build -N` — *list before claiming*.

**O2. CTest lists fewer tests than you registered.** *(runner)

- **Meaning:** some registrations never reached `CTestTestfile.cmake`.
- **Likely causes:** an `add_test` inside an `if(BUILD_TESTING)` block skipped at configure time (stale configure with testing off), a *commented-out* add_subdirectory in the root (the component's tests were never read), or your `add_test` sits after an early `return()` in a guard.
- **Diagnose:** `grep -n "add_subdirectory" CMakeLists.txt srcscope/CMakeLists.txt` — is the path wired? Then `rm -rf build && cmake -S . -B build && ctest --test-dir build -N` — a clean configure is the honest base.
- **Fix:** restore the wiring; reconfigure; re-list.
- **Prevent:** wire additions as a trio — recipe file + `add_subdirectory` + guarded `tests/` — and rerun `-N` immediately (tour.md §Recipe A's checklist).

**O3. Test "executable not found" when ctest runs it.** *(runner → build layer)*

- **Meaning:** the command ctest resolved is a path that doesn't exist — usually the target name inside `add_test(COMMAND ...)` is misspelled, or that target was never built.
- **Diagnose:**
  ```bash
  ctest --test-dir build -V -R <failing-name>    # the exact command ctest intends to run
  find build -name <expected-binary>             # does it exist at all?
  cmake --build build --target help              # what targets does the recipe claim?
  ```
- **Fix:** correct the name inside `add_test(COMMAND <target> ...)` to a *real* target name, and build first (`cmake --build build`) before ctest. CMake's `add_test` accepts non-target names too (it can run raw programs) — if you meant a target, spell it exactly.
- **Prevent:** same-name rule for target vs test name is a *typo* here: `add_test(NAME srcscope_test COMMAND srcscope_tests)` — one letter apart; -V shows the actual command; typo fails loudly at ctest, not quietly at configure.

**O4. `undefined reference to '...'` at build time.** *(build — the linker)*

- **Meaning:** some `.o` file *promised* a function's existence (its declaration came from a header), but no object defines it, or its library wasn't linked.
- **Real capture** (test executable compiles but its link fails because temp CMake lost `target_link_libraries(srcscope_tests PRIVATE srcscope_core)`):
  ```text
  /usr/bin/ld: CMakeFiles/srcscope_tests.dir/counting_test.cpp.o: in function
      `test_empty_text_has_zero_lines()':
  .../srcscope/tests/counting_test.cpp:41:(.text+0x12c): undefined reference to
      `count_lines(std::__cxx11::basic_string<char, ...> const&)'
  /usr/bin/ld: ... more undefined references to `count_lines(...)' follow
  ```
- **Likely causes:** (1) the file defining the caller function isn't in `add_library`/`add_executable`'s source list; (2) the target that calls it lacks `target_link_libraries(... <library>)`; (3) declaration/definition *names* differ (a header says `count_lines`, the .cpp says `countLines`).
- **Diagnose:** read the *first* undefined symbol: which *caller* was it in? Find where that caller's library/exe is declared. Read its `target_link_libraries`.
- **Fix:** add the missing source file to its target, or add the library link (authentic recipe: `target_link_libraries(srcscope_tests PRIVATE srcscope_core)`).
- **Prevent:** new test *files* that call shared functions always go with the *link* line in the same edit.

**O5. `fatal error: <header>.hpp: No such file or directory`.** *(build — the compiler)*

- **Meaning:** a `#include` inside a translation unit cannot resolve to a real file path.
- **Real capture:**
  ```text
  .../srcscope/tests/counting_test.cpp:16:10: fatal error: counting.hpp: No such file or directory
  #include "counting.hpp"
  ```
- **Likely causes:** (1) the *library* carrying the header's include path is unreachable — e.g. its `target_include_directories(... PUBLIC ...)` line missing, or the *consumer's* link to that library missing (the include path travels with the link — that's why this error surfaced in Part G's table); (2) typos in the `#include` text; (3) a header got renamed or moved without updating its includers.
- **Diagnose:** `find . -name <header>` — exists? Then `grep -rn "target_include_directories" <containing>/CMakeLists.txt` and the consumer's `target_link_libraries` row.
- **Fix:** add the PUBLIC include dir (or the missing link), rebuild.
- **Prevent:** the two CMake lines ship *together*: a new header under `src/` gets its dir visibility *and* at least one consumer editorially linked before any of them compiles.

**O6. Compiler errors *inside* a test macro (e.g. within `CHECK(...)`).** *(build — the compiler)*

- **Meaning:** your expression mis-compiles; the macro wraps it so error text shows the wrapped form.
- **Diagnose:** compile just the *expression* at its own file top, standalone — often one real fix, no macro involved (a missing semicolon inside a lambda-fragment; mismatched parens; unknown type because a header wasn't included).
  ```bash
  g++ -std=c++20 -Wall -Wextra -Wpedantic -c /tmp/isolated_expr.cpp
  ```
- **Fix:** the standalone error points at the expression's real line; fix there.
- **Prevent:** the CMake compile step gives the *same* diagnostics you'd have in isolation — use the *first* error line only; `grep -A3 "error:"` then stop reading and fix.

**O7. CMake configure itself errors.** *(configure)*

- **Real captures we can quote:**
  ```text
  # two targets with the same name:
  CMake Error at CMakeLists.txt:6 (add_executable):
    add_executable cannot create target "srcscope" because another target with
    the same name already exists. ... See documentation for policy CMP0002 ...

  # duplicate test NAME in one directory:
  CMake Error at CMakeLists.txt:5 (add_test):
    add_test given test NAME "same" which already exists in this directory.

  # configuring from a directory without CMakeLists.txt (you are not where you think):
  CMake Error: The source directory ".../wrong-place" does not appear to contain CMakeLists.txt.
  ```
- **Meaning:** contract violations, not ambiguous behavior — CMake refuses to configure and names the collision.
- **Likely causes:** copy-pasted `add_executable` twice; two `add_test` with identical NAMEs; invoking cmake from a directory that wasn't the repo root (`-S .` selecting the *wrong* tree).
- **Diagnose:** read the *whole* first error block (location line + the line above it naming the file and its line number).
- **Fix:** remove one of the duplicates, correct the name, or `cd` to the repo root and configure against the intended `-S` path.
- **Prevent:** new component boilerplate via tour.md's *Recipe A* rather than repasted lines.

**O8. The build directory "belongs to somewhere else".** *(configure — a stale or moved cache)*

- **Meaning:** `build/CMakeCache.txt` remembers decisions (build type, generator, toolchain, options) *and* its own absolute path. Configuring a directory that moved (or copying build trees around) breaks that memory.
- **Real capture:**
  ```text
  CMake Error: The current CMakeCache.txt directory .../elsewhere/b/CMakeCache.txt
  is different than the directory .../b where CMakeCache.txt was created.
  This may result in binaries being created in the wrong place. ...
  ```
- **Fix:** delete the build dir (`rm -rf build`) and configure again. The cache is regenerated-from-nothing every time ([build.md §3](build.md)).
- **Prevent:** one build directory per combination (Debug vs Release vs sanitizers), and never move `build/` around — treat it as disposable.

**O9. Test framework not found / `GTest::gtest` unresolved.** *(configure/build — after a roadmap milestone adds a framework later)*

- **Meaning:** you're referencing a dependency the project *doesn't actually contain yet*.
- **Diagnose:** `grep -rn "FetchContent\|find_package\|GTest" .` — a framework in the field today's recipes? (None are, by design — Part F.) Read *this repo's own* CMake as it is; no phantom targets.
- **Fix:** don't add a framework *because a chat or forum had better flows*; the roadmap decides when (Part F's dependency selection gets its answer with the milestone that asks for it).
- **Prevent:** install nothing "just in case"; *the test suite's dependency is C++20 + CMake + CTest alone* today.

**O10. FetchContent download failure.** *(configure — only after a framework milestone)*

- **Symptom (this will not be reachable until a framework milestone exists; note kept for after):**
  ```text
  CMake Error: ... error: unable to access 'https://github.com/google/googletest/...'
  ```
- **Meaning:** the pinned dependency cannot be fetched — not configured without a network connection, or the URL/pin is wrong.
- **Fix:** (a) confirm the exact pin and URL against the milestone's recipe; (b) if offline: pre-populate [CMake's FetchContent staging dir](https://cmake.org/cmake/help/latest/module/FetchContent.html) or switch to the vendored copy (Part F's trade-offs) — the *pinned version* is the invariant worth reproducing in your machine's mirror; (c) ask for a documented *offline procedure* before changing the dependency mechanism.
- **Prevent:** CI and local dev should agree on the *same set* of external fetches; keep the *pin* stable across the repo's registered mirrors.

**O11. Passes locally, fails in CI/another machine.** *(runner — the environment differences)*

- **Likely causes:** locale/timezone/CWD/HOME dependencies (Part N's table above); clock granularity along a *filesystem boundary* (`std::filesystem::last_write_time` compared to a value a *different filesystem* wrote — one may tick by seconds, the other by nanoseconds); path-length and case-sensitivity assumptions (Linux vs macOS); iteration order of unordered containers (`std::unordered_map` iterates in hash-dependent, machine-dependent order — never assert an order you can't derive); *different build types* (a Release build is a different program than Debug — see Part E's story and O15).
- **Diagnose:** the failing job's log → find *what exactly differed*. Then bisect along one axis at a time: same code on a different machine (env?), same machine, different build type (optimizer?), same inputs twice (nondeterminism?). Only one of those is always your real bug.
- **Fix:** make fixture output byte-stable; sort/hash anything unordered before comparing; pin times via cached *mockable* clock (not wall clock) when needed.
- **Prevent:** test env-independence rules as a checklist item ([Part N](#part-n--integration-tests-and-real-world-testing)) appended before "marking complete" entry in [checklist.md](../checklist.md).

**O12. Passes alone (`ctest -R <name>`), fails in the full suite.** *(runner — shared state between tests)*

- **Likely causes:** another test leaves a *temp path* your test assumed fresh; a shared static counter is no longer in its initial state because a previous test touched it; a global file made "once" by an earlier test is reused by this one; stdout/stderr interleaving differs when suites run in parallel.
- **Diagnose:** run the suite with `--output-on-failure`; then run *only the neighboring tests in registry order* (`ctest -R "<a>|<b>"`) and watch whether the *combined* run breaks it.
- **Fix:** cut the shared state; every test's fixtures are sandboxed (TempDir pattern; Part M's isolation rules).
- **Prevent:** tests create their own inputs from scratch; no test may count on another test's leftovers.

**O13. A test is intermittent ("flaky").** *(build or runner — real nondeterminism)*

- **Common shapes:** (a) concurrency (Part L); (b) `std::unordered_*` iteration order (don't assert it); (c) uninitialized memory — ASan/UBSan (or plain luck on one machine) make the value *seem* stable — until the next compiler revision shifts the stack. A nondeterministic test against real UB is doing *exactly the right job*: fix the code, not the suite.
- **Diagnose:** `ctest --test-dir build --repeat until-fail:20 -R <name>` pins whether variance exists; if it does, the cause is real nondeterminism (Part L's guidance) — not timing.
- **Prevent:** no wall-clock assumptions; no unordered-iteration assumptions; a sanitizer run before declaring any suite stable.

**O14. A test hangs indefinitely.** *(runner — you will want to break out by hand)*

- **Meaning:** the test process doesn't exit; classic causes are *waiting* on something that never arrives (a deadlock from Part L; `std::cin` reads; socket closes not arriving in another thread).
- **Diagnose:**
  ```bash
  ctest --test-dir build -V -R <hung>            # see it start; then Ctrl-C the run — the process list is in `ps`
  ps aux | grep <test-binary-name>               # which pid is alive?
  gdb -p <pid> --batch -ex "bt" 2>/dev/null      # where's it parked? (a free lecture, do that shamelessly)
  ```
- **Fix:** fix the *product code* meaning of "hang" (Part L's mutex/condition variable discipline), or give the test a `--timeout` (a ctest runner option; per-test `set_tests_properties(... TIMEOUT 60)` is the finer version of the same control) so CI doesn't burn time on the chance it's *eventually* fine.
- **Prevent:** loops that wait for *event* signals (`std::latch`, condition-variable wakeups) never "wait a bit".

**O15. A test fails only in Release mode.** *(build — the optimizer changed observable behavior)*

- **Likely causes:** (1) **assertions compiled out** — plain `assert()` from `<cassert>` vanishes with `NDEBUG` (which Release defines); a suite that relies on it then passes *vacuously*. This repo's check()-based tests are built to survive both build types (Part E); this entry is the *look behind that rule* if you inherit assert-style tests. (2) **UB that Debug hides, Release exposes** (uninitialized locals: Debug fills them with a distinctive byte pattern, Release doesn't; signed overflow behaves differently across optimization levels — UBSan catches these at the use site). (3) floating-point reassociation/simplification.
- **Diagnose:**
  ```bash
  grep CMAKE_BUILD_TYPE build/CMakeCache.txt       # which directory was actually built in? (mismatches happen)
  # Release + UBSan, in a fresh directory:
  cmake -S . -B build-ubrel -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_FLAGS=-fsanitize=undefined -DCMAKE_EXE_LINKER_FLAGS=-fsanitize=undefined
  cmake --build build-ubrel && ctest --test-dir build-ubrel --output-on-failure
  ```
- **Fix:** run the same check()-style suite in *both* build types (Part E) and fix the UB the sanitizer names. Never let the suite's meaning change between build types — "passes in Release" must mean the same thing as in Debug.
- **Prevent:** before declaring a milestone done, run the suite in **Debug** (default) *and* **Release** — the [checklist](../checklist.md) demands both.

**O16. A test fails only under sanitizers.** *(build — real bug found)*

- **Meaning:** an ASan/UBSan instrument caught an *actual memory or UB event* that no test CHECK had the reach to detect (out-of-bounds access, use-after-free, signed overflow, uninitialized use…).
- **Diagnose:** the report is self-diagnosing: two stacks — where the violation happened, and where the affected memory came from (Part K's full anatomy). Read them top-down, fix the code, rerun.
- **Fix:** resolve the violation the tool names; rerun the whole suite; note the resolved finding in `PROGRESS.md` if it matters to the milestone's ship state.
- **Prevent:** sanitizer builds accompany *every memory-touching* phase here (Phase II onward, per the roadmap), so findings surface when they can still be fixed cheaply.

**O17. Tests pass... but nothing was tested.** *(the green lie; runner + build)*

- **Real case study:** Part E's "one green that lies" — my broken source didn't compile; a pipe (`| grep`) masked the *build* failure; ctest ran the *previous* binary; everything passed. Everything.
- **Symptoms of the lie:** every test green while you *know* you just changed something; stdout lines look *older* than what should be expected (e.g. old printed byte counts); a build "completes" in an unrealistically short time.
- **Diagnose:** break the *known-changing* path deliberately (Part E Direction 1): if a wrong CREATE-value DOESN'T fail the suite, the suite isn't testing the live source — the binary is stale. Confirm with a from-scratch pair of builds.
- **Fix:** `rm -rf build && cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure` — the clean-rebuild ritual ([build.md §7](build.md)).
- **Prevent:** never chain critical builds through pipes that can eat exit codes (`x && y` only — no `x | grep`); the pipeline loses the build's nonzero status and you inherit a green lie.

**O18. Different results on different machines.** *(runner — machine-specific dependence)*

- **Likely causes:** width assumptions on built-in integer types (a plain `int` is *at least* 16 bits by the standard — prefer `<cstdint>`'s exact-width types like `std::uint32_t` where width matters); endianness when data crosses machines as bytes (this repo is little-endian x86/ARM in practice — a future item, documented not assumed); *filesystem* semantics across machines (case sensitivity, symlink behavior — Linux vs macOS differ); clock granularity differences when timestamps enter verdicts (O11).
- **Diagnose:** run the same suite on both machines, then narrow to *one test*, then bisect *that test's* expectations.
- **Fix:** spell the width (`std::uint32_t` etc.), sort/hash unordered data before comparing, and replace wall-clock comparisons with *mockable* time when time *is* the business logic.
- **Prevent:** tests run against Part N's environment-independence rules; "no wall-clock / no locale / no unordered-iteration order in verdicts" is the standing refresher.

**Also worth having ready:** `ctest: command not found` — CMake wasn't installed where PATH can see it (Part C re-covers the package); and "the test that got renamed, then never re-registered" — a `#`-done name change in `add_test(NAME ...)` is a *registry* change; re-run `ctest -N` and update anything that filters by the old name (scripts, CI filters, your muscle memory). When in doubt about *where you are*, `ctest -N` prints its `Test project <path>` line first — the fastest "which project am I actually testing?" check ([Part D §4](#part-d--know-the-project-before-testing-it)).

---

### The one-move fix for most of these

`rm -rf build && cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure` — from a *clean* build directory your recipes are re-derived from scratch, and a stale binary, a stale registry, or a stale test status cannot outlive it. It fixes cache staleness (O8), O17's lie, and half of O1's "why aren't my tests running" (Part O #1, #2, #17), all with no code edits. When that *doesn't* fix your symptom, the symptom is a real one, and this encyclopedia is exactly why the long menu above paginated its diagnoses separately.

---

<a id="part-p--code-coverage"></a>
## Part P — Code coverage *(Advanced; optional tooling, used when a milestone asks)*

### What coverage means

**Coverage** answers: of the code you built, how much did the tests *execute*? Two flavors matter:

- **Line coverage** — did each executable line run, yes/no.
- **Branch coverage** — at each decision (`if`, `&&`, loop), did *both* directions get exercised?

The numbers in .Reports — the controlled term — say *what ran*, never *whether the checks were right*. A fully "covered" line can hold an unchecked bug; a double-checked *unimplemented feature* still counts as covered. Coverage is a *hole finder*, not a correctness measure.

### The honest expectation: what 100% does, doesn't

Getting 100% means no line went unvisited. It does *not* mean: every input class arrived at every line (remember the classes from Part I), every *pair* of branch directions was exercised (that needs branch coverage), or the *checks* were strong — one vacuous `CHECK(true)` raises a line's coverage just as much as a contract-level test. That's why the repo's discipline treats coverage as a *gap finder* and never a goal number: chase *assertion quality* first (Part I), use coverage to find unvisited paths, and document the leftovers you deliberately skipped ("documented limitations" beats invisible holes).

### The tooling route for this repo (GCC/Clang)

The toolchain's built-in route needs nothing beyond GCC/Clang. First enable coverage *for a fresh build directory* (not your daily one):

```bash
cmake -S . -B build-cov -DCMAKE_CXX_FLAGS=--coverage -DCMAKE_EXE_LINKER_FLAGS=--coverage
cmake --build build-cov
ctest --test-dir build-cov --output-on-failure
```

Coverage data lands next to each object file (`.gcno` at compile time, `.gcda` at test run time). Then ask `gcov` (ships with GCC) to produce annotated reports.

**For one file's summary+annotation** (real behavior on this machine, in `build-cov`):

```bash
cd build-cov/srcscope/CMakeFiles/srcscope_core.dir/src
gcov -b counting.cpp.gcno        # -b = also report branch coverage
```

```text
File '.../srcscope/src/counting.cpp'
Lines executed:100.00% of 8
Branches executed:100.00% of 10
Taken at least once:100.00% of 10
Creating 'counting.cpp.gcov'
```

The per-line annotated report (`counting.cpp.gcov`) looks like:

```text
        8:    3:int count_lines(const std::string& text) {
       62:    7:    for (char c : text) {
branch  2 taken 85%
       46:    8:        if (c == '\n') {
branch  0 taken 20% (fallthrough)
branch  1 taken 80%
        9:    9:            ++lines;
```

How to read: a count (`62:`) tells you *how many times* that line ran across the whole suite; `branch N taken…` lines show whether both directions were exercised. Our own suite got counting.cpp to 100% lines *and* branches — good coverage, *because the tests were designed against the rules*; the same report would also include inlined standard-library template lines with messier numbers — you use coverage to check *your* files' reports, not to judge your project by `<string>`'s template innards.

(The output surfaces a *lot* per file; when presentation matters — e.g. reports for a release milestone later — an installed `lcov`/`genhtml` or `gcovr` turns these into browsable pages. Both are uninstalled here and currently *optional*, not dependencies, and neither is needed before a milestone calls for it.)

Reminder: coverage only measures *in the binary that ran*. Coverage of `build-cov` does not speak for a sanitizer build or a Release build — those run their own binaries (and each is worth its own run when a milestone explicitly asks for it; Week 39's hardening milestone is where this route is applied whole-suite).

---

<a id="part-q--testing-in-ci"></a>
## Part Q — Testing in CI *(Advanced; where the roadmap gets to "automatic")*

### Why CI exists for tests

Two reasons, in the repo's own terms: **it runs the whole suite on every change** — nobody's memory of "I'll run tests later" can drop the ball — and **it runs in a clean environment**, which converts *works-on-my-machine* into a recurring measurable state (O11 becomes a fixed daily fact instead of a mystery). It also keeps 40-week-long projects honest at the *end*: a "clean checkout can build and test the projects" is exactly the Week 40 release criterion.

### Clean-environment setup, in the reproducible order

CI never assumes memory; every job runs *the same documented commands Part C/H produce*:

1. Fresh runner (no caches carried in).
2. Install: compiler + CMake + Ninja or make (Part C's package list per distro; pin what you can).
3. `cmake -S . -B build -DCMAKE_BUILD_TYPE=<type>` — Debug is the default; Release and sanitizer jobs use their own directories.
4. `cmake --build build` (parallelism if the runner allows it).
5. `ctest --test-dir build --output-on-failure` — the *same command* you run locally; no special "CI mode" to maintain.
6. **Report failures loudly:** with `--output-on-failure`, a failed test's *own* stdout lands directly inside the failing job's log. Leave the ctest exit code to the runner's verdict step; a nonzero ctest exit ends the job red.

### A ready-to-apply GitHub Actions shape (add only when wanted)

The repository has **no CI configuration today** (no `.github/` directory; the roadmap's CI mentions are Week 39–40 acceptance criteria). When you or a future milestone want it, the minimal honest starting shape is:

```yaml
# .github/workflows/test.yml — proposed shape for a future milestone; NOT committed today.
name: tests
on: [push, pull_request]
jobs:
  test:                      # one job = one clean environment
    strategy:
      matrix:                # both supported compilers on the documented Linux target
        compiler: [g++-12, clang-16]
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: sudo apt-get update && sudo apt-get install -y cmake build-essential ninja-build
      - run: cmake -S . -B build -G Ninja && cmake --build build
      - run: ctest --test-dir build --output-on-failure
  sanitizers:                # the roadmap asks "where supported" — one asan/ubsan job covers that
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: sudo apt-get update && sudo apt-get install -y cmake build-essential ninja-build
      - run: cmake -S . -B build-asan -G Ninja -DENABLE_SANITIZERS=ON && cmake --build build-asan
      - run: ctest --test-dir build-asan --output-on-failure
```

Why a *matrix* and a *separate sanitizer job*: the matrix proves the two supported compilers both build the suite; the sanitizer job keeps the slower instrumented build from inflating the fast job's time while remaining a *go/no-go* requirement the same way.

### Reproducibility — the "same commands both places" rule

The commands CI runs must be *the ones documented in this repo*: the four-line block from Part H. That's not idealism; it's the reason a failing CI log can be reproduced locally bytyping *the same lines*. Tolerance for a divergence is fine if it's deliberate (an extra `-D` flag for a CI-runner-specific reason) — *it gets documented in that workflow's comments*, or the local script that mirrors it. Both builds should make the same claims.

---

<a id="part-r--practical-exercises"></a>
## Part R — Practical exercises *(progressive; the roadmap's actual milestones, no new dependencies)*

Progression rule: exercises unlock with the milestone that provides their subject. Advanced-tagged ones reference future phases and are *optional until their milestone*; the first three are fully available today. Every exercise below follows the same shape: objective → files → the behavior to pin → how to run → acceptance criteria → common mistakes → optional challenge.

### Exercise 1 — your first *hand-written* test *(Beginner; available today)*

- **Objective:** add a brand-new test function to the existing suite end-to-end (no CMake edits needed — this teaches the *inner* registration).
- **Prerequisites:** Part E complete; repo green.
- **Files to edit:** `srcscope/tests/counting_test.cpp` (one function + its `main()` call).
- **Requirements:** pin one rule that the file's existing tests don't cover yet — e.g. a line containing *only spaces* still counts: `count_lines("   ") == 1` (the text has no trailing newline, and rule 2 does not exclude non-text characters).
- **Edge cases:** a line with a space and no newline; a *tab* instead of a space.
- **Run:** `cmake --build build --target srcscope_tests && ./build/bin/srcscope_tests` then full `ctest --test-dir build --output-on-failure`.
- **Acceptance:** `Total Tests: 2` unchanged; your `ok line count_lines("   ") == 1`; suite green.
- **Common mistakes:** adding the function but forgetting the `main()` call (this test never runs — verify it appears in the direct run's output); a typo inside the CHECK expression quietly changes the pinned rule — read the passing `ok` line as printed and confirm it says what you meant.
- **Optional challenge:** make the test fail on purpose by changing `count_lines()` for a *temporary* moment (e.g. count only non-empty text lines), watch it localize, restore.

### Exercise 2 — argument handling as a pure CTest test *(Beginner; available today)*

- **Objective:** wire a *new registered test* end-to-end (the full three wires: no C++ edit at all — a registration-only exercise).
- **Prerequisites:** Part E + the three commands section of Part G.
- **Files:** `srcscope/tests/CMakeLists.txt` — add one line:
  ```cmake
  add_test(NAME srcscope_args COMMAND srcscope a b c)
  ```
- **Requirements:** the CLI must accept arguments (it currently ignores them and exits 0 — this *pins* "arguments don't crash"; Milestone 01 later makes the assertion meaningful beyond the exit path).
- **Run:** `cmake -S . -B build && ctest --test-dir build -R srcscope_args --output-on-failure` (a recipe change is picked up by configure; the build step auto-reconfigures too).
- **Acceptance:** `-N` lists 3 tests; `-R srcscope_args` passes.
- **Common mistakes:** editing `CMakeLists.txt` while the `build/` directory is configured *before* re-running ctest (a recipe change needs a re-configure — the build does it for you, ctest alone does not); forgetting that `COMMAND srcscope` resolves to the *binary path*, so the test doesn't depend on your CWD.
- **Optional challenge:** give the program *opinions* temporarily (`return 2;` when it receives zero arguments) and watch `-R runs` fail while `-R args` passes — per-behavior names localize (the START.md taste experiment, in ctest form).

### Exercise 3 — a string utility's full behavior set *(available today; M02 seed)*

- **Objective:** write the *word-counting* twins of `count_lines` — a function you'll genuinely need in M02 — and give it a full contract-level suite.
- **Prerequisites:** Parts E + I (the seven questions).
- **Files to create:** `srcscope/src/counting.hpp` (+ declaration), `srcscope/src/counting.cpp` (implementation of `int count_words(const std::string& text)` — a *word* is any whitespace-separated token, per the roadmap's "whitespace-based, not linguistic" note), and *its own test file* `srcscope/tests/counting_words_test.cpp` with an `add_executable`/`add_test` pair (`counting_words_test` target + `srcscope_words` CTest name).
- **Requirements/example inputs:** `"one two three"` → 3; `""` → 0; `"   "` → 0 (spaces only, no words); `"a\nb"` → 2 (the newline is whitespace); no trailing-newline surprises — words don't care.
- **Edge cases:** duplicates (`"a a a"` → 3), tabs vs spaces equivalence, single word without separators.
- **Run:** your own target + ctest `-R srcscope_words`.
- **Acceptance:** all the suite's rules green; one *empty-class* test and one *boundary* test present; suite still green after the milestone's normal flow re-run.
- **Common mistakes:** deciding semantics *after* the tests (the contract ("whitespace-separated") must be in the header's comment first, tests after); creating a second main-style file without registering it (O1/O2).
- **Optional challenge:** implement `count_bytes` the same way and fold all three counters into one small documented family, keeping each *one rule per check*.

### Exercise 4 — a vector-like container *(Phase II, Week 6; Milestone 08)*

- **Objective:** pin `tinycore::Vector<T>`'s elementary invariants: growth, size/capacity relationship, indexing.
- **Prerequisites:** Phase II's containers arrive in [Roadmap.md §Phase II](../Roadmap.md) (Weeks 5–6); Part M's naming and isolation rules apply directly.
- **Requirements/example inputs:** empty at start; `push_back` grows; capacity never below size; out-of-range access rejected under a documented policy; duplicates allowed; destructor destroys every live element exactly once (spy classes from Part K).
- **Run:** `tinycore/tests/` target + ctest `-R tinycore` (it exists from Week 5 on).
- **Acceptance:** every elementary rule + at least one failure path + a differential test against `std::vector` (Week 12).
- **Common mistakes:** asserting *iterator* order invariants; forgetting a moved-from test (Week 7 turns it into a rule).
- **Challenge:** add a non-trivial element type (a spy object) to prove element construction/destruction pairing.

### Exercise 5 — copy and move behavior *(Phase II, Weeks 5–8; Milestones 07–08)*

- **Objective:** pin independence of copies, completeness of moves, and self-assignment safety.
- **Prerequisites:** Phase II's ownership chapters.
- **Requirements:** copy of a String/Vector yields two independent objects (each kept its own elements; changing one leaves the other untouched); move *transfers* ownership and leaves the moved-from state defined (Part J's moved-from rules); `x = x;` is safe and leaves the value intact.
- **Run:** `-R tinycore` and your element-spy counters (each rule should also check live-count before/after).
- **Acceptance:** the Week 8 "moves transfer without double-delete" criterion, plus every rule in its checks.
- **Common mistakes:** testing the *optimizer's* merge instead of the rule; forgetting that after a move the *source's* state is an independent contract (say what it is, then pin exactly that).
- **Challenge:** the rule-of-five test — for a type declaring a custom destructor, verify what the copy/move set *does*, rather than being silent about it.

### Exercise 6 — RAII and resource cleanup *(Phase II, Week 8; Milestone 10)*

- **Objective:** pin `tinycore::UniquePtr`'s exactly-once delete, prohibition-of-copy, null behavior.
- **Requirements/examples:** move construction/assignment transfer; both null-pointer and owned states in `reset()`; destruction visible through a counting deleter.
- **Run:** `-R tinycore`.
- **Acceptance:** *every* "no double delete / zero leak" claim, backed by the spy object pattern + LeakSanitizer build.
- **Common mistakes:** debating whether C++ "catches" double delete in Debug — don't: the *test* exists to catch it under *both* build types.
- **Challenge:** instrument the custom deleter and assert its call count by hand.

### Exercise 7 — file operations *(Phase I stays in scope, Milestones 02–03)*

- **Objective:** handle the file cases (miss/empty/no-trailing-newline/pedant size).
- **Requirements:** the CLI's counting on a TempDir fixture file; an unreadable file reported distinctly (M02); no crash on directories (M03 scanning).
- **Run:** a registered test per behavior (`srcscope_counts_file`, ...) over a fixture-based C++ temp file.
- **Acceptance:** every behavior + aggregated results naming the source file; complete per-behavior isolation; nothing leftover afterwards (fixture destructor).
- **Common mistakes:** tests assuming the CLI's current working directory (which is the *build* directory — Part M's rule) instead of a fixture path; asserting *absolute* path printing (machine-specific).

### Exercise 8 — process execution *(Phase III, Weeks 13–16; Milestones 15–16)*

- **Objective:** launch a compiler from C++ and *verify exit status + reaped child*.
- **Requirements:** a *successful* compile returns success; a *failing* one returns failure; a *missing* compiler reports harmlessly; pipes don't leak; *no shell command is assembled by concatenating untrusted arguments* (the whole criterion is in [checklist.md §7 forge — Process execution](../checklist.md)).
- **Run:** for a real temporary source file; assert on the return status.
- **Acceptance:** exit status handling proven for success + failure + missing-compiler; silence of the *hang* paths (a failed child never blocks).
- **Common mistakes:** reaching for `system()`/`popen()` shell-string assembly instead of explicit exec-with-argument-lists (the roadmap's own criterion: *no shell command assembled by concatenating untrusted arguments*); asserting *message content* that varies across compiler versions — prefer exit-status-level results.
- **Challenge:** enforce an upper ram/time budget while waiting for a misbehaving child.

### Exercise 9 — networking *(Phase V, Weeks 29–36; Milestones 32–38)*

- **Objective:** parser-level tests *without sockets* first, then loopback integration on real sockets.
- **Requirements:** the incremental `httpkit` parser reads *fragments* well (Week 32's acceptance criterion is exactly that); the finite parser states (request-line parse fails on garbage) are unit-tested *before* binding the real 127.0.0.1 port (M33+); no external network — only loopback, bound ports from an ephemeral range.
- **Run:** `ctest -R httpkit` per suite; `set_tests_properties(... TIMEOUT 30)` accompanies every socket-based test so nothing can hang the runner.
- **Acceptance:** fragmentation cases *and* framing cases; the brief and deterministic shutdown cases; bounded resource paths.
- **Common mistakes:** asserting *connection timing*, not event handling (Part L); leaving filters open across tests (slow suites, hidden dependencies).
- **Challenge:** run the suite under TSan and see if *additional* interleavings show anything the plain build missed.

### Exercise 10 — concurrent components *(Phase V, Weeks 36,39-40; Milestones 38–40)*

- **Objective:** producer-consumer bounded queue + graceful shutdown, with deterministic sync primitives.
- **Requirements:** the queue bounds (a blocked producer when full — with a defined policy), two consumers don't drop or duplicate (exact-preservation invariants); shutdown lets idle clients *finish, not hang*; all with `std::latch`/condition-variable handshakes (Part L), never sleep-based ordering.
- **Run:** the TSan build directory (a separate `build-tsan` directory with `-fsanitize=thread`, once its phase arrives) *plus* the plain builds (`ctest` all).
- **Acceptance:** invariants and shutdown all green plain *and* under TSan; no sleeps, no random waits.
- **Common mistakes:** intermittent "it passed" logic; betting on *scheduling* instead of *synchronization*; treating "no TSan finding" as proof (Part L's warning — design review + TSan, together).
- **Challenge:** run a stress loop with N producers/consumers and assert your *invariant* checks (not exact counts over time — leave history to the milestones that want it; M40 comments this in its acceptance criteria).

**Exercises 4–10 are labeled by their phase for a reason:** attempt them when the roadmap delivers their subject. Until then, "what would I need to have this?" is a good way to remember why *that* phase exists.

---

<a id="part-s--checklists-and-quick-reference"></a>
## Part S — Checklists and quick reference *(Beginner + every milestone)*

### The testing checklist (before marking any milestone complete)

This fits under the "Before marking complete" block of [checklist.md §2](../checklist.md); tick with evidence, not memory. Later-phase items are annotated with when they start applying.

```text
□ Build succeeds, from a clean build directory (rm -rf build; the four-line ritual)      [always]
□ Everything compiles — production code and tests, no warnings left unexplained           [always]
□ Every intended test is registered:  ctest --test-dir build -N shows the expected names  [always]
□ Normal cases covered (the seven questions' answer for question 1)                       [always]
□ Boundary cases covered (smallest, largest, first, last)                                 [always]
□ Invalid / unexpected input covered (or explicitly documented as out of scope)           [always]
□ Failure paths covered where practical (reported errors, not silent ones)                [always —
                                                                     practical from M02 file work on]
□ Resources cleaned up (fixture destructors run; no leftovers in the temp root)            [file/process phases]
□ Tests are independent (any one passes alone; suite passes in any order incl. --schedule-random)
□ Tests are deterministic (same run → same verdict; no sleeps/wall-clock in the verdict)   [always]
□ Each test is ONE behavior: the failure message names what broke without reading code     [always]
□ Documentation updated (contract in header comments; behavior changes in PROGRESS.md; Done block)
□ CI green where applicable (Part Q; applies when CI exists)
□ Any intentionally-skipped check has a written N/A reason — nothing hidden behind silence
```

Sanitizer and Release additions (Phase II onward):

```text
□ Suite run + green in Release (build-release)                     [from Phase II]
□ Suite run + green in a sanitizer build (-DENABLE_SANITIZERS=ON)  [from Phase II]
□ Threaded code additionally green under TSan                      [from Phase V]
```

### The quick command reference

```bash
#
# ── daily block (from the repo root) ────────────────────────────────────
cmake -S . -B build                                # configure (refreshes recipes + test registry)
cmake --build build                                # compile + link everything
ctest --test-dir build --output-on-failure         # run the whole suite, verbose on failure
#
# ── targeted ────────────────────────────────────────────────────────────
ctest --test-dir build -N                          # list registered tests (first 'why invisible?' check)
ctest --test-dir build -R counting --output-on-failure   # one test only (name regex)
ctest --test-dir build -V                          # everything, per-test commands + outputs shown
cmake --build build --target srcscope_tests        # rebuild one target
./build/bin/srcscope_tests                         # run a test binary directly (raw ok/FAILED lines)
#
# ── build types, each in its own directory ──────────────────────────────
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake -S . -B build-asan    -DENABLE_SANITIZERS=ON                 # ASan + UBSan
cmake -S . -B build-ninja   -G Ninja                               # optional fast generator
ctest --test-dir <that-dir> --output-on-failure                    # the suite per directory
#
# ── runner knobs (later use, here for reference) ────────────────────────
ctest --test-dir build --parallel <n>              # concurrent test execution (isolated tests only)
ctest --test-dir build --schedule-random           # order-independence discipline
ctest --test-dir build --timeout 60                # hang guard for the run
ctest --test-dir build --repeat until-fail:20 -R <name>   # chase intermittents
#
# ── reset ───────────────────────────────────────────────────────────────
rm -rf build && cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure
```

### Glossary (the words used in this guide, in one place)

| Word | Meaning here |
|:-----|:-------------|
| assertion / check | a stated comparison of expected vs actual whose failure is recorded |
| boundary | the exact edge of valid input (first, last, empty, exactly-one) |
| coverage | the fraction of code the test runs touched; a *gap finder*, not a goal |
| deterministic | same input → same verdict, always (no sleeps, clocks, hashes, races) |
| exit status | the number a returning program tells its caller; 0 = success ritual |
| fake / stub / mock | hand-written stand-ins with growing verification power (Part M) |
| fixture | the shared setup/teardown several tests need, as RAII (Part M) |
| flaky | a test whose verdict changes between equivalent runs — treat as a bug |
| framework | answers: declare/assert/report/discover tests (Part F) |
| harness | the less common name for what a *runner + registration* together do |
| regression | formerly-working behavior broken by a later change (Part A) |
| registration | `add_test(...)` making ctest know a test exists (distinct from building it!) |
| runner | the tool that launches registered tests and reports verdicts (CTest here) |
| sanitizers | compile-time instrument pairs that catch memory/UB/races at run time (Part K–L) |
| test executable | the compiled program containing your checks; a normal binary, judged by exit code |
| test suite | the whole collection, run by `ctest --test-dir <dir>` |
| undefined behavior (UB) | what C++ leaves unspecified — can't be *tested*, must be *avoided or caught* (sanitizers) |
| unit / integration / system / e2e | intensity levels of tests (Part A's table) |

---

· **Related reads:** [build.md](build.md) — how to run, what's on disk, the clean-rebuild ritual · [tour.md](tour.md) — what every CMake line means · [lost.md](lost.md) — the symptom → remedy map · [checklist.md](../checklist.md) — which behaviors deserve a test, ticked with evidence · [Roadmap.md](../Roadmap.md) — the milestone whose acceptance criteria you are helping pass · [operation_manual.md](../operation_manual.md) — the session loop this testing loop serves

