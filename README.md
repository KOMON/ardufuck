# Ardufuck

An interactive Brainfuck interpreter for Arduboy

## TODO

* Enable single character input for the `,` instruction
* Spruce up the UI a bit
* Reset more of the VM between runs (i.e. clear input/output)
* An in-game debugging mode would be cool
* Allow the debugger to import/execute arbitrary BF programs or lua libs

## Controls

The sketch starts in the "Edit - Move" mode. You can tell which mode you're in by the color of the LED:

| LED       | MODE               |
------------|---------------------
| WHITE     | Edit - Move        |
| RED       | Edit - Editing     |
| BLUE      | Run - Paused       |
| BLUEGREEN | Run - Running      | 
| GREEN     | Run - Fast Forward |
| PURPLE    | Run - Complete     |

Here's a description of the controls in each mode:

### Edit - Move (White LED)
This mode is for navigating and reading the program. You can navigate the program as if it was a grid, moving up and down wrapped lines.

- D-Pad L/R/U/D -- Move the cursor around the program
- A -- Enter mode "Edit - Editing"
- B + Down -- Enter mode "Run - Paused"

### Edit - Editing (Red LED)
This mode is for editing the program. You can move by single characters back and forth, and cycle through the set of Brainfuck instructions under the cursor

- D-Pad L/R -- Move the cursor back and forth
- D-Pad U/D -- Cycle through Brainfuck instructions at current character
- A -- Exit mode "Edit - Editing"
- B + Down -- Enter mode "Run - Paused"

### Run - Paused (Blue LED)
This is a standby mode before running the program

- A -- Run program
- B + Up -- Enter mode "Edit - Move"

### Run - Running (Bluegreen LED)
In this mode the Brainfuck VM executes one instruction per frame.

- A -- Pause program
- B + Up -- Enter mode "Edit - Move"
- Hold D-Pad R -- Enter mode "Run - Fast Forward"

### Run - Fast Forward (Green LED)
In this mode the brainfuck VM executes 8 instructions per frame.

- Release D-Pad R -- Exit mode "Run - Fast Forward"

## Run -- Complete (Purple LED)
The program has completed running

- A -- Return to mode "Run - Paused", rewinding the VM

## Hacking

1. You'll need to install
  - The Arduino IDE 
  - CMake
  - A C++11 compatible compiler 
1. In the Arduino IDE:
  - Install the library `Arduboy2` @ 6.0.0
  - Open the git directory as a sketch
  - Set up the target board as Arduino Leonardo

### Building the debugger

This project includes a nice little debugger written in lua.

To build the debugger:

1. `cd debugger`
1. `cmake -S . -B build`
1. `cd build`
1. `make`

The debugger executes by default a file in the same directory called `debugger.lua`. Just run `./debugger`

To change the program that is initially loaded you can adjust the `program` local.

### Debugger instructions

The debugger has a simple text-based interface

```
Code:
++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.
^                                                                                                         
Data:
000 000 000 000 000 000 000 000 000 000 000 000 000 000 000 000 000 000 000 000 
 ^                                                                              
Done: false
Input:  Output: 
n(ext) | b(reak) X | d(elete) X| i(nput) | q(uit) | f(orward) | (b)a(ck) | e(xec)
> 
```

You can see that we display the current program, and a cursor to mark where execution currently is, as well as a view into the surrounding data cells, and some state of the VM.

the instructions you can give at the prompt:

- `n`/`next` -- Execute the next instruction
- `b X`/`break X` -- set a breakpoint for the given instruction, e.g. `b .` will break on each instance of `.`
- `d X`/`delete X` -- delete the breakopint set for the given instruction
- `i`/`input` -- Prompt for input, will only take the first character
- `q`/`quit` -- Quit the debugger
- `f`/`forward` -- Move the cursor forward an instruction without executing it
- `a`/`back` -- Move the cursor backward an instruction without executing it
- `e`/`exec` -- Execute the program until it finishes or we hit a breakpoint
