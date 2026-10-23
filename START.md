# START here — Day 1

Six short parts get you from zero to: tools verified, build passing, test passing, code touched on purpose, progress log started, and your first milestone underway. Do them in order. The other docs can wait — each is linked exactly where it becomes useful.

**Time:** about 45–60 minutes.

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

Run every command from the repo root (the folder containing `README.md`). Each command is explained fully in [guides/build.md](guides/build.md) later — today, just watch it work.

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
[ 50%] Building CXX object srcscope/CMakeFiles/srcscope.dir/src/main.cpp.o
[100%] Linking CXX executable ../bin/srcscope
[100%] Built target srcscope
```

### 3. Test — run the registered tests

```bash
ctest --test-dir build --output-on-failure
```

Expected:

```text
Test project <path>/build
    Start 1: srcscope_runs
1/1 Test #1: srcscope_runs ....................   Passed    0.00 sec

100% tests passed out of 1
```

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

❑ Ticks checklist.md section 1: "build the skeleton from a clean build directory and run the tests".

---

## Part 3 — Touch the code, safely (15 min)

This is a playground: breaking things on purpose is *the* way to learn what each piece does. Git makes that safe.

### 1. Look at the only source file

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
1/1 Test #1: srcscope_runs ....................***Failed   0.00 sec
        Start 1: srcscope_runs
        ...
exit code: 1
...
The following tests FAILED:
	  1 - srcscope_runs (Failed)
```

(The wording differs slightly between CTest versions; look for `Failed`, `exit code: 1`, and `0% tests passed`.)

Two facts worth having for the next 40 weeks:

1. A test here is just a program the test runner launches. **Exit code `0` = pass, anything else = fail.** That's the entire verdict mechanism.
2. `--output-on-failure` prints what the failed program wrote, so you can see *why* it failed.

❑ Ticks checklist.md section 1: "deliberately break the program and confirm CTest reports a failure".

### 4. Restore — the safety net that makes a playground safe

Fix the file however you like, or let git throw your edits away and bring back the last committed state:

```bash
git restore srcscope/src/main.cpp
cmake --build build
ctest --test-dir build --output-on-failure
```

Green again. `git restore <file>` says: "this file, as of the last commit, please" — it **discards your uncommitted edits** to that file, nothing else. That's the emergency brake you'll use whenever experimenting goes sideways (full first-aid kit: [guides/lost.md](guides/lost.md)).

---

## Part 4 — Start the log (10 min)

The plan builds over 40 weeks. The thing that survives gaps, busy weeks, and your memory is a small file that always states where you are and what's next. Full rules live in [guides/log.md](guides/log.md); today you only need the first entry.

### 1. Create it from the template

```bash
cp PROGRESS.template.md PROGRESS.md
```

### 2. Fill in "Current state"

Example of a well-filled section (you'll have your own values):

```markdown
## Current state

- Current roadmap week: 1
- Current milestone: 01 — Argument Inspector
- Status: active
- Last verified commit: ac52765 (modify gitignore)
- Last known-good build command: cmake -S . -B build && cmake --build build
- Last known-good test command: ctest --test-dir build --output-on-failure
- Current blocker: none
- Next physical action: Print the current working directory in srcscope/src/main.cpp using std::filesystem::current_path()
- Time available before next session: about 1 hour today
- Deferred ideas: none yet (they go to BACKLOG.md when they appear)
- Recovery debt (if any): none
```

The only line with real power is **next physical action**. Rules:

- Must be a verb + a thing + a file: *"Print the current working directory in srcscope/src/main.cpp"*.
- Never vague: *"work on Milestone 01"*, *"clean things up"* — bad. A future you, tired and rusty, must be able to start in 30 seconds.

### 3. Log today as a session

In `PROGRESS.md` there's a "Session log" table — newest first, one short row per session:

| Date | What happened | Next action |
|:-----|:--------------|:------------|
| 2026-10-23 | First build + test pass; broke and fixed a test on purpose; started M01 | Print cwd in srcscope/src/main.cpp |

### 4. Checkpoint commit

This file (plus any fixes) should now be committed. The plan says: commit when a milestone's acceptance criteria pass — starting the log counts:

```bash
git add PROGRESS.md
git commit -m "start progress log; day 1 self-check done"
```

❑ Ticks checklist.md section 1: "create PROGRESS.md, record the current milestone, target outcome, and next action".

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
5. PROGRESS.md: mark M01 done + write the next action (M02: File Counter)
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
- Milestone 04 (Week 4) arrives → [guides/tests.md](guides/tests.md) before it
- Week 5 adds the `tinycore/` directory → recipes in [guides/tour.md](guides/tour.md)
- you end any session → [guides/log.md](guides/log.md)
- the weekly rhythm, sessions, recovery → [operation_manual.md](operation_manual.md), skim once this week
- any kind of confusion → [guides/lost.md](guides/lost.md)

---

**You are here:** [README](README.md) · **Next:** Milestone 01 in [Roadmap.md](Roadmap.md) · **Lost:** [guides/lost.md](guides/lost.md)
