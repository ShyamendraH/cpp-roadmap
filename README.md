# C++ Systems Builder

A project-driven roadmap for rebuilding C++ fluency and using it to build three substantial systems projects:

- **`forge`** — a C/C++ compilation and build CLI
- **`termkit`** — a reusable terminal UI (TUI) library
- **`httpkit`** — a reusable HTTP server library
- **`tinycore`** — a learning-oriented collection of low-level components developed alongside the products

The plan emphasizes small, verifiable milestones rather than long assignments. Each stage produces a working result, introduces only the concepts needed next, and includes acceptance criteria so progress is measurable.

## Documentation

| Document                                             | Purpose                                                                                                                                       |
| ---------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------- |
| [C++ Systems Builder Roadmap](Roadmap.md)            | The 40-week plan with 46 milestones, timelines, deliverables, dependencies, and acceptance criteria.                                          |
| [C++ Coaching Operation Manual](operation_manual.md) | The day-to-day workflow for continuing, recovering after interruptions, debugging blockers, and completing milestones reliably.               |
| [Detailed Checklist](checklist.md)                   | A reusable verification checklist for setup, each milestone, C++ fluency, `tinycore`, all three products, integration, and release readiness. |

## How to use this repository

1. Read the [roadmap](Roadmap.md) to understand the overall sequence.
2. Read the [operation manual](operation_manual.md) and create a `PROGRESS.md` file at the repository root using its template.
3. Use the [detailed checklist](checklist.md) as a verification ledger; record milestone-specific evidence in `PROGRESS.md`.
4. Start with **Week 1, Milestone 01: Argument Inspector**.
5. Work on one milestone at a time. Verify its acceptance criteria and commit the result before moving on.
6. Use the weekly buffer and consolidation weeks for catch-up, testing, and recovery. Do not skip correctness gates just to preserve the calendar.
7. Keep optional ideas in a backlog instead of expanding the active milestone.

## Planning assumptions

- **Timeline:** 40 weeks, with explicit buffer and consolidation capacity.
- **Weekly rhythm:** five focused work sessions, one flexible buffer/review day, and one rest day.
- **Language:** C++20.
- **Initial platform:** Linux/POSIX.
- **Build and test tools:** CMake and CTest.
- **Priority:** correctness, understanding, useful software, and repeatable verification over feature count.

The 40-week schedule is a planning baseline, not a guarantee that every feature will be production-ready by the deadline. If work slips, preserve the dependency order and acceptance criteria, reduce scope where appropriate, and move the end date when necessary.

## Project principles

- Learn concepts when a project needs them, then apply them immediately.
- Build `tinycore` components to understand their internals; use the C++ standard library in product code by default unless a custom component has demonstrated a concrete advantage.
- Test normal behavior, edge cases, and failure paths.
- Keep resource ownership and cleanup explicit.
- Treat production readiness as something demonstrated through testing, resource limits, security checks, documentation, and operational behavior—not merely a working demo.
- Leave a clear next action at the end of every session so work can resume after interruptions.

## First milestone

Start small: build the **Argument Inspector**, a C++ program that prints command-line arguments and the current working directory. Compile it, test it with zero and multiple arguments, commit it, and record the next action in `PROGRESS.md`.

Do not begin by implementing the entire standard library or designing every future project. Build one working slice, verify it, and continue.

---

**Recommended starting point:** [Roadmap — Week 1, Milestone 01](Roadmap.md#phase-i--re-entry-and-development-habits-weeks-14)
