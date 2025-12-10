# Duration

## Overview

> [!NOTE]
> In MIDX, **all sequence durations** are calculated using the **LCM (Least Common Multiple)**.
> The LCM ensures that sequences of different lengths are synchronized by guaranteeing:
> 1. Each sequence completes an **integer** number of cycles
> 2. All sequences end **simultaneously**
> 3. The global pattern repeats **coherently**

This applies specifically to:
- [Tracks](documentation/track.md) (when combining multiple parameters)
- [Parallel]() operator** `|` (when playing sequences simultaneously)
- [Crossfade]() operator** `><` (when blending between sequences)




## ASCII Diagram

```
Sequence A: [1 2 3] 3 steps
Sequence B: [a b]   2 steps

LCM(3, 2) = 6 total steps

┌─────────────────────────────┐
│  Combined sequence (A | B)  │
└─────────────────────────────┘

A:   1   2   3   1   2   3   │ 2 complete cycles (6/3 = 2)
     -───────↻   -───────↻   │
                             │
B:   a   b   a   b   a   b   │ 3 complete cycles (6/2 = 3)
     -──-↻   -───↻   -───↻   │

```

## Advantages

- **Automatic synchronization**: No need to manually duplicate values
- **Natural polyrhythms**: Easy creation of complex patterns
- **Coherent repetition**: The global pattern loops cleanly
- **No partial cycles**: All sequences end simultaneously
