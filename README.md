# Arabic-compiler

## Overview

Arabic-compiler is a compiler project designed to process a programming language with Arabic-like or right-to-left (RTL) syntax. The project leverages Flex (for lexical analysis) and Bison (for parsing) to translate source code into an intermediate representation, supporting features such as symbol tables, quadruplets (for intermediate code), and control flow constructs.

## Features
- Lexical analysis using Flex (`analyseur.l`)
- Syntax analysis using Bison (`parser.y`)
- Symbol table management (`symbolTable.c`, `symbolTable.h`)
- Intermediate code generation with quadruplets (`quadruplet.c`, `quadruplet.h`)
- Stack management for expressions and control flow (`pile.c`, `pile.h`)
- Support for conditional statements, loops, functions, and logical expressions
- Test suite with various example programs in the `tests/` directory

## File Structure
- `analyseur.l`: Flex file for lexical analysis
- `parser.y`: Bison file for grammar and parsing rules
- `symbolTable.c/h`: Implementation of the symbol table
- `quadruplet.c/h`: Intermediate code (quadruplet) generation and management
- `pile.c/h`: Stack utilities for parsing and code generation
- `output.txt`: Output file for generated code or results
- `tests/`: Directory containing sample source files and test cases

## Building the Project
1. Make sure you have Flex and Bison installed on your system.
2. Generate the lexer and parser:
   ```sh
   flex analyseur.l
   bison -d parser.y
   gcc -o Arabic-compiler lex.yy.c parser.tab.c symbolTable.c quadruplet.c pile.c -lfl
   ```
   This will produce an executable named `Arabic-compiler` (or `Arabic-compiler.exe` on Windows).

## Running the Compiler
To compile a source file, run:
```sh
./Arabic-compiler < input_file.rtl
```
Replace `input_file.rtl` with the path to your source code. Output and intermediate results will be written to `output.txt`.

## Tests
Sample test files are available in the `tests/` directory. You can try them with:
```sh
./Arabic-compiler < tests/test.txt
```
Check `output.txt` for the results.