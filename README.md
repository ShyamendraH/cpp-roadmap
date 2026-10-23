# C++ Systems Builder

A guided C++ playground. The build system, a component, and tests already work — you learn C++ by building real systems, one small milestone at a time.

**Who this is for:** you can read basic C++ (variables, loops, functions), but CMake, tests, and project structure feel like a black box. No prior CMake or testing experience needed — the guides below explain all of it.

**The one rule:** follow [START.md](START.md) in order. It tells you what to do *and* what you do **not** need to read yet. Don't skip to the other files.

## What you actually get

- A **working playground** — `srcscope/` builds, one test passes, nothing is set up by hand
- A **plan** — [Roadmap.md](Roadmap.md): 46 milestones over 40 weeks; you follow one at a time
- A **way of working** — build → test → log → commit, so every session leaves a working state

## The lay of the land

| File | Job | When you need it |
|:-----|:----|:-----------------|
| [START.md](START.md) | Day-1 path: tools → first build → first test → first log entry | **Now** |
| [Roadmap.md](Roadmap.md) | The 40-week plan of milestones with acceptance criteria | One milestone at a time |
| [guides/tour.md](guides/tour.md) | Every file and CMake line, explained in plain words | The first time a CMake file confuses you |
| [guides/build.md](guides/build.md) | How building / testing actually works, debug & peek under the hood | When commands feel like magic |
| [guides/tests.md](guides/tests.md) | How tests are wired and how to write your own safely | Milestone 01 onward (before Milestone 04) |
| [guides/log.md](guides/log.md) | Exact rules for `PROGRESS.md` and `BACKLOG.md` | Every session's end |
| [guides/lost.md](guides/lost.md) | Symptom → remedy map, git first aid | **The moment you feel lost** |
| [operation_manual.md](operation_manual.md) | Session rhythm and recovery procedures | After your first session, then as needed |
| [checklist.md](checklist.md) | Verification ledger ("tick only with evidence") | With each milestone |
| [PROGRESS.template.md](PROGRESS.template.md) | Starter copy for your own `PROGRESS.md` | Day 1 (START.md step shows how) |
| [BACKLOG.template.md](BACKLOG.template.md) | Starter copy for your own `BACKLOG.md` | First time a fun idea isn't for now |
| `srcscope/` | The component you build in Phase I | Where your code goes |

Each guide starts with a "when to read this" line, so you never read something early by accident.

## Returning to the repo (quick reference)

Run from the repo root; details in [guides/build.md](guides/build.md):

```bash
cmake -S . -B build                                # configure (once, or after confusing things)
cmake --build build                                # build
ctest --test-dir build --output-on-failure         # test
./build/bin/srcscope                               # run
```

New here? That block is explained in [START.md](START.md).

## The loop you'll repeat for every milestone

```text
open PROGRESS.md → read the next action
   → implement one small behavior        (srcscope/src, srcscope/tests)
   → build and test until green
   → tick checklist.md items with evidence
   → commit
   → write the next action in PROGRESS.md
```

Never leave the repo not building, and never leave a session without a written next action. That is 80% of finishing a 40-week plan.

## Scope

C++20 · GCC or Clang · Linux/POSIX first · CMake + CTest. The 40 weeks are a planning baseline, not a deadline: if a milestone slips, reduce scope and move the date — never skip acceptance criteria.

---

**Start here:** [START.md](START.md) · **Lost:** [guides/lost.md](guides/lost.md)
