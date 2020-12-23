//set RAM[100] - RAM[110]: -1


(LOOP)
// if (i==n) goto END

@i
D=M
@n
D=D-M // i-n
@END
D; JEQ // i-n == 0

// RAM[arr+i] = -1
@arr
D=M
@i
A=D+M // A = arr+i -> pointer
M=-1  // M always represents RAM[A]

// i++
@i
M=M+1

@LOOP
0;JMP

(END)
@END
0;JMP