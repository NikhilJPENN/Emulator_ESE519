
The code compiles using the following command:
gcc emul8.c gamul.c -o emulator -lGL -lGLU -lglut -std=c99 -Wall


Please run the ./emulator .

gamul.c,gamul.h ,emul8.c,colortest.c ,beep.aiff


Please change the name of the game in the load_file function ,ie char *filename = "PONG" in file emul8.c ,please change it to other game file by bringing the game in the same folder as the code.

gamul.c file contains the function definitions of load_file,fetch_opcode()
In fetch_opcode() we get opcode from memory buffer(which is filled by the game file)
In decode_opcode() we decode the opcode obtained and update the registers accordingly



The game runs accordingly with player 1 and 2  using 
keys 1 and Q
keys 4 and W  respectively







