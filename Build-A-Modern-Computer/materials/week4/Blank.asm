// Rectangle.asm
// Draw a filled rectangle at the screen's top left corner.
// The rectangle's width is 16 pixels, and its height is RAM[0].
// Usage: put a non-negative number (rectangle's height) in RAM[0]

// for (i=0; i < n; i++) P
//    draw 16 black pixels at the beginning of row i
// }

//  addr = SCREEN
//  n = RAM[0]
//  i = 0
//
//  LOOP:
//    if i > n goto END
//      RAM[addr] = -1  // 111111111111111
//    // advance to next row
//    addr = addr + 32  // 32 words ina row (32 * 16 = 512)
//    i = i + 1
//    goto LOOP
//  END:
//    goto END

(STANDBY)
  @KBD
  D = M
  @FILL
  D;JNE
  @CLEAR
  0;JMP
  @STANDBY
  0;JMP

(FILL)
  @i
  M=0

(LOOP_1)
  @i
  D = M
  @8192
  D = D - A
  @STANDBY
  D;JEQ
  @i
  D = M
  @SCREEN
  A = D + A
  M = -1      // RAM[16384]=1111 1111 1111 1111
  @i 
  M = M + 1
  @LOOP_1
  0;JMP

(CLEAR)
  @i
  M = 0       // RAM[16] = 0

(LOOP_2)
  @i
  D = M
  @8192
  D = D - A   // D = RAM[16]-8192
  @STANDBY
  D;JEQ       // If D=0, jump to A=STANDBY+1
  @i
  D = M
  @SCREEN    // A=16384, M=RAM[16384]
  A = D + A  // A=RAM[16]+16384
  M = 0      // Set RAM[16384] = 0000 0000 0000 0000
  @i
  M = M + 1  // RAM[16]=RAM[16]+1
  @LOOP_2
  0;JMP
