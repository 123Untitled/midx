# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

MIDX is a domain-specific language for live MIDI sequencing and music composition.
It features a custom lexer, Pratt parser, and real-time MIDI playback engine with CoreMIDI integration on macOS.

## Build System

This project uses a custom Zsh build script (`make.sh`) instead of Make or CMake.

### Building

```bash
./make.sh              # Build the project
./make.sh run          # Build and run
./make.sh clean        # Clean build artifacts
./make.sh re           # Clean and rebuild
./make.sh test         # Build and launch in tmux with nvim to live edit test files
```

The build script:
- Uses `clang++` with C++23 standard
- Supports `ccache` for faster compilation
- Compiles all `.cpp` files in `sources/` directory
- Links against CoreMIDI, CoreAudio, and CoreFoundation frameworks (macOS)
- Generates `compile_commands.json` for IDE integration
- Outputs executable: `./midx`

### Compiler Settings

- Standard: C++23
- Optimization: `-O0` (debug)
- Sanitizers: AddressSanitizer enabled by default (ThreadSanitizer commented out)
- Warnings: Strict (`-Wall -Wextra -Werror -Wpedantic -Weffc++`)
- Error limit: 1 (stops at first error)

## Project Structure

```
include/          # Header files (.hpp)
  ├── language/   # Language implementation (lexer, parser, AST, analyzer)
  ├── coremidi/   # CoreMIDI wrapper classes
  ├── core/       # Core utilities (type traits, containers, memory)
  ├── monitoring/ # File monitoring and IPC
  ├── time/       # Timing and clock utilities
  └── midi/       # MIDI engine and constants

sources/          # Implementation files (.cpp)
  ├── language/   # Lexer, parser, AST, analyzer implementations
  ├── coremidi/   # CoreMIDI implementations
  ├── monitoring/ # File monitoring, server, client
  └── main.cpp    # Entry point

*.midx            # MIDILang source files (language syntax files)
```

## Architecture

### Language Pipeline

1. **Lexer** (`lx::lexer`): Tokenizes `.midx` source files into tokens
   - Located in: `include/language/lexer.hpp`, `sources/language/lexer.cpp`
   - Produces: `tk::tokens` (token stream)
   - Handles: identifiers, notes, numbers, parameters, references, tempo, modulo

2. **Parser** (`pr::parser`): Pratt parser that builds an AST
   - Located in: `include/language/parser.hpp`, `sources/language/parser.cpp`
   - Uses precedence-based parsing with levels (expression vs sequence)
   - Parser is currently being reworked (see `parser/precedence.hpp`, `parser/table.hpp`)
   - Produces: `as::tree` (abstract syntax tree)

3. **AST** (`as::tree`): Arena-based AST representation
   - Located in: `include/language/ast/tree.hpp`, `sources/language/ast/tree.cpp`
   - Node types: `include/language/ast/node.hpp`
   - Uses type-erased storage with alignment-aware allocation
   - Nodes: program, group, parallel, crossfade, track, parameter, atomic_values, references, tempo, modulo, permutation
   - Storage strategy: contiguous byte array with remapping for indirection

4. **Analyzer** (`mx::analyzer`): Semantic analysis and preparation
   - Located in: `include/language/analyzer.hpp`, `sources/language/analyzer.cpp`
   - Coordinates lexer and parser

5. **Evaluator** (`as::eval`): Real-time AST evaluation engine
   - Located in: `include/language/ast/eval.hpp`, `sources/language/ast/eval.cpp`
   - Core evaluation system that traverses the AST and generates MIDI events
   - Uses a two-phase time-based evaluation strategy for edge detection
   - Integrates with `mx::midi_engine` and `mx::highlight_tracker`

6. **Player** (`mx::player`): Real-time MIDI playback controller
   - Located in: `include/player.hpp`, `sources/player.cpp`
   - Orchestrates the evaluation process based on musical clock time
   - Interfaces with `as::eval` and `mx::midi_engine`

### Key Subsystems

**File Monitoring & Hot Reload**
- `mx::monitor`: Watches `.midx` files for changes
- `mx::server`: Unix socket server for IPC
- `mx::protocol`: Communication protocol for reload triggers
- When a `.midx` file changes, the application re-parses and updates the player

**MIDI Engine**
- Located in: `include/midi/midi_engine.hpp`
- Wraps CoreMIDI for cross-platform MIDI output
- Handles note on/off events with timing

**Timing System**
- `mx::clock`: Musical clock for sequencing
- `time::realtime`: High-resolution timing
- `time::bpm`: Tempo calculations
- Uses `mx::frac` (rational numbers) for precise musical timing

**CoreMIDI Integration**
- C++ wrapper around macOS CoreMIDI framework
- Classes: `coremidi::client`, `coremidi::output`, `coremidi::input`, `coremidi::source`, `coremidi::destination`
- Located in: `include/coremidi/`, `sources/coremidi/`

**Highlight Tracker**
- `mx::highlight_tracker`: Tracks active tokens during evaluation for editor highlighting
- Located in: `include/language/highlight_tracker.hpp`, `sources/language/highlight_tracker.cpp`
- Maintains two maps (_now and _old) to detect changes between evaluation cycles
- Generates JSON output for real-time syntax highlighting in the editor
- Uses an optimization strategy: switches from checking mode to force mode when changes are detected

### Evaluation System

The evaluation system is the core of MIDX's real-time MIDI sequencing. It traverses the AST and produces MIDI events based on musical time.

**Core Components**

1. **Evaluator (`as::eval`)**
   - Main evaluation engine that walks the AST recursively
   - Maintains references to the AST tree, tokens, and highlight tracker
   - Handles edge detection using a hash-based system to trigger notes only at boundaries
   - Supports crossfade state management for smooth transitions between sequences

2. **Evaluation Frame (`as::frame`)**
   - Represents a single evaluation context as the evaluator traverses the AST
   - Contains:
     - `node`: Current AST node index
     - `hash`: Computed hash for edge detection (uses rehashing for uniqueness)
     - `time`: Current local time within this node
     - `last`: Previous evaluation time (used for edge detection)
   - `propagate()` method creates child frames with updated hash

3. **Track Result (`as::track_result`)**
   - Accumulates MIDI parameters for a single track evaluation
   - Contains accumulators for: trigger, note, channel, velocity, gate, probability, octave, semitone
   - `flush()` sends accumulated MIDI events to the engine (currently unused, replaced by expr_result)

4. **Expression Result (`as::expr_result`)**
   - Accumulates MIDI events across multiple notes and channels
   - Uses a sparse array indexed by (channel << 7 | note) for efficient event storage
   - Tracks active events and their counts for averaging
   - `accumulate()` processes track results, handling probability and parameter application
   - `merge()` combines results from parallel or crossfade evaluations
   - `flush()` computes averaged gate/velocity and sends events to MIDI engine

5. **Random Generator (`mx::random`)**
   - Simple random number generator for probability parameter
   - Seeded with process ID
   - Used to randomly skip notes based on `:pr` (probability) parameter

**Two-Phase Evaluation Strategy**

The evaluator uses two time values (`time` and `last`) to detect edges:
- `time`: Current evaluation time
- `last`: Previous evaluation time (denominator 0 indicates no previous time)

**Edge Detection**:
- Edges trigger new notes and parameter changes
- Detected when crossing node boundaries or when time wraps
- Uses an optimization: when `last` is valid, evaluates nodes converging until paths diverge
- After divergence, sets `last.den = 0` and continues with single-phase evaluation

**Node Evaluation Methods**

Each AST node type has a specialized evaluation function:

- **`program`**: Root node, iterates over all top-level children
- **`group`**: Sequential composition, selects child based on accumulated durations
- **`parallel`**: Evaluates all children with same time, merges results
- **`tempo`**: Time scaling, divides child duration by tempo factors, highlights active tempo
- **`modulo`**: Time wrapping, clamps time within modulo values, highlights active modulo
- **`track`**: MIDI track container, evaluates parameter nodes and accumulates into track_result
- **`parameter`**: Like group, but for parameter sequences (`:nt`, `:vl`, etc.)
- **`atomics`**: Leaf values, computes step index and retrieves value, marks token as highlighted
- **`references`**: Indirection, looks up referenced node and evaluates it
- **`crossfade`**: Blends two sequences:
  - Discrete parameters (note, trig, chan): alternates between sides using error accumulation
  - Continuous parameters (velo, gate, prob, octa, semi): linear interpolation
  - Maintains per-node crossfade state in `_cross` map

**Evaluation Flow**

1. `evaluate()` is called with current musical time
2. Creates an `expr_result` to accumulate events
3. Calls `program()` which dispatches to child nodes
4. Each node:
   - Applies modulo to wrap time within its duration
   - Uses two-phase strategy to detect edges
   - Calls specialized evaluation based on node type
   - Marks active tokens for highlighting
5. Parameter accumulators detect edges and accumulate values
6. Track results are accumulated into expr_result
7. Final `flush()` averages values and sends MIDI events

**Highlight Integration**

During evaluation, active tokens are marked for highlighting:
- `atomics` nodes: Highlights the active value token
- `tempo` nodes: Highlights the active tempo modifier
- `modulo` nodes: Highlights the active modulo operator
- `references` nodes: Highlights the active reference token
- Highlight tracker generates JSON for the editor to display

### AST Node System

The AST uses an arena allocator pattern with:
- Type-erased storage in `_nodes` (byte vector)
- Remapping layer `_remap` for indirection
- Separate storage for values (`_values`), fractions (`_fracs`), and references (`_refs`)
- All nodes have an `as::header` containing type and duration
- Durations represented as `mx::frac` (numerator/denominator)

Node type hierarchy:
- Container nodes: `group`, `parallel`, `permutation` (have child ranges)
- Leaf nodes: `atomic_values` (parameter values), `references` (identifier refs)
- Modifier nodes: `tempo` (time scaling), `modulo` (time wrapping)
- Playback nodes: `track` (MIDI track), `parameter` (MIDI parameter)
- Composition nodes: `crossfade` (blend two sequences)

### Parser Development

The parser is undergoing a rework to use Pratt parsing with operator precedence:
- New files: `include/language/parser/precedence.hpp`, `include/language/parser/table.hpp`, `include/language/parser/rules.hpp`
- Precedence levels defined in `pr::level` enum
- Currently being migrated from older implementation

## Language Syntax (MIDILang)

MIDX (`.midx` files) is a sequencing language with:
- Parameters: `:nt` (note), `:vl` (velocity), `:tr` (trigger), `:ga` (gate), `:ch` (channel), `:pr` (probability), `:oc` (octave), `:se` (semitone)
- Operators: `|` (parallel), `\` (tempo decrease), `%` (clamp length), `^` (tempo increase), `><` (crossfade)
- References: `&name` to reference named sequences
- Comments: `~` at start of line
- See `hello.midx` for examples

## Development Workflow

### Making Changes to the Parser

1. Read relevant parser files first: `include/language/parser.hpp`, `sources/language/parser.cpp`
2. Understand the Pratt parsing approach in `include/language/parser/precedence.hpp`
3. The parser is currently being reworked - check recent git commits for context
4. Test changes with example `.midx` files like `hello.midx` in `tests/` directory

### Making Changes to the AST

1. Node definitions are in `include/language/ast/node.hpp`
2. Tree implementation in `include/language/ast/tree.hpp` and `.cpp`
3. Remember: AST uses arena allocation with alignment requirements
4. All nodes must start with `as::header`
5. Use `make_node<T>()` for proper alignment

### Making Changes to the Evaluation System

1. Core evaluator is in `include/language/ast/eval.hpp` and `sources/language/ast/eval.cpp`
2. Each node type requires a specialized evaluation method (e.g., `group()`, `tempo()`, `track()`)
3. Understand the two-phase evaluation strategy: `time` and `last` are used for edge detection
4. When adding new node types:
   - Add evaluation method to `as::eval` class
   - Update `dispatch()` switch statement to handle new type
   - Decide if node should mark tokens for highlighting
   - Ensure proper frame propagation with hash updates
5. Parameter accumulators are in `include/language/ast/param_accum.hpp`
6. Test with various time signatures and tempo changes to verify edge detection

### Testing

- Run `./make.sh run` to build and execute
- Test files are `.midx` files in the `tests/` directory
- The application has built-in file watching - edit `.midx` files while running
- Use `./make.sh test` to launch tmux session with nvim and the running executable

### Debugging

- AddressSanitizer is enabled by default
- Debug symbols: `-g3` with `-gdwarf-4`
- Compilation logs saved to `.log` files during build (auto-deleted on success)
- Error limit set to 1 for focused debugging

## Important Conventions

- **Namespaces**:
  - `mx::` - main project namespace
  - `lx::` - lexer
  - `tk::` - tokens
  - `pr::` - parser
  - `as::` - AST
  - `pa::` - parameters
  - `sx::` - syntax
  - `an::` - analyzer/diagnostics

- **Type aliases**: `mx::u8`, `mx::u16`, `mx::u32`, `mx::u64`, `mx::i8`, `mx::i16`, `mx::i32`, `mx::i64`, `mx::usz` (size_t), `mx::isz` (signed size)

- **Custom containers**: The project implements custom containers in `include/core/containers/` rather than using STL

- **No RTTI**: The build has `-fno-rtti` commented out but may be enabled in the future

- **Alignment**: AST nodes use `alignas(std::max_align_t)` for proper memory alignment

## Platform-Specific Notes

- **macOS only**: CoreMIDI framework is macOS-specific
- Linux support partially stubbed out but not fully implemented (see ALSA comments in `make.sh`)
- Uses macOS system calls for timing (`mach_absolute_time`)
