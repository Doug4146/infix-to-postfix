# Infix to Postfix Converter and Evaluator

This simple toy program converts infix expressions to postfix notation and evaluates them. It supports basic arithmetic operations: addition, subtraction, multiplication, and division.

## Features
- Converts infix expressions (e.g., `3 + (4 * 2)`) to postfix notation (`342*+`).
- Evaluates the postfix expression and returns the result with floating-point precision.
- Supports multiple expressions as command-line arguments.

## Requirements
- GCC Compiler
  - Tested with MinGW GCC 14.2.0:
    
## Usage
This program **requires the use of command-line arguments** to input infix expressions. You must provide the expressions as arguments when running the program.

1. Compile the program:
   ```bash
   gcc -o infix_converter infix_converter.c
2. Run the program with infix expressions as arguments:
   ```bash
   ./infix_converter "3+(4*2)" "7-3/2"
