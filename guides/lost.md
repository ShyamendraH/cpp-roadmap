# When you feel lost — the triage map

Open this file the moment anything is confusing. Nothing here is long. Find your symptom, follow the one pointer, get back to the milestone.

"You are here": anywhere → **lost**

**This file's job:** never leave you more than one link from "the thing to do now". Symptom → single action. Git first aid at the bottom.

---

## 1. "I don't know what to do right now"

| Situation | Do this |
|:----------|:--------|
| No idea where the plan stands | Your `PROGRESS.md` (how it's created and read: [guides/log.md](log.md)) → read `Next physical action` → do exactly that |
| PROGRESS.md doesn't exist | Follow [START.md](../START.md) Part 4 (10 minutes) |
| PROGRESS exists but is empty/stale | Fill `Current state` using the example in [guides/log.md §Current state](log.md) → then the next action |
| Don't remember what the current milestone requires | Open [Roadmap.md](../Roadmap.md), find **only** your phase's week — that's all you need |
| First day / everything before this looks unread | [START.md](../START.md). Ignore every other doc until its steps finish. |
| Overwhelmed by the repo's files | You need exactly three things: your `PROGRESS.md` ([guides/log.md](log.md)), your roadmap week, one command set from [guides/build.md §command reference](build.md). The rest is lookup-on-demand ([README's map](../README.md)). |

## 2. Build/test misbehaving

| Symptom | First move |
|:--------|:-----------|
| `cmake: command not found` | Not installed → install lines in [START.md Part 1](../START.md) |
| `CMake 3.20 or higher is required` | Your CMake is old → upgrade from the same install lines; don't try to edit the repo files to dodge it |
| `CMakeLists.txt ... no such file` / "no source dir" | You are not in the repo root → `cd` to the folder with `README.md`; the `-S`/`-B` paths are interpreted from your current directory |
| Weird cache / cache declared a different path (e.g. after moving the folder) | `rm -rf build && cmake -S . -B build` — the build dir is disposable by design |
| Compiler error lines | Read the **first** error only; fix; rebuild. Cascade errors after it are usually echoes of the first one ([build.md §under the hood](build.md)). |
| `undefined reference to ...` | **Link** error: a promise without a definition — usually a missing source file in `add_executable` ([tour.md §4](tour.md)) |
| Test fails and you can't see why | Run it alone with output: `ctest --test-dir build -R <name> --output-on-failure` → [guides/tests.md](tests.md) |
| "My test isn't running" | `ctest --test-dir build -N` — if absent, the `add_test` registration is missing ([tests.md §anatomy](tests.md)) |
| Everything sideways, diagnostics confused | **Nuclear-but-safe option:** `rm -rf build`, then the four "clean rebuild" commands ([build.md §clean rebuild](build.md)) |

## 3. Git first aid — the safety net, in escalation order

Playground experiments are allowed to be risky — these commands undo nearly anything short of rewriting history:

```bash
git status                        # 1. what changed? always start here
git diff                          # 2. look at what is uncommitted, file by file

git restore <file>                # 3. one file: discard my edits, back to last commit
git restore .                     #    all files: same, repo-wide  ⚠ discards uncommitted work

git log --oneline -5              # 4. what did I commit recently? (reading history breaks nothing)
git checkout <hash> -- <file>     #    (rare) resurrect an older version of one file
```

Rules so fear never shapes how you play:

- Commits are permanent-ish (they stay in `git reflog`, but don't rely on it as a routine tool). Uncommitted edits vanish with `restore`. **Checkpoint-commit before risky experiments; restore is for throwing *away*.**
- `build*/` never needs restoring — it's regenerable ([tour.md §7](tour.md)); delete freely and configure again.
- Committed something ugly? In this repo's solo playground: fix forward with a new commit. (History editing like `reset --hard` is a later skill, not a day-1 need.)

## 4. "The repo itself looks wrong"

| Suspect | Check |
|:--------|:------|
| A build recipe looks edited beyond recognition | `git diff CMakeLists.txt srcscope/` → restore what you didn't mean; structure intact? compare against [tour.md §3–6](tour.md) |
| A doc link within the guides feels wrong | All guides' targets are listed in [README's map](../README.md); if a file is truly missing, it's a bug in this repo — fix the link to the right doc |
| You suspect your work broke plumbing, and can't say what | `git status` + `git diff` to identify which of the three wires ([tests.md §anatomy](tests.md)) is touched; restore that file |

## 5. "I'm stuck / demotivated / falling behind"

| Symptom | Protocol |
|:--------|:---------|
| Same milestone blocked ≥ 2 focused sessions | [operation_manual §7](../operation_manual.md): write the milestone id + minimal repro + exact commands → restart with the hint ladder instead of guessing |
| A missed day or three | [operation_manual §8.A](../operation_manual.md): resume from `PROGRESS.md`'s next action; no re-doing, no marathon payback |
| A missed week or more | §8.B: re-entry — read PROGRESS + run build/test + look at recent commits + one small fix, then next action |
| Too big, feel behind, project ballooning feeling | §8.D: reduce the release to something *honest but smaller*; backlog grows, plan stays safe |
| Illness / life | §8.H: pause the schedule, return via one-week re-entry; health dominates the plan |
| The "bad day" minimum, if only 20 minutes exist | [operation_manual §4](../operation_manual.md): read next action → run build+test → one small concrete step → write the next action |

## 6. The ground rules that make being lost rare in the first place

1. **Never end a session with a non-building repo** — green build/test, or a written, honest state in `PROGRESS.md`.
2. **Never end a session without a written next action** — that one line makes tomorrow easy.
3. **Commit checkpoint before risky things**; restore for throwing away; never fight the build by hand — delete `build/` and regenerate.

---

· **[Back to README](../README.md)** · every symptom above traces back to three docs: your `PROGRESS.md` ([guides/log.md](log.md)) · your roadmap week · [guides/build.md](build.md)
