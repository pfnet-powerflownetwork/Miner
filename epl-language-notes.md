PFNETPL Overview and Programming Guide

Introduction

The PFNETPL programming language empowers PFNET job authors to craft complex algorithms that miners can solve for bounties.

PFNETPL derives inspiration from the C programming language, inheriting its basic operators and functions, while omitting FOR, WHILE, or DO loops to ensure bounded execution. Instead, it uses the PFNETPL-specific REPEAT construct.

Authors are responsible for implementing concise verification logic to validate solutions within a predefined WCET (Worst-Case Execution Time), ensuring compatibility across nodes in a distributed network.

Program Structure

A typical PFNETPL program includes:

Global Variable Arrays (Mandatory)

Optional Storage Declarations

A main Function (Mandatory)

A verify Function (Mandatory)

Optional User-Defined Functions

Global Variables

All storage in PFNETPL is achieved via global arrays representing standard 32-bit and 64-bit data types.
Named variables are not allowed; instead, data is stored in type-specific arrays, declared as follows:

array_int    SIZE
array_uint   SIZE
array_long   SIZE
array_ulong  SIZE
array_float  SIZE
array_double SIZE

Key Points:

Only one instance of each data type array is permitted.

The combined size of all arrays must not exceed the maximum memory limit (TBD).

Arrays must be declared at the program's start, preceding all function definitions.

Access array elements using type-specific prefixes (i, u, l, ul, f, d) and zero-based indexing. For example:

i[0] = 100;
u[5] = u[3] * 10;
d[6] = u[5] / i[5];

Casting occurs automatically based on this precedence:

int32_t -> uint32_t -> int64_t -> uint64_t -> float -> double

Boundary checks are applied when variables with calculated indices are used, e.g.,:

u[0] = u[u[3]];

is evaluated as:

u[0] = ((u[3] < MAX_ALLOWED) ? u[u[3]] : 0);

VM-Initialized Variables

Each iteration initializes 12 unsigned int variables (m[0] through m[11]):

m[0] - m[9]: Random 32-bit unsigned integers

m[10]: Run number

m[11]: Iteration number

These allow for randomized algorithm inputs and control logic for specific intervals.

Submitting Data for Validation

Complex algorithms often require submitting partial solutions for verification. This involves using unsigned integers from the u[] array.

Declarations for Submission:

submit_sz   SIZE  // Number of values to submit
submit_idx  START // Start index in u[] for submission

For example, in a TSP algorithm, the miner might send the computed path to validate against bounty requirements.

Iteration Data Storage

PFNETPL supports storing data between iterations to allow for iterative improvements in solutions. This is optional and uses the s[] array:

For iteration 0, s[] is initialized to zero.

For subsequent iterations, s[] retains data from prior solutions.

Functions

All PFNETPL statements must reside within functions. Key rules include:

The main function cannot be called by other functions.

The main function can optionally call the verify function.

Recursive calls are disallowed.

Function names follow the format: [a-zA-Z_][a-zA-Z0-9_]*.

Statements within functions are enclosed in {}.

Example:

function example {
    statement1;
    statement2;
}
example();

"main" Function:

Mandatory and responsible for executing the algorithm and determining bounty/POW validity. Validation can occur via:

Calling the verify function.

Direct use of verify_bty and verify_pow statements.

"verify" Function:

Mandatory and includes logic to validate submitted solutions. This function must ensure:

verify_bty checks bounty conditions.

verify_pow validates POW via MD5 hashing.

Example Verification Statements:

verify_bty (u[1000] == 0);
verify_pow (u[25], u[1001], u[823], u[123]);

Looping with REPEAT

PFNETPL replaces traditional loops with the REPEAT construct to enforce termination. Format:

repeat( <counter>, <iterations>, <max_iterations> ) {
    statement1;
    statement2;
}

Nested REPEAT loops are limited to 32 levels.

Supported Operators

PFNETPL supports most C operators. Examples include:

Arithmetic: +, -, *, /, %

Logical: &&, ||, !

Bitwise: &, |, ^, ~

Comparison: ==, !=, <, >

Built-In Functions

PFNETPL includes common math functions and a custom GCD function. Examples:

pow(x, y): Exponentiation

sqrt(d): Square root

gcd(x, y): Greatest common divisor

This overview maintains the original functionality while enhancing readability and clarity. Let me know if further modifications are needed!
