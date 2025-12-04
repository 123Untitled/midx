```
░▒▓██████████████▓▒░░▒▓█▓▒░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░ 
░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░  
░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░ 
```
# 🥁 midx

A domain-specific language for live MIDI sequencing and music composition.

## ⚙️ Requirements

- macOS (CoreMIDI framework required)
- clang++ with C++23 support
- MIDI interface (virtual or physical)

## 📖 Language Syntax

### Parameters

MIDI parameters define the properties of a track:

- `:nt` - Note (MIDI note number or name like `C#3`, `A4`)
- `:tr` - Trigger (1 = play note, 0 = rest)
- `:vl` - Velocity (note volume, 0-127)
- `:ga` - Gate (note duration)
- `:ch` - Channel (MIDI channel, 0-15)
- `:pr` - Probability (chance of playing, 0-100)
- `:oc` - Octave (octave shift)
- `:se` - Semitone (semitone offset)

### Operators

- `|` - **Parallel**: Play sequences simultaneously
- `><` - **Crossfade**: Gradually transition between two sequences
- `\` - **Tempo decrease**: Slow down the sequence
- `^` - **Tempo increase**: Speed up the sequence
- `%` - **Modulo**: Constrain sequence length

### References

- `&name` - Reference an expression
- `name :tr 1 0 1 0;` - Define an expression

### Comments

- `~` - Line comment

### Exemple
```

m
	:tr \2 ((%8 (^4 1 0 0 (%3 ^2 1 0) 1 0 0)) | ^4 1 0 0 1 0)
	:nt \4 60 63 56 53 | (\1.5 67 70 72) %4 68 67
	:ch 2 2 ^4 0 0
	:vl ^4 100 80 80
	:pr ^2 100 100
	:oc 0 0 0 0 1
;

b
	:tr %4 \1.5 1 %3 ^4 1 0 0 %1 ^4 1
	:nt \4 60 63 56 53
	:oc 0 0 0 1 0 2
	:se 0 0 7
	:ch 1
;

&m | &b

```



## 🔌 Platform Support

- **macOS**: Full support with CoreMIDI
- **Linux**: Partial support (ALSA integration in progress)
