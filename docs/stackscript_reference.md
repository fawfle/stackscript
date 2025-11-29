# **StackScript Reference**

## **Overview**
StackScript is a [stack oriented](https://en.wikipedia.org/wiki/Stack-oriented_programming) programming language primarily inspired by the FORTH programming language. All working data is stored on a single stack of integers. The language has operators that act on the data on top of the stack (such as basic arithmetic operations), if and else statements for control flow, and functions in the form of words.

## **Syntax**
All operators and statements are in postfix notation. Instructions are separated by spaces and whitespace is ignored. Read rest for more details.

```
1 1 + # results in 2 on the stack
10 3 - # results in 7 on the stack
25 2 > # results in 1 on the stack since 25 > 2

5 REPEAT: 1; # adds 1 to the stack 5 times

in 5 = IF: "equals 5" println; ELSE: "not 5" println; # accepts user input and prints if it's 5 or not
```

## **Literals**
StackScript supports integer literals and strings to a limited extent. Any number literal `n` will be put on the stack when evaluated. Chars `"c"` are converted to their integer representations when evaluated. Strings `"abc"` are converted to a list of chars when evaluated. See [Chars and Strings](#chars-and-strings) for more details.


## **Operators**
### Arithmetic
- \[+\] (b a -- b + a) sums the two numbers on top of the stack and pushes the result onto the stack
- \[-\] (b a -- a - b) subtracts the first number on the stack from the second and pushes the result onto the stack
- \[*\] (b a -- b * a) multiplies the two numbers on top of the stack and pushes the result onto the stack
- \[/\] (b a -- b / a) divides the second number on top of the stack by the first and pushes the result onto the stack

### Conditional
- \[>\] (b a -- b > a) compares if the second number is greater than the first and pushes `1` onto the stack if it is and `0` otherwise.
- \[<\] (b a -- b < a) same as above but compares if the top number is greater than the second.
- \[=\] (b a -- b = a) compares equality of the top two numbers on the stack.
- \[!\] (a -- !a) inverts the truthiness of the top value. Truthy values become `0`, `0` becomes `1`.
- \[!=\] (b a -- b != a) same as equality but inverts the result.
- \[>=, <=\] same as < or > but checks if equal as well.

## **Basic Keywords**
- \[drop\] (a -- ) pops the number on the top of the stack
- \[dup\] (a -- a a) duplicates the number on top of the stack
- \[swap\] (b a  -- a b) swaps the two numbers on top of the stack
- \[nswap\] swaps
- \[input | in\] accepts a number input from the user.
- \[cinput | cin\] accepts a char input from the user. The char is converted to an integer.
- \[print | p\] (a -- ) prints the top value of the stack and removes it
- \[cprint | cp\] (a -- ) prints the top value of the stack as if it was a char and removes it
- \[peek / cpeek\] (a -- a) prints the top value of the stack without removing it
- \[ln\] prints a new line
- \[println | cln / cprintln | cpln\] prints with new line

## **Blocks**
Blocks are sequences of statements enclosed beginning with a `:` and ending with a `;`. They are treated as single statements by the interpreter.

```
# an example block
: 1 1 + ;
```

The primary use of blocks is to group statements for loops and words.

## **Conditionals**
StackScript supports IF and IF ELSE statements. They take the form of `IF statement` and `IF statement ELSE statement` respectively. IF statements check if the value on top of the stack is truthy (not zero) and if it is, they execute the statement. If the optional else statement is specified, the else statement will be executed on a falsy value. IF statements can be nested anywhere.

```
# takes the user input and checks if it's greater than 5
in
5 > IF: "greater than 5" println; ELSE: "less than 5" println;
```

## **Loops**
StackScript supports `REPEAT` loops and `DO ... WHILE ...` loops.

### Repeat
Repeat loops have the form `n REPEAT statement` where the statement is repeated n times. Before the statement is repeated, the current iteration (starting at 0) is pushed onto the stack.

```
# prints the first 5 squares
5 REPEAT: dup * println;
```

### Do While Loops
Do while loops have the form `DO statement WHILE condition` where the statement is executed, followed by the condition statement. The loop finally checks if the top element is truthy (consuming it in the process) and repeats the whole process if it is. Because of the way StackScript works, the `WHILE` part is essentailly syntactic sugar to make the intention of the condition more readable. Therefore the `WHILE` clause can be omitted and written as `DO statement` where the statement includes both the `statement` and `condition`.

```
# continually read input until the user enters "e"
REPEAT: cin; WHILE: "e" !=;
```

## **Words**
Words are the functions of stackscript. They are defined using the `DEF` keyword followed by an identifier in the form `DEF [IDENTIFIER] statement`.

Technically, words are always aliases to statements. In practice, words are almost always followed by block statements. Words can be called from anywhere as long as they have been defined previously. Words can also be used recursively.
```
# A word definition. the parenthesis are a comment showing what the word expects on the stack and what it returns similar to FORTH style comments.
DEF SQUARE (n -- n'): dup *;
```

## **Details**
### Truthy and Falsy Values
Any value other than 0 is truthy. 0 is false. Conditionals always return 1 if true.

### Statement Side Effects
All statements that perform operations or checks on the stack remove the numbers. This is usually helpful like in the case of addition (`1 1 +` -> `2`) or conditional statements (`1 IF ...` consumes 1 before continuing) but can result in removing unwanted values. To avoid this, use `dup` and `swap` to create a copy on top of the stack (to compare a number `n` while keeping it, `n dup = 0`).

### Chars and Strings
Since the stack only supports integer values, strings can't easily be added. Chars are just integers interpreted differently and only matter when accepting input or printing. `cprint` will convert the integer on the stack to a char before printing and `cinput` will accept input as if it was a char. (`cinput` followed by 1 will result in `31` while `input` will result in `1`). Char/string literals are converted ints when evaluated. Strings push each char onto the stack in order (`hello world` -> `h e l l o w o r l d`). However, a string followed by ANY print statement will print the entire string (`Hello world println` will print `Hello world`).

### Comments
\# will comment out everything following it on the line.

Parenthesis will comment out everything between them `... ( comment ) ...`.
