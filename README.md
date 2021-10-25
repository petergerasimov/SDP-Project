# SDP-Project
A simple interpreter for a language that resembles BASIC.

Supported keywords: 
```basic
LАBEL, GOTO, LET, READ, PRINT, IF, ENDIF, ELSE, WHILE, DONE
```

Supported operators: 
```basic
=, ==, !=, <, <=, >, >=, +, *, /, -, %, &&, ||, !,()
```

Works with integers and arrays of integers.

# Data manipulation

```basic
LET
``` 
declares a variable. A variable has to be declared before use.

# Input and output

```basic
READ
``` 
Reads standard input.
```basic
PRINT
``` 
Prints to standard output.

# Program flow control

```basic
IF ... {ELSE ...} ENDIF
```
Used to perform comparisons or make decisions.

```basic
WHILE ... DONE
```
Repeat a section of code while the specified condition is true.

```baic
GOTO
```
Jumps to label set with ```LABEL```
