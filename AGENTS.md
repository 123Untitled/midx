# Repository Guidelines

## Project Structure & Module Organization
- Core C++ sources live in `sources/`; public headers in `include/`. Keep new modules paired (`sources/foo.cpp`, `include/foo.hpp`).
- The CLI binary outputs to `midx` at repo root. Build products and `compile_commands.json` are managed by `make.sh`.
- DSL examples and smoke fixtures live in `tests/*.midx`; use them when validating parser/player changes.
- Additional language notes are in `documentation/` (`language.md`, `permutations.md`).

## Build, Test, and Development Commands
- `./make.sh` — default build with clang++ (C++23) and AddressSanitizer; regenerates `compile_commands.json`.
- `./make.sh run` — build then launch `midx`.
- `./make.sh test` — build then start a tmux session with `tests/hello.midx` open and `midx` running; use to iterate quickly on the DSL.
- `./make.sh clean` or `./make.sh rm` — remove objects, logs, binary, and compile DB. `./make.sh rebuild` does clean + build.

## Coding Style & Naming Conventions
- C++23 with strict warnings (`-Wall -Wextra -Werror`) and ASan enabled. Prefer `mx::` aliases/types and RAII helpers already used across the codebase.
- Files and identifiers use lower_snake_case (e.g., `string_pool`, `application`). Constructors place member init lists on separate lines; use trailing return types (`auto foo() -> type`).
- Indent with tabs; align wrapped parameters/args; keep brace style consistent with existing sources.
- Avoid RTTI (`-fno-rtti`); favor explicit ownership over raw pointers. Keep includes ordered: corresponding header first, then local modules.

## Testing Guidelines
- No formal unit framework yet; rely on sample `.midx` programs in `tests/` to exercise parser, analyzer, and player paths.
- When adding language features, provide a minimal `.midx` example under `tests/` and verify it plays/parsings via `./make.sh run` or `./make.sh test`.
- If you change MIDI I/O, sanity-check on macOS CoreMIDI; Linux support is partial and should be guarded.

## Commit & Pull Request Guidelines
- Commit messages follow short, imperative summaries (e.g., `fix coremidi ableton bug`). Squash only if changes are tightly related.
- PRs should describe behavior changes, note platform tested (macOS/Linux), and link related issues. Include before/after notes for DSL or protocol changes.
- Add screenshots or short notes when altering user-facing MIDI behavior, defaults, or the DSL syntax.
