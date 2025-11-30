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

5. **Player** (`mx::player`): Real-time MIDI playback from AST
   - Located in: `include/player.hpp`, `sources/player.cpp`
   - Evaluates AST nodes based on musical time
   - Interfaces with `mx::midi_engine`

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
