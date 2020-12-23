// compute RAM[1] = 1+2+3+4+...+n
// usage: put a number in RAM[0]

@R0
D=M
@n
M=D // n=R0
@i
M=1 // i=1
@sum
M=0 // sum=0

(LOOP)
@i
D=M
@n
D=D-M  // i - n
@STOP
D; JGT // if i > n goto STOP

@sum
D=M
@i
D=D+M
@sum
M=D  // sum=sum+1
@i
M=M+1 // i=i+1
@LOOP
0;JMP

(STOP)
@sum
D=M
@R1
M=D // RAM[1]=sum

(END)
@END
0;JMP



