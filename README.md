# MIDX

A domain-specific language for live MIDI sequencing and music composition.

## Features

- **Real-time MIDI sequencing** with precise musical clock
- **Custom DSL** with lexer, Pratt parser, and AST evaluation engine
- **CoreMIDI integration** (macOS) for native MIDI output
- **Hot-reload** - edit `.midx` files while the sequencer is running
- **Powerful operators**: parallel (`|`), crossfade (`><`), tempo modifiers (`\` `^`), modulo (`%`)
- **MIDI parameters**: `:nt` (note), `:tr` (trigger), `:vl` (velocity), `:ga` (gate), `:ch` (channel), `:pr` (probability), `:oc` (octave), `:se` (semitone)
- **Named references** with `&name` for reusable sequences
- **Arena-based AST** with type-erased storage for efficient memory management

## Requirements

- macOS (CoreMIDI framework required)
- clang++ with C++23 support
- MIDI interface (virtual or physical)

## Building

```bash
./make.sh              # Build the project
./make.sh run          # Build and run
./make.sh clean        # Clean build artifacts
./make.sh re           # Clean and rebuild
./make.sh test         # Launch in tmux with nvim for live editing
```

The build script uses:
- Custom Zsh build system (no Make/CMake)
- `ccache` for faster compilation (optional)
- AddressSanitizer enabled by default
- Generates `compile_commands.json` for IDE integration

## Quick Start

Create a `.midx` file with a simple sequence:

```
a
    :tr 1 0 0 | 1 0
    :nt 60 67 68
    :ch 1
;

&a
```

Run it:
```bash
./make.sh run
```

This creates a track named `a` with:
- **Trigger pattern**: `1 0 0` parallel with `1 0` (two rhythmic layers)
- **Notes**: 60 (C4), 67 (G4), 68 (G#4)
- **MIDI channel**: 1

The final line `&a` references and plays the track.

## Language Syntax

### Parameters

MIDI parameters define the properties of a track:

- `:nt` - Note (MIDI note number or name like `C#3`, `A4`)
- `:tr` - Trigger (1 = play note, 0 = rest)
- `:vl` - Velocity (note volume, 0-127)
- `:ga` - Gate (note duration)
- `:ch` - Channel (MIDI channel, 0-15)
- `:pr` - Probability (chance of playing, 0.0-1.0)
- `:oc` - Octave (octave shift)
- `:se` - Semitone (semitone offset)

### Operators

- `|` - **Parallel**: Play sequences simultaneously
- `><` - **Crossfade**: Gradually transition between two sequences
- `\` - **Tempo decrease**: Slow down the sequence
- `^` - **Tempo increase**: Speed up the sequence
- `%` - **Modulo**: Constrain sequence length

### References

- `&name` - Reference a named sequence
- `name = :tr 1 0 1 0;` - Define a named sequence

### Comments

- `~` - Line comment

## Example

A more complex composition with multiple tracks:

```
~ Bass track
a
    :tr ^2 ^4 1 0 0 | 1 0
    :ch 1
    :nt \2 60 67 68 68
;

~ Lead track
b
    :tr ^4 1 0 0 | 1 0 0 0 | 1 0 0 0 0
    :nt \4 79 77 75 %4 (74 | ^2 94 96)
    :ch 2 2 2 ^2 0 2
    :se 0 0 0 7 0 0 0 0
;

~ Drums
dr
    :tr \1 (^4 ^2 ^8 (%4 1))
    :ch ^4 4 4 4 4 5 4 4 4
;

~ Kick
kc
    :tr ^2 1 0 0 0 1 0 0 0
    :ch 3
;

~ Main composition
m
    &a |
    &b |
    &dr |
    &kc |
;

~ Play with crossfade
(%32 \1 &m) >< (%32 \1 &m)
```

This example demonstrates:
- Multiple named tracks with different instruments
- Tempo modifiers (`^2`, `\4`) for polyrhythmic patterns
- Parallel operator (`|`) for layering tracks
- Crossfade operator (`><`) for smooth transitions
- Nested sequences with parentheses
- Modulo operator (`%`) for rhythmic constraints

## Project Structure

```
include/          # Header files
  ├── language/   # Language implementation (lexer, parser, AST, analyzer)
  ├── coremidi/   # CoreMIDI wrapper classes
  ├── midi/       # MIDI engine and constants
  ├── monitoring/ # File monitoring and IPC
  └── time/       # Timing and clock utilities

sources/          # Implementation files
  ├── language/   # Lexer, parser, AST, analyzer implementations
  ├── coremidi/   # CoreMIDI implementations
  └── main.cpp    # Entry point
```

## Architecture

The MIDX pipeline:

1. **Lexer** (`lx::lexer`) - Tokenizes `.midx` source files
2. **Parser** (`pr::parser`) - Pratt parser builds an AST
3. **Analyzer** (`mx::analyzer`) - Semantic analysis
4. **Evaluator** (`as::eval`) - Real-time AST evaluation with edge detection
5. **Player** (`mx::player`) - Orchestrates playback based on musical clock
6. **MIDI Engine** (`mx::midi_engine`) - Sends MIDI events via CoreMIDI

The evaluator uses a two-phase time-based strategy for edge detection, maintaining `time` and `last` values to trigger notes only at boundaries.

## Development

For detailed development instructions, see [`CLAUDE.md`](CLAUDE.md).

**Key namespaces:**
- `mx::` - main project
- `lx::` - lexer
- `tk::` - tokens
- `pr::` - parser
- `as::` - AST
- `pa::` - parameters

**Custom containers:** The project implements custom containers in `include/core/containers/` rather than using STL.

**Testing:** Use `./make.sh test` to launch a tmux session with nvim and the running executable for live editing of `.midx` test files.

## Platform Support

- **macOS**: Full support with CoreMIDI
- **Linux**: Partial support (ALSA integration in progress)
