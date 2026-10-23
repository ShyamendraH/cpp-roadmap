# The tour — every file, and what the lines in them mean

Read this once — the first time a CMake file or repo structure feels unexplained. After that it's a lookup: each section stands alone.

"You are here": [README](../README.md) → (day 1 done) → **tour**

**This file's job:** decode the playground — every file's purpose, every CMake line's plain-English meaning, the vocabulary, and the recipes for growing the repo without breaking anything. How builds *execute* is [build.md](build.md); how tests *run* is [tests.md](tests.md).

---

## 1. The whole repo, one screen

```text
Cpp-practice/
├── CMakeLists.txt            ← the master recipe (section 3)
├── srcscope/                 ← Phase I component — your playground target
│   ├── CMakeLists.txt        ← recipe: how srcscope gets built (section 4)
│   ├── src/main.cpp          ← the only C++ so far (section 5)
│   └── tests/CMakeLists.txt  ← recipe: declares tests (section 6)
├── .gitignore                ← keeps build output out of git (section 7)
├── README.md                 ← the front door and file map
├── START.md                  ← day-1 walkthrough
├── Roadmap.md                ← the 40-week plan (46 milestones)
├── guides/                   ← tour (this), build, tests, log, lost
├── operation_manual.md       ← session rhythm & recovery procedures
├── checklist.md              ← verification ledger (tick with evidence)
├── PROGRESS.template.md      ← your session dashboard, starter copy
├── BACKLOG.template.md       ← your parking lot, starter copy
└── .commandcode/             ← tooling config, gitignored — ignore it
```

`build/` will appear when you configure. It's generated and gitignored — a scratch area, safe to delete at any time.

---

## 2. The 60-second mental model of CMake

Three ideas explain every line you'll meet:

1. **CMake is a recipe reader, not a builder.** You write recipes in `CMakeLists.txt`. Running `cmake` translates them into actual commands (compiler and linker invocations). Running the build executes those commands.
2. **Targets are the nouns.** "Build a program named `srcscope` from this source file." One recipe line, one target. Tests are registered to be run later, like ingredients lists read from a pantry.
3. **Directories inherit.** A subdirectory's `CMakeLists.txt` runs *as part of* the parent configure — it sees the parent's settings (C++ standard, warnings, output folder) unless it overrides them.

Keep those in mind and the files below read like instructions, not incantations.

---

## 3. The root `CMakeLists.txt` — explained line by line

```cmake
cmake_minimum_required(VERSION 3.20)

project(cpp_systems_builder
    VERSION 0.1.0
    DESCRIPTION "C++ Systems Builder: srcscope, tinycore, forge, termkit, httpkit"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "..." FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_compile_options(-Wall -Wextra -Wpedantic)

option(ENABLE_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
if(ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

include(CTest)

add_subdirectory(srcscope)
# add_subdirectory(tinycore)  # Week 5
# add_subdirectory(forge)     # Week 13
# add_subdirectory(termkit)   # Week 21
# add_subdirectory(httpkit)   # Week 29
```

| Line(s) | What it does — in plain words |
|:--------|:------------------------------|
| `cmake_minimum_required(VERSION 3.20)` | A compatibility agreement. If someone runs an older CMake, it stops with "you need CMake 3.20". Every `CMakeLists.txt` starts with one — required, non-negotiable. |
| `project(...)` | Names the whole build ("cpp\_systems\_builder"), a version for packaging, a description, and `CXX` = "compile C++". The `LANGUAGES CXX` matters: without it CMake would also hunt for a C compiler. |
| `CMAKE_CXX_STANDARD 20` | Turns into the compiler flag `-std=c++20`. All targets below inherit it. |
| `..._REQUIRED ON` | Fail loudly if the compiler can't do C++20, instead of silently degrading. |
| `..._EXTENSIONS OFF` | Use exact `-std=c++20`, not gnu-style `-std=gnu++20`. Keeps code portable and behavior predictable. |
| the build-type `if` | If the user never said Debug/Release, default to **Debug** (unoptimized with debug info — the friendly mode for learning). The `FORCE`+`CACHE` combo makes the choice stick across re-runs. |
| `CMAKE_RUNTIME_OUTPUT_DIRECTORY` | "Put every final executable into `build/bin/`, whatever subdirectory it was born in." That's why the run command is always `./build/bin/srcscope`. `${CMAKE_BINARY_DIR}` is the build root (the `-B build` folder). |
| `add_compile_options(-Wall -Wextra -Wpedantic)` | Extra words to the compiler for **every target**: `-Wall` common warnings, `-Wextra` more of them, `-Wpedantic` flag non-standard code. Warnings don't stop the build — they're the smell-detecting nose. |
| `option(ENABLE_SANITIZERS ...) OFF` | A user-facing switch, default off. Flip it with `cmake -B build-asan -DENABLE_SANITIZERS=ON`. When on... |
| `add_compile_options / add_link_options(-fsanitize=...)` | ...compiler *and* linker both get `-fsanitize=address,undefined` (memory-error and undefined-behavior detectors), plus `-fno-omit-frame-pointer` so the detector's reports show readable stack traces. Programs built this way run slower — that's why it's a separate build directory. |
| `include(CTest)` | Switches on the testing machinery. Also defines the `BUILD_TESTING` option (ON by default) that components use (section 4). |
| `add_subdirectory(srcscope)` | "Now read `srcscope/CMakeLists.txt` as part of this configure." One line per component. |
| the commented lines | Placeholders for the plan's future components. Leave them; uncomment when the roadmap installs that component. |

**Why Debug/Release builds never share a directory** is covered in [build.md §build types](build.md).

---

## 4. `srcscope/CMakeLists.txt` — a component recipe

```cmake
add_executable(srcscope
    src/main.cpp)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
```

- `add_executable(srcscope src/main.cpp)` — "a program named `srcscope`, built from this source file". This is one **target**. Later, as more `.cpp` files appear, they join this list. If you remove a source file from that list, expect a *linker* error ("undefined reference to ...") rather than a compile error — see [build.md §under the hood](build.md) for why that distinction matters.
- `if(BUILD_TESTING)` — tests build only when testing is enabled (it's ON by default thanks to `include(CTest)`). Off, this whole branch is skipped: nice for packaging a shipped product without test code. Don't remove it — the guard is correct structure.

Everything else (C++20, warnings, `build/bin` output) arrives from the root file by inheritance.

---

## 5. `srcscope/src/main.cpp` — the code today

```cpp
#include <iostream>

int main(int, char**) {
    std::cout << "srcscope skeleton: implement Milestone 01 (see Roadmap.md, Week 1).\n"
              << "Build and test commands: see operation_manual.md, section 3.\n";
    return 0;
}
```

Line by line:

| Line | Meaning |
|:-----|:--------|
| `#include <iostream>` | Brings in standard input/output (`std::cout`). The `<...>` form searches system headers. |
| `int main(int, char**)` | The program's entry point. `int argc` = number of arguments, `char** argv` = them as strings. Here they're deliberately *unnamed* — valid C++, and it silences "unused parameter" warnings. Milestone 01 names them and uses them. |
| `std::cout << ... << ...` | Stringing together output, one `<<` per piece. `\n` ends lines. |
| `return 0` | "Success." Any non-zero code = failure. Tests read exactly this, which is the entire test verdict mechanism ([tests.md §what a test is](tests.md)). |

This file is the seed of everything Phase I builds. Progress guidelines: modify freely; `git restore` is always there (see [lost.md §git first aid](lost.md)).

---

## 6. `srcscope/tests/CMakeLists.txt` — where tests are declared

```cmake
# Smoke test: the CLI must build and exit cleanly.
add_test(NAME srcscope_runs COMMAND srcscope)
```

- `add_test(NAME ... COMMAND ...)` — registers a **test**: a name (must be unique) and a command to run. `COMMAND srcscope` refers to the *target* built above; CMake resolves it to the actual binary path, so it works on every machine.
- Registered tests are collected into generated files (`CTestTestfile.cmake`) inside `build/`. When you run `ctest`, it reads those files, runs each command, and checks the exit code.
- Testing conventions in this repo: name tests `srcscope_<behavior>` (`srcscope_runs`, then e.g. `srcscope_args`). Filters and details: [tests.md](tests.md).

---

## 7. `.gitignore` — the "never commit this" list

```text
build/         build-*/      ← every build directory (Debug, Release, asan...)
compile_commands.json        ← a build artifact you'll generate for inspection
.cache/                      ← editor tooling caches
.vscode/  .idea/  *.swp  *~  ← editor droppings
.commandcode                 ← local tooling config
```

Anything listed never enters git. Consequence to remember: deleting `build/` is *always* safe — it regenerates with one command (see [lost.md §the build broke](lost.md)).

---

## 8. Vocabulary you'll see everywhere (glossary)

| Word | Meaning in this repo |
|:-----|:---------------------|
| **CMake** | The recipe-reader: turns `CMakeLists.txt` into real build commands. |
| **CMakeLists.txt** | One recipe file per directory, read top to bottom. |
| **Configure** | The first step: `cmake -S . -B build` reads recipes and writes the real build files into `build/`. |
| **Build** | The second step: `cmake --build build` runs the compiler/linker via those generated files. |
| **Target** | A named thing CMake can build — here, the program `srcscope`. |
| **Source file** | The `.cpp` files a target is built from. |
| **Build directory** (`build/`) | The scratch area for generated files and binaries. "Out-of-source": your source tree stays clean. |
| **CTest** | The test runner: reads registered `add_test`s and reports pass/fail by exit code. |
| **`BUILD_TESTING`** | Switch the `CTest` module provides; OFF skips every component's test directory. |
| **`add_subdirectory`** | "Include this subdirectory's recipe in the configure" — the monorepo mechanism. |
| **Flags / options** | Extra words to the compiler or linker: warnings, `-std=c++20`, sanitizers. |
| **Build type** | Debug (comfortable: no optimization, full debug info) or Release (fast). See [build.md](build.md). |

---

## 9. Recipes — growing the repo without breaking it

Two times in the plan you'll add structure. Use these exact shapes. The rule of safety: **add lines freely, never delete working ones.**

### Recipe A — a new component (first used in Week 5, `tinycore/`)

```text
tinycore/
├── CMakeLists.txt
├── include/tinycore/   # public headers — what other code is allowed to use
├── src/                # implementation files
└── tests/
```

`tinycore/CMakeLists.txt`:

```cmake
add_library(tinycore
    src/string.cpp)                 # targets can be libraries instead of programs

target_include_directories(tinycore PUBLIC include)
# PUBLIC = anyone who links tinycore automatically also gets this include path

if(BUILD_TESTING)
    add_subdirectory(tests)         # same guard as srcscope
endif()
```

Then the root `CMakeLists.txt` gets its matching line (the commented placeholder becomes real):

```cmake
add_subdirectory(tinycore)
```

Three-part checklist for a new component: (1) recipe file, (2) `add_subdirectory` uncommented in root, (3) tests directory guarded by `BUILD_TESTING`. Verify with a build+test cycle before moving on.

### Recipe B — a real (non-smoke) test, when the code grows one (due around Milestone 04)

A test here is a tiny extra program: it runs checks and returns 0 only if they all hold (one failed `assert` → non-zero exit → ctest reports Failed):

```cmake
# in srcscope/tests/CMakeLists.txt
add_executable(srcscope_tests
    counting_test.cpp)              # your test file, same directory

add_test(NAME srcscope_counting COMMAND srcscope_tests)
```

```cpp
// counting_test.cpp (shape to copy, not code to paste blindly)
#include <cassert>
void test_counts_words() { /* arrange, act, assert: assert(...) */ }
int main() {
    test_counts_words();
    // one check-function per behavior — that's the taste
    return 0;   // stays 0 until an assert fails → nonzero → ctest reports Failed
}
```

The full doctrine — what to test, how to name, how to debug a failing test — lives in [tests.md](tests.md); the CMake side is exactly these two lines per test.

---

· **Next reads:** [build.md](build.md) — what these commands do on disk · [tests.md](tests.md) — the test system · [lost.md](lost.md) — when something doesn't match this document
