// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
// Assembler.sh Mult.asm - create hack file to run test file

@R0
D=M
@n      // 16
M=D     // n = R0 = 6
@i      // 17
M=1     // i = 1
@mul    // 18
M=0     // mul = 0

(LOOP)
    @i          // 17
    D=M         // 1
    @n          // 16
    D=D-M       // i-n 
    @STOP
    D;JGT       // if i > n GOTO STOP
 
    @R1
    D=M         // RAM[1] 
    @mul        // 18
    M=D+M       // mul = mul+RAM[1]
    @i          // 17
    M=M+1       // i = i + 1
    @LOOP
    0;JMP

(STOP)
    @mul
    D=M
    @R2
    M=D         // RAM[2] = mul

(END)
    @END
    0;JMP