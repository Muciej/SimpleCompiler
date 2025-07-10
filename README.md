# SimpleCompiler
Project created for subject Formal Languages and Translatation Technologies. It is a compiler of simple imperative language that was proposed by lecturer. Compiler generates code for virtual machine with given assembler instructions set.

## Build and usage

Project utilizes make and in order build it following commands should be executed inside the `src` directory:
```bash
make compiler
```

Then, you can run compiler with:
```bash
./compiler input_file output_file debug_file
```

To clean files created during compilation you can use:
```bash
make clean
```

Debug file is optional and if present, will be filled with debug data from the compiler.
The compiler supports compilation of a simple language into the machine code for a provided virtual machine.

## Language definition

Below is presented the grammar of the language supported by the compiler:

```
 program_all -> procedures main

 procedures -> procedures PROCEDURE proc_head IS VAR declarations BEGIN commands END
 | procedures PROCEDURE proc_head IS BEGIN commands END
 |

 main -> PROGRAM IS VAR declarations BEGIN commands END
 | PROGRAM IS BEGIN commands END

 commands -> commands command
 | command

 command -> identifier := expression;
 | IF condition THEN commands ELSE commands ENDIF
 | IF condition THEN commands ENDIF
 | WHILE condition DO commands ENDWHILE
 | REPEAT commands UNTIL condition;
 | proc_head;
 | READ identifier;
 | WRITE value;

 proc_head -> identifier ( declarations )

 declarations -> declarations , identifier
 | identifier

 expression -> value
 | value + value
 | value - value
 | value * value
 | value / value
 | value % value

 condition -> value = value
 | value != value
 | value > value
 | value < value
 | value >= value
 | value <= value

 value -> num
 | identifier
```

## Virtual machine

Compiler outputs files with commands compilant with the assemby language for a fictional virtual machine. Machine consists of order counter `k` and an sequence of memory cells `p_i`, where i = 0,1,2,... . Cell `p_0` is an accumulator and the virtual machine operates on a natural numbers (with 0). Machine starts with an order `0` and proceeds to execute next orders until the `HALT` order is encountered. Below is the list of possible orders with their interpretation and cost of execution:

```
Order       Interpretation                                                  Cost

GET i       saves given number into the pi cell and k <- k + 1              100
PUT i       shows the contains of pi cell and k <- k + 1                    100
LOAD i      p0 <- pi and k <- k + 1                                         10
STORE i     pi <- p0 and k <- k + 1                                         10
LOADI i     p0 <- ppi and k <- k + 1                                        10
STOREI i    ppi <- p0 and k <- k + 1                                        10
ADD i       p0 <- p0 + pi and k <- k + 1                                    10
SUB i       p0 <- max{p0 −pi,0} and k <- k + 1                              10
ADDI i      p0 <- p0 + ppi and k <- k + 1                                   10
SUBI i      p0 <- max{p0 −ppi ,0} and k <- k + 1                            10
SET         x p0 <- x and k <- k + 1                                        10
HALF        p0 <- ⌊p0/2⌋ and k <- k + 1                                      5
JUMP        j k <- j                                                        1
JPOS        j if p0 > 0 to k <- j else k <- k + 1                           1
JZERO       j if p0 = 0 to k <- j else k <- k + 1                           1
JUMPI       i k <- pi                                                       1
HALT        stop the execution                                              0
```

## Example program with translation
```
1  [ Display number's binary form ]
2  PROGRAM IS
3  VAR n, p
4  BEGIN
5    READ n;
6    REPEAT
7        p:=n/2;
8        p:=2*p;
9        IF n>p THEN
10          WRITE 1;
11      ELSE
12          WRITE 0;
13      ENDIF
14      n:=n/2;
15   UNTIL n=0;
16 END
```

```
0 GET 1
1 LOAD 1
2 HALF
3 STORE 2
4 LOAD 2
5 ADD 0
6 STORE 2
7 LOAD 1
8 SUB 2
9 JZERO 13
10 SET 1
11 PUT 0
12 JUMP 15
13 SET 0
14 PUT 0
15 LOAD 1
16 HALF
17 STORE 1
18 LOAD 1
19 JPOS 1
20 HALT
```

