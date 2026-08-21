# String and format handling in function blocks

How function blocks size their string buffers, and the rules that keep
`printf`/`scanf` in a block from reading or writing past what it was given.

Applies to `Common/Components/core/stringfn.c` (the `string_format*` and
`stringfn_scanf8` blocks) and to any block that formats, scans, or takes a
free-text parameter.

## The three string size limits must nest

```
EHS_FB_FORMAT_MAX <= EHS_STRING_LENGTH_MAX <= EHS_DATA_TABLE_STRING_DEFAULT_LENGTH
```

Each bounds a buffer that the next one along gets copied into, so an inversion
means some copy is bounded by more than its destination holds. `globals.h`
enforces it with `#error` on every build; `base_config.h` also checks the
right-hand relation and usually fires first. If a target looks like it needs to
sit outside these bounds, the buffer sizing is wrong, not the limits.

The values are per-target and easy to misjudge: on `base_small` all three are
**256**; on `base_full` they are 512 / 2048 / **32768**. Never size a stack
buffer from `EHS_DATA_TABLE_STRING_DEFAULT_LENGTH` — see the stack note in
[`DEVELOPING.md`](DEVELOPING.md). `EHS_FB_FORMAT_MAX` bounds only a format
arriving on a run-time input port; a format taken from a block parameter lives
in the block context, sized from the parameter, and is not capped by it.

Check a target's actual values rather than assuming - the platform's
`target_config.h` chooses which `base_config.h` it pulls in, and a `-I` to the
other base does not override that.

## A free-text parameter must have its spaces un-escaped

The SODL parameter line is space separated, so Lucid replaces every space
*inside* a parameter value with the ASCII Unit Separator 0x1F
(`escape_param_spaces` in `LucidApplicationBuilder/SODL.cpp`). The target has to
put them back.

A block that reads its parameter with `EhsGetRecordFromString` gets that for
free — the conversion is part of tokenising. A block that takes the whole
parameter as **one free-text value** — a format string, a search string —
usually just `EhsStrcpy`s it, and then every space the user typed stays an
unprintable control character. `EhsParamUnescapeSpaces` (`hal_string.c`) is the
one-line fix; call it right after the copy.

The symptom is that spaces silently vanish from the block's output: a Format of
`"Temp: %s"` renders as `Temp:alpha`. It is easy to misread as a formatting bug,
because it usually gets noticed next to a `%`.

## Never count `%` to decide how many arguments a format takes

Any block that builds a `printf` call by switching on a placeholder count must
get that count from `EhsFormatScan` (`Common/HAL/string/hal_string.c`). Do not
scan for `'%'`, and do not decide validity by looking at the single character
after the `'%'`.

Both shortcuts were in `stringfn.c` and both were wrong in the same direction —
they *under*-count. A specifier carrying a flag, width or precision (`%10s`,
`%.5s`, `%-8s`, `%05d`) matches neither `fmt[i+1]=='s'` nor a one-character ban
list, so it was passed to `EhsSnprintf` uncounted: `"%s and %.5s"` supplied one
argument for two conversions and printf read whatever followed in the argument
list, printing stack contents as a string. The one-character ban list was also
bypassable — `%n` is rejected but `%5n` was not, giving an arbitrary write.

This is reachable from the wire, not just from the diagram: `stringfn_formats8`
declares a `fmt` **input port** (index 8), so the format can be runtime data.

`EhsFormatScan` walks the real C99 grammar
(`%[flags][width][.precision][length]conversion`), understands `%%` as
consuming nothing, and rejects `*`, length modifiers, `%n` and any conversion
outside the one argument class the caller can supply. Its returned count is
exactly what printf will consume.

Related invariants in the same path:

- **Loop counters over a format string must not be `ehs_uint8`.** They were, and
  `for (i=0; i<EhsStrlen(f)-1; i++)` wrapped 255→0 and spun forever on any
  format of 257+ characters. `EhsStrlen` is `#define`d to `strlen` on every
  target, so `strlen("")-1` is `SIZE_MAX` and an *empty* format hung too. (The
  `ehs_sint16 EhsStrlen` fallback in `hal_string.h` returns −1 and is benign,
  which is why this never showed up in host testing.)
- **`EhsParseEscapeChars` is bounded now** and takes the output capacity. It
  never expands (each input character yields at most one output character) and
  is safe in place, which is what lets the format blocks escape the parameter
  once into their own context at init rather than into a
  `EHS_DATA_TABLE_STRING_DEFAULT_LENGTH` stack buffer every scan — that buffer
  was 32 KB on `base_full`.
- **A run function logs every scan.** Latch format errors (see
  `EhsFormat_report` in `stringfn.c`) or one mistyped format floods the log.
- `stringfn_scanf8` still has the counting defect and additionally lets `sscanf`
  write through an output-row pointer with no field width. It needs the same
  treatment plus a width injected from `EHS_FB_OUT_S_CAP`.

