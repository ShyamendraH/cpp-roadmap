# C++ Systems Builder Roadmap

## A fixed 40-week plan for rebuilding fluency and shipping three products

**Plan version:** 1.0  
**Language baseline:** C++20  
**Initial platform:** Linux/POSIX  
**Primary outcomes:** `forge` (C/C++ build CLI), `termkit` (TUI
library), `httpkit` (HTTP server library), and `tinycore` (learning-oriented reusable components).

---

## 1. Planning assumptions

This is a calendar-based plan with recovery capacity built in—not a promise that every feature will be production-ready by a particular date.

| Planning variable           | Baseline                                                                                                 |
|:----------------------------|:---------------------------------------------------------------------------------------------------------|
| Duration                    | 40 weeks                                                                                                 |
| Normal workdays             | 5 days/week                                                                                              |
| Weekly buffer/review day    | 1 day/week                                                                                               |
| Full rest day               | 1 day/week                                                                                               |
| Focused time                | About 2–3 hours on each normal workday                                                                   |
| Planned productive capacity | 10–15 focused hours/week                                                                                 |
| Explicit contingency        | Buffer day each week, plus Weeks 4, 8, 12, 16, 20, 24, 28, 32, 36 and 40 as consolidation/recovery weeks |
| Default build system        | CMake + CTest                                                                                            |
| Compiler                    | GCC or Clang with C++20                                                                                  |
| Scope boundary              | Linux/POSIX first; portability beyond that is a later extension                                          |

**Why 40 weeks?** It gives small milestones room to breathe, while
leaving explicit capacity for review, illness, busy periods, debugging, and rework. The plan prioritizes a coherent, tested first release over implementing every possible feature.

**Time accounting:** A normal week has five focused work sessions, one
flexible buffer/review session, and one day off. The buffer day is not mandatory extra work. Use it for catch-up, tests, review, or rest. If a week is busy, reduce scope rather than borrowing from sleep or turning every rest day into a workday.

**Fixed timeline interpretation:** Each milestone has a target week. If
a milestone misses its exit criteria, use the designated buffer/recovery capacity and the operating manual. Do not silently skip correctness gates. If a delay cannot be absorbed, move the affected milestone and preserve the order of dependencies; the roadmap’s end date then shifts by the actual delay.

---

## 2. Product and repository structure

Use a monorepo initially so tests, examples, and integration work remain straightforward.

```text
cpp-systems/
├── CMakeLists.txt
├── tinycore/
│   ├── include/tinycore/
│   ├── tests/
│   └── benchmarks/
├── srcscope/
├── forge/
├── termview/
├── termkit/
├── netlab/
├── httpkit/
├── examples/
└── docs/
```

Each product should remain independently buildable. `tinycore` is a learning library, not a reason to avoid the standard library in real code. Reuse a custom component only when its correctness, API, and fit have been demonstrated. Keep protocol-specific code in `httpkit`, terminal-specific code in `termkit`, and process/build behavior in `forge`.

### Shared engineering rules

- Use C++20; avoid adding newer language features merely for novelty.
- Compile with strong warnings and test in debug builds.
- Use CMake and CTest from the start.
- Use Git commits at each completed milestone.
- Add tests for new behavior and at least one failure/edge case.
- Use AddressSanitizer and UndefinedBehaviorSanitizer during development
  when supported.
- Use `std::vector`, `std::string`, `std::unique_ptr`, and other
  standard types in product code by default. `tinycore` implementations are learning exercises until proven otherwise.
- Do not write your own compiler, linker, TLS implementation, HTTP/2
  stack, or complete STL.
- Every product must have a README with build, test, run, scope, and
  limitation instructions.

---

## 3. Weekly operating rhythm

Use the same rhythm throughout the plan:

| Day   | Default use                                                            | Expected output                                 |
|:------|:-----------------------------------------------------------------------|:------------------------------------------------|
| Day 1 | Start the milestone; inspect requirements; implement the smallest path | A tiny working slice                            |
| Day 2 | Continue implementation                                                | Main behavior works                             |
| Day 3 | Handle edge cases and tests                                            | Failure cases covered                           |
| Day 4 | Refactor only where needed; document design decisions                  | Code is understandable                          |
| Day 5 | Complete acceptance checks and commit                                  | Milestone is either done or has a clear blocker |
| Day 6 | Buffer/review/catch-up; optional light study                           | Recover schedule or consolidate                 |
| Day 7 | Rest                                                                   | No required work                                |

A milestone that takes longer than one week can span several weeks. The weekly rhythm does not mean a milestone must be rushed to fit a week.

---

# Phase I — Re-entry and development habits (Weeks 1–4)

## Week 1 — Milestone 01: Argument Inspector

**Deliverable:** `srcscope` starts as a small CLI that prints argument
indexes, values, and the current working directory.

**Learn/review:** `main`, `argc`/`argv`, basic types, strings,
functions, compile/run cycle.

**Acceptance criteria** - Handles zero, one, and multiple arguments. -
Prints output deterministically. - Builds with C++20 and warnings enabled. - Code is committed with a short README.

## Week 2 — Milestone 02: File Counter

**Deliverable:** Count bytes, lines, and whitespace-separated words in
one file.

**Learn/review:** streams, loops, functions, error handling.

**Acceptance criteria** - Empty file and missing file are handled. -
Read errors are reported. - Counting semantics are documented; word count is explicitly whitespace-based, not linguistic.

## Week 3 — Milestones 03–04: Batch Scanner and Source Inspector

**Deliverable:** Scan multiple paths, aggregate counts, find TODOs,
extract include-like directives, and report duplicate lines.

**Learn/review:** `std::vector`, filesystem, references, `const`,
maps/sets, string processing.

**Acceptance criteria** - A bad file does not prevent processing other
files. - Results identify the file and line where applicable. - Document limitations: this is not a full C++ parser. - Add tests for comments/strings and common false positives.

## Week 4 — Milestones 05–06: Tests, CMake, and Consolidation

**Deliverable:** Repeatable tests, clean CMake configuration, compiler
warnings, README, and a reliable build/test command.

**Acceptance criteria** - One command configures/builds; one command
runs tests. - A failing test makes the test command fail. - A clean checkout can build using the documented steps.

**Week 4 is a planned consolidation week.** If the earlier work is late,
use this week to finish it. If on schedule, improve tests and debug one deliberately introduced bug.

---

# Phase II — `tinycore`: lifetime, storage, and data structures (Weeks 5–12)

## Week 5 — Milestone 07: `tinycore::String` v0

**Deliverable:** An owning byte string with construction, destruction,
size, and basic access.

**Learn:** RAII, raw storage, object lifetime, null termination.

**Acceptance criteria** - Empty and non-empty strings work. - Allocation
and deallocation are paired. - Embedded null bytes are either supported by the API or explicitly excluded. - Tests cover size and contents.

## Week 6 — Milestone 08: `tinycore::Vector<T>` v0

**Deliverable:** Dynamic array with size, capacity, indexing,
`push_back`, and growth.

**Learn:** templates, raw storage, construction/destruction, amortized
complexity.

**Acceptance criteria** - Works with integers and a non-trivial test
type. - Live objects are destroyed exactly once. - `size <= capacity` always holds. - Out-of-range behavior is documented and tested.

## Week 7 — Milestone 09: Vector Ownership and Exception Safety

**Deliverable:** Copy/move construction and assignment, `reserve`, and
safer reallocation.

**Learn:** value categories, move semantics, exception guarantees,
invalidation.

**Acceptance criteria** - Copying creates independent contents. - Moving
transfers ownership without double-free. - Self-assignment is safe. - Throwing element construction is tested where practical. - Iterator/reference invalidation rules are documented.

## Week 8 — Milestone 10: `tinycore::UniquePtr<T>` and Consolidation

**Deliverable:** Minimal exclusive-ownership pointer, then custom
deleter only if the core is sound.

**Learn:** exclusive ownership, move-only types, RAII.

**Acceptance criteria** - Copying is prohibited. - Moving transfers
ownership. - Deletion occurs once. - Prefer `std::unique_ptr` in production code unless the custom type has a specific purpose.

**Week 8 is a consolidation/recovery week.** Do not start a new
component until String and Vector invariants are tested.

## Week 9 — Milestone 11: `tinycore::RingBuffer<T>`

**Deliverable:** Bounded circular buffer used for recent diagnostics.

**Learn:** modular arithmetic, bounded memory, overflow policy.

**Acceptance criteria** - Empty/full/wraparound behavior is explicit. -
Tests cover repeated wraparound. - The overflow policy is documented: reject, overwrite-oldest, or another clearly specified choice.

## Week 10 — Milestone 12: `tinycore::HashMap<K,V>` v0

**Deliverable:** Hash table with insertion, lookup, removal, and
collision handling.

**Learn:** hashing, equality, load factor, amortized complexity.

**Acceptance criteria** - Collision-heavy tests pass. - Rehashing
preserves all entries. - Duplicate-key behavior is defined. - Invalid or unsupported key types are documented.

## Week 11 — Milestone 13: `tinycore::HashSet<T>`

**Deliverable:** Set built on proven hash-table machinery.

**Learn:** uniqueness, equality contracts, abstraction reuse.

**Acceptance criteria** - Duplicate insertion is handled consistently. -
Insert/find/erase are tested. - Differential tests compare supported operations with `std::unordered_set`.

## Week 12 — Milestone 14: Container Bake-off and Consolidation

**Deliverable:** Differential tests, sanitizers, a small benchmark
suite, and documentation of complexity and limitations.

**Acceptance criteria** - Test empty, normal, boundary, collision, and
repeated-growth cases. - Run sanitizers where available. - Record benchmark methodology; do not optimize based on one noisy measurement. - Fix correctness bugs before adding new containers.

**Explicitly deferred:** balanced trees, a complete allocator framework,
lock-free containers, and full STL iterator compatibility.

---

# Phase III — `forge`: the C/C++ build CLI (Weeks 13–20)

## Week 13 — Milestone 15: Launch a Compiler

**Deliverable:** Run GCC/Clang from C++ and report success/failure.

**Learn:** POSIX process creation, waiting, exit status.

**Acceptance criteria** - A successful compilation returns success. - A
failed compilation reports failure. - No shell command is built by concatenating untrusted arguments.

## Week 14 — Milestone 16: Capture Diagnostics

**Deliverable:** Capture or stream compiler output while preserving exit
status.

**Learn:** pipes, file descriptors, process lifecycle.

**Acceptance criteria** - Diagnostics are visible for failed builds. -
Child processes are reaped. - Failure paths do not hang or leak descriptors.

## Week 15 — Milestone 17: Compile One Source

**Deliverable:** CLI accepts source, compiler, flags, and output path.

**Acceptance criteria** - Builds one C file and one C++ file. - Paths
containing spaces work. - Compiler and linker errors are distinguishable enough to diagnose.

## Week 16 — Milestone 18: Multiple Translation Units and Consolidation

**Deliverable:** Compile multiple source files to object files, then
link.

**Acceptance criteria** - Multi-file C and C++ examples build. - Object
files and output paths are predictable. - Debug and release flags are separated. - README documents supported behavior.

**Week 16 is a planned recovery/review week.** If process handling or
linking is unstable, stay here until it is reliable.

## Week 17 — Milestone 19: Project Configuration

**Deliverable:** Read and validate a simple project configuration with
sources, flags, include directories, and targets.

**Acceptance criteria** - Invalid configuration produces a useful error
before compilation. - Relative paths have documented resolution rules. - No arbitrary shell evaluation is required.

## Week 18 — Milestone 20: Build Graph

**Deliverable:** Model targets and dependencies, order builds, and
detect cycles.

**Learn:** graph representation, traversal, topological ordering.

**Acceptance criteria** - Dependencies are built before dependents. -
Cycles are reported with useful context. - Independent targets can be identified.

## Week 19 — Milestone 21: Incremental Builds

**Deliverable:** Rebuild only stale targets using timestamps and
compiler-generated header dependency files.

**Acceptance criteria** - An unchanged second build does not recompile
everything. - Changing a source rebuilds affected outputs. - Changing a tracked header rebuilds dependent sources. - Deleted or stale outputs are recovered correctly.

## Week 20 — Milestone 22: Parallel Build and Consolidation

**Deliverable:** Bounded parallel compilation, debug/release presets,
cancellation, tests, and `forge` v1 documentation.

**Acceptance criteria** - Parallel and serial builds produce equivalent
outputs. - Concurrency is bounded. - Failure propagation and cancellation are tested. - `forge` can build a real multi-file project.

**Week 20 is a consolidation week.** Do not add advanced build features
until incremental and parallel behavior is trustworthy.

---

# Phase IV — `termkit`: terminal engine and TUI library (Weeks 21–28)

## Week 21 — Milestone 23: Styled Terminal Output

**Deliverable:** A small demonstration of color, cursor movement, and
clearing.

**Acceptance criteria** - Style/cursor state is restored on normal
exit. - Unsupported terminal assumptions are documented.

## Week 22 — Milestone 24: Fixed Screen Layout

**Deliverable:** Draw a title bar, bordered panel, and status line at
defined coordinates.

**Acceptance criteria** - Resizing and small terminal dimensions are
handled. - Writes remain within the intended screen area.

## Week 23 — Milestone 25: Screen Buffer

**Deliverable:** In-memory cell grid and diff-based redraw.

**Acceptance criteria** - Unchanged cells are not needlessly redrawn. -
Buffer bounds and clipping are tested. - A test can validate the generated output without a real terminal.

## Week 24 — Milestone 26: Input Decoder and Consolidation

**Deliverable:** Decode ordinary keys, arrow keys, escape sequences, and
resize events.

**Acceptance criteria** - Partial escape sequences spanning multiple
reads are handled. - Input timing/ambiguity behavior is documented. - Terminal mode restoration works on error paths.

**Week 24 is a recovery week.** Do not start a widget framework while
the terminal lifecycle is unreliable.

## Week 25 — Milestone 27: Live Build Dashboard

**Deliverable:** Display `forge` status, recent logs, progress, and
selectable targets.

**Acceptance criteria** - UI remains responsive while builds run. -
Output is bounded. - Build failures are visible and actionable.

## Week 26 — Milestone 28: Extract Public TUI API

**Deliverable:** Separate backend, events, application lifecycle, and
rendering API.

**Acceptance criteria** - A second example uses the API without copying
internals. - Ownership and event-loop responsibilities are documented.

## Week 27 — Milestones 29–30: Layout and Basic Widgets

**Deliverable:** Rows/columns, padding, clipping, lists, buttons, text
input, focus, and scrolling.

**Acceptance criteria** - Nested layouts resize correctly. - Focus
navigation is predictable. - Widget state and rendering are separated enough to test.

## Week 28 — Milestone 31: `termkit` v1 and Consolidation

**Deliverable:** Fake-terminal tests, supported-terminal documentation,
examples, themes, and installation instructions.

**Acceptance criteria** - A separate application builds against the
public API. - Resize, input, redraw, and cleanup are tested. - Unicode display-width limitations are documented.

**Week 28 is a planned consolidation week.** If widgets are incomplete,
release a smaller honest v1 instead of adding more widget types.

---

# Phase V — `httpkit`: TCP, HTTP/1.1, and server runtime (Weeks 29–36)

## Week 29 — Milestone 32: Byte Buffer

**Deliverable:** Append, consume, and compact bytes safely.

**Acceptance criteria** - Partial consumption preserves remaining
bytes. - Bounds and capacity are explicit. - Tests include empty input and repeated compaction.

## Week 30 — Milestone 33: TCP Echo Server

**Deliverable:** Accept clients, receive bytes, and send them back.

**Acceptance criteria** - Multiple clients can connect/disconnect. -
Socket errors are handled. - File descriptors are closed on every exit path.

## Week 31 — Milestone 34: Slow Clients and Resource Bounds

**Deliverable:** Handle partial writes, timeouts, and bounded per-client
buffering.

**Acceptance criteria** - Slow readers do not cause unbounded memory
growth. - Idle connections cannot block the server indefinitely. - Disconnects do not crash the process.

## Week 32 — Milestone 35: HTTP Request-Line Parser

**Deliverable:** Incremental parser for method, target, and HTTP
version.

**Acceptance criteria** - Fragmented and malformed input are
distinguished. - Input length is bounded. - Parser tests do not require sockets.

## Week 33 — Milestone 36: Header Parser

**Deliverable:** Parse and validate headers with explicit size/count
limits.

**Acceptance criteria** - Repeated headers are represented
deliberately. - Malformed syntax and oversized headers are rejected. - Normalization rules are documented.

## Week 34 — Milestone 37: Body Framing

**Deliverable:** Handle `Content-Length`, chunked transfer coding,
incomplete bodies, and conflicting framing.

**Acceptance criteria** - Body sizes are bounded. - Conflicting or
ambiguous framing is rejected safely. - Fragmentation tests cover every parser state.

## Week 35 — Milestone 38: Response Writer and Minimal HTTP Server

**Deliverable:** Generate responses and serve basic routes to an
independent HTTP client.

**Acceptance criteria** - Status line, headers, and body framing are
valid. - Persistent-connection behavior is explicit. - Invalid requests receive safe error responses where possible.

## Week 36 — Milestones 39–40: Server API, Concurrency, and Consolidation

**Deliverable:** Reusable server API with bounded concurrency,
connection limits, timeouts, and graceful shutdown.

**Acceptance criteria** - Separate application can register routes. -
Independent clients make progress. - Queue/connection limits and overload behavior are explicit. - Shutdown does not hang on idle clients. - Protocol behavior is tested against a maintained client.

**Week 36 is a consolidation week.** If concurrency or framing is not
reliable, ship a smaller single-threaded, bounded first version and document the limitation rather than falsely claiming production readiness.

---

# Phase VI — Integration and release (Weeks 37–40)

## Week 37 — Milestone 41: Build the Workspace with `forge`

**Deliverable:** Build `tinycore`, `termkit`, and examples with your own
tool.

**Acceptance criteria** - Clean build succeeds. - Incremental build
skips unaffected work. - Failures preserve useful diagnostics.

## Week 38 — Milestones 42–43: Integrated Developer Console

**Deliverable:** `forge` + `termkit` interactive interface, plus an HTTP
build-status endpoint.

**Acceptance criteria** - Users can choose targets, inspect output,
cancel builds, and recover from errors. - HTTP endpoint exposes only intended status data. - UI and API remain responsive during builds.

## Week 39 — Milestones 44–45: Hardening and Benchmarks

**Deliverable:** Sanitizer runs, regression suite,
malformed-input/failure tests, and repeatable benchmarks.

**Acceptance criteria** - Known sanitizer findings are resolved or
explicitly tracked. - Resource limits are exercised by tests. - Benchmarks record environment, workload, and repeated measurements. - No optimization is accepted without evidence.

## Week 40 — Milestone 46: Release

**Deliverable:** Versioned release of the three products and `tinycore`,
with installation instructions, examples, CI, and documented limitations.

**Acceptance criteria** - Clean checkout can build and test the
projects. - Public APIs and supported platforms are documented. - Known limitations and security boundaries are explicit. - Release tags and reproducible commands are provided.

**Week 40 is the final consolidation/recovery week.** If the full scope
has slipped, release the smallest correct versions and move unfinished hardening/features into a clearly named post-plan backlog. Do not hide incomplete requirements under a “production-ready” label.

---

## 4. What is deliberately not in the 40-week critical path?

These are optional extensions after the core milestones:

- Balanced tree map/set and complete iterator categories.
- Custom allocator framework and lock-free data structures.
- Full C++ lexical/parser implementation for `srcscope`.
- Make/Ninja compatibility, complex compiler discovery, remote caching,
  and cross-compilation in `forge`.
- Full Unicode grapheme shaping and all terminal emulators in `termkit`.
- Windows console backend.
- HTTP/2, HTTP/3, WebSockets, compression, and advanced middleware in
  `httpkit`.
- Implementing cryptography or TLS yourself.

Add an extension only when the core release is stable or the extension is essential to your intended use case.

---

## 5. External references

Use these as references when a milestone reaches the relevant topic—not as prerequisites to binge-watch before starting.

- C++ reference: https://en.cppreference.com/w/cpp.html
- CMake tutorial: https://cmake.org/cmake/help/latest/guide/tutorial/
- GoogleTest: https://github.com/google/googletest
- AddressSanitizer: https://clang.llvm.org/docs/AddressSanitizer.html
- Compiler Explorer: https://godbolt.org/
- HTTP Semantics, RFC 9110: https://www.rfc-editor.org/rfc/rfc9110
- HTTP/1.1, RFC 9112: https://www.rfc-editor.org/rfc/rfc9112
- OpenSSL: https://www.openssl.org/

**The first task is Week 1, Milestone 01.** Do not start by implementing
the entire standard library. Get one small program working, test it, commit it, and move forward.
