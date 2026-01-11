# Lexical-Analyser
A Lexical Analyzer implemented in C that scans a C source file and identifies tokens such as keywords, identifiers, operators, literals, and symbols while ignoring comments and whitespace.
This project implements the lexical analysis phase of a compiler.
It reads a C program line by line and character by character, groups characters into meaningful tokens, and classifies them according to predefined lexical rules.

The analyzer filters out single-line and multi-line comments, ignores whitespace, and outputs valid tokens with their respective types.

✨ **Features**

Reads and analyzes C source files

Identifies and classifies:

Keywords

Identifiers

Operators

Numeric constants

String literals

Special symbols

Ignores whitespace

Handles both single-line (//) and multi-line (/* */) comments

Modular implementation using header and source files

Pure C implementation (no external libraries)

🛠️ **Tech Stack**

Language: C

Concepts Used:

File handling

Character-by-character parsing

String processing

Compiler design fundamentals

Modular programming

📂 **Project Structure**
Lexical-Krishna/
├── main.c        # Entry point and file handling
├── lexical.c    # Core lexical analysis logic
├── lexer.c      # Token identification functions
├── lexer.h      # Token definitions & declarations
└── a.out        # Compiled binary

▶️ Build & Run
Compile
gcc *.c -o lexer

Run
./lexer lexical.c
