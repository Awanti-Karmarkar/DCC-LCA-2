# Bully Algorithm Simulation — C

A terminal-based, interactive simulation of the **Bully Election Algorithm** used in distributed systems. When a coordinator process crashes, this program walks through the full election process step-by-step, printing every message exchanged between processes so you can follow the algorithm in real time.

---

## Table of Contents

- [What is the Bully Algorithm?](#what-is-the-bully-algorithm)
- [How the Algorithm Works](#how-the-algorithm-works)
- [Features](#features)
- [File Structure](#file-structure)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Compilation](#compilation)
  - [Running the Program](#running-the-program)
- [Usage Guide](#usage-guide)
  - [Initial Setup](#initial-setup)
  - [Menu Options](#menu-options)
- [Example Walkthrough](#example-walkthrough)
- [Sample Output](#sample-output)
- [Code Structure](#code-structure)
- [Key Design Decisions](#key-design-decisions)
- [Limitations](#limitations)

---

## What is the Bully Algorithm?

The **Bully Algorithm** (Garcia-Molina, 1982) is a classic **leader election algorithm** for distributed systems. It is used to elect a new coordinator whenever the current one fails.

The core rule is simple:

> **The process with the highest ID always wins — it "bullies" everyone else out of the race.**

This simulation models a set of numbered processes (`P1` through `Pn`) where each process can be active or failed. When no coordinator exists, any active process can start an election.

---

## How the Algorithm Works

### Election Steps

1. **A process notices the coordinator is down** (or it recovers and has a higher ID than the current coordinator).
2. **It sends an `ELECTION` message** to all processes with a higher ID.
3. **Any higher-ID process that is alive replies with `OK`**, meaning "back off, I'll handle it", and starts its own election.
4. **The initiator that receives no `OK` replies** declares itself the new coordinator.
5. **The new coordinator broadcasts a `COORDINATOR` message** to all lower-ID processes to announce the result.

### Why "Bully"?

At every step, a higher-ranked process can bully a lower-ranked one out of the election simply by sending an OK. The highest alive process will always end up bullying everyone else into submission.

### Trigger Conditions

| Event | Election Triggered? |
|---|---|
| Coordinator process fails | ✔ Yes — next lower active process initiates |
| A recovered process has higher ID than coordinator | ✔ Yes — recovered process initiates |
| A process fails but is not the coordinator | ✘ No |
| A recovered process has lower ID than coordinator | ✘ No |

---

## Features

- **Interactive menu** — fail, recover, and inspect processes at any time
- **Step-by-step election trace** — every `ELECTION` and `OK` message is printed with directional arrows
- **Live status bar** — shows all process states after every change
- **Coordinator broadcast** — shows the final `COORDINATOR` announcement to all lower processes
- **Input validation** — guards against invalid process numbers and operating on already-failed/active processes
- **Recursive election cascade** — accurately models how each higher process takes over and restarts the election
- **Step counter** — each recursive election phase is numbered `[STEP 1]`, `[STEP 2]`, etc.

---

## File Structure

```
.
├── bully_algorithm.c     # Full source code
└── README.md             # This file
```

---

## Getting Started

### Prerequisites

- A C compiler: `gcc` (Linux/macOS) or MinGW/MSVC (Windows)
- Standard C library (no external dependencies)

### Compilation

**Linux / macOS:**
```bash
gcc -o bully bully_algorithm.c
```

**Windows (MinGW):**
```bash
gcc -o bully.exe bully_algorithm.c
```

### Running the Program

```bash
./bully
```

---

## Usage Guide

### Initial Setup

When you launch the program, you will be prompted for:

1. **Number of processes** — enter a number between 1 and 10.
2. **Status of each process** — enter `1` for Active, `0` for Failed.

The program will automatically assign the **highest active process** as the initial coordinator.

**Example input for 7 processes with P5 failed:**
```
Enter number of processes: 7

Enter status of each process [1 = Active, 0 = Failed]:
  Process P1: 1
  Process P2: 1
  Process P3: 1
  Process P4: 1
  Process P5: 0
  Process P6: 1
  Process P7: 1
```

---

### Menu Options

```
┌─── MENU ──────────────────────────────────┐
│  1. Display process status                │
│  2. Fail a process                        │
│  3. Recover a process                     │
│  4. Manually start an election            │
│  5. Show current coordinator              │
│  6. Exit                                  │
└───────────────────────────────────────────┘
```

| Option | Description |
|---|---|
| **1. Display** | Prints a table of all processes and their current status, highlighting the coordinator |
| **2. Fail a process** | Marks a process as failed. If it is the coordinator, an election is automatically triggered from the next lower active process |
| **3. Recover a process** | Marks a failed process as active again. If the recovered process has a higher ID than the current coordinator, it immediately starts a new election (as per the Bully Algorithm) |
| **4. Manual election** | Lets you pick any active process to start an election manually — useful for testing or classroom demonstration |
| **5. Show coordinator** | Prints the current coordinator process number |
| **6. Exit** | Exits the simulation |

---

## Example Walkthrough

**Scenario:** 7 processes, P5 already failed, P7 is coordinator. Now fail P7.

**Step 1 — Fail the coordinator (P7):**
- The program detects P7 (coordinator) has crashed.
- It finds the next lower active process: **P6**.
- P6 initiates the election.

**Step 2 — P6 sends ELECTION to P7:**
- P7 is dead → no response.
- No higher process responded.
- P6 declares itself coordinator.
- P6 broadcasts `COORDINATOR` to P1–P5.

**Result:** P6 is the new coordinator.

---

Now **recover P7** (higher ID than coordinator P6):

**Step 1 — P7 recovers:**
- P7's ID (7) > current coordinator P6's ID (6).
- P7 must start a new election immediately.

**Step 2 — P7 sends ELECTION:**
- No higher process exists.
- No responses received.
- P7 declares itself coordinator.
- P7 broadcasts `COORDINATOR` to all lower processes.

**Result:** P7 is coordinator again.

---

## Sample Output

```
╔══════════════════════════════════════════════╗
║       BULLY ALGORITHM  SIMULATION            ║
╚══════════════════════════════════════════════╝

  ★  Initial coordinator is P7  ★
  Status │  P1:OK  │  P2:OK  │  P3:OK  │  P4:OK  │  P5:FAIL │  P6:OK  │ P7[COORD]│

  ────────────────────────────────────────────────────
  [STEP 1] Process 3 starts ELECTION
  ────────────────────────────────────────────────────
  P3 will now send ELECTION msg to all higher-ID processes.

  P3  ──ELECTION──►  P4  ... P4 is ALIVE  →  replies OK  ✔
  P3  ──ELECTION──►  P5  ... P5 is DEAD   →  no response  ✘
  P3  ──ELECTION──►  P6  ... P6 is ALIVE  →  replies OK  ✔
  P3  ──ELECTION──►  P7  ... P7 is ALIVE  →  replies OK  ✔

  P3 got OK from P7 (highest responder).
  P3 steps BACK and lets P7 take over.

  ────────────────────────────────────────────────────
  [STEP 2] Process 7 starts ELECTION
  ────────────────────────────────────────────────────
  P7 will now send ELECTION msg to all higher-ID processes.

  ╔══════════════════════════════════════════╗
  ║  No higher process responded.            ║
  ║  >>> P7 declares itself COORDINATOR <<<  ║
  ╚══════════════════════════════════════════╝

  P7 now broadcasts COORDINATOR message to all lower processes:
  P7  ──COORDINATOR──►  P6  (acknowledged)
  P7  ──COORDINATOR──►  P4  (acknowledged)
  P7  ──COORDINATOR──►  P3  (acknowledged)
  P7  ──COORDINATOR──►  P2  (acknowledged)
  P7  ──COORDINATOR──►  P1  (acknowledged)

  ★  Coordinator after election: P7  ★
```

---

## Code Structure

| Function | Purpose |
|---|---|
| `main()` | Program entry point — handles setup, menu loop, and dispatches actions |
| `election(int initiator)` | Recursive election function — sends ELECTION messages, handles OK responses, declares winner, and broadcasts COORDINATOR |
| `findInitiator()` | Finds the highest-ID active process below the (just-failed) coordinator to start an election |
| `display()` | Prints a formatted table of all process statuses |
| `printBar()` | Prints a compact one-line status bar showing all process states |
| `divider()` | Prints a horizontal rule for visual separation between election steps |

---

## Key Design Decisions

**Recursive election cascade** — the `election()` function calls itself with the highest responder's ID. This cleanly models the real algorithm where each higher process takes over and re-runs the election from its own perspective, rather than jumping directly to the winner.

**`findInitiator()` uses a saved coordinator value** — when the coordinator fails, its ID is temporarily preserved so `findInitiator()` can scan downward from the correct starting point before `coordinator` is reset to `-1`.

**Recovery triggers election immediately** — when a recovered process has a higher ID than the current coordinator, it calls `election()` on itself without waiting for user confirmation. This matches the Bully Algorithm specification.

**Live status bar after every state change** — `printBar()` is called at the end of every case that modifies process state, giving you an at-a-glance overview without needing to select the display option manually.

---

## Limitations

- **Simulated, not distributed** — all processes run in a single program on one machine. There is no actual network communication or concurrency.
- **Maximum 10 processes** — set by the `MAX` constant; can be increased by changing the `#define`.
- **No timeout modeling** — in a real distributed system, a process detects coordinator failure via a timeout. Here, failure is triggered manually by the user.
- **Sequential message delivery** — messages are processed one by one in order. Real systems deal with out-of-order delivery and network partitions.
