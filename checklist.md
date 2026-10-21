# C++ Systems Builder — Detailed Checklist

A practical, checkable companion to the [roadmap](cpp_systems_builder_roadmap.md) and [operation manual](cpp_coaching_operation_manual.md).

Use this file as a **verification ledger**, not as another assignment. Check an item only when you have actually done it and can point to evidence: code, a test, a command, documentation, a commit, or a reproducible demonstration.

**Checkbox key:** `[ ]` means not yet verified; `[x]` means verified.

Only check an item when you have evidence: code, a test, a command, documentation, a commit, or a reproducible demonstration. If an item does not apply, record `N/A` and a short reason in `PROGRESS.md`. Keep milestone-specific notes and evidence there; this file is the reusable master checklist.

## 1. Start-of-journey setup

### Repository and workflow

- [ ] Create the project repository and use the planned monorepo structure.
- [ ] Add the roadmap, operation manual, checklist, and README.
- [ ] Initialize Git and make the initial commit.
- [ ] Create `PROGRESS.md` from the operation manual's template.
- [ ] Add a `.gitignore` for build directories, generated binaries, IDE files, and temporary output.
- [ ] Decide on a consistent naming and formatting style.
- [ ] Keep a small backlog for optional ideas; do not mix it into the active milestone.
- [ ] Record the current milestone, target outcome, and next action in `PROGRESS.md`.

### Toolchain

- [ ] Confirm the compiler supports the required C++20 features.
- [ ] Confirm CMake is installed and working.
- [ ] Confirm CTest runs a trivial test.
- [ ] Confirm Git is installed and configured.
- [ ] Create a minimal program and compile it from the command line.
- [ ] Create a minimal CMake project and build it.
- [ ] Enable useful compiler warnings.
- [ ] Learn how to switch between Debug and Release builds.
- [ ] Verify the project can be built from a clean build directory.
- [ ] Record exact build and test commands in the README.

## 2. Repeat for every milestone

### Before coding

- [ ] Read the milestone description and its acceptance criteria.
- [ ] Write the smallest observable outcome in one sentence.
- [ ] Identify the prerequisite concepts needed for this milestone.
- [ ] List the inputs, outputs, and important failure cases.
- [ ] Define what is explicitly out of scope.
- [ ] Break the work into tasks small enough to complete in one session.
- [ ] Identify at least one test or manual check for the main behavior.
- [ ] Record the first implementation step in `PROGRESS.md`.

### During implementation

- [ ] Build the smallest working slice before adding polish.
- [ ] Keep ownership and lifetime decisions explicit.
- [ ] Prefer standard-library facilities in product code unless the milestone is specifically about implementing a learning component.
- [ ] Compile after each meaningful change.
- [ ] Add or update tests as behavior is introduced.
- [ ] Test one normal case before exploring edge cases.
- [ ] Keep temporary experiments separate from the finished implementation.
- [ ] Avoid adding unrelated features or dependencies.
- [ ] Record blockers and the next concrete experiment rather than repeatedly guessing.

### Before marking complete

- [ ] Every acceptance criterion is met or explicitly deferred with a reason.
- [ ] The project builds from a clean build directory.
- [ ] Relevant tests pass.
- [ ] At least one edge case or failure path has been tested.
- [ ] Warnings introduced by the change have been resolved or explained.
- [ ] No debug-only workaround is required for normal use.
- [ ] Public behavior and limitations are documented where needed.
- [ ] The example or demonstration still works.
- [ ] The change has been committed to Git.
- [ ] `PROGRESS.md` records evidence, remaining limitations, and the next milestone.

## 3. Weekly planning and recovery

### Start of week

- [ ] Confirm the current milestone and its exit criteria.
- [ ] Choose a realistic weekly target that fits the available time.
- [ ] Split the target into five focused sessions.
- [ ] Reserve the buffer/review day for catch-up, testing, or rest.
- [ ] Identify the smallest useful result for a busy week.
- [ ] Keep one full rest day free of required work.

### End of week

- [ ] Review what actually shipped, not just hours spent.
- [ ] Run the relevant tests.
- [ ] Write down the most important thing learned.
- [ ] Record unresolved bugs and blockers.
- [ ] Remove or defer scope that does not support the current milestone.
- [ ] Set the next session's first action.
- [ ] Update the target dates if the plan has genuinely shifted.

### Bad-day minimum

- [ ] Open the project and read the recorded next action.
- [ ] Spend at least 20 minutes on one small, concrete step if able.
- [ ] If productive work is not realistic, write a one-line restart note and stop without guilt.
- [ ] Do not compensate for a bad day by sacrificing sleep or turning the rest day into an obligation.

### If behind schedule

- [ ] Identify whether the delay is caused by scope, a concept gap, a technical blocker, or reduced availability.
- [ ] Reduce optional scope first.
- [ ] Break the remaining task into smaller outcomes.
- [ ] Use the buffer day or the next consolidation week.
- [ ] Preserve prerequisite order and correctness gates.
- [ ] Move the calendar when necessary rather than falsely marking work complete.
- [ ] Re-enter through the operation manual's recovery procedure.

## 4. C++ fluency checklist

Use this section as a periodic self-check. You do not need to master every item before beginning the next milestone; learn each concept when the roadmap calls for it.

### Core language and value model

- [ ] Explain the difference between declaration, definition, and initialization.
- [ ] Explain scope, lifetime, storage duration, and object lifetime.
- [ ] Distinguish values, references, pointers, and `const` references.
- [ ] Explain copy construction, copy assignment, move construction, and move assignment.
- [ ] Explain RAII and why destructors should release owned resources.
- [ ] Understand `const` correctness and `constexpr` at a practical level.
- [ ] Understand overload resolution and basic templates.
- [ ] Understand `std::span`, `std::string_view`, and their lifetime constraints.
- [ ] Recognize undefined behavior and avoid relying on accidental results.
- [ ] Use exceptions deliberately and understand basic exception-safety guarantees.
- [ ] Understand when `noexcept` matters.
- [ ] Use `enum class`, `optional`, `variant`, and `expected`-style error modeling where appropriate and available.
- [ ] Understand iterator invalidation for the containers being used.
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
- [ ] Distinguish Debug and Release build behavior.
- [ ] Explain the purpose of CMake targets, include directories, and link dependencies.

## 5. `tinycore` learning components

These are learning exercises. They are not automatically suitable for production use.

### Shared requirements for each component

- [ ] State the component's intended scope and non-goals.
- [ ] Define ownership, lifetime, and invalidation rules.
- [ ] Write tests before optimizing.
- [ ] Test empty, minimal, typical, boundary, and invalid inputs as applicable.
- [ ] Test copy/move behavior when relevant.
- [ ] Test resource cleanup on success and failure.
- [ ] Use sanitizers where supported.
- [ ] Compare behavior against a standard-library reference where possible.
- [ ] Document known limitations.
- [ ] Benchmark only after correctness is established.
- [ ] Keep the implementation small enough to understand and explain.

### `String`

- [ ] Define empty-string behavior.
- [ ] Define size and capacity semantics.
- [ ] Test construction from empty and non-empty input.
- [ ] Test copy and move operations.
- [ ] Test growth and reallocation.
- [ ] Test null termination if the design promises it.
- [ ] Test self-assignment and self-referential operations if supported.
- [ ] Test allocation failure behavior as far as practical.
- [ ] Document invalidation rules for pointers or views into the string.

### `Vector<T>`

- [ ] Define size, capacity, and empty-state invariants.
- [ ] Correctly construct and destroy elements.
- [ ] Test growth and reallocation.
- [ ] Test copy and move semantics.
- [ ] Test non-trivial element types.
- [ ] Test types that are movable but not copyable.
- [ ] Test element-construction failure and cleanup.
- [ ] Test reserve, resize, push, pop, and access behavior that the API supports.
- [ ] Document iterator/reference invalidation.
- [ ] Avoid accessing uninitialized storage as if objects already existed.

### `UniquePtr<T>`

- [ ] Define exclusive ownership clearly.
- [ ] Ensure copying is disabled.
- [ ] Test move construction and move assignment.
- [ ] Test destruction and reset behavior.
- [ ] Test null state behavior.
- [ ] Ensure no double deletion occurs.
- [ ] Consider array ownership separately rather than assuming scalar ownership covers it.
- [ ] Prefer `std::unique_ptr` in actual product code.

### `RingBuffer<T>`

- [ ] Define full and empty behavior.
- [ ] Define whether capacity is fixed or can grow.
- [ ] Test wrap-around at both ends.
- [ ] Test zero-capacity behavior or prohibit it explicitly.
- [ ] Test repeated fill/drain cycles.
- [ ] Test non-trivial element lifetimes.
- [ ] Define overflow behavior: reject, overwrite, or another explicit policy.
- [ ] Document whether the implementation is thread-safe; do not imply it is if it is not.

### `HashMap<K, V>` and `HashSet<T>`

- [ ] Define key equality and hash requirements.
- [ ] Test empty state, insertion, lookup, update, and removal as supported.
- [ ] Test collisions deliberately.
- [ ] Test rehashing and load-factor behavior.
- [ ] Test duplicate-key behavior.
- [ ] Test missing-key behavior.
- [ ] Test iteration behavior and invalidation rules.
- [ ] Test copy/move behavior if supported.
- [ ] Compare results against `std::unordered_map` or `std::unordered_set`.
- [ ] Do not promise stable iteration order unless it is intentionally implemented.

## 6. `srcscope` milestones

- [ ] Parse command-line arguments safely.
- [ ] Handle zero arguments and malformed arguments.
- [ ] Print a clear usage message.
- [ ] Handle paths containing spaces.
- [ ] Handle unreadable files without crashing.
- [ ] Distinguish file errors from normal end-of-file.
- [ ] Count files, lines, or bytes according to clearly documented definitions.
- [ ] Test empty files and files without a trailing newline.
- [ ] Test nested directories and the chosen symlink policy.
- [ ] Avoid following symlinks indefinitely.
- [ ] Produce deterministic output where practical.
- [ ] Add tests for both valid and invalid inputs.
- [ ] Document supported options and limitations.

## 7. `forge` build CLI

### Process execution

- [ ] Launch the compiler without constructing an unsafe shell command.
- [ ] Pass arguments as distinct arguments rather than relying on shell quoting.
- [ ] Handle paths containing spaces and unusual characters.
- [ ] Capture standard output and standard error according to the chosen design.
- [ ] Preserve the compiler's exit status.
- [ ] Handle a missing compiler executable.
- [ ] Handle compiler failure without reporting a successful build.
- [ ] Avoid deadlocks when capturing process output.
- [ ] Handle interrupted or terminated child processes appropriately.

### Build model

- [ ] Compile one source file.
- [ ] Support multiple translation units.
- [ ] Pass include paths, compiler flags, and linker flags predictably.
- [ ] Separate compile and link steps where required.
- [ ] Define configuration-file syntax and validation rules.
- [ ] Report invalid configuration with a useful location or field name.
- [ ] Model source-to-object and object-to-executable dependencies.
- [ ] Rebuild when a source or relevant input changes.
- [ ] Avoid rebuilding unaffected targets unnecessarily.
- [ ] Handle deleted or renamed inputs.
- [ ] Ensure parallel builds do not write the same output concurrently.
- [ ] Limit parallelism to a reasonable resource budget.
- [ ] Stop or clean up work sensibly after a failed task.
- [ ] Keep build output understandable and actionable.

### Release gate

- [ ] A fresh checkout can build `forge`.
- [ ] Automated tests cover argument parsing and build decisions.
- [ ] Integration tests invoke a real compiler in a temporary project.
- [ ] Tests cover success, compile failure, missing compiler, and malformed configuration.
- [ ] Documentation includes examples, supported scope, and known limitations.
- [ ] No claim is made that `forge` is a complete replacement for mature build systems.

## 8. `termkit` terminal UI library

### Terminal handling

- [ ] Detect terminal versus redirected input/output where needed.
- [ ] Keep terminal-specific behavior behind a clear abstraction.
- [ ] Handle terminal dimensions changing during execution.
- [ ] Restore terminal state on normal exit.
- [ ] Restore terminal state on handled errors and interruption paths where feasible.
- [ ] Avoid leaving the user's terminal in raw mode after failure.
- [ ] Handle escape sequences that arrive in partial reads.
- [ ] Handle unknown or unsupported input sequences safely.
- [ ] Keep input decoding separate from widget behavior.
- [ ] Avoid assuming every terminal supports every color or control sequence.

### Rendering and API

- [ ] Define the screen buffer and coordinate conventions.
- [ ] Clip drawing operations to valid bounds.
- [ ] Handle wide or non-ASCII characters according to the documented scope.
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

- [ ] Check every relevant socket/system-call result.
- [ ] Close file descriptors on all appropriate paths.
- [ ] Handle partial reads and writes.
- [ ] Handle interrupted system calls where applicable.
- [ ] Set explicit connection and resource limits.
- [ ] Define behavior for clients that connect but send no data.
- [ ] Define timeouts and maximum request sizes.
- [ ] Avoid unbounded buffering.
- [ ] Avoid unbounded thread or task creation.
- [ ] Handle shutdown while clients are active.
- [ ] Log errors without leaking sensitive data.

### HTTP parsing and framing

- [ ] Parse the request line strictly enough to reject malformed input.
- [ ] Validate header syntax and limits.
- [ ] Handle duplicate headers according to documented rules.
- [ ] Define supported HTTP versions and methods.
- [ ] Correctly determine message-body framing.
- [ ] Reject ambiguous or invalid framing rather than guessing.
- [ ] Handle incomplete requests and premature disconnects.
- [ ] Enforce maximum header count, header size, and body size.
- [ ] Generate valid response status lines and headers.
- [ ] Avoid writing conflicting or invalid body-framing headers.
- [ ] Test malformed input and connection-close behavior.
- [ ] Keep parser tests independent of live network tests.

### Concurrency and shutdown

- [ ] State the threading/concurrency model.
- [ ] Protect shared state or avoid sharing it.
- [ ] Test simultaneous clients.
- [ ] Test slow clients and resource exhaustion scenarios.
- [ ] Ensure one client's failure does not corrupt unrelated connections.
- [ ] Define graceful shutdown behavior.
- [ ] Stop accepting new connections when shutdown begins.
- [ ] Ensure worker threads/tasks are joined or otherwise safely completed.
- [ ] Run ThreadSanitizer where supported and useful.

### Security and production-readiness boundaries

- [ ] Apply strict size and time limits.
- [ ] Avoid parsing malformed input permissively when that creates ambiguity.
- [ ] Test request smuggling-relevant framing ambiguities within the supported HTTP/1.1 scope.
- [ ] Do not implement cryptography yourself.
- [ ] Use a maintained TLS library if HTTPS is added.
- [ ] Document that HTTP/2, HTTP/3, TLS, compression, and other features are unsupported unless actually implemented and tested.
- [ ] Review RFC 9110 and RFC 9112 requirements relevant to implemented behavior.
- [ ] Test under load without treating a single benchmark as proof of production readiness.
- [ ] Document deployment assumptions and operational limitations.

### Release gate

- [ ] Parser unit tests cover valid and invalid messages.
- [ ] Integration tests cover real socket behavior.
- [ ] Tests cover slow clients, disconnects, limits, and shutdown.
- [ ] No unbounded resource path is known to remain in the supported feature set.
- [ ] Public API examples compile and run.
- [ ] Documentation states the supported protocol subset clearly.

## 10. Integration and final release

### Integration

- [ ] The monorepo builds from a clean checkout.
- [ ] Each product can be built and tested independently.
- [ ] `forge` can build the workspace's supported targets.
- [ ] `termkit` can display useful build status without becoming coupled to `forge` internals.
- [ ] `httpkit` can expose the intended build/status endpoint with explicit resource limits.
- [ ] Shared code has clear ownership and dependency boundaries.
- [ ] No circular dependency has been introduced.
- [ ] Integration failures have dedicated tests.

### Quality and documentation

- [ ] All test suites pass in a clean build.
- [ ] Sanitizer runs have been performed where supported.
- [ ] Debug and Release builds have both been checked.
- [ ] Public APIs have concise usage examples.
- [ ] Each product README includes build, test, run, scope, and limitations.
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
- [ ] Confirm that the repository is understandable to someone who did not build it.

## 11. Milestone completion record

Copy this section into `PROGRESS.md` for each milestone.

```markdown
## Milestone: [number and name]

- **Target week:**
- **Started:**
- **Completed:**
- **Status:** Not started / In progress / Blocked / Complete
- **Outcome in one sentence:**

### Acceptance criteria
- [ ] Criterion 1
- [ ] Criterion 2
- [ ] Criterion 3

### Verification evidence
- **Build command:**
- **Test command and result:**
- **Manual test or demo:**
- **Commit:**

### Notes
- **What I learned:**
- **Remaining limitations:**
- **Deferred scope and reason:**
- **Next concrete action:**
```

## 12. Final self-assessment

Do not grade yourself by whether every checkbox is marked. Grade yourself by whether you can demonstrate and explain the important behavior.

- [ ] I can explain the ownership and lifetime model of the code I wrote.
- [ ] I can reproduce and diagnose a representative failure.
- [ ] I can build and test the repository from a clean checkout.
- [ ] I can explain the design trade-offs and the limitations of each product.
- [ ] I can show tests for important normal, edge, and failure cases.
- [ ] I can identify which code is educational and which code is intended for reuse.
- [ ] I can explain what would be required before deploying the HTTP server in a real production environment.
- [ ] I have documented what is complete, what is incomplete, and what I would do next.

**Rule:** A checked box means evidence exists. It does not mean “I read about it once.”
