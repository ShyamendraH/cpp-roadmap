# How tests work — and how to write your own without breaking anything

Read when you add your first test (the worked example in §3 fits Milestone 01), and definitely before Milestone 04.

"You are here": [README](../README.md) → [tour](tour.md) → [build](build.md) → **tests**

**This file's job:** the verdict mechanism, the anatomy of the wires that make a test exist, two worked examples of adding tests today, the rules that protect the repo's structure, and the taste experiments that teach you what a test smell is. Assert-libraries and framework choices come much later — the plan introduces a real structure at Milestone 04; everything here is the foundation it stands on.

---

## 1. What a test is in this repo — the whole mechanism

A test is **a registered command whose exit code is the verdict**:

```text
add_test  → ctest learns the name + what executable to launch
exit  0   → Passed
exit ≠ 0  → Failed
```

No framework, no magic class, no "test runner library". One number decides pass or fail. Everything else (what the program prints, which files it touches) is you making that number *meaningful*.

The prototype already exploits this: `srcscope` prints two lines and `return`s 0. The smoke test says: "the CLI must be able to build and exit cleanly" — and CTest enforces it by checking `0`.

## 2. Anatomy of a test — three wires that must all be connected

```cmake
# 1. the root: include(CTest)                    → enables the testing machinery + BUILD_TESTING=ON
# 2. a component:
if(BUILD_TESTING)                                  → the guard: skip all tests by flipping one switch
    add_subdirectory(tests)                        → wire 3a: read the tests recipe
endif()

# 3. the tests recipe (srcscope/tests/CMakeLists.txt):
add_executable(srcscope_tests counting_test.cpp)   → wire 3b: tests are C++ programs: build them
add_test(NAME srcscope_counting COMMAND srcscope_tests)
                                                   → wire 3c: register so ctest knows
```

Memory aid: `add_executable` builds it, `add_test` registers it. **A missing registration is the classic silent failure** — the program builds, but `ctest -N` won't list it and you'll wonder why nothing ran. When in doubt:

```bash
ctest --test-dir build -N          # what tests exist right now?
```

## 3. Worked example 1 — your second test, in 3 minutes

The smoke test runs `srcscope` with no arguments. Milestone 01 requires good behavior with arguments too — so make this requirement executable right now. Add to `srcscope/tests/CMakeLists.txt`:

```cmake
# args variant: handles arguments without crashing (Milestone 01 behavior)
add_test(NAME srcscope_args COMMAND srcscope a b c)
```

Reconfigure + run (a recipe change needs a reconfigure; the build step does it for you too):

```bash
cmake -S . -B build
ctest --test-dir build --output-on-failure
```

Expected — both tests passing:

```text
    Start 1: srcscope_runs
1/2 Test #1: srcscope_runs ....................   Passed    0.00 sec
    Start 2: srcscope_args
2/2 Test #2: srcscope_args ....................   Passed    0.00 sec

100% tests passed out of 2
```

Today it passes because the skeleton exits 0 regardless. After Milestone 01 lands, this test genuinely guards "arguments don't crash the program". Look at the trick you just learned: **the trailing words after the command become the program's arguments** — `ctest` does not shell-interpolate them, so filenames with spaces work as-is, which the earlier plan cares about (see checklist `srcscope` section).

Run a single test with a name filter:

```bash
ctest --test-dir build -R args --output-on-failure          # only srcscope_args
```

## 4. What these tests can see — and what their limits are

Exit-code tests guard **shapes of behavior**: doesn't crash, exits with something expected. They cannot see what's *printed*. For printing, two complementary tools:

1. **Human diff, pasted into the terminal:**

```bash
./build/bin/srcscope a b c
```

The Milestone 01 acceptance criteria ask for exact behavior (each argument, index and value, plus the cwd). Compare what appears with what the criteria say — that's manual verification, recorded in `PROGRESS.md` as "manual test or demo".

2. **Upgrade later to automatic:** when a function becomes testable *in itself* (returns a value rather than printing), unit tests take over. The recipe at [tour.md §Recipe B](tour.md) (`counting_test.cpp` + `assert`) is the shape, designed around exactly the "counting" work of Milestones 02–03.

A design rule worth adopting early: **print at the edges, compute in testable functions.** The roadmap deliberately schedules the test upgrade at Milestone 04, once `srcscope` has real behavior to pin.

## 5. Naming and hygiene — soft rules that pay off as tests multiply

- **Name pattern:** `srcscope_<behavior>` — `srcscope_runs`, `srcscope_args`, later `srcscope_missing_file`. The pattern pays off when `ctest -R args` selects four related tests.
- **One test = one behavior,** like one paragraph = one idea. A failure should announce what concept broke by its name alone.
- **No order dependence.** `ctest` may run tests as a set or individually; each must pass alone. Never write test 2 assuming test 1's side effects.
- **Keep the test command deterministic.** Same run, same verdict. No `/tmp` files that linger, no clock/time luck, no network tinkering.
- **Failures must print evidence.** Plain `assert` prints the failed expression; a message helps a week later — `assert(!out.empty() && "cwd line missing from output")`. How to chase a failing test: [build.md §debugging](build.md).
- **Commit the tests with their feature.** Green `ctest` from a fresh checkout is the milestone-done line on the checklist.

## 6. Fencing — never break the intended shape

The repo's structure carries two implicit promises:

1. **The plumbing stays:** `include(CTest)` in root, `if(BUILD_TESTING)` per component, `add_test` lines in tests recipes. You may *always add*; treat *removing* as an alarm-bell act, not a convenient fix.
2. **Failing = visible.** No `set_tests_properties(... DISABLED)` dances to hide work; if a test becomes wrong, fix or delete *both* the code and the test together, consciously.

Concrete protections while you explore:

```bash
git status                        # always know what is changed
git diff                          # see what will go in
git restore srcscope/tests/CMakeLists.txt   # recipe experiment went wrong → wire back to committed state
```

```bash
rm -rf build && cmake -S . -B build && ctest --test-dir build --output-on-failure
# reset the whole build side of the equation — also fully safe
```

Two experiments worth doing once each, just to *see* the machinery (both recoverable with the above):

- Comment the `add_test` line out → `ctest -N` lists nothing; the "my test isn't running" moment in miniature.
- Give two tests the same NAME → CMake refuses the registration with a warning; names are protected, which is what makes `-R <name>` filtering trustworthy.

## 7. Taste experiments — finding your own test instincts

Run each once, watch what changes, restore the file, remember the feeling. These aren't busywork; they're how you get a nose for what tests catch before you *need* the nose:

1. **Break the smoke test.** `return 1;` in main (day-1 classic). Watch ctest's report anatomy — exit code, output block, `The following tests FAILED`. Restore.
2. **Give the program an opinion.** Temporarily make `main` `return 2;` when it receives *no* arguments at all. `ctest -R runs` (the no-args test) fails while `ctest -R args` still passes — per-behavior names localize exactly which rule broke, with no cross-checking needed. Restore.
3. **Watch a name become a lie.** Temporarily make `main` `return 3;` when it *has* arguments, then run `-R args`: it fails, and the one visible truth is that the *name no longer matches the behavior*. When criteria change, the test changes with them — in the same commit. Restore.
4. **Test the empty case prematurely.** `add_test(NAME srcscope_empty COMMAND srcscope)` — redundant with `srcscope_runs` today, and the exact shape of the "empty file" requirement in Milestone 02. Names predict requirements.

The muscle you're building: every acceptance-criteria phrase in the roadmap should *smell like* a future test name. When you can read criteria as test names before coding, the plan starts writing its own verification ledger, which is where the checklist's "test one normal case / one edge case first" comes from.

## 8. When frameworks arrive (Milestone 04 sets the option)

The plan's Week 4 adds the real test structure; [tour.md §Recipe B](tour.md) already shows the shape, and your assert-style `counting_test.cpp` matches it. A framework (GoogleTest-style `TEST()` blocks, richer failure messages) is a comfort, not a new concept: everything above — one executable, exit-code verdicts, registration — stays identical. Do not adopt a framework before a milestone asks; the first hand-rolled asserts are what make the framework's value *visible* later.

---

· **Related reads:** [build.md](build.md) — how to run/debug failing tests · [tour.md §Recipes](tour.md) — the CMake wiring of new tests · [checklist.md](../checklist.md) — which behaviors deserve a test
