// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

(STANDBY)
// assuming address of ScreenPtr is 16 
// assuming address of NumRegister is 17

   @8192       // A=8192
   D=A         // D=8192
   @NumRegister
   M=D         // M = 256*512/16 = 8192
   
   @SCREEN     // find the start point of screen - A=16834
   D=A         // D=16834
   @ScreenPtr  // A=16, M=RAM[16]
   M=D         // set pointer to the first register
               // M=RAM[16]=16834

   @KBD        // A=KBD, M=RAM[KBD]
   D=M         // 1->press keyboard
               // D=RAM[KBD]

   @CLEAR
   D; JEQ      // if D=RAM[KBD]=0 then jump to CLEAR

   @FILL
   D; JNE

   @STANDBY
   0; JMP

(CLEAR)
   @ScreenPtr   // A=16
   A=M          // A=16834, so that M = RAM[16834]
   M=0          // set RAM[16834] = 0 - white

   @ScreenPtr   // move to next register: A = 16, M = RAM[16] = 16834
   M=M+1        // M = RAM[16] + 1 = 16835

   @NumRegister // NumRegister's address is unknown, 
                // RAM[NumRegister] = 8192
   M=M-1
   D=M

   @STANDBY     // if NumRegister = 0 then go back
   D;JEQ

   @CLEAR       // else continue clear the screen
   0;JMP

(FILL)
   @ScreenPtr
   A=M
   M=-1          // black

   @ScreenPtr
   M=M+1

   @NumRegister
   M=M-1
   D=M

   @STANDBY
   D;JEQ

   @FILL
   0;JMP




