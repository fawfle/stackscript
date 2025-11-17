# Project Report

## Problem Statement
Problem: How can we create a beginner-friendly programming language in a creative way?

We will create a programming language interpreter that follows the stack oriented paradigm. Stack oriented programming languages work by manipulating data on a stack. The simplicity of hte operations makes them a good choice for developing programming languages.

## Related Data Structure Alternatives

### Stacks (Chosen Data Structure)
The programming language is a stack-based language because the syntax becomes simple. For example, to evaluate 1+2, the syntax for this language would become:

> 1 2 + PRINT

To evaluate this, the stack would first add 1 and 2 to the stack. The stack now holds the values [1,2], where the last element is the top of the stack. When the + arrives, the 2 and 1 will be popped from the stack and added. Then, the resulting value (3) will be added to the stack. When the PRINT comes, the 3 will be popped from the stack and then printed.

### Dynamic Arrays
The stack is being implemented through dynamic arrays. One benefit of using dynamic arrays to implement stacks is that it is efficient to add elements at the end of the array and pop them off from the end of the array.

Although dynamic arrays are used to implement the stack, they were not used in their raw form. This is because it is much easier and understandable to use peek(), pop(), and push() methods than consistently accessing the last element in the array. Moreover, having the stack automatically resize itself when using the push() method makes programming much easier for the programmer. The abstraction of a stack class makes the program readable and avoids the need of implementing the functions every time.

## Operation Algorithms and Complexity

## Testing Strategy

## When Does the Chosen Data Structure Fail?

## Limitations and future work