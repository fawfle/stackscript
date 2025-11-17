# Stack Oriented Language Interpreter

## Problem Statement
Create a programming language interpreter that follows the stack oriented paradigm. Stack oriented programming languages work by manipulating data on a stack. The simplicity of the operations makes them a good choice for developing programming languages.

## Operations
Our program's operations are split into two main categories, the interpreter and the language itself. The interpreter will need to be able to tokenize an input into data it’s able to parse. The language will support basic operations as well as functions or “words” that are equivalent to combinations of other operations.

The primary operations our program needs are associated with the language we will develop for. The most common operations are the basic arithmetic operations like addition and multiplication which act on the top two elements of the stack. We also plan to implement control flow operations like if and then.

## Data Structures
Our program’s chosen data structure is the stack. Stack oriented programming relies on the LIFO property of the stack to efficiently perform calculations. They have the advantage of instant lookup for elements at the top and are specifically ordered by the order of additions. Unordered structures like trees and hash tables don’t preserve the correct order and traditional linked lists (not a “linked list” used to implement a stack) have unnecessary operations and complexity.

We will also likely use hash tables to store the location of “words” since they will be associated with a key. Hash tables have constant time lookup for key value pairs and “words” making them quicker than other structures. “Words” also have no inherent order so traversal advantages like those in BSTs don’t apply.

## Example

An example program might look like

```
1 2 + 5 *
```

which would compute the result of (1 + 2) * 5. More complex functions might take the form of

```
1 5 - 2 = IF THEN 1 p; ELSE 0 p;
```

which would compute the equivalent of 

```
if (1 - 5 == 2):
    print(1)
else:
    print(0)
```

Words could be reusable functions that are just a list of other instructions. Parenthesis contain comments on the input/output of the function.

```
# takes in a single number on the stack and squares it
SQUARE (n): dup *

# raises the number on the top of the stack to the next number on the stack
POW (n p -- n): 1 - 0 > IF swap dup * swap POW; ELSE drop;
```
