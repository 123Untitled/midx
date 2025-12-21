# MIDX TODO

## Language Features

### Scale System
- [ ] Implement `.scale` lookup table for scale transformations
  - Example: `.scale C minor`
  - Lookup table: `jump[127] = {0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, -1}`
  - Implement multiple modes: follow movement, go down, go up, random, etc.
  - Decide how to choose `+` or `-` direction

### Note Names
- [ ] Accept `Cb`, `Fb`, `B#`, `E#` as valid note names


### Parameters

- [ ] Add random step parameter `:rd`
  - Example: `:rd 0 0 (tr | nt) 0 vl`
  - Enable/disable random step selection (0 or 1)
  - Consider adding random value parameter

- [ ] Add monophonic parameter
  - Force only one note at a time

- [ ] Implement parameter reference

## Operators

### Logical Superposition Operators
- [ ] `<+>` - Positive superposition (AND logic)
- [ ] `<->` - Negative superposition (NAND logic)
- [ ] `<^>` - Exclusive superposition (XOR logic)
- [ ] `<=>` - Equivalence superposition (XNOR logic)

## Neovim Integration

- [ ] Bug when opening multiple vim instances with MIDX files
- [ ] Disable `<>` in match parent (`()[]{}` only)
- [ ] `leader+space` → play/stop toggle
- [ ] MIDI note live selector/listener
- [ ] Generate different colors when multiple highlights on same step


## Bugs to Fix

### Critical
- [ ] Fix: `Pushing range start=0 count=0`
- [ ] Fix track separator after identifier
  ```midx
  world ::
    :tr 0 1 0 1
    :nt [B2 (55 | 62 29) D5 F7];
  ```


### Tempo

### Modulo
- [ ] Highlight modulo operator on first step
- [ ] Modulo `%0.3` seen as 1 virtual step
  - Detect if edge is true when step != 1


## Improvements

- [ ] Implement smarter fraction reduction system

## Data Structure Ideas

### Trigger Array
Explore alternative data structures for triggers:

**Option 1**: Two separate arrays
- Array of triggers
- Array of steps before next trigger

**Option 2**: Single array of structs
```cpp
struct {
    bool trig;
    uint step_to_next;
};
```

# Optimization

- [ ] fill one midi packet all evaluations (new clock implementation)
