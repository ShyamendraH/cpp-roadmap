# START here — Day 1

Do these in order. About 30–45 minutes total. Each part says what to type, what you should see, and what to do next. Skip nothing, read nothing else yet — every other doc is linked exactly where you need it.

**You will finish with:** tools checked, first build green, first test green, code touched and restored on purpose, log started, Milestone 01 underway.

---

## Part 1 — Tools (5–10 min)

### 1. Check you have them

| Tool | Check with | You need | What it is |
|:-----|:-----------|:---------|:-----------|
| CMake | `cmake --version` | 3.20 or newer | generates the actual build commands from file recipes |
| Compiler | `g++ --version` (or `clang++ --version`) | C++20 support (GCC 11+, Clang 14+) | compiles your code |
| Git | `git --version` | any recent version | saves versions of your work |

Every command prints one line with its version. If a command is not found, or the version is too old, install:

```bash
# Debian / Ubuntu
sudo apt update && sudo apt install -y cmake build-essential git
# Fedora
sudo dnf install -y cmake gcc-c++ make git
# Arch
sudo pacman -S --needed cmake gcc make git
```

(macOS works for Phase I via `xcode-select --install` and `brew install cmake`, but this repo is Linux/POSIX-first, so use Linux when you can.)

### 2. Check git knows who you are

```bash
git config --get user.name
git config --get user.email
```

If either prints nothing, set it once (any value you like):

```bash
git config --global user.name "Your Name"
git config --global user.email "you@example.com"
```

❑ This completes half of [checklist.md](checklist.md) section 1 — "tools installed" and "git configured". Tick those two boxes now; this walkthrough will tell you when other boxes in section 1 are done.

---

## Part 2 — First build (5 min)

Run every command from the repo root (the folder with `README.md`). Just run these — what they mean is explained in [guides/build.md](guides/build.md) for later.

### 1. Configure — generate the real build files

```bash
cmake -S . -B build
```

`-S .` = "the source is here", `-B build` = "put generated stuff in `build/`". Expected output (excerpt):

```text
-- The CXX compiler identification is GNU 12.2.0
-- Detecting CXX compiler ABI info - done
-- Configuring done
-- Generating done
```

The version digits don't matter. "Configuring done / Generating done" is the success signal.

### 2. Build — compile everything the recipes describe

```bash
cmake --build build
```

Expected (excerpt):

```text
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

### 3. Test — run the registered tests

```bash
ctest --test-dir build --output-on-failure
```

Expected:

```text
Test project <path>/build
    Start 1: srcscope_runs
1/2 Test #1: srcscope_runs ....................   Passed    0.00 sec
    Start 2: srcscope_counting
2/2 Test #2: srcscope_counting ................   Passed    0.00 sec

100% tests passed out of 2
```

(What each test does — `srcscope_runs` the full CLI, `srcscope_counting` the counting logic — is [guides/testing.md](guides/testing.md); today they just need to be green.)

### 4. Run the program

```bash
./build/bin/srcscope
```

Expected:

```text
srcscope skeleton: implement Milestone 01 (see Roadmap.md, Week 1).
Build and test commands: see operation_manual.md, section 3.
```

A four-line ceremony: **configure → build → test → run**. That is the whole build system of this repo. When a later milestone says "meet the acceptance criteria", it means this ceremony still works after your changes.

❑ Ticks checklist.md section 1: "build the skeleton and run the tests".

---

## Part 3 — Touch the code, safely (15 min)

This is a playground: breaking things on purpose is *the* way to learn what each piece does. Git makes that safe.

### 1. Look at the program's source file

```bash
cat srcscope/src/main.cpp
```

```cpp
#include <iostream>

int main(int, char**) {
    std::cout << "srcscope skeleton: implement Milestone 01 (see Roadmap.md, Week 1).\n"
              << "Build and test commands: see operation_manual.md, section 3.\n";
    return 0;
}
```

In one glance: print two lines, return `0`. For a line-by-line reading of everything in this repo, that's [guides/tour.md](guides/tour.md) later.

### 2. Make it yours (edit → build → run)

Open `srcscope/src/main.cpp` in any editor and change the printed text — for example, make it greet you by name. Then:

```bash
cmake --build build
./build/bin/srcscope
```

Your new message appears. You just learned the daily loop: **edit → build → run**. (No need to re-run `cmake -S . -B build` for plain code edits — the next build re-checks the recipes automatically.)

### 3. Break it on purpose, and watch what tests catch

Edit `main.cpp` so it returns `1` instead of `0`:

```cpp
    return 1;
```

Then run the test step again:

```bash
ctest --test-dir build --output-on-failure
```

Expected:

```text
    Start 1: srcscope_runs
1/2 Test #1: srcscope_runs ....................***Failed    0.00 sec
srcscope skeleton: implement Milestone 01 (see Roadmap.md, Week 1).
Build and test commands: see operation_manual.md, section 3.

    Start 2: srcscope_counting
2/2 Test #2: srcscope_counting ................   Passed    0.00 sec

50% tests passed, 1 tests failed out of 2

The following tests FAILED:
	  1 - srcscope_runs (Failed)
```

(The wording differs slightly between CTest versions; look for `Failed`, the printed program output, and `50% tests passed`.)

Three facts worth having for the next 40 weeks:

1. A test here is just a program the test runner launches. **Exit code `0` = pass, anything else = fail.** That's the entire verdict mechanism.
2. `--output-on-failure` prints what the failed program wrote, so you can see *why* it failed.
3. `srcscope_counting` stayed green: each test covers a different behavior, so the failure is *localized* — you know what broke without opening a file.

❑ Ticks checklist.md section 1: "break the program and confirm CTest reports a failure".

### 4. Restore — the safety net that makes a playground safe

Fix the file however you like, or let git throw your edits away and bring back the last committed state:

```bash
git restore srcscope/src/main.cpp
cmake --build build
ctest --test-dir build --output-on-failure
```

Green again. `git restore <file>` throws away your uncommitted edits to that file and brings back the last committed version. Nothing else changes. That's your emergency brake (full kit: [guides/lost.md](guides/lost.md)).

---

## Part 4 — Start the log (5 min)

You need a place-marker so future-you knows where to start. It takes 2 minutes. Full guide: [guides/log.md](guides/log.md).

### 1. Create it from the template

```bash
cp PROGRESS.template.md PROGRESS.md
```

### 2. Fill in three lines

Open `PROGRESS.md` and fill in the `Now` part. Example (use your own values):

```markdown
## Now

- Milestone: 01 — Argument Inspector (Roadmap Week 1)
- Next: Print the current working directory in srcscope/src/main.cpp
- Stuck on: none
```

**Next** is the only line that matters. Rules:

- One step, with a file: *"Print the current working directory in srcscope/src/main.cpp"*.
- Never vague (*"work on M01"*), never blank. Tired future-you must be able to start in 30 seconds.

### 3. Log today as one row

In the `Log` table, add one short row:

| Date | Did | Next |
|:-----|:----|:-----|
| 2026-10-26 | First build + test green; broke and fixed a test; started M01 | Print cwd in srcscope/src/main.cpp |

### 4. Checkpoint commit

```bash
git add PROGRESS.md
git commit -m "start progress log; day 1 self-check done"
```

❑ Ticks checklist.md section 1: "create PROGRESS.md, write milestone + Next".

---

## Part 5 — Your first milestone (the real work begins)

Open [Roadmap.md](Roadmap.md), read **only** the section *Week 1 — Milestone 01: Argument Inspector*. In short, `srcscope` must print:

1. the number of arguments it received,
2. each argument — index and value,
3. the current working directory.

And it must handle zero, one, and many arguments, build with warnings on, and its behavior must be identical every run. Those are its **acceptance criteria** — the definition of done. Implement first, then verify each one; criteria are never skipped to protect a schedule.

A plan that fits today's loop:

```text
1. Count & print arguments  → build → run `./build/bin/srcscope a b c` → check
2. Print the working directory → build → run → check
3. Try zero / one / many args → make sure none crash
4. Tests still green → commit: "feat(srcscope): argument inspector (M01)"
5. PROGRESS.md: add a Done block for M01 + set Next to M02's first step
```

Two C++ facts you'll need (look up anything else as you go — that's the intended way):

```cpp
// What main receives when a program runs with arguments:
//   ./build/bin/srcscope a b c   →  argc = 4, argv = ["srcscope", "a", "b", "c"]
// argv[0] is always the program path — "zero arguments" still gives argc == 1.

// The current working directory (C++20):
#include <filesystem>
std::filesystem::current_path().string()   // e.g. "/home/you/cpp-practice"
```

Print them line by line with `std::cout`, exactly like the skeleton does.

**Stuck?** [guides/lost.md](guides/lost.md) has the symptom → remedy map. Blocked for two sessions? [operation_manual.md](operation_manual.md) §7 shows how to ask for help with real evidence.

---

## Part 6 — What to read, exactly when

Not everything at once. The map in [README.md](README.md) says for each file *when* it's needed. Today, nothing more is required. First glances come at:

- you touch the build for the second time → [guides/build.md](guides/build.md)
- Milestone 04 (Week 4) arrives → [guides/testing.md](guides/testing.md) before it
- Week 5 adds the `tinycore/` directory → recipes in [guides/tour.md](guides/tour.md)
- you end a session (2 min) → [guides/log.md](guides/log.md)
- the weekly rhythm, sessions, recovery → [operation_manual.md](operation_manual.md), skim once this week
- any kind of confusion → [guides/lost.md](guides/lost.md)

---

**You are here:** [README](README.md) · **Next:** Milestone 01 in [Roadmap.md](Roadmap.md) · **Lost:** [guides/lost.md](guides/lost.md)
