# 2048-Bit Big Integer Calculator

A command-line arbitrary precision integer calculator implemented in C to perform arithmetic operations on integers up to 2048 bits, exceeding the limits of native C data types.

# Features

- Addition
- Subtraction
- Multiplication
- Division
- Modulus
- Exponentiation
- Integer comparison
- Support for negative numbers

# Technologies Used
- C Language
- GCC
- Dynamic Memory Allocation
- Custom BigInt Data Structure
- Valgrind
  
# Implementation Details
- Implemented a custom `BigInt` data structure using arrays to store digits.
- Supports arithmetic operations on integers up to 2048 bits (approximately 617 decimal digits).
- Uses custom algorithms for large integer arithmetic without external libraries.

# Memory Safety
The project was tested using Valgrind and achieves:
- Zero memory leaks
- Zero memory errors

# Project Highlights
- Overcomes native integer size limitations in C.
- Implements arbitrary precision arithmetic from scratch.
- Provides efficient handling of large integer computations.

# Author
Vaibhavi Jadhav
