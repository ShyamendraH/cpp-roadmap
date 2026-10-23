# How building works — commands, what's on disk, and looking under the hood

Read once after day 1, or whenever a build command feels like magic. It's also the reference you'll return to when builds act up.

"You are here": [README](../README.md) → [tour](tour.md) → **build**

**This file's job:** the exact meaning and timing of every build command, what the `build/` directory contains, build types, sanitizers, and the "under the hood" drills: watching the real compiler commands, using `gdb`, and reading generated artifacts. Git first aid lives in [lost.md](lost.md).

---

## 1. The pipeline in one picture

```text
CMakeLists.txt (recipes)
      │  cmake -S . -B build          ← configure: recipes → generated build files
      ▼
build/            (Makefile*, CTestTestfile.cmake, cache...)
      │  cmake --build build         ← build: generated files → compiler/linker
      ▼
build/bin/srcscope
      │  ctest --test-dir build      ← test: runs every registered add_test
      ▼
exit code 0 = pass, non-zero = fail
```

CMake never compiles anything itself. It reads recipes and writes real commands; the build step executes them. Keeping these two phases separate in your head explains most surprises.

## 2. The command reference

Run everything from the repo root.

| Command | What it means · when you run it |
|:--------|:--------------------------------|
| `cmake -S . -B build` | **Configure.** `-S .` source dir is here; `-B build` output goes to `build/`. Run once before the first build, and after you knowingly change recipes (CMakeLists.txt). It also re-runs itself automatically when the build notices a recipe changed — so when in doubt, just run the build. |
| `cmake --build build` | **Build.** Compiles what changed, links, puts results in `build/bin/`. Safe to re-run any time; it does nothing if nothing changed. |
| `ctest --test-dir build --output-on-failure` | **Test.** Runs all registered tests from `build/`. `--output-on-failure` shows the failing test's printed output — near-useless without it, so keep it in muscle memory. |
| `./build/bin/srcscope` | **Run.** Any arguments you pass become `argv`. |
| `ctest --test-dir build -R <name>` | Run only tests whose name matches — e.g. `-R args`. Handy when one test hurts. |
| `ctest --test-dir build -N` | **List** all registered tests without running them. First command when you think "why aren't my tests running?" |

Cheap and idempotent: re-running configure never hurts anything except a few seconds.

## 3. What lives in `build/` (and what you may ignore forever)

A peek after configure + build:

```text
build/
├── bin/srcscope              ← the program. The only part you "use"
├── CMakeCache.txt            ← your configure choices (build type, sanitizer switch...)
├── CMakeFiles/               ← generated make/compile bookkeeping — ignore
├── Makefile                  ← the generated build script the build step executes
├── CTestTestfile.cmake       ← the list of registered tests (generated from add_test)
├── compile_commands.json     ← only if enabled — every real compile command, as JSON
├── srcscope/                 ← per-component intermediates (.o files, test log dir)
└── Testing/Temporary/LastTest.log  ← ctest's full run log from last time
```

Three learn-worthy artifacts:

- `CMakeCache.txt` — the **memory** of your configure. `grep CMAKE_BUILD_TYPE build/CMakeCache.txt` tells you what type this directory was configured as.
- `CTestTestfile.cmake` — open it and you'll see your `add_test`s in generated form. The registry is not magic.
- `Testing/Temporary/LastTest.log` — everything ctest did, even after output scrolls away.

**The golden property:** `build/` is gitignored and regenerable. Deleting it is always safe and fixes a surprising number of weird states:

```bash
rm -rf build && cmake -S . -B build
```

## 4. Build types — Debug, Release, sanitizers

| Type | Compiler roughly becomes | When to use |
|:-----|:-------------------------|:------------|
| **Debug** (default) | `-g` (debug info), no optimization | Daily work, gdb, learning. Configured automatically if you don't choose. |
| **Release** | `-O3 -DNDEBUG` (fast, no debug info) | Measuring, benchmarking, "does it still break?" checks |
| Sanitized Debug | Debug **+** `-fsanitize=address,undefined` | When suspecting memory bugs / UB |

Never mix types in one directory — configure each type into its own directory (the `if` in the root CMakeLists only applies Debug when nothing was chosen):

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
ctest --test-dir build-release --output-on-failure
```

```bash
cmake -S . -B build-asan -DENABLE_SANITIZERS=ON
cmake --build build-asan
ctest --test-dir build-asan --output-on-failure
./build-asan/bin/srcscope          # slower builds speaking loudly when you're wrong
```

A finding looks like a red banner naming file, line, and what the memory did wrong (e.g. "stack-buffer-overflow... READ"). Sanitizers are the best habit-per-effort investment in this whole roadmap: turn them on for every milestone dealing with memory — Phase II onward.

Which type is this directory? `grep CMAKE_BUILD_TYPE build/CMakeCache.txt`. If the line shows no value, the root recipe's Debug fallback is in effect.

**Sanitizer details worth knowing:** the `-fsanitize` flags must reach *both* the compile step and the link step; the root `CMakeLists.txt` already does both (`add_compile_options` + `add_link_options`). If you ever see `undefined reference to __asan_*` at link time, someone removed the link half — restore it rather than working around it.

## 5. Under the hood — watch the real commands

### Verbose build (the ultimate reveal)

```bash
cmake --build build --verbose
```

One of the printed lines is the actual compile of main.cpp, shape like:

```text
cd build/srcscope && /usr/bin/c++ -g -std=c++20 -Wall -Wextra -Wpedantic -MD -MT ... -MF ... -o ... -c /your/path/srcscope/src/main.cpp
```

Read it as: `c++` compiling `main.cpp` with debug info, standard 20, three warning levels, writing an object file. Every `-Wall -Wextra -Wpedantic`, the `-std=c++20` and the `-g` — all of them are what those mysterious root CMakeLists lines become. Recipes → commands. That's the whole magic.

### The one-line hand-compile equivalent

For one file, the same result is just:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic srcscope/src/main.cpp -o build/bin/srcscope
```

CMake earns its keep the moment there are many files, build types, tests, and sanitizer switches — but underneath, it is exactly this.

### compile_commands.json — the compiler's diary

Turn on with a one-time flag, then read it as JSON:

```bash
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
grep -o '"command": "[^"]*"' build/compile_commands.json | head -2
```

Every entry = one translation unit + its exact flags — the file humans and tools read to know "what would the compiler do here".

### The 4 stages of one `.cpp` → program (approximate but keeps you honest)

```text
main.cpp ──preprocess──► expanded text ──translate──► assembly ──assemble──► object file (.o) ──link──► srcscope
             (g++ -E)            (g++ -S)             (g++ -c)                     (g++ -o)
```

Why this matters in practice:

- "undefined reference to X" = a **link** error: code compiled but a promised definition is missing (missing file in `add_executable`, or a typo'd name).
- "No such file or directory" for a header = a **preprocess** error: missing include path.

## 6. Debugging — a real taste of gdb

The build already embeds debug symbols in Debug mode. Try:

```bash
gdb ./build/bin/srcscope
```

In gdb:

```text
(gdb) break main          stop at the entry point
(gdb) run                 run until there, or: run arg1 arg2
(gdb) list                show source lines around you
(gdb) next                step to the next statement ("n")
(gdb) print argc          look at anything in scope ("p argc")
(gdb) print argv[1]       even array elements
(gdb) bt                  full call stack when something crashed
(gdb) quit                (y to confirm the stop)
```

Rule of thumb: gdb when you need to *watch a value change*, printing + `assert` when you need to *prove a property* (the way of tests.md). Milestones will tell you which they want; the habit of one print-and-build instead of ten mystery rebuilds saves hours.

## 7. Clean rebuild — the "it builds" vetting ritual

Any milestone claims "done" only after this works from nothing:

```bash
rm -rf build
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Why the wipe: incremental builds are fast **because** state is left around; a clean run proves your *recipes* describe the project — nothing was quietly carried over from a hidden cache. Each build directory is independent, so a Debug failure and a Release success can coexist; when hunting a type-dependent difference, compare two directories side by side instead of flipping one back and forth.

---

· **Related:** [tour.md](tour.md) — what the recipe lines mean · [tests.md](tests.md) — the test side · [lost.md](lost.md) — when this breaks
