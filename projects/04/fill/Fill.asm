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
//Entry point
@ISETTER
0;JMP

//Set i to 0
(ISETTER)
@i
M=0

//From there always jump to keyboard listener
@KBDLISTENER
0;JMP

//listen to KBD output
(KBDLISTENER)
@KBD
D=M

//Jump to Loop that activates all screen bits if KBD != 0
@LOOPACTIVATEBIT
D;JNE

//Jump to loop that deactivates all screen bits if KBD == 0
@LOOPDEACTIVATEBIT
D;JEQ

//else unconditional jump to KBDLISTENER
@KBDLISTENER
0;JMP

//If KBD is non-zero (i.e. a keystroke is pressed)
(LOOPACTIVATEBIT)
  //Loop through all screen pixel and color them
  //We will apply following logic:
  // Array[SCREEN + i] = 1

  //Setting up termination conditions
  //Check if keystroke memory address - i - screen memory address = 0, means end of screen map
  @i
  D = M

  @SCREEN
  D = D + A

  @KBD
  D=A-D
  
  //Don't set i to 0 now or we'll overflow
  //if the user keep the touch pressed
  //so we just allows ourselves to get back into the loop
  //and exit again, and again
  @KBDLISTENER
  D;JEQ

  @SCREEN
  D=A
  @i
  A = D + M
  M = -1

  //increment i 
  @i
  M = M + 1

  //if not, jump back to loop beginning
  @LOOPACTIVATEBIT
  0;JMP

(LOOPDEACTIVATEBIT)
  //Loop through all screen pixel and color them
  //We will apply following logic:
  // Array[SCREEN + i] = 0

  //Setting up termination conditions
  //Check if keystroke memory address - i = 0, means end of screen map
  @i
  D=M

  @SCREEN
  D = D + A
  
  @KBD
  D=A-D
  @ISETTER
  D;JEQ


  @SCREEN
  D=A
  @i
  A = D + M
  M = 0

  //increment i 
  @i
  M = M + 1

  //if not, jump back to loop beginning
  @LOOPDEACTIVATEBIT
  0;JMP

