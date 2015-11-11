# ???

This is the ??? programming language.
Have you ever been writing a play, essay, or other work of literature and thought to yourself, "Man, wouldn't it be great if I could embed a program in this using punctuation?"
Well now you can.

### Overview

??? is a trivial [Brainfuck](https://en.wikipedia.org/wiki/Brainfuck) substitution, meaning that a trivial replacement of operators will result in an equivalent BF program. Unsurprisingly, this means that ??? works the same way as BF: it operates on an array of memory cells (also called the tape), each initially set to 0, and has a pointer initially pointing to the first memory cell. It also means that it's a Turing complete language.

### Commands

| Command | Description                                                 |
|:-------:|:----------------------------------------------------------- |
| `;`     | Move the pointer to the right                               |
| `-`     | Move the pointer to the left                                |
| `.`     | Increment the memory cell under the pointer                 |
| `,`     | Decrement the memory cell under the pointer                 |
| `!`     | Output the character signified by the cell at the pointer   |
| `?`     | Input a character and store it in the cell at the pointer   |
| `"`     | Jump to the matching `"` if the cell under the pointer is 0 |
| `'`     | Toggles whether subsequent `"` commands start or end a loop |

Initially, `"` begins a loop.

### Notes

The creators of the language would like to extend a special thank you to Stack Exchange users **aditsu**, **Doorknob**, **Reto Koradi**, and **Martin Büttner** for inspiration and suggesting improvements/fixes.
