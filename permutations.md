# Tree Expansion System: Algorithms and Mathematics

A formal specification of the tree-based sequence/permutation expansion system, independent of implementation language.

---

## Table of Contents

1. [Mathematical Model](#mathematical-model)
2. [Node Types and Semantics](#node-types-and-semantics)
3. [Expansion Algorithms](#expansion-algorithms)
4. [LCM-Based Cycling](#lcm-based-cycling)
5. [Efficient Computation Algorithms](#efficient-computation-algorithms)
6. [Time-Based Indexing](#time-based-indexing)
7. [Formal Proofs](#formal-proofs)
8. [Complexity Analysis](#complexity-analysis)

---

## Mathematical Model

### Basic Definitions

**Atom**: A tuple `(name, duration)` where:
- `name` ∈ String
- `duration` ∈ ℝ⁺ (positive real numbers)

**Block**: A sequence of atoms:
```
Block = [atom₁, atom₂, ..., atomₙ]
```

**Expansion**: A list of blocks:
```
Expansion = [block₁, block₂, ..., blockₘ]
```

**Block Duration**: The sum of all atom durations in a block:
```
duration(block) = Σ duration(atomᵢ) for all atoms in block
```

---

## Node Types and Semantics

### 1. Atom Node

**Definition**: `Atom(name, duration)`

**Expansion**: Returns a single block containing one atom.

```
expand(Atom(n, d)) = [[(n, d)]]
```

**Properties**:
- Block count: 1
- Total duration: `d`

**Example**:
```
Atom("A", 2.5) → [[(A, 2.5)]]
```

---

### 2. Sequence Node (Seq)

**Definition**: `Seq([child₁, child₂, ..., childₙ])`

**Semantics**: Concatenates children horizontally using LCM-based cycling.

**Expansion Algorithm**:

```
Algorithm: EXPAND_SEQ(children)
Input: List of child nodes
Output: List of blocks

1. For each child cᵢ:
   - blocks_i = expand(cᵢ)
   - count_i = |blocks_i|

2. Compute L = lcm(count₁, count₂, ..., countₙ)

3. result = []
4. For i = 0 to L-1:
   - row = []
   - For each child index j:
     - block_idx = i mod count_j
     - row = row ⊕ blocks_j[block_idx]  // concatenate
   - result.append(row)

5. Return result
```

where `⊕` denotes list concatenation.

**Properties**:
- Block count: `lcm(count₁, count₂, ..., countₙ)`
- Each child cycles through its blocks at its own rate
- All children are synchronized by LCM

**Example**:
```
Seq([
  Atom("A", 1),           // 1 block
  Perm([                  // 3 blocks
    Atom("1", 1),
    Atom("2", 1),
    Atom("3", 1)
  ]),
  Atom("B", 1)            // 1 block
])

LCM(1, 3, 1) = 3

Expansion:
  Block 0: [A, 1, B]
  Block 1: [A, 2, B]
  Block 2: [A, 3, B]
```

---

### 3. Permutation Node (Perm)

**Definition**: `Perm([child₁, child₂, ..., childₙ])`

**Semantics**: Cycles through children in round-robin fashion using LCM.

**Expansion Algorithm**:

```
Algorithm: EXPAND_PERM(children)
Input: List of child nodes
Output: List of blocks

1. For each child cᵢ:
   - blocks_i = expand(cᵢ)
   - count_i = |blocks_i|

2. Compute L = lcm(count₁, count₂, ..., countₙ)

3. result = []
4. indices = [0, 0, ..., 0]  // one index per child

5. For iteration = 0 to L-1:
   - For each child index j from 0 to n-1:
     - result.append(blocks_j[indices[j]])
     - indices[j] = (indices[j] + 1) mod count_j

6. Return result
```

**Properties**:
- Block count: `lcm(count₁, count₂, ..., countₙ) × n`
- Children are visited in round-robin order
- Each child's blocks cycle based on LCM

**Example**:
```
Perm([
  Seq([Atom("X", 1), Atom("Y", 1)]),  // 1 block: [X,Y]
  Atom("Z", 2)                         // 1 block: [Z]
])

LCM(1, 1) = 1
Block count = 1 × 2 = 2

Iteration 0:
  - Child 0, index 0: [X, Y]
  - Child 1, index 0: [Z]

Result: [[X,Y], [Z]]
```

---

## LCM-Based Cycling

### Why LCM?

The Least Common Multiple ensures that:
1. Each child completes an integer number of cycles
2. The overall pattern repeats after LCM iterations
3. No partial cycles occur

### Mathematical Justification

For children with block counts `c₁, c₂, ..., cₙ`:

```
L = lcm(c₁, c₂, ..., cₙ)
```

Each child `i` makes exactly `L / cᵢ` complete cycles through its blocks.

**Proof**:
- Child `i` has `cᵢ` blocks
- In `L` iterations, child `i` is accessed `L` times (for Seq) or `L` times total (for Perm)
- Since `L` is divisible by `cᵢ`, we have `L / cᵢ` complete cycles
- Therefore: `L / cᵢ ∈ ℤ` ✓

### Computing LCM

For two numbers:
```
lcm(a, b) = (a × b) / gcd(a, b)
```

For multiple numbers:
```
lcm(a₁, a₂, ..., aₙ) = reduce(lcm, [a₁, a₂, ..., aₙ])
                      = lcm(...lcm(lcm(a₁, a₂), a₃)..., aₙ)
```

---

## Efficient Computation Algorithms

### Block Count (Without Expansion)

**For Atom**:
```
block_count(Atom) = 1
```

**For Seq**:
```
block_count(Seq(children)) = lcm(block_count(c₁), ..., block_count(cₙ))
```

**For Perm**:
```
block_count(Perm(children)) = lcm(block_count(c₁), ..., block_count(cₙ)) × n
```

**Complexity**: O(n × log(max_count)) where n is number of children

---

### Total Duration (Without Expansion)

**Definition**: Sum of all atom durations across all blocks.

**For Atom**:
```
total_duration(Atom(n, d)) = d
```

**For Seq and Perm**:

The key insight: each child contributes based on how many times its blocks are used.

```
Algorithm: TOTAL_DURATION(node)
Input: A Seq or Perm node
Output: Total duration

1. children = node.children
2. L = block_count(node) for Seq
   OR L = lcm(block_count(c₁), ...) for Perm

3. total = 0
4. For each child cᵢ:
   - count_i = block_count(cᵢ)
   - duration_i = total_duration(cᵢ)
   - repetitions = L / count_i
   - total += duration_i × repetitions

5. Return total
```

**Mathematical Formula**:
```
total_duration(node) = Σ (total_duration(cᵢ) × L/count_i)
```

where:
- `cᵢ` is the i-th child
- `L` is the LCM (for Seq) or the base LCM (for Perm)
- `count_i` is the block count of child `cᵢ`

**Why This Works**:

Each child `cᵢ` produces `count_i` blocks with combined duration `total_duration(cᵢ)`.

In the parent's expansion:
- The parent produces `L` blocks (for Seq)
- Each of child `cᵢ`'s blocks appears `L/count_i` times
- Total contribution: `total_duration(cᵢ) × (L/count_i)`

**Complexity**: O(n) where n is total number of nodes in tree

---

### Block Durations (Without Full Expansion)

**For Atom**:
```
block_durations(Atom(n, d)) = [d]
```

**For Seq**:

```
Algorithm: BLOCK_DURATIONS_SEQ(children)
Input: List of child nodes
Output: List of block durations

1. L = lcm(block_count(c₁), ..., block_count(cₙ))
2. For each child cᵢ:
   - durations_i = block_durations(cᵢ)

3. result = []
4. For i = 0 to L-1:
   - block_duration = 0
   - For each child index j:
     - idx = i mod |durations_j|
     - block_duration += durations_j[idx]
   - result.append(block_duration)

5. Return result
```

**For Perm**:

```
Algorithm: BLOCK_DURATIONS_PERM(children)
Input: List of child nodes
Output: List of block durations

1. L = lcm(block_count(c₁), ..., block_count(cₙ))
2. For each child cᵢ:
   - durations_i = block_durations(cᵢ)

3. result = []
4. indices = [0, 0, ..., 0]

5. For iteration = 0 to L-1:
   - For each child index j:
     - result.append(durations_j[indices[j]])
     - indices[j] = (indices[j] + 1) mod |durations_j|

6. Return result
```

**Complexity**: O(B) where B is the number of output blocks

---

## Time-Based Indexing

### Problem Statement

Given a time `t ∈ ℝ⁺`, find the atom `(name, duration)` that is active at time `t`.

### Interpretation

Blocks are played **sequentially**:
- Block 0 plays from time `[0, d₀)`
- Block 1 plays from time `[d₀, d₀+d₁)`
- Block i plays from time `[Σⱼ₌₀ⁱ⁻¹ dⱼ, Σⱼ₌₀ⁱ dⱼ)`

Within each block, atoms play **sequentially** based on their durations.

### Algorithm

```
Algorithm: GET_ATOM_AT_TIME(node, t)
Input: A node and time t
Output: Name of atom active at time t

1. If t < 0:
   - Error: "Time must be non-negative"

2. durations = block_durations(node)

3. cumulative_time = 0
4. For block_idx = 0 to |durations|-1:
   - next_time = cumulative_time + durations[block_idx]

   - If cumulative_time ≤ t < next_time:
     // Found the correct block
     - time_in_block = t - cumulative_time
     - block = get_block(node, block_idx)  // expand and get specific block

     - atom_time = 0
     - For each (name, duration) in block:
       - If atom_time ≤ time_in_block < atom_time + duration:
         - Return name
       - atom_time += duration

     - Return last atom name in block  // edge case: at end

   - cumulative_time = next_time

5. Error: "Time t is beyond total duration"
```

### Mathematical Definition

Let:
- `B = [block₀, block₁, ..., blockₘ₋₁]` be the expansion
- `dᵢ = duration(blockᵢ)` be the duration of block i
- `Dᵢ = Σⱼ₌₀ⁱ⁻¹ dⱼ` be the cumulative time before block i

For time `t`, find block index `k` such that:
```
Dₖ ≤ t < Dₖ₊₁
```

Within block `blockₖ = [atom₀, atom₁, ..., atomₙ₋₁]`, find atom index `j` such that:
```
Σᵢ₌₀ʲ⁻¹ duration(atomᵢ) ≤ (t - Dₖ) < Σᵢ₌₀ʲ duration(atomᵢ)
```

Return `atomⱼ.name`.

### Complexity

- **Time**: O(B + A) where B is number of blocks and A is average atoms per block
- **Space**: O(B) for storing block durations

**Optimization**: For repeated queries, cache the expansion.

---

## Formal Proofs

### Theorem 1: LCM Ensures Complete Cycles

**Statement**: For Seq or Perm with children having block counts `c₁, c₂, ..., cₙ`, using `L = lcm(c₁, ..., cₙ)` ensures each child completes an integer number of cycles.

**Proof**:
1. By definition of LCM: `L mod cᵢ = 0` for all i
2. Therefore: `L / cᵢ ∈ ℤ` for all i
3. In L iterations, child i is accessed L times (Seq) or L times total (Perm)
4. Since `L / cᵢ` is an integer, child i completes exactly `L / cᵢ` full cycles
5. ∴ No partial cycles occur ∎

---

### Theorem 2: Total Duration Formula Correctness

**Statement**: For a node with LCM `L` and children with block counts `cᵢ` and total durations `dᵢ`:
```
total_duration(node) = Σᵢ (dᵢ × L/cᵢ)
```

**Proof**:

Let child `cᵢ` have blocks `[b₁, b₂, ..., b_cᵢ]` with durations `[δ₁, δ₂, ..., δ_cᵢ]`.

Then: `dᵢ = Σⱼ δⱼ` (total duration of child i)

In the parent's expansion (Seq case):
- Parent produces `L` blocks
- In block k, child i contributes block `b_(k mod cᵢ)`
- Each block `bⱼ` of child i appears in positions: j, j+cᵢ, j+2cᵢ, ..., j+(L/cᵢ-1)cᵢ
- Count: `L/cᵢ` times

Total contribution from child i:
```
Σⱼ (δⱼ × L/cᵢ) = (Σⱼ δⱼ) × L/cᵢ = dᵢ × L/cᵢ
```

Summing over all children:
```
total_duration = Σᵢ (dᵢ × L/cᵢ) ∎
```

---

### Theorem 3: Seq Block Count

**Statement**: For `Seq([c₁, c₂, ..., cₙ])` where child i produces `kᵢ` blocks:
```
block_count(Seq) = lcm(k₁, k₂, ..., kₙ)
```

**Proof**:

By the expansion algorithm, Seq produces `L = lcm(k₁, ..., kₙ)` blocks.

For each output block index `i ∈ [0, L)`:
- Each child j contributes its block at index `i mod kⱼ`
- Since `L mod kⱼ = 0`, all children complete full cycles
- The pattern repeats after L blocks

Any `L' < L` would not be divisible by all `kⱼ`, causing incomplete cycles.

∴ `block_count(Seq) = lcm(k₁, ..., kₙ)` ∎

---

### Theorem 4: Perm Block Count

**Statement**: For `Perm([c₁, c₂, ..., cₙ])` where child i produces `kᵢ` blocks:
```
block_count(Perm) = lcm(k₁, k₂, ..., kₙ) × n
```

**Proof**:

By the expansion algorithm:
- Compute `L = lcm(k₁, ..., kₙ)`
- For `L` iterations, cycle through all n children
- Each iteration visits n children in order
- Total blocks: `L × n`

∴ `block_count(Perm) = L × n` ∎

---

## Complexity Analysis

### Space Complexity

**Expansion**:
- Atom: O(1)
- Seq: O(L × A) where L is block count, A is average atoms per block
- Perm: O(L × n × A) where n is number of children

**Efficient Methods**:
- `block_count()`: O(1) per node, O(N) for tree with N nodes
- `total_duration()`: O(1) per node, O(N) for tree
- `block_durations()`: O(B) where B is output block count

### Time Complexity

**Expansion**:
- Atom: O(1)
- Seq: O(L × n × A) where L is LCM, n is children count, A is avg atoms per child block
- Perm: O(L × n × A)

**Efficient Methods**:
- `block_count()`: O(n × log M) where M is max block count (for LCM computation)
- `total_duration()`: O(N) where N is total nodes in tree
- `block_durations()`: O(B) where B is output block count
- `get_atom_at_time(t)`: O(B + A) where A is atoms in selected block

### When to Use Each Method

| Operation | Use Expansion | Use Efficient Method |
|-----------|---------------|---------------------|
| Need all atoms | ✓ | |
| Count blocks | | ✓ |
| Total duration | | ✓ |
| Query single time | | ✓ |
| Multiple time queries | ✓ (cache) | |
| Block durations | | ✓ |

---

## Examples with Full Traces

### Example 1: Nested Seq and Perm

```
Tree = Seq([
  Atom("A", 1),
  Perm([
    Seq([Atom("1", 1), Atom("2", 1)]),
    Atom("3", 2)
  ]),
  Atom("B", 1)
])
```

**Step-by-step expansion**:

1. Inner Seq:
   - Child 1: Atom("1", 1) → 1 block
   - Child 2: Atom("2", 1) → 1 block
   - LCM(1, 1) = 1
   - Result: 1 block [[1, 2]]

2. Perm:
   - Child 1: Seq → 1 block [[1, 2]]
   - Child 2: Atom("3", 2) → 1 block [[3]]
   - LCM(1, 1) = 1
   - Round-robin for 1 iteration:
     - Iteration 0, child 0: [1, 2]
     - Iteration 0, child 1: [3]
   - Result: 2 blocks [[1, 2], [3]]

3. Outer Seq:
   - Child 1: Atom("A") → 1 block [[A]]
   - Child 2: Perm → 2 blocks [[1, 2], [3]]
   - Child 3: Atom("B") → 1 block [[B]]
   - LCM(1, 2, 1) = 2

   - Block 0: [A] + [1, 2] + [B] = [A, 1, 2, B]
   - Block 1: [A] + [3] + [B] = [A, 3, B]

   - Result: 2 blocks

**Block durations**:
- Block 0: 1 + 1 + 1 + 1 = 4
- Block 1: 1 + 2 + 1 = 4

**Total duration**: 4 + 4 = 8

**Time queries**:
- t=0.5 → Block 0, time 0.5 → "A"
- t=2.5 → Block 0, time 2.5 → "2"
- t=5.0 → Block 1, time 1.0 → "3"

---

### Example 2: LCM Cycling with Different Sizes

```
Tree = Seq([
  Perm([Atom("1", 1), Atom("2", 1), Atom("3", 1)]),  // 3 blocks
  Perm([Atom("a", 1), Atom("b", 1)])                 // 2 blocks
])
```

**Expansion**:

1. Perm 1: 3 blocks [[1], [2], [3]]
2. Perm 2: 2 blocks [[a], [b]]
3. LCM(3, 2) = 6

Seq produces 6 blocks:
```
i=0: i mod 3 = 0, i mod 2 = 0 → [1] + [a] = [1, a]
i=1: i mod 3 = 1, i mod 2 = 1 → [2] + [b] = [2, b]
i=2: i mod 3 = 2, i mod 2 = 0 → [3] + [a] = [3, a]
i=3: i mod 3 = 0, i mod 2 = 1 → [1] + [b] = [1, b]
i=4: i mod 3 = 1, i mod 2 = 0 → [2] + [a] = [2, a]
i=5: i mod 3 = 2, i mod 2 = 1 → [3] + [b] = [3, b]
```

**Cycles**:
- Perm 1: 6/3 = 2 complete cycles (1,2,3, 1,2,3)
- Perm 2: 6/2 = 3 complete cycles (a,b, a,b, a,b)

**Duration calculation** (without expansion):
- Perm 1: block_count=3, total_duration=3, repetitions=6/3=2, contribution=3×2=6
- Perm 2: block_count=2, total_duration=2, repetitions=6/2=3, contribution=2×3=6
- Total: 6 + 6 = 12 ✓

---

## Implementation Guidelines

### Language-Agnostic Pseudocode

```
CLASS Node:
    ABSTRACT METHOD expand() -> List[Block]
    ABSTRACT METHOD block_count() -> Integer
    ABSTRACT METHOD total_duration() -> Real
    ABSTRACT METHOD block_durations() -> List[Real]

    METHOD get_atom_at_time(t: Real) -> String:
        // Use algorithm from Time-Based Indexing section
        ...

CLASS Atom EXTENDS Node:
    FIELD name: String
    FIELD duration: Real

    METHOD expand():
        RETURN [[(name, duration)]]

    METHOD block_count():
        RETURN 1

    METHOD total_duration():
        RETURN duration

    METHOD block_durations():
        RETURN [duration]

CLASS Seq EXTENDS Node:
    FIELD children: List[Node]

    METHOD expand():
        // Use EXPAND_SEQ algorithm
        ...

    METHOD block_count():
        counts = MAP(child.block_count(), children)
        RETURN lcm(counts)

    METHOD total_duration():
        // Use TOTAL_DURATION algorithm
        ...

    METHOD block_durations():
        // Use BLOCK_DURATIONS_SEQ algorithm
        ...

CLASS Perm EXTENDS Node:
    FIELD children: List[Node]

    METHOD expand():
        // Use EXPAND_PERM algorithm
        ...

    METHOD block_count():
        counts = MAP(child.block_count(), children)
        RETURN lcm(counts) * LENGTH(children)

    METHOD total_duration():
        // Use TOTAL_DURATION algorithm (same as Seq)
        ...

    METHOD block_durations():
        // Use BLOCK_DURATIONS_PERM algorithm
        ...
```

---

## References and Further Reading

### Mathematical Concepts
- **Least Common Multiple (LCM)**: Number theory, divisibility
- **Greatest Common Divisor (GCD)**: Euclidean algorithm
- **Cyclic patterns**: Modular arithmetic

### Related Algorithms
- **Round-robin scheduling**: Similar to Perm expansion
- **Interleaving sequences**: Related to Seq expansion
- **Time-based state machines**: Similar to time indexing

### Applications
- **Music sequencing**: Notes, rhythms, patterns
- **Animation timelines**: Keyframes, interpolation
- **Procedural generation**: Patterns, variations
- **Scheduling**: Task allocation, resource management

---

## Appendix: Notation Reference

| Symbol | Meaning |
|--------|---------|
| ℝ⁺ | Positive real numbers |
| ℤ | Integers |
| ∈ | Element of |
| Σ | Summation |
| ⊕ | Concatenation |
| lcm(a, b, ...) | Least common multiple |
| gcd(a, b) | Greatest common divisor |
| \|S\| | Cardinality (size) of set/list S |
| [a, b) | Half-open interval: a ≤ x < b |
| mod | Modulo operation |
| ∴ | Therefore |
| ∎ | End of proof |

---

*This document provides a complete, language-agnostic specification of the tree expansion system, suitable for implementation in any programming language.*
