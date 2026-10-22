# C++ Systems Builder Roadmap

A 40-week plan of **46 milestones** across six phases. Every milestone states what to build, what to learn, and how to know it is done.

Read this file once end to end. After that, read only the phase you are working in.

**Baseline:** C++20 · GCC or Clang · Linux/POSIX · CMake + CTest

---

## 1. How the plan works

| Planning variable | Baseline |
|:------------------|:---------|
| Duration | 40 weeks |
| Work rhythm | 5 focused sessions + 1 buffer/review day + 1 rest day per week |
| Focused time | 2–3 hours per session (about 10–15 hours per week) |
| Recovery capacity | The weekly buffer day, plus Weeks 4, 8, 12, 16, 20, 24, 28, 32, 36, and 40 as consolidation weeks |
| Toolchain | CMake + CTest, GCC or Clang with C++20 |
| Scope boundary | Linux/POSIX first; portability is a later extension |

Three rules keep the schedule honest:

1. **Milestones are correctness gates, not calendar promises.** If a milestone misses its acceptance criteria, use buffer or consolidation capacity. Never skip a gate to protect a date.
2. **If a delay cannot be absorbed, move the end date.** Preserve dependency order and acceptance criteria; the 40 weeks are a baseline, not a deadline.
3. **The buffer day is not extra work.** Use it for catch-up, testing, review, or rest. When a week is busy, cut scope — never sleep or cancel the rest day.

## 2. Repository layout

One monorepo keeps tests, examples, and integration work simple. Each product must also build independently. Only `srcscope/` exists at the start; every other component is added in its phase, together with its `add_subdirectory()` line.

```text
cpp-systems/
├── CMakeLists.txt
├── tinycore/       # learning components: String, Vector, UniquePtr, RingBuffer, HashMap
│   ├── include/tinycore/
│   ├── tests/
│   └── benchmarks/
├── srcscope/       # Phase I: source-inspection CLI (the warm-up project)
├── forge/          # Phase III: C/C++ build CLI
├── termkit/        # Phase IV: reusable TUI library
├── termview/       # Phase IV: apps built on termkit to exercise the API
├── httpkit/        # Phase V: reusable HTTP/1.1 server library
├── netlab/         # Phase V: experiments built on httpkit
├── examples/
└── docs/
```

Keep the boundaries clean: protocol code in `httpkit`, terminal code in `termkit`, process and build code in `forge`.

## 3. Weekly rhythm

| Day | Default use | Expected output |
|:----|:------------|:----------------|
| 1 | Start the milestone, inspect requirements, implement the smallest path | A tiny working slice |
| 2 | Continue implementation | Main behavior works |
| 3 | Handle edge cases and tests | Failure cases covered |
| 4 | Refactor only where needed, document design decisions | Code is understandable |
| 5 | Complete acceptance checks and commit | Milestone done, or a clear blocker |
| 6 | Buffer: review, catch-up, optional light study | Schedule recovered or consolidated |
| 7 | Rest | No required work |

A milestone may span more than one week. The rhythm is a default pace, not a requirement to rush.

## 4. Engineering conventions

These apply to every phase:

- Use C++20. Add newer language features only when a milestone needs them.
- Compile with strong warnings enabled; develop in Debug, verify in Release.
- Use CMake and CTest from the first commit.
- Test every new behavior plus at least one edge or failure case.
- Run AddressSanitizer and UndefinedBehaviorSanitizer where supported.
- Prefer `std::vector`, `std::string`, `std::unique_ptr`, and friends in product code. `tinycore` types are learning exercises until they have tests and a demonstrated reason to be reused.
- Commit at each completed milestone; keep generated build output out of Git.
- Give every product a README covering build, test, run, scope, and limitations.
- Never write your own compiler, linker, TLS, or complete STL.

---

# Phase I — Re-entry and development habits (Weeks 1–4)

**Goal:** rebuild the compile–test–commit habit on small CLI tools, ending with a trustworthy CMake + CTest setup.

## Week 1 — Milestone 01: Argument Inspector

- **Build:** `srcscope` starts as a small CLI that prints each argument (index and value) and the current working directory.
- **Learn:** `main`, `argc`/`argv`, basic types, strings, functions, the compile-and-run cycle.
- **Done when:** zero, one, and multiple arguments are handled; output is deterministic; it builds with C++20 and warnings on; the code is committed with a short README.

## Week 2 — Milestone 02: File Counter

- **Build:** count bytes, lines, and whitespace-separated words in one file.
- **Learn:** streams, loops, functions, error handling.
- **Done when:** empty files and missing files are handled; read errors are reported; counting semantics are documented (word count is whitespace-based, not linguistic).

## Week 3 — Milestones 03–04: Batch Scanner and Source Inspector

- **Build:** scan multiple paths, aggregate counts, find TODOs, extract include-like directives, and report duplicate lines.
- **Learn:** `std::vector`, `std::filesystem`, references, `const`, maps and sets, string processing.
- **Done when:** one bad file does not stop the others; results name the file and line where applicable; tests cover comments and strings as false positives; limitations are documented (this is not a full C++ parser).

## Week 4 — Milestones 05–06: Tests, CMake, and Consolidation

- **Build:** real tests for the Phase I behavior, warnings resolved, and a documented one-command build/test flow. The skeleton already ships the CMake, CTest, and warning plumbing.
- **Learn:** test structure, warning hygiene, telling a real failure from a configuration mistake.
- **Done when:** one command configures and builds, one command runs the tests, a failing test makes the test command fail, and a clean checkout builds using the documented steps.

**Consolidation week.** If earlier work is late, finish it here. If on schedule, improve the tests and debug one deliberately introduced bug.

---

# Phase II — `tinycore`: lifetime, storage, and data structures (Weeks 5–12)

**Goal:** implement the containers you use every day — once — so ownership and lifetime stop being abstract. Product code returns to the standard library afterwards.

## Week 5 — Milestone 07: `tinycore::String` v0

- **Build:** an owning byte string with construction, destruction, size, and basic access.
- **Learn:** RAII, raw storage, object lifetime, null termination.
- **Done when:** empty and non-empty strings both work; allocation and deallocation are paired; embedded null bytes are supported or explicitly excluded; tests cover size and contents.

## Week 6 — Milestone 08: `tinycore::Vector<T>` v0

- **Build:** a dynamic array with size, capacity, indexing, `push_back`, and growth.
- **Learn:** templates, raw storage, construction and destruction, amortized complexity.
- **Done when:** it works with integers and a non-trivial test type; live objects are destroyed exactly once; `size <= capacity` always holds; out-of-range behavior is documented and tested.

## Week 7 — Milestone 09: Vector Ownership and Exception Safety

- **Build:** copy and move construction and assignment, `reserve`, and safer reallocation.
- **Learn:** value categories, move semantics, exception guarantees, invalidation.
- **Done when:** copies are independent; moves transfer ownership without double-free; self-assignment is safe; throwing element construction is tested where practical; iterator and reference invalidation rules are documented.

## Week 8 — Milestone 10: `tinycore::UniquePtr<T>` and Consolidation

- **Build:** a minimal exclusive-ownership pointer; add a custom deleter only if the core is sound.
- **Learn:** exclusive ownership, move-only types, RAII.
- **Done when:** copying is prohibited, moving transfers ownership, deletion happens exactly once, and it is clear why product code still prefers `std::unique_ptr`.

**Consolidation week.** Do not start a new component until String and Vector invariants are tested.

## Week 9 — Milestone 11: `tinycore::RingBuffer<T>`

- **Build:** a bounded circular buffer, used later for recent diagnostics.
- **Learn:** modular arithmetic, bounded memory, overflow policy.
- **Done when:** empty, full, and wrap-around behavior is explicit; tests cover repeated wraparound; the overflow policy is documented (reject, overwrite-oldest, or another clearly specified choice).

## Week 10 — Milestone 12: `tinycore::HashMap<K, V>` v0

- **Build:** a hash table with insertion, lookup, removal, and collision handling.
- **Learn:** hashing, equality, load factor, amortized complexity.
- **Done when:** collision-heavy tests pass; rehashing preserves all entries; duplicate-key behavior is defined; unsupported key types are documented.

## Week 11 — Milestone 13: `tinycore::HashSet<T>`

- **Build:** a set built on the proven hash-table machinery.
- **Learn:** uniqueness, equality contracts, abstraction reuse.
- **Done when:** duplicate insertion is handled consistently; insert, find, and erase are tested; differential tests compare supported operations with `std::unordered_set`.

## Week 12 — Milestone 14: Container Bake-off and Consolidation

- **Build:** differential tests, sanitizer runs, a small benchmark suite, and documented complexity and limitations.
- **Learn:** measurement discipline, differential testing.
- **Done when:** empty, normal, boundary, collision, and repeated-growth cases are covered; sanitizers run where available; benchmark methodology is recorded (one noisy measurement proves nothing); correctness bugs are fixed before any new container is started.

**Consolidation week. Explicitly deferred:** balanced trees, a full allocator framework, lock-free containers, and complete STL iterator compatibility.

---

# Phase III — `forge`: the C/C++ build CLI (Weeks 13–20)

**Goal:** drive a real compiler from C++ — process control, dependency graphs, incremental builds, and bounded parallelism.

## Week 13 — Milestone 15: Launch a Compiler

- **Build:** run GCC or Clang from C++ and report success or failure.
- **Learn:** POSIX process creation, waiting, exit status.
- **Done when:** a successful compilation returns success, a failed one reports failure, and no shell command is assembled by concatenating untrusted arguments.

## Week 14 — Milestone 16: Capture Diagnostics

- **Build:** capture or stream compiler output while preserving the exit status.
- **Learn:** pipes, file descriptors, process lifecycle.
- **Done when:** diagnostics are visible for failed builds, child processes are reaped, and failure paths neither hang nor leak descriptors.

## Week 15 — Milestone 17: Compile One Source

- **Build:** a CLI that accepts a source file, compiler, flags, and output path.
- **Learn:** argument parsing, invoking the compiler correctly.
- **Done when:** it builds one C file and one C++ file, paths containing spaces work, and compiler versus linker errors are distinguishable enough to diagnose.

## Week 16 — Milestone 18: Multiple Translation Units and Consolidation

- **Build:** compile several source files to objects, then link them.
- **Learn:** translation units, the compile/link split.
- **Done when:** multi-file C and C++ examples build; object and output paths are predictable; debug and release flags are separated; the README documents supported behavior.

**Consolidation week.** If process handling or linking is unstable, stay here until it is reliable.

## Week 17 — Milestone 19: Project Configuration

- **Build:** read and validate a simple project configuration with sources, flags, include directories, and targets.
- **Learn:** file formats, validation, error reporting.
- **Done when:** invalid configuration produces a useful error before compilation; relative path resolution rules are documented; no arbitrary shell evaluation is involved.

## Week 18 — Milestone 20: Build Graph

- **Build:** model targets and dependencies, order builds, and detect cycles.
- **Learn:** graph representation, traversal, topological ordering.
- **Done when:** dependencies build before dependents, cycles are reported with useful context, and independent targets can be identified.

## Week 19 — Milestone 21: Incremental Builds

- **Build:** rebuild only stale targets, using timestamps and compiler-generated header dependency files.
- **Learn:** staleness rules, header dependencies.
- **Done when:** an unchanged second build recompiles nothing unnecessary; changing a source rebuilds affected outputs; changing a tracked header rebuilds dependent sources; deleted or stale outputs are recovered correctly.

## Week 20 — Milestone 22: Parallel Build and Consolidation

- **Build:** bounded parallel compilation, debug/release presets, cancellation, tests, and `forge` v1 documentation.
- **Learn:** worker pools, failure propagation, cancellation.
- **Done when:** parallel and serial builds produce equivalent outputs; concurrency is bounded; failure propagation and cancellation are tested; `forge` can build a real multi-file project.

**Consolidation week.** Do not add advanced build features until incremental and parallel behavior is trustworthy.

---

# Phase IV — `termkit`: terminal engine and TUI library (Weeks 21–28)

**Goal:** own the terminal — raw mode, screen buffers, input decoding, and a reusable layout and widget API.

## Week 21 — Milestone 23: Styled Terminal Output

- **Build:** a small demo of color, cursor movement, and clearing.
- **Learn:** escape sequences, terminal state.
- **Done when:** style and cursor state are restored on normal exit, and unsupported-terminal assumptions are documented.

## Week 22 — Milestone 24: Fixed Screen Layout

- **Build:** a title bar, bordered panel, and status line at defined coordinates.
- **Learn:** coordinates, clipping, resize handling.
- **Done when:** resizing and very small terminal dimensions are handled, and writes stay within the intended screen area.

## Week 23 — Milestone 25: Screen Buffer

- **Build:** an in-memory cell grid with diff-based redraw.
- **Learn:** buffer representation, minimal redraw.
- **Done when:** unchanged cells are not redrawn; buffer bounds and clipping are tested; the generated output can be validated without a real terminal.

## Week 24 — Milestone 26: Input Decoder and Consolidation

- **Build:** decode ordinary keys, arrow keys, escape sequences, and resize events.
- **Learn:** incremental parsing, input ambiguity.
- **Done when:** partial escape sequences spanning multiple reads are handled; input timing and ambiguity behavior is documented; terminal mode restoration works on error paths.

**Consolidation week.** Do not start a widget framework while the terminal lifecycle is unreliable.

## Week 25 — Milestone 27: Live Build Dashboard

- **Build:** display `forge` status, recent logs, progress, and selectable targets.
- **Learn:** async work in a UI, bounded output.
- **Done when:** the UI stays responsive while builds run, output is bounded, and build failures are visible and actionable.

## Week 26 — Milestone 28: Extract Public TUI API

- **Build:** separate backend, events, application lifecycle, and rendering API.
- **Learn:** API design, ownership boundaries.
- **Done when:** a second example uses the API without copying internals, and ownership and event-loop responsibilities are documented.

## Week 27 — Milestones 29–30: Layout and Basic Widgets

- **Build:** rows and columns, padding, clipping, lists, buttons, text input, focus, and scrolling.
- **Learn:** layout algorithms, focus management.
- **Done when:** nested layouts resize correctly, focus navigation is predictable, and widget state is separated from rendering enough to test.

## Week 28 — Milestone 31: `termkit` v1 and Consolidation

- **Build:** fake-terminal tests, supported-terminal documentation, examples, themes, and installation instructions.
- **Learn:** testability without a real terminal.
- **Done when:** a separate application builds against the public API; resize, input, redraw, and cleanup are tested; Unicode display-width limitations are documented.

**Consolidation week.** If widgets are incomplete, release a smaller honest v1 instead of adding more widget types.

---

# Phase V — `httpkit`: TCP, HTTP/1.1, and server runtime (Weeks 29–36)

**Goal:** implement HTTP/1.1 correctly — bounded parsing, explicit framing, concurrency, and shutdown.

## Week 29 — Milestone 32: Byte Buffer

- **Build:** append, consume, and compact bytes safely.
- **Learn:** ownership of partially consumed data.
- **Done when:** partial consumption preserves remaining bytes; bounds and capacity are explicit; tests include empty input and repeated compaction.

## Week 30 — Milestone 33: TCP Echo Server

- **Build:** accept clients, receive bytes, and send them back.
- **Learn:** sockets, accept loops, descriptor lifetime.
- **Done when:** multiple clients can connect and disconnect; socket errors are handled; descriptors are closed on every exit path.

## Week 31 — Milestone 34: Slow Clients and Resource Bounds

- **Build:** handle partial writes, timeouts, and bounded per-client buffering.
- **Learn:** backpressure, timeouts, resource limits.
- **Done when:** slow readers cannot cause unbounded memory growth; idle connections cannot block the server indefinitely; disconnects do not crash the process.

## Week 32 — Milestone 35: HTTP Request-Line Parser

- **Build:** an incremental parser for method, target, and HTTP version.
- **Learn:** incremental parsing, error classification.
- **Done when:** fragmented and malformed input are distinguished; input length is bounded; parser tests run without sockets.

## Week 33 — Milestone 36: Header Parser

- **Build:** parse and validate headers with explicit size and count limits.
- **Learn:** header syntax, normalization rules.
- **Done when:** repeated headers are represented deliberately; malformed syntax and oversized headers are rejected; normalization rules are documented.

## Week 34 — Milestone 37: Body Framing

- **Build:** handle `Content-Length`, chunked transfer coding, incomplete bodies, and conflicting framing.
- **Learn:** message framing, ambiguity rejection.
- **Done when:** body sizes are bounded; conflicting or ambiguous framing is rejected safely; fragmentation tests cover every parser state.

## Week 35 — Milestone 38: Response Writer and Minimal HTTP Server

- **Build:** generate responses and serve basic routes to an independent HTTP client.
- **Learn:** response framing, persistent connections.
- **Done when:** status line, headers, and body framing are valid; persistent-connection behavior is explicit; invalid requests receive safe error responses where possible.

## Week 36 — Milestones 39–40: Server API, Concurrency, and Consolidation

- **Build:** a reusable server API with bounded concurrency, connection limits, timeouts, and graceful shutdown.
- **Learn:** concurrency models, overload behavior, shutdown.
- **Done when:** a separate application can register routes; independent clients make progress; queue, connection-limit, and overload behavior is explicit; shutdown does not hang on idle clients; protocol behavior is tested against a maintained client.

**Consolidation week.** If concurrency or framing is unreliable, ship a smaller single-threaded, bounded first version and document the limitation rather than claiming production readiness.

---

# Phase VI — Integration and release (Weeks 37–40)

**Goal:** prove the pieces work together, then release them honestly.

## Week 37 — Milestone 41: Build the Workspace with `forge`

- **Build:** build `tinycore`, `termkit`, and the examples with your own tool.
- **Done when:** a clean build succeeds, an incremental build skips unaffected work, and failures preserve useful diagnostics.

## Week 38 — Milestones 42–43: Integrated Developer Console

- **Build:** a `forge` + `termkit` interactive interface, plus an HTTP build-status endpoint.
- **Done when:** users can choose targets, inspect output, cancel builds, and recover from errors; the endpoint exposes only intended status data; the UI and API stay responsive during builds.

## Week 39 — Milestones 44–45: Hardening and Benchmarks

- **Build:** sanitizer runs, a regression suite, malformed-input and failure tests, and repeatable benchmarks.
- **Done when:** known sanitizer findings are resolved or explicitly tracked; resource limits are exercised by tests; benchmarks record environment, workload, and repeated measurements; no optimization is accepted without evidence.

## Week 40 — Milestone 46: Release

- **Build:** a versioned release of the three products and `tinycore`, with installation instructions, examples, CI, and documented limitations.
- **Done when:** a clean checkout can build and test the projects; public APIs and supported platforms are documented; known limitations and security boundaries are explicit; release tags and reproducible commands are provided.

**Final consolidation week.** If scope has slipped, release the smallest correct versions and move unfinished work into a clearly named post-plan backlog. Do not hide incomplete requirements under a "production-ready" label.

---

## 5. Deliberately out of scope

Optional extensions after the core milestones, added only when a release is stable or a concrete use case demands them:

- Balanced-tree containers and the complete STL iterator model
- A custom allocator framework and lock-free data structures
- A full C++ parser for `srcscope`
- Make/Ninja compatibility, complex compiler discovery, remote caching, and cross-compilation for `forge`
- Full Unicode grapheme handling and a Windows console backend for `termkit`
- HTTP/2, HTTP/3, WebSockets, compression, and middleware for `httpkit`
- Hand-written cryptography or TLS

## 6. References

Reach for these when a milestone needs them — not as prerequisites to study first.

- C++ reference: https://en.cppreference.com/w/cpp.html
- CMake tutorial: https://cmake.org/cmake/help/latest/guide/tutorial/
- GoogleTest: https://github.com/google/googletest
- AddressSanitizer: https://clang.llvm.org/docs/AddressSanitizer.html
- Compiler Explorer: https://godbolt.org/
- HTTP Semantics, RFC 9110: https://www.rfc-editor.org/rfc/rfc9110
- HTTP/1.1, RFC 9112: https://www.rfc-editor.org/rfc/rfc9112
- OpenSSL: https://www.openssl.org/
