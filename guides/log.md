# Logging — PROGRESS.md and BACKLOG.md, exactly how

Read once after START.md, then again end-of-first-week. After that, it's the backdrop you follow without thinking.

"You are here": [README](../README.md) → [START](../START.md) → **log**

**This file's job:** field-by-field instructions for the two files you maintain — `PROGRESS.md` (where you are, what's next) and `BACKLOG.md` (ideas parking lot) — with filled examples to match. When to update what lives here; session rhythm itself is [operation_manual.md](../operation_manual.md).

---

## 1. Why these two files exist

The 40-week plan has one steady killer, and it isn't difficulty — it's **context loss**. A week of busy days, illness, or simply time, and "where was I again?" costs an hour of archaeology — or kills momentum outright. The design answer:

- `PROGRESS.md` — the **state** that survives context loss: current milestone, last-known-good commands, and one concrete *next physical action* so tomorrow you starts in 30 seconds.
- `BACKLOG.md` — the **storage** for attractive ideas that are not for now, so they don't distract milestones and don't die.

Both files are *yours* — the roadmap and checklist stay generic; these record your artifacts (commit hashes, evidence, dates, lessons).

## 2. `PROGRESS.md`, section by section

Create it once (START.md step 4 does this):

```bash
cp PROGRESS.template.md PROGRESS.md
```

### "Current state" — the load-bearing section

Filled for real, the template block looks like this. Compare yours against it — that's the whole standard:

```markdown
- Current roadmap week: 3
- Current milestone: 03 — Batch Scanner
- Status: active
- Last verified commit: 1f3c9a2 (feat(srcscope): batch scanner scans multiple paths (M03 partial))
- Last known-good build command: cmake -S . -B build && cmake --build build
- Last known-good test command: ctest --test-dir build --output-on-failure
- Current blocker: none
- Next physical action: Add a duplicate-lines report test using a two-line fixture file
- Time available before next session: 2 hours, Thursday evening
- Deferred ideas: (BACKLOG.md has: --tree flag for srcscope)
- Recovery debt (if any): none
```

Notes on the load-bearing lines:

- **Status** ∈ `not started / active / blocked / done` — the template's four. Note "blocked" is honest status, not an embarrassment: *current blocker* names what it needs.
- **Last verified commit** — the hash shown by `git log --oneline -1` from the last time build+test were green. It's your re-entry point (see recovery protocols in operation_manual §8).
- **Next physical action** — the only line whose quality the plan really hinges on. Rules:
  - A verb + a thing + a file: *"Print the current working directory in srcscope/src/main.cpp"*.
  - Never vague: *"work on M02"*, *"clean things up"*, *"investigate"*. A tired, rusty future-you must be able to start it in 30 seconds without re-reading anything.
  - When things work but there's nothing to implement *yet*: leave the next action of the *next session* (build the test, do the manual check, commit) — the line is never blank at day's end.

### Session log — one row per session, newest first

```markdown
| Date       | What happened                                            | Next action                          |
|:-----------|:---------------------------------------------------------|:-------------------------------------|
| 2026-10-27 | Scanner reports per-file counts; README updated; 1st leak found in arg paths | Add duplicate-lines fixture + test |
| 2026-10-23 | Day 1: build+test green, broke and restored smoke test, started M01 | Print cwd in main.cpp               |
```

Keep each row to a single line: what shipped, what blocked, then next. If it's ballooning, the extra words belong in the milestone section.

### "Milestones" — one section per milestone, newest first

Duplicate the template's milestone block for each milestone as it starts. Real example of a *completed* entry:

```markdown
### Milestone 01 — Argument Inspector

- Target week: 1
- Started: 2026-10-23
- Completed: 2026-10-25
- Status: done
- Outcome in one sentence: srcscope prints argc, every argument with index, and cwd; handles 0/1/many args.

Acceptance criteria      ← copied from Roadmap, verbatim
- [x] zero / one / multiple arguments handled
- [x] deterministic output
- [x] builds C++20 with warnings on
- [x] committed with short README mention in PROGRESS

Verification evidence
- Build command: cmake -S . -B build && cmake --build build        (clean rebuild from rm -rf)
- Test command and result: ctest --test-dir build --output-on-failure → 100% tests passed out of 2
- Manual test or demo: ./build/bin/srcscope "" one "two words" → argc line, 3 indexed args, cwd (see Session log 10-25)
- Commit: feat(srcscope): argument inspector (M01)  (hash b7f2d11)

Notes
- What I learned: argv[0] is the program name — "zero arguments" still has argc == 1
- Remaining limitations: no paths-with-spaces test yet; M03's multi-file work will reveal
- Deferred scope and reason: none
- Next concrete action: (M02) File Counter — first test: counting empty file
```

Structure worth copying: the acceptance criteria are **ticked by evidence** — never from memory. If you can't name the command or commit for a criterion, it's unticked. That habit is precisely what [checklist.md](../checklist.md)'s rule means ("tick only with evidence").

## 3. When to update — concrete moments

| Event | You | Update |
|:------|:----|:-------|
| After every session (the last 5 minutes) | mandatory | Session log row + Current state (the rule: next action is never blank) |
| Hit acceptance criteria of a milestone | mandatory | Milestone section → `done` + evidence + git commit |
| Started a milestone | mandatory | New milestone section → `active` |
| A milestone *stuck* two sessions | mandatory | `Status: blocked`, blocker named, then follow [operation_manual §8.C](../operation_manual.md) |
| Best idea that shouldn't be done now | mandatory | one section in `BACKLOG.md`, as it appears |
| Scheduling, dependencies and plan drift | when noticed | Roadmap gets a scope note, PROGRESS keeps the trail |
| Weekly review | end of week | Session log answers: what works, what's broken, what's unclear, next milestone safe? |
| End of phase review | weeks 4/8/12/... | operation_manual §11 protocol, results live in PROGRESS |

Rule of thumb to sanity check you haven't under-logged: could you rebuild your last session's state purely from `PROGRESS.md`, with zero other oracle sources needed? If not, fix now.

## 4. `BACKLOG.md`, the parking lot

Same template, same discipline:

```bash
cp BACKLOG.template.md BACKLOG.md
```

What belongs:

- A compelling but *optional* feature that isn't this milestone's work — e.g., during M02 writing the file counter: `--tree flag for srcscope`.
- Something spotted missing during testing but out of scope: e.g., "tests on the arg paths" from M03's session.
- Never: the active milestone, its risks, or actual v1 work. That goes to PROGRESS/test files, not here.

Each entry is small but well-named — five fields (template has them):

```markdown
### --tree flag for srcscope           (name: what it adds)
- Problem it solves: scanning a directory tree recursively by hand is tedious for M03 tests
- Depends on milestone: 03 (batch scanner produces the number)
- Evidence that would justify it: real M03 testing keeps re-touching directory trees manually
- Effort estimate: S
- Decision date: —
```

Promotion is a decision, triggered by either the milestone needing it or the promised evidence arriving: the item then moves *into* a milestone section of `PROGRESS.md` with a date — it doesn't sit next to one. Two hard rules ([manual §10](../operation_manual.md)):

1. Never unblock current work by adding backlog items on a whim — scope growth is always a *decision* with a reason.
2. The backlog is storage, not a promise list. If an entry survives several reviews untouched, finally do it or delete it.

## 5. Git habits — the log's safety wire

- Commit when a milestone's criteria pass; small commits when they leave a useful state; branch/checkpoint before risky refactors.
- `PROGRESS.md` and `BACKLOG.md` are **committed** too — the log travels with the repo; on any machine you get the same recovery state. (`.commandcode`, `build*`, editor droppings never enter; the .gitignore already covers them.)
- Update rules live here; undo commands live in [lost.md §git first aid](lost.md).

---

· **Related reads:** [START.md](../START.md) — your first entry · [operation_manual §2, §10](../operation_manual.md) — the policy behind these files · [lost.md](lost.md) — from "git status completely lost" to visible state
