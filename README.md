# C++ Systems Builder

A 40-week plan to rebuild C++ fluency by building real systems:

- **`tinycore`** — low-level components you implement yourself to learn what the standard library does
- **`srcscope`** — a source-inspection CLI (Phase I warm-up)
- **`forge`** — a C/C++ build CLI
- **`termkit`** — a reusable terminal UI (TUI) library
- **`httpkit`** — a reusable HTTP/1.1 server library

Every milestone produces a working result with explicit acceptance criteria, so progress is verifiable rather than assumed.

## Documentation

| Document | Purpose |
|----------|---------|
| [Roadmap](Roadmap.md) | The 40-week plan: 46 milestones with deliverables and acceptance criteria |
| [Operation Manual](operation_manual.md) | Session workflow, progress tracking, and recovery procedures |
| [Checklist](checklist.md) | Verification ledger and definition of done |
| [Progress template](PROGRESS.template.md) | Starter file: copy to `PROGRESS.md` and track your own progress |

## How to use this repository

1. Skim the [roadmap](Roadmap.md) once to see the sequence.
2. Copy [`PROGRESS.template.md`](PROGRESS.template.md) to `PROGRESS.md` at the repo root.
3. Start with [Week 1, Milestone 01](Roadmap.md#week-1--milestone-01-argument-inspector).
4. Work one milestone at a time: meet its acceptance criteria, then commit.
5. Tick [checklist](checklist.md) items only when you can point to evidence.
6. End every session by recording the next action in `PROGRESS.md`.

## Build and test

Requires CMake 3.20+ and a C++20 compiler. Run from the repo root:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/bin/srcscope
```

The skeleton builds and the smoke test passes out of the box. The [operation manual](operation_manual.md) covers Debug/Release builds, sanitizers, and how to add components and tests.

## Scope

- **Timeline:** 40 weeks — 5 focused sessions + 1 buffer day + 1 rest day per week (about 10–15 focused hours).
- **Language:** C++20 with GCC or Clang.
- **Platform:** Linux/POSIX first; portability is a later extension.
- **Build and test:** CMake and CTest.

The 40-week schedule is a planning baseline, not a deadline. If a milestone slips, reduce scope and move the end date — never skip acceptance criteria.

## First milestone

Build the **Argument Inspector**: print `argc`, every argument, and the current working directory. Compile it, run it with zero and several arguments, commit, and write the next action in `PROGRESS.md`.

Don't build the standard library or design future projects yet. Get one working slice done, verify it, and move on.

---

**Start here:** [Roadmap — Week 1, Milestone 01: Argument Inspector](Roadmap.md#week-1--milestone-01-argument-inspector)
