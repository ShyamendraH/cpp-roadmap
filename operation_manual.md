# C++ Coaching Operation Manual

## How to continue, recover, and complete the 40-week roadmap

**Purpose:** This document governs how to execute the roadmap when
motivation, available time, understanding, or technical progress varies. Keep it beside the roadmap and update it as the source of truth for your current state.

---

## 1. Operating principles

1.  **The roadmap is a sequence of verified capabilities, not a race.**
    A calendar target is useful, but correctness gates are not skipped
    to protect a date.
2.  **Always leave a working state.** End a session with code that
    builds, a failing test that is understood, or a written next action.
    Avoid leaving an opaque half-finished refactor.
3.  **One active milestone at a time.** Side ideas go into a backlog;
    they do not displace the current milestone without an explicit
    decision.
4.  **Learn just in time.** Learn a concept when the current
    implementation needs it, then apply it immediately.
5.  **`tinycore` must earn reuse.** Implementing a container is a
    learning exercise. Product code should use the standard library
    unless the custom implementation has demonstrated correctness and a
    real reason to be used.
6.  **No shame-based recovery.** A missed day is ordinary schedule
    variance. Resume from the written next action; do not restart the
    phase or punish yourself with marathon sessions.
7.  **A working demo is not production readiness.** Security, resource
    limits, tests, failure behavior, documentation, and measured
    operation are separate acceptance requirements.

---

## 2. Source of truth and progress record

Maintain one file at the repository root named `PROGRESS.md`. Update it at the end of each session and milestone.

Use this template:

```markdown
# Current Progress

- Current roadmap week:
- Current milestone:
- Status: not started / active / blocked / done
- Last verified commit:
- Last known-good build command:
- Last known-good test command:
- Current blocker:
- Next physical action (must be small and concrete):
- Time available before next session:
- Deferred ideas:
- Recovery debt (if any):
```

**Good next action:** “Add a test for `Vector<int>::reserve(0)` and run
CTest.”

**Bad next action:** “Work on Vector,” “learn templates,” or “fix
everything.”

The progress record should let you return after a week away without rereading the entire chat or reverse-engineering your own intentions.

### Git checkpoints

- Commit when a milestone’s acceptance criteria pass.
- Use small descriptive commits during implementation when they leave
  the repository in a useful state.
- Before risky refactors, create a branch or a checkpoint commit.
- Keep generated build directories and local secrets out of version
  control.
- Record compiler version and important build commands in the README or
  project docs.

---

## 3. The normal work session

For a 2–3 hour session, use this structure as a default. Shorten it on busy days.

| Segment       | Typical duration | Action                                              |
|:--------------|-----------------:|:----------------------------------------------------|
| Re-entry      |         5–10 min | Read `PROGRESS.md`, run the last build/test command |
| Focus block 1 |        35–50 min | Implement one narrow behavior                       |
| Break         |         5–10 min | Step away from the screen                           |
| Focus block 2 |        35–50 min | Add tests or complete the behavior                  |
| Verification  |        15–25 min | Build, test, inspect output, reproduce edge cases   |
| Handoff note  |            5 min | Record result, blocker, and next action             |

Do not spend the first hour deciding what to do. The next action should already be in `PROGRESS.md`.

### The 20-minute minimum on bad days

If energy or time is poor, do only this:

1.  Open the repository and read the next action. 2.  Run the existing tests/build. 3.  Make one small improvement, or write a test that clarifies the next
    bug.
4.  Record the next action.

A 20-minute session counts as maintaining continuity. It does not need to produce a large feature.

### Busy-day rule

If you have less than 20 minutes or are genuinely unavailable, skip the session. Do not turn the next day into a double shift. Use the weekly buffer day if needed and continue normally.

---

## 4. Weekly planning and schedule protection

The roadmap assumes five normal work sessions, one buffer/review day, and one full rest day each week.

At the start of the week:

1.  Read the current milestone’s acceptance criteria. 2.  Split the work into three to five small tasks. 3.  Identify one likely failure mode and write a test for it. 4.  Decide what you will deliberately *not* build this week. 5.  Reserve the buffer day; do not pre-fill it with extra scope.

At the end of the week:

- What works now that did not work before?
- Which acceptance criteria pass?
- What is still broken or unclear?
- Is the next milestone still safe to start?
- Does the timeline need to move?

### Recovery capacity

The roadmap includes a weekly buffer day and explicit consolidation weeks. The buffer is for debugging, illness, busy periods, and review. Consolidation weeks are not invitations to add new features; they protect the plan from predictable overruns.

If the buffer is consumed for two consecutive weeks, reduce scope in the next week. If the same milestone remains blocked after two focused sessions, stop adding code and diagnose the blocker.

---

## 5. How to start or resume any milestone

Use this procedure each time:

### Step 1 — Read only the relevant section

Read the current milestone and its immediate predecessor. Do not reread the entire roadmap unless dependencies are unclear.

### Step 2 — Write a tiny specification

Record: - Input and output. - The behavior required. - One normal case. - Two failure or edge cases. - What is explicitly out of scope.

### Step 3 — Establish a baseline

Build and run the current code before editing. If it already fails, record the failure first.

### Step 4 — Implement the smallest vertical slice

Make one end-to-end behavior work. Avoid broad abstractions until a second use case demonstrates the need.

### Step 5 — Test the invariants

Test both successful behavior and failure cases. For memory-owning code, think about lifetime and exceptions. For processes, think about exit status and cleanup. For terminals, think about resize and restoration. For HTTP, think about fragmentation, malformed framing, and resource limits.

### Step 6 — Review the design

Ask: - Who owns each resource? - What happens on failure? - What is the bound on memory/time/queued work? - What assumptions does the API make? - What test would falsify my understanding?

### Step 7 — Commit and record

If acceptance criteria pass, commit and mark the milestone done. Otherwise, mark it active or blocked and write the next concrete action.

---

## 6. How to work with the C++ coach

When requesting help, provide enough evidence to make the session efficient.

### For an implementation review

Send: - The milestone ID and intended behavior. - Relevant code files or a minimal reproduction. - Compiler and version. - Exact build/test command. - Full error output, not just the final line. - Expected versus actual behavior. - What you already tried.

Ask for a review in this order: 1. Correctness and undefined behavior. 2. Ownership, lifetime, and error paths. 3. Tests and missing edge cases. 4. API and architecture. 5. Performance, only where relevant.

### Hint ladder

Use hints in this order to preserve learning:

1.  **Nudge:** point to the concept or invariant that is probably wrong. 2.  **Diagnostic:** propose a tiny test or experiment. 3.  **Focused explanation:** explain the relevant C++ or systems
    concept.
4.  **Patch review:** inspect your attempted fix. 5.  **Worked example:** provide a minimal example if you remain blocked.

Do not insist on solving every problem unaided. The goal is to learn, not to maximize time spent stuck. But avoid pasting a full solution before you have tried to identify the invariant or reproduce the bug.

### Useful request template

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

---

## 7. Recovery protocols

### A. Missed one to three days

- Resume from `PROGRESS.md`.
- Run the last known-good build and tests.
- Do not redo completed work.
- Continue with the smallest next action.

### B. Missed a week or more

Use a re-entry session: 1. Read `PROGRESS.md` and the current milestone. 2. Run the build and tests. 3. Inspect the last few commits. 4. Fix one small issue or add one clarifying test. 5. Update the next action.

Do not restart the project just because the context feels rusty.

### C. A milestone exceeds its target week

1.  Identify whether the problem is missing knowledge, an oversized
    specification, a bug, or an environment issue.
2.  Split the remaining work into a smaller vertical slice. 3.  Keep completed behavior and tests. 4.  Use the buffer/recovery week. 5.  If still blocked after two focused sessions, ask for a review with
    logs and a minimal reproduction.
6.  Move the calendar target if needed; do not bypass acceptance
    criteria.

### D. The project feels too large

Reduce it to the minimum useful release. Examples: - Single-threaded HTTP server before a worker pool. - One layout primitive before a complete widget system. - Compile/link targets before advanced dependency discovery. - Basic hash table before custom allocators and tree containers.

Keep the extension as a backlog item rather than letting it block the milestone.

### E. A custom `tinycore` component is consuming too much time

- Compare its remaining work with the concept it is meant to teach.
- Stop adding features once the target learning outcome and tests are
  achieved.
- Use the standard library in the product.
- Record the component’s limitations and continue.
- Revisit it only if a later project creates a concrete requirement.

### F. A major design is wrong

- Stop feature work.
- Create a small reproduction or failing test.
- Identify the violated invariant.
- Write down the smallest correction.
- Refactor in small commits.
- Re-run tests after each meaningful change.

Do not rewrite a working project from scratch unless you can identify why incremental repair is more expensive.

### G. Environment or toolchain failure

- Record the exact command and full output.
- Check compiler version, build configuration, environment variables,
  and dependencies.
- Reproduce with a minimal program.
- Separate toolchain failure from application failure.
- Keep a last-known-good build or commit.

Do not change several system packages, compiler flags, and code paths simultaneously; that destroys diagnostic information.

### H. Illness, family needs, or prolonged interruption

Pause the schedule without accumulating “repayment hours.” When returning, use the one-week re-entry procedure. Extend the end date if required. The project plan is subordinate to health and real-life constraints.

---

## 8. Definition of done

A milestone is done only when its stated acceptance criteria pass. Use the following general checklist in addition to the milestone-specific criteria:

- [ ] The intended behavior is implemented.
- [ ] The code builds with the project’s normal compiler settings.
- [ ] At least one normal case and relevant edge/failure cases are
  tested.
- [ ] Resource ownership and cleanup are understood.
- [ ] Errors are actionable rather than silently ignored.
- [ ] The public interface is no larger than necessary.
- [ ] Known limitations are written down.
- [ ] The next milestone’s prerequisite is satisfied.
- [ ] The result is committed and `PROGRESS.md` is updated.

For higher-risk components, add the relevant sanitizer, fuzz, concurrency, security, or load tests. Do not apply every test category indiscriminately to a trivial early milestone.

---

## 9. Engineering-specific checklists

### Memory-owning types (`String`, `Vector`, pointers)

- Are constructed objects distinguished from allocated storage?
- Is every object destroyed exactly once?
- Are copy and move operations correct?
- What happens if allocation or construction throws?
- Are invalidated references/iterators documented?
- Do tests cover empty, boundary, and growth cases?

### Build CLI (`forge`)

- Are arguments passed without unsafe shell concatenation?
- Are child processes reaped?
- Are exit statuses and diagnostics preserved?
- Are paths and relative-path rules consistent?
- Can stale outputs or deleted dependencies be detected?
- Is concurrency bounded and cancellation tested?

### TUI (`termkit`)

- Is terminal state restored after normal exit and failure?
- Are partial input sequences handled?
- Does resize stay within buffer bounds?
- Are output queues bounded?
- Can rendering and widget logic be tested without a real terminal?
- Are terminal/Unicode limitations documented?

### HTTP server (`httpkit`)

- Are parser inputs incremental and bounded?
- Are `Content-Length` and transfer framing validated consistently?
- Are malformed and conflicting messages rejected safely?
- Are sockets, requests, and responses owned unambiguously?
- Are connection count, header/body sizes, queues, and timeouts bounded?
- Does overload have a defined behavior?
- Is shutdown tested with idle, slow, and disconnected clients?
- Is TLS delegated to a maintained implementation?

---

## 10. Scope control and optional quests

Keep a `BACKLOG.md` for attractive but nonessential work. Every backlog item should state: - What problem it solves. - Which milestone it depends on. - What evidence would justify doing it. - A rough effort estimate.

Examples to defer until core releases are stable: - Balanced trees and a complete STL-style iterator model. - Custom allocator framework and lock-free data structures. - Full C++ parser for `srcscope`. - Remote caching or complex cross-compilation in `forge`. - Full Unicode grapheme handling and a Windows terminal backend. - HTTP/2, HTTP/3, WebSockets, and compression.

Only promote an item into the active plan if it is necessary for the intended product or offers a clear learning return that outweighs its cost.

---

## 11. End-of-phase reviews

At the end of Weeks 4, 8, 12, 16, 20, 24, 28, 32, 36, and 40, run a short review:

1.  List completed milestones and their verified commits. 2.  List known bugs and limitations. 3.  Confirm that the next phase’s prerequisites work. 4.  Identify schedule drift and why it happened. 5.  Reduce the next phase’s scope if actual weekly capacity is lower
    than planned.
6.  Choose one improvement to the workflow, not ten.

If you are consistently completing less than 70% of planned work for three weeks, lower the planned scope or weekly hours. Do not respond by eliminating the rest day or increasing pressure.

---

## 12. Final release criteria

The plan is complete when: - `tinycore` has tests, documented limitations, and benchmark evidence for its learning components. - `forge` can compile/link multi-file projects, track dependencies, perform incremental builds, and handle failure. - `termkit` provides a documented reusable API with input, layout, widgets, rendering, and cleanup tests. - `httpkit` has a clearly scoped HTTP/1.1 implementation, bounded resources, protocol tests, concurrency/shutdown behavior, and documented security limitations. - The integrated developer console demonstrates the libraries working together. - A clean checkout can build and test using documented commands.

A 40-week target is a planning baseline, not evidence that a server is production-ready. If a critical safety or correctness gate remains incomplete, publish a smaller honest release and continue hardening it.

---

## 13. First action

Begin with **Week 1, Milestone 01: Argument Inspector**.

Your first session should do only this: 1. Create the repository and a minimal CMake project. 2. Write `main` to print `argc`, each argument, and the current working directory. 3. Compile and run it with zero, one, and several arguments. 4. Commit the working version. 5. Add the next action to `PROGRESS.md`.

Do not implement `Vector`, design the full repository, or start a second product before this first milestone is complete.
