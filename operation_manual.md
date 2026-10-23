# Operation Manual

How to run sessions, keep the weekly rhythm, and recover when the plan and reality disagree. Skim it once **after** your first session ([START.md](START.md)), then return whenever something breaks down. This file stays about *how you work*; tool commands live in the guides.

- Day-1 path: [START.md](START.md) — the one file a newcomer follows first
- Guides: [tour](guides/tour.md) (what every file is) · [build](guides/build.md) (commands, debugging) · [tests](guides/tests.md) · [log](guides/log.md) (progress discipline) · [lost](guides/lost.md) (when confused)
- Plan and milestones: [Roadmap](Roadmap.md) — verification ledger: [Checklist](checklist.md)
- Rolling state: `PROGRESS.md` (copy [`PROGRESS.template.md`](PROGRESS.template.md)); ideas: `BACKLOG.md` (copy [`BACKLOG.template.md`](BACKLOG.template.md))

---

## 1. Working principles

1. **One milestone at a time.** New ideas go to `BACKLOG.md`; they enter the active plan only by an explicit decision.
2. **Always leave a working state.** End every session with code that builds, a failing test you understand, or a written next action — never an unexplained half-finished refactor.
3. **Learn just in time.** Learn a concept when the current milestone needs it, then apply it immediately.
4. **`tinycore` must earn reuse.** Custom components exist to teach. Product code uses the standard library unless the custom component has tests and a demonstrated reason.
5. **Recovery is routine.** A missed day is ordinary schedule variance. Resume from the written next action; don't restart the phase or repay time with marathon sessions.
6. **A working demo is not production readiness.** Security, resource limits, failure behavior, documentation, and measured operation are separate acceptance requirements.

## 2. Progress record: `PROGRESS.md`

Copy [`PROGRESS.template.md`](PROGRESS.template.md) to the repository root and update it at the end of every session. It holds three things: your current state, a session log, and one record per milestone. It is the only file you must read when returning after time away. Field-by-field rules, filled examples, and the `BACKLOG.md` parking lot: [guides/log.md](guides/log.md).

**Git checkpoints**

- Commit when a milestone's acceptance criteria pass.
- Make small commits during implementation whenever they leave the repository in a useful state.
- Branch or checkpoint before a risky refactor.
- Keep build directories and local secrets out of version control.
- Record the compiler version and build commands in the README.

## 3. Build and test commands

Run all commands from the repository root. The complete reference — the configure/build/test pipeline, what lives in the build directory, build types, sanitizers, under-the-hood inspection, and debugging — is [guides/build.md](guides/build.md). Day one is walked through in [START.md](START.md).

One definition stands above everything: **"it builds" means a clean rebuild.** Wipe the build directory, configure, build, test — all green ([guides/build.md §clean rebuild](guides/build.md)).

### Structure changes

- **Adding a component** (Week 5 and later phases): the exact directory shape, its `CMakeLists.txt`, and the `add_subdirectory` wire — [guides/tour.md §Recipes](guides/tour.md).
- **Adding a test**: how a test comes into existence (three wires) and the assert-shape recipe — [guides/tests.md](guides/tests.md), recipes in [guides/tour.md](guides/tour.md).
- **Anything looks wrong or unexplained**: [guides/lost.md](guides/lost.md) triages by symptom.

## 4. The work session

A default 2–3 hour session (shorten it on busy days):

| Segment | Duration | Action |
|:--------|---------:|:-------|
| Re-entry | 5–10 min | Read `PROGRESS.md`; run the last build/test command |
| Focus block 1 | 35–50 min | Implement one narrow behavior |
| Break | 5–10 min | Step away from the screen |
| Focus block 2 | 35–50 min | Add tests or complete the behavior |
| Verification | 15–25 min | Build, test, inspect output, reproduce edge cases |
| Handoff | 5 min | Record the result, blocker, and next action |

Never spend the first hour deciding what to do — the next action is already in `PROGRESS.md`.

**The 20-minute minimum (bad days)**

1. Read the next action.
2. Run the existing build and tests.
3. Make one small improvement, or write a test that pins down the next bug.
4. Record the next action.

Continuity is the win; a short session does not need to produce a feature.

**Busy days:** with less than 20 minutes available, skip the session. Don't turn the next day into a double shift — use the buffer day and continue normally.

## 5. The weekly rhythm

At the start of the week:

1. Read the current milestone's acceptance criteria.
2. Split the work into 3–5 tasks that each fit one session.
3. Pick one likely failure mode and plan a test for it.
4. Decide what you will deliberately *not* build this week.
5. Keep the buffer day empty; don't pre-fill it with scope.

At the end of the week, answer in `PROGRESS.md`:

- What works now that didn't before?
- Which acceptance criteria pass?
- What is still broken or unclear?
- Is the next milestone still safe to start?
- Does the timeline need to move?

**Recovery capacity.** The buffer day is for debugging, illness, busy periods, and review. Consolidation weeks are not invitations to add features; they protect the plan from predictable overruns. If the buffer is consumed two weeks in a row, cut scope the following week. If a milestone stays blocked after two focused sessions, stop adding code and diagnose the blocker.

## 6. Starting or resuming a milestone

1. **Read only what you need.** The current milestone and its predecessor — not the whole roadmap.
2. **Write a tiny spec.** Inputs and outputs, required behavior, one normal case, two failure or edge cases, and what is explicitly out of scope.
3. **Establish a baseline.** Build and run the current code before editing. If it already fails, record that failure first.
4. **Implement the smallest vertical slice.** One end-to-end behavior. No broad abstractions until a second use case proves the need.
5. **Test the invariants.** Cover success and failure. Memory-owning code: lifetime and exceptions. Processes: exit status and cleanup. Terminals: resize and restoration. HTTP: fragmentation, framing, and resource limits.
6. **Review the design.** Who owns each resource? What happens on failure? What bounds memory, time, and queued work? What assumptions does the API make? What test would falsify your understanding?
7. **Commit and record.** Acceptance criteria pass → commit and mark the milestone done. Otherwise mark it active or blocked and write the next concrete action.

## 7. Working with a C++ coach

Provide enough evidence to make the session efficient:

- Milestone ID and intended behavior
- Relevant code or a minimal reproduction
- Compiler and version
- Exact build/test command
- Full error output, not just the last line
- Expected vs actual behavior
- What you already tried

Ask for review in this order: correctness and undefined behavior → ownership, lifetime, and error paths → tests and missing edge cases → API and architecture → performance, only where relevant.

**Hint ladder.** Escalate one step at a time; don't jump to a full solution before trying to identify the invariant yourself.

1. **Nudge** — point to the concept or invariant that is probably wrong.
2. **Diagnostic** — propose a tiny test or experiment.
3. **Focused explanation** — explain the relevant C++ or systems concept.
4. **Patch review** — inspect your attempted fix.
5. **Worked example** — a minimal example, if you are still blocked.

Staying stuck is not the goal; learning is.

**Request template**

```text
Milestone: M08 — Vector<T> v0
Goal:
Current behavior:
Expected behavior:
Build/test command:
Compiler and version:
Error or failing test:
Relevant code:
What I tried:
Please start with a hint and a diagnostic test, not a full rewrite.
```

## 8. Recovery protocols

**A. Missed 1–3 days.** Resume from `PROGRESS.md`: run the last known-good build and tests, then take the smallest next action. Don't redo completed work.

**B. Missed a week or more.** Re-entry session: read `PROGRESS.md` and the current milestone, run the build and tests, inspect the last few commits, fix one small issue or add one clarifying test, then update the next action. Context feels rusty; nothing is actually lost.

**C. A milestone overruns its week.** Decide whether the cause is missing knowledge, an oversized spec, a bug, or the environment. Split the remaining work into a smaller vertical slice, keep completed behavior and tests, and use the buffer/recovery week. If two focused sessions still leave it blocked, request a review with logs and a minimal reproduction. Move the calendar target rather than bypassing acceptance criteria.

**D. The project feels too large.** Reduce to the minimum useful release: single-threaded HTTP before a worker pool, one layout primitive before a widget system, compile/link targets before advanced dependency discovery, a basic hash table before custom allocators. Move the rest to the backlog.

**E. A `tinycore` component is eating time.** Compare the remaining work against the concept it teaches. Stop adding features once the learning outcome and tests are achieved, use the standard library in the product, record the component's limitations, and move on.

**F. A major design is wrong.** Stop feature work. Create a small reproduction or failing test. Identify the violated invariant. Write the smallest correction. Refactor in small commits, re-running tests after each. Don't rewrite a working project from scratch unless incremental repair is clearly more expensive.

**G. Toolchain or environment failure.** Record the exact command and full output. Check compiler version, build configuration, environment variables, and dependencies. Reproduce with a minimal program. Separate toolchain failure from application failure. Keep a last-known-good commit. Don't change several packages, flags, and code paths at once — that destroys diagnostic information.

**H. Illness, family, or prolonged interruption.** Pause the schedule without accumulating "repayment hours." Return via the one-week re-entry in B. Extend the end date if needed. The plan is subordinate to health.

## 9. When a milestone is done

A milestone is done only when its acceptance criteria pass **and** the "Before marking complete" items in the [checklist](checklist.md) are ticked with evidence. For higher-risk components, add the relevant sanitizer, fuzz, concurrency, security, or load tests — but don't apply every category to a trivial early milestone.

## 10. Backlog rule (`BACKLOG.md`)

Keep attractive-but-optional work in `BACKLOG.md`, not in the active milestone. Each item states: what problem it solves, which milestone it depends on, what evidence would justify doing it, and a rough effort estimate. Promote an item only when the intended product needs it or the learning return clearly outweighs its cost.

## 11. End-of-phase reviews

At the end of Weeks 4, 8, 12, 16, 20, 24, 28, 32, 36, and 40:

1. List completed milestones and their verified commits.
2. List known bugs and limitations.
3. Confirm the next phase's prerequisites work.
4. Identify schedule drift and its cause.
5. Cut next-phase scope if real weekly capacity is below plan.
6. Pick one workflow improvement — exactly one.

If you consistently complete less than 70% of planned work for three weeks, lower the planned scope or hours. Never the rest day.

## 12. Final release criteria

The plan is complete when:

- `tinycore` has tests, documented limitations, and benchmark evidence for its components.
- `forge` compiles and links multi-file projects, tracks dependencies, builds incrementally, and handles failure.
- `termkit` provides a documented reusable API with input, layout, widget, rendering, and cleanup tests.
- `httpkit` has a scoped HTTP/1.1 implementation with bounded resources, protocol tests, concurrency and shutdown behavior, and documented security limitations.
- The integrated developer console shows the libraries working together.
- A clean checkout builds and tests everything with documented commands.

40 weeks is a planning baseline, not proof that a server is production-ready. If a correctness or safety gate is incomplete, publish a smaller honest release and keep hardening it.

---

**Start here:** [START.md](START.md) · **Front door:** [README](README.md)
