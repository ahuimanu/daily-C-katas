# C++ Daily Katas — Modern Systems Practice

This directory contains small, focused C++20 katas used to refresh and harden modern C++ skills with a **systems / native-plugin mindset**.

These are not algorithm puzzles.
They are **lifetime, ownership, and correctness exercises**.

Each kata is designed to take **20–30 minutes**.

---

## Purpose

The goal of these katas is to build and maintain fluency with modern C++ (C++17/20) features that are **essential for native, long-lived, and lifecycle-hostile environments**, such as:

- Flight simulator plugins (e.g., X-Plane)
- Audio / telemetry pipelines
- Lock-free or low-latency systems
- Dynamically loaded modules
- Reloadable subsystems

The emphasis is on:

- deterministic lifetimes
- explicit ownership
- zero-leak / zero-double-free guarantees
- correctness under move, scope exit, and teardown

---

## Kata Structure

Each kata:

- is **self-contained**
- uses **no frameworks**
- avoids unnecessary scaffolding
- relies on:
  - the type system
  - RAII
  - explicit scope
  - assertions

Verification is done by:

- running the program
- letting assertions and crashes expose mistakes

---

## Rotation Topics

Katas rotate through these areas:

- RAII and ownership
- `std::optional` / expected-style results (no exceptions)
- `std::span` and zero-copy boundaries
- Lock-free SPSC ring buffers
- `<ranges>` for explicit data pipelines
- Simple, explicit state machines

Each topic builds toward writing safe, maintainable native code.

---

## How to Use

- Open the daily kata
- Implement it in a single `.cpp` file
- Compile and run
- Fix until assertions pass
- Move on

No overthinking.  
No polishing.  
Consistency beats cleverness.

---

## Long-Term Goal

By repeating these katas, the goal is to be able to:

- design plugin subsystems as composable RAII components
- reload and tear down cleanly
- reason about ownership without comments
- trust scope and types instead of discipline

This is how modern C++ is meant to be used.

---

## Kata 1 — RAII File Guard

**Topic:** RAII / ownership  
**Duration:** ~25 minutes  

### Goal

Implement a minimal RAII wrapper that guarantees a `FILE*` is closed exactly once.

### Focus

- Deterministic cleanup via destructor
- Non-copyable, move-only ownership
- Safe moved-from state
- Explicit scope as a lifecycle boundary

### Key Mechanics

- Resource acquired in constructor
- Resource released in destructor
- Move constructor transfers ownership and nulls the source
- Move assignment closes existing resource before taking ownership

### Verification

- `assert(file)` after open
- Move to a new guard:
  - `assert(!file)`
  - `assert(other)`
- End inner scope to force destructor execution
- Reopen same file with append mode and write again

### Takeaway

> Scope defines lifetime.  
> RAII makes cleanup deterministic, even under move and teardown.

---

## Kata 2 — Strict Integer Parsing with `std::optional`

**Topic:** `std::optional` / explicit failure  
**Duration:** ~20 minutes  

### Kata 2 Goal

Parse a strictly formatted integer without exceptions, using `std::optional` to signal failure.

### Kata 2 Focus

- Representing failure explicitly
- Rejecting malformed input early
- Avoiding exceptions and locale-dependent parsing
- Boundary-safe numeric conversion

### Kata 2 Key Mechanics

- Empty input → failure
- Reject any whitespace anywhere
- Accept optional leading `+` / `-`
- All remaining characters must be digits
- Detect overflow / underflow for `int`
- Require full consumption of input

### Kata 2 Verification

- Known-good values succeed (`123`, `-7`, `INT_MAX`, `INT_MIN`)
- Malformed input returns `std::nullopt`
- Overflow and underflow return `std::nullopt`

### Kata 2 Takeaway

> `std::optional` forces the caller to acknowledge failure.  
> Silent fallbacks and exceptions are not acceptable at system boundaries.

---

## Kata 3 — Lock-Free SPSC Ring Buffer

**Topic:** Lock-free data structures / atomics  
**Duration:** ~30 minutes  

### Kata 3 Goal

Implement a single-producer / single-consumer (SPSC) ring buffer with no locks and fixed capacity.

### Kata 3 Focus

- Lock-free coordination using atomics
- Correct memory ordering (`acquire` / `release`)
- Distinguishing full vs empty without extra flags
- FIFO correctness under wraparound

### Kata 3 Key Mechanics

- One slot is always unused to distinguish full from empty
- Producer owns `head`, consumer owns `tail`
- `push`:
  - Loads `tail` with `acquire`
  - Stores new `head` with `release`
- `pop`:
  - Loads `head` with `acquire`
  - Stores new `tail` with `release`
- No locks, no condition variables, no resizing

### Kata 3 Verification

- Construct with capacity `8` (payload capacity `7`)
- Push values `1..7` successfully
- Next push fails (buffer full)
- Pop values `1..7` in FIFO order
- Next pop fails (buffer empty)

### Kata 3 Takeaway

> Lock-free does not mean “clever.”  
> It means **strict ownership, minimal sharing, and correct memory ordering**.

---

## ---

## Kata 4 — `<ranges>` Transform + Filter Pipeline

**Topic:** `<ranges>` / declarative data pipelines  
**Duration:** ~20 minutes  

### Kata 4 Goal

Process a sequence using a composable `<ranges>` pipeline with no explicit loops.

### Kata 4 Focus

- Declarative, lazy data processing
- Separating *what* from *how*
- Eliminating index and loop control errors
- Clean materialization at the boundary

### Kata 4 Key Mechanics

- `views::filter` to select values (`> 0`)
- `views::transform` to map values (square)
- `views::take` to limit results
- Lazy evaluation until materialized into a container

### Kata 4 Verification

- Input: `{ -3, -2, -1, 0, 1, 2, 3, 4, 5 }`
- Output: `{ 1, 4, 9 }`
- Assert full vector equality

### Kata 4 Takeaway

> `<ranges>` turns data flow into a readable, testable pipeline.  
> Control logic disappears; intent becomes explicit.
— `<ranges>` Transform + Filter Pipeline

---

## Kata 5 — Explicit State Machine (Flight Leg Lifecycle)

**Topic:** Explicit state machines / `std::optional`  
**Duration:** ~20 minutes  

### Kata 5 Goal

Model a small, explicit state machine for an aircraft flight leg with well-defined transitions.

### Kata 5 Focus

- Making state and transitions explicit
- Rejecting invalid transitions deterministically
- Avoiding implicit flags or ad-hoc conditionals
- Using `std::optional` to represent invalid state changes

### Kata 5 Key Mechanics

- `State` and `Event` modeled as `enum class`
- `transition(State, Event)` returns:
  - `State` for valid transitions
  - `std::nullopt` for invalid transitions
- No side effects; pure transition function

### Kata 5 Verification

- Drive the valid “happy path” through all states:
  `parked → taxi_out → takeoff → cruise → approach → landed → taxi_in → parked`
- Assert invalid transitions return `std::nullopt`:
  - `parked + touchdown`
  - `cruise + rotate`
  - `taxi_in + begin_approach`

### Kata 5 Takeaway

> Explicit state machines replace hidden assumptions with enforced rules.  
> Invalid transitions become impossible to ignore.
