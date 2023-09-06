// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here


//Assign iterator i to a register
@i
D=1
M=D
//Assign sum, i.e. mult result to another register
@R2
D=0
M=D
//Step into the loop
(LOOP)
//if iterator <= R1, goto STOP
    //load i
    @i
    D=M
    //load R1
    @R1
    //Verify that R1 - i >=  0, i.e that R1 >= iterator
    D=M-D
    @STOP
    D;JLT
    //If we step in here, it means the above was satisfied, i > R1
    //Load R0
    @R0
    D = M
    //increment sum by R0
    @R2
    M = M + D
    //increment iterator i
    @i
    D = M + 1
    M = D
    //Jump Back to (LOOP)
    @LOOP
    0;JMP
//Else, add R0 to sum
(STOP)
0;JMP

