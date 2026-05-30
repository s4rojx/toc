# NFA to DFA Conversion in C

This project implements the subset construction algorithm to convert an NFA into an equivalent DFA. It supports multiple transitions on the same symbol.

## Files

- `main.c` - main program
- `nfa.h`, `nfa.c` - NFA data structure, input, and move function
- `dfa.h`, `dfa.c` - subset construction and DFA output
- `Makefile`, `build.bat`, `run_sample.bat` - convenience build and run commands

## Limits

- NFA states must be numbered from `0` to `n-1`
- Maximum NFA states: `20`
- Maximum alphabet symbols: `26`

The program uses bitmasks for state sets, so subset operations are fast and simple.

## Manual Input Format

```text
number_of_states
alphabet_size
alphabet_symbols_separated_by_spaces
start_state
number_of_accept_states
accept_states_separated_by_spaces
number_of_transitions
from symbol to
from symbol to
...
```

Each transition is written as `from symbol to`, for example `0 1 2`.

## Example: Strings Ending With `01`

Use this input example:

```text
3
2
0 1
0
1
2
4
0 0 0
0 1 0
0 0 1
1 1 2
```

Meaning:

- States: `0`, `1`, `2`
- Alphabet: `0`, `1`
- Start state: `0`
- Accept state: `2`
- Transitions:
  - `0 --0--> 0`
  - `0 --1--> 0`
  - `0 --0--> 1`
  - `1 --1--> 2`

This NFA accepts strings over `{0,1}` that end with `01`.

## Compile and Run

### Five Steps on Windows

1. Install a C compiler.
   Recommended: install MinGW-w64 or MSYS2. After installing, make sure `gcc` is added to the Windows `PATH`.

2. Close and reopen VS Code.
   This refreshes the terminal so it can detect the new `gcc` command.

3. Open a terminal in this project folder:

   ```powershell
   cd C:\Users\saroj\Desktop\TOC
   ```

4. Build and start the program:

   ```powershell
   .\build.bat
   ```

5. Enter the NFA details when the program asks.

   ```text
   3
   2
   0 1
   0
   1
   2
   4
   0 0 0
   0 1 0
   0 0 1
   1 1 2
   ```

Using GCC:

```bash
gcc -Wall -Wextra -std=c11 main.c nfa.c dfa.c -o nfa_to_dfa
./nfa_to_dfa
```

On Windows PowerShell with MinGW GCC:

```powershell
gcc -Wall -Wextra -std=c11 main.c nfa.c dfa.c -o nfa_to_dfa.exe
.\nfa_to_dfa.exe
```

Or run `build.bat`, which builds and starts the program:

```powershell
.\build.bat
```

Then enter the NFA details manually.

You can also run the sample automatically:

```powershell
.\run_sample.bat
```

Expected DFA state mapping for the sample:

```text
D0 = {0}
D1 = {0,1}
D2 = {0,2}  (accept)
```

Expected transition table:

```text
State       0           1
D0          D1          D0
D1          D1          D2
D2          D1          D0
```
