# LCM (Least Common Multiple) in MIDX

## Overview

In MIDX, **all sequence durations** are calculated using the **LCM (Least Common Multiple)**.

This applies specifically to:
- **Tracks** (when combining multiple parameters)
- **Parallel operator** `|` (when playing sequences simultaneously)
- **Crossfade operator** `><` (when blending between sequences)

The LCM ensures that sequences of different lengths are synchronized by guaranteeing:
1. Each sequence completes an **integer** number of cycles
2. All sequences end **simultaneously**
3. The global pattern repeats **coherently**

## Basic Principle

When multiple sequences are combined (in parallel `|`, crossfade `><`, or within a track), the system computes the LCM of the block count of each sequence to determine the total duration.

### Formula

```
total_duration = lcm(block_count₁, block_count₂, ..., block_countₙ)
```

Each sequence `i` then completes exactly `lcm / block_count_i` cycles.

## ASCII Diagram

```
Sequence A: [1 2 3]           3 blocks
Sequence B: [a b]             2 blocks

LCM(3, 2) = 6 total blocks

┌─────────────────────────────┐
│  Combined sequence (A | B)  │
└─────────────────────────────┘

Block: 0   1   2   3   4   5   │ (6 blocks total)
       ─   ─   ─   ─   ─   ─   │
A:     1   2   3   1   2   3   │ 2 complete cycles (6/3 = 2)
       ↻───────↻   ↻───────↻   │
                                │
B:     a   b   a   b   a   b   │ 3 complete cycles (6/2 = 3)
       ↻───↻   ↻───↻   ↻───↻   │
                                │
Result:                         │
     [1,a] [2,b] [3,a] [1,b] [2,a] [3,b]
```

## Concrete Example

### Parallel Operator `|`

```midx
kick :nt C1 D1 E1      ~ 3 values
snare :nt C2 D2        ~ 2 values

~ Total pattern has LCM(3, 2) = 6 steps
~ kick plays 2 times: C1 D1 E1 | C1 D1 E1
~ snare plays 3 times: C2 D2 | C2 D2 | C2 D2
```

### Track with Multiple Parameters

```midx
track
  :nt C3 D3 E3       ~ 3 notes
  :vl 64 127         ~ 2 velocities

~ Track duration = LCM(3, 2) = 6 steps
~ Notes cycle 2 times: C3 D3 E3 | C3 D3 E3
~ Velocities cycle 3 times: 64 127 | 64 127 | 64 127
```

## Advantages

- **Automatic synchronization**: No need to manually duplicate values
- **Natural polyrhythms**: Easy creation of complex patterns
- **Coherent repetition**: The global pattern loops cleanly
- **No partial cycles**: All sequences end simultaneously

## Special Cases

### Same-length sequences
```
A: [1 2 3]    3 blocks
B: [a b c]    3 blocks

LCM(3, 3) = 3 → each sequence plays once
```

### Simple ratio sequences
```
A: [1 2 3 4]  4 blocks
B: [a b]      2 blocks

LCM(4, 2) = 4 → A plays once, B plays twice
```

### Coprime numbers
```
A: [1 2 3]    3 blocks
B: [a b c d e]  5 blocks

LCM(3, 5) = 15 → A plays 5 times, B plays 3 times
```

## Implementation

In the MIDX codebase, each AST node has:
- A type (`as::type`)
- A computed duration (`mx::frac dur`)

The duration is stored as a **rational fraction** (`mx::frac`) for perfect precision:
```cpp
struct header {
    as::type type;
    mx::frac dur;  // numerator/denominator
};
```

This avoids rounding errors during musical time calculations.
