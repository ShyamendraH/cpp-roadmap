# Verification Checklist

Companion to the [Roadmap](Roadmap.md) and [Operation Manual](operation_manual.md). Procedures and reasoning live in those two; this file is the ledger.

**Rule:** tick an item only when you can point to evidence — code, a test, a command, documentation, a commit, or a reproducible demo. If an item does not apply, write `N/A` and a short reason in `PROGRESS.md`. Milestone-specific notes and evidence belong in `PROGRESS.md`; this file stays reusable.

`[ ]` not yet verified · `[x]` verified

## 1. Setup (once)

### Repository and workflow

- [ ] Create the repository with the planned monorepo structure.
- [ ] Add the roadmap, operation manual, checklist, README, and progress template.
- [ ] Initialize Git and make the initial commit.
- [ ] Create `PROGRESS.md` from [`PROGRESS.template.md`](PROGRESS.template.md), and `BACKLOG.md`.
- [ ] Add a `.gitignore` for build directories, generated binaries, IDE files, and temporary output.
- [ ] Decide on naming and formatting conventions.
- [ ] Record the current milestone, target outcome, and next action in `PROGRESS.md`.

### Toolchain

- [ ] Confirm the compiler supports the required C++20 features.
- [ ] Confirm CMake is installed and working.
- [ ] Confirm CTest runs a trivial test.
- [ ] Confirm Git is installed and configured.
- [ ] Compile a minimal program from the command line.
- [ ] Build a minimal CMake project.
- [ ] Enable useful compiler warnings.
- [ ] Switch between Debug and Release builds successfully.
- [ ] Build from a clean build directory.
- [ ] Record the exact build and test commands in the README.

## 2. Every milestone

### Before coding

- [ ] Read the milestone and its acceptance criteria.
- [ ] Write the smallest observable outcome in one sentence.
- [ ] List the inputs, outputs, and important failure cases.
- [ ] Define what is explicitly out of scope.
- [ ] Split the work into tasks that fit one session each.
- [ ] Identify one test or manual check for the main behavior.
- [ ] Record the first step in `PROGRESS.md`.

### While coding

- [ ] Build the smallest working slice before adding polish.
- [ ] Keep ownership and lifetime decisions explicit.
- [ ] Prefer standard-library facilities in product code (unless this milestone *is* the learning component).
- [ ] Compile after each meaningful change.
- [ ] Add or update tests as behavior is introduced.
- [ ] Test one normal case before exploring edge cases.
- [ ] Keep temporary experiments separate from the finished implementation.
- [ ] No unrelated features or dependencies.
- [ ] Record blockers as the next concrete experiment, not repeated guessing.

### Before marking complete

- [ ] Every acceptance criterion is met, or explicitly deferred with a reason.
- [ ] The project builds from a clean build directory.
- [ ] Relevant tests pass.
- [ ] At least one edge case or failure path is tested.
- [ ] Warnings introduced by the change are resolved or explained.
- [ ] Resource ownership and cleanup are understood and documented.
- [ ] Errors are actionable, not silently ignored.
- [ ] No debug-only workaround is required for normal use.
- [ ] Public behavior and limitations are documented where needed.
- [ ] The example or demonstration still works.
- [ ] The change is committed to Git.
- [ ] `PROGRESS.md` records evidence, remaining limitations, and the next milestone.

## 3. Weekly loop

### Start of week

- [ ] Confirm the current milestone and its exit criteria.
- [ ] Choose a weekly target that fits the available time.
- [ ] Split it into five focused sessions.
- [ ] Keep the buffer/review day free.

### End of week

- [ ] Review what actually shipped, not hours spent.
- [ ] Run the relevant tests.
- [ ] Write down the most important thing learned.
- [ ] Record unresolved bugs and blockers.
- [ ] Defer scope that does not support the current milestone.
- [ ] Set the next session's first action.

### Bad-day minimum

- [ ] Open the project and read the recorded next action.
- [ ] Spend 20 minutes on one small, concrete step, if able.
- [ ] If productive work is not realistic, write a one-line restart note and stop.

Behind schedule? Use the recovery protocols in the [operation manual](operation_manual.md) — not more checkboxes.

## 4. C++ fluency self-check

Use periodically. You don't need every item before the next milestone; learn each concept when the roadmap calls for it.

### Core language and value model

- [ ] Explain the difference between declaration, definition, and initialization.
- [ ] Explain scope, lifetime, storage duration, and object lifetime.
- [ ] Distinguish values, references, pointers, and `const` references.
- [ ] Explain copy construction, copy assignment, move construction, and move assignment.
- [ ] Explain RAII and why destructors release owned resources.
- [ ] Apply `const` correctness and `constexpr` at a practical level.
- [ ] Understand overload resolution and basic templates.
- [ ] Understand `std::span`, `std::string_view`, and their lifetime constraints.
- [ ] Recognize undefined behavior and avoid relying on accidental results.
- [ ] Use exceptions deliberately and understand basic exception-safety guarantees.
- [ ] Understand when `noexcept` matters.
- [ ] Use `enum class`, `optional`, `variant`, and `expected`-style error modeling where appropriate.
- [ ] Understand iterator invalidation for the containers in use.
- [ ] Explain the difference between a data race and a logical race.
- [ ] Use atomics only when their memory-ordering requirements are understood.

### Build and debugging

- [ ] Explain the roles of preprocessing, compilation, assembly, and linking.
- [ ] Explain declarations and definitions across translation units.
- [ ] Diagnose a missing symbol at link time.
- [ ] Diagnose an include-path or missing-header error.
- [ ] Use a debugger to inspect state and step through code.
- [ ] Use sanitizers to investigate memory errors and undefined behavior.
- [ ] Read compiler diagnostics from the first relevant error.
- [ ] Reproduce a bug with a minimal test case.
- [ ] Explain differences between Debug and Release build behavior.
- [ ] Explain the purpose of CMake targets, include directories, and link dependencies.

## 5. `tinycore` learning components

Learning exercises — not automatically suitable for production use.

### Shared requirements for every component

- [ ] State the component's intended scope and non-goals.
- [ ] Define ownership, lifetime, and invalidation rules.
- [ ] Write tests before optimizing.
- [ ] Test empty, minimal, typical, boundary, and invalid inputs as applicable.
- [ ] Test copy and move behavior where relevant.
- [ ] Test resource cleanup on success and failure.
- [ ] Use sanitizers where supported.
- [ ] Compare behavior against a standard-library reference where possible.
- [ ] Document known limitations.
- [ ] Benchmark only after correctness is established.
- [ ] Keep the implementation small enough to explain.

### `String`

- [ ] Define empty-string behavior, and size and capacity semantics.
- [ ] Test construction from empty and non-empty input.
- [ ] Test copy and move operations.
- [ ] Test growth and reallocation.
- [ ] Test null termination if the design promises it.
- [ ] Test self-assignment and self-referential operations if supported.
- [ ] Test allocation-failure behavior as far as practical.
- [ ] Document invalidation rules for pointers or views into the string.

### `Vector<T>`

- [ ] Define size, capacity, and empty-state invariants.
- [ ] Construct and destroy elements correctly.
- [ ] Test growth and reallocation.
- [ ] Test copy and move semantics.
- [ ] Test non-trivial element types.
- [ ] Test types that are movable but not copyable.
- [ ] Test element-construction failure and cleanup.
- [ ] Test reserve, resize, push, pop, and access behavior the API supports.
- [ ] Document iterator and reference invalidation.
- [ ] Never access uninitialized storage as if objects already existed.

### `UniquePtr<T>`

- [ ] Define exclusive ownership clearly.
- [ ] Ensure copying is disabled.
- [ ] Test move construction and move assignment.
- [ ] Test destruction and reset behavior.
- [ ] Test null-state behavior.
- [ ] Ensure no double deletion occurs.
- [ ] Treat array ownership separately from scalar ownership.
- [ ] Prefer `std::unique_ptr` in actual product code.

### `RingBuffer<T>`

- [ ] Define full and empty behavior.
- [ ] Define whether capacity is fixed or can grow.
- [ ] Test wrap-around at both ends, and repeated fill/drain cycles.
- [ ] Test zero-capacity behavior or prohibit it explicitly.
- [ ] Test non-trivial element lifetimes.
- [ ] Define overflow behavior: reject, overwrite, or another explicit policy.
- [ ] Document whether the implementation is thread-safe; never imply it is if it is not.

### `HashMap<K, V>` and `HashSet<T>`

- [ ] Define key equality and hash requirements.
- [ ] Test empty state, insertion, lookup, update, and removal as supported.
- [ ] Test collisions deliberately.
- [ ] Test rehashing and load-factor behavior.
- [ ] Test duplicate-key and missing-key behavior.
- [ ] Test iteration behavior and invalidation rules.
- [ ] Test copy and move behavior if supported.
- [ ] Compare results against `std::unordered_map` or `std::unordered_set`.
- [ ] Never promise stable iteration order unless it is intentionally implemented.

## 6. `srcscope`

- [ ] Parse command-line arguments safely.
- [ ] Handle zero and malformed arguments; print a clear usage message.
- [ ] Handle paths containing spaces.
- [ ] Handle unreadable files without crashing.
- [ ] Distinguish file errors from normal end-of-file.
- [ ] Count files, lines, or bytes according to clearly documented definitions.
- [ ] Test empty files and files without a trailing newline.
- [ ] Test nested directories and the chosen symlink policy without following symlinks indefinitely.
- [ ] Produce deterministic output where practical.
- [ ] Add tests for both valid and invalid inputs.
- [ ] Document supported options and limitations.

## 7. `forge` build CLI

### Process execution

- [ ] Launch the compiler without constructing an unsafe shell command.
- [ ] Pass arguments as distinct arguments, not through shell quoting.
- [ ] Handle paths containing spaces and unusual characters.
- [ ] Capture stdout and stderr according to the chosen design without deadlocking.
- [ ] Preserve the compiler's exit status.
- [ ] Handle a missing compiler executable.
- [ ] Handle compiler failure without reporting a successful build.
- [ ] Handle interrupted or terminated child processes appropriately.

### Build model

- [ ] Compile one source file, then multiple translation units.
- [ ] Pass include paths, compiler flags, and linker flags predictably.
- [ ] Separate compile and link steps where required.
- [ ] Define configuration-file syntax and validation rules.
- [ ] Report invalid configuration with a useful location or field name.
- [ ] Model source-to-object and object-to-executable dependencies.
- [ ] Rebuild when a source or relevant input changes; don't rebuild unaffected targets.
- [ ] Handle deleted or renamed inputs.
- [ ] Ensure parallel builds never write the same output concurrently.
- [ ] Limit parallelism to a reasonable resource budget.
- [ ] Stop or clean up work sensibly after a failed task.
- [ ] Keep build output understandable and actionable.

### Release gate

- [ ] A fresh checkout can build `forge`.
- [ ] Automated tests cover argument parsing and build decisions.
- [ ] Integration tests invoke a real compiler in a temporary project.
- [ ] Tests cover success, compile failure, missing compiler, and malformed configuration.
- [ ] Documentation includes examples, supported scope, and known limitations.
- [ ] No claim is made that `forge` replaces mature build systems.

## 8. `termkit` terminal UI library

### Terminal handling

- [ ] Detect terminal versus redirected input/output where needed.
- [ ] Keep terminal-specific behavior behind a clear abstraction.
- [ ] Handle terminal dimensions changing during execution.
- [ ] Restore terminal state on normal exit, and on handled errors and interruption where feasible.
- [ ] Never leave the terminal in raw mode after failure.
- [ ] Handle escape sequences that arrive in partial reads.
- [ ] Handle unknown or unsupported input sequences safely.
- [ ] Keep input decoding separate from widget behavior.
- [ ] Avoid assuming every terminal supports every color or control sequence.

### Rendering and API

- [ ] Define the screen buffer and coordinate conventions.
- [ ] Clip drawing operations to valid bounds.
- [ ] Handle wide or non-ASCII characters within the documented scope.
- [ ] Avoid unnecessary redraws when the screen has not changed.
- [ ] Keep layout calculations separate from rendering.
- [ ] Define widget ownership and lifetime expectations.
- [ ] Handle terminal resize events without corrupting state.
- [ ] Provide a minimal example that is easy to build and run.
- [ ] Test output against a fake or captured terminal where practical.
- [ ] Document terminal compatibility and unsupported features.

### Release gate

- [ ] Examples work in a normal terminal.
- [ ] Output remains understandable when redirected, or redirection is explicitly unsupported.
- [ ] Terminal state restoration is tested.
- [ ] Resize and partial-input cases are tested.
- [ ] Public API examples compile.
- [ ] Known limitations are documented.

## 9. `httpkit` HTTP server library

### Socket and resource lifecycle

- [ ] Check every relevant socket and system-call result.
- [ ] Close file descriptors on all appropriate paths.
- [ ] Handle partial reads and writes, and interrupted system calls where applicable.
- [ ] Set explicit connection and resource limits.
- [ ] Define behavior for clients that connect but send no data.
- [ ] Define timeouts and maximum request sizes.
- [ ] Avoid unbounded buffering, threads, and tasks.
- [ ] Handle shutdown while clients are active.
- [ ] Log errors without leaking sensitive data.

### HTTP parsing and framing

- [ ] Parse the request line strictly enough to reject malformed input.
- [ ] Validate header syntax and limits.
- [ ] Handle duplicate headers according to documented rules.
- [ ] Define supported HTTP versions and methods.
- [ ] Determine message-body framing correctly.
- [ ] Reject ambiguous or invalid framing rather than guessing.
- [ ] Handle incomplete requests and premature disconnects.
- [ ] Enforce maximum header count, header size, and body size.
- [ ] Generate valid response status lines and headers.
- [ ] Never write conflicting or invalid body-framing headers.
- [ ] Test malformed input and connection-close behavior.
- [ ] Keep parser tests independent of live network tests.

### Concurrency and shutdown

- [ ] State the threading/concurrency model.
- [ ] Protect shared state or avoid sharing it.
- [ ] Test simultaneous clients, slow clients, and resource-exhaustion scenarios.
- [ ] Ensure one client's failure cannot corrupt unrelated connections.
- [ ] Define graceful shutdown behavior.
- [ ] Stop accepting new connections when shutdown begins.
- [ ] Join worker threads/tasks or otherwise complete them safely.
- [ ] Run ThreadSanitizer where supported and useful.

### Security and production boundaries

- [ ] Apply strict size and time limits.
- [ ] Reject malformed input rather than parsing it permissively into ambiguity.
- [ ] Test request-smuggling-relevant framing ambiguities within the supported HTTP/1.1 scope.
- [ ] Do not implement cryptography yourself; use a maintained TLS library if HTTPS is added.
- [ ] Document that HTTP/2, HTTP/3, TLS, compression, and similar features are unsupported unless actually implemented and tested.
- [ ] Review the RFC 9110 and RFC 9112 requirements relevant to implemented behavior.
- [ ] Test under load without treating a single benchmark as proof of production readiness.
- [ ] Document deployment assumptions and operational limitations.

### Release gate

- [ ] Parser unit tests cover valid and invalid messages.
- [ ] Integration tests cover real socket behavior.
- [ ] Tests cover slow clients, disconnects, limits, and shutdown.
- [ ] No unbounded resource path remains in the supported feature set.
- [ ] Public API examples compile and run.
- [ ] Documentation states the supported protocol subset clearly.

## 10. Integration and final release

### Integration

- [ ] The monorepo builds from a clean checkout.
- [ ] Each product builds and tests independently.
- [ ] `forge` can build the workspace's supported targets.
- [ ] `termkit` can display useful build status without coupling to `forge` internals.
- [ ] `httpkit` can expose the intended build-status endpoint with explicit resource limits.
- [ ] Shared code has clear ownership and dependency boundaries.
- [ ] No circular dependency has been introduced.
- [ ] Integration failures have dedicated tests.

### Quality and documentation

- [ ] All test suites pass in a clean build.
- [ ] Sanitizer runs are performed where supported.
- [ ] Debug and Release builds are both checked.
- [ ] Public APIs have concise usage examples.
- [ ] Each product README covers build, test, run, scope, and limitations.
- [ ] Error messages help users identify the next action.
- [ ] Logs avoid secrets and unnecessary sensitive data.
- [ ] Known defects and deferred features are listed honestly.
- [ ] Release notes describe what changed and what is not supported.

### Release and handoff

- [ ] Create a reproducible release build.
- [ ] Run the documented clean-checkout procedure.
- [ ] Tag the release in Git.
- [ ] Capture a short demo or terminal recording if useful.
- [ ] Write a brief retrospective: what works, what was cut, what was learned.
- [ ] List the next three improvements separately from the shipped release.
- [ ] Confirm the repository is understandable to someone who did not build it.

## 11. Milestone completion record

Record each milestone in `PROGRESS.md` using the milestone section from [`PROGRESS.template.md`](PROGRESS.template.md). Tick acceptance criteria from evidence, not from memory.

## 12. Final self-assessment

Grade yourself by what you can demonstrate and explain, not by how many boxes are ticked.

- [ ] I can explain the ownership and lifetime model of the code I wrote.
- [ ] I can reproduce and diagnose a representative failure.
- [ ] I can build and test the repository from a clean checkout.
- [ ] I can explain the design trade-offs and limitations of each product.
- [ ] I can show tests for the important normal, edge, and failure cases.
- [ ] I can identify which code is educational and which is intended for reuse.
- [ ] I can explain what deploying the HTTP server in production would still require.
- [ ] I have documented what is complete, what is incomplete, and what I would do next.
