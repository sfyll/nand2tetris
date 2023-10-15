#ifndef HACK_RAM_H
#define HACK_RAM_H

// Defines and Constants
#define RAM_SIZE 32768      // 32K 16-bit words
#define STACK_START 256
#define STACK_END 2047

//Function return code if function doesn't organically return anything
typedef enum{
    HACK_MEMORY_SUCCESS,
    HACK_MEMORY_ERROR
} HackMemoryStatus;

// Memory structure for the Hack RAM
typedef struct {
    unsigned short RAM[RAM_SIZE]; // Entire RAM. Each entry is 16-bit.

    // Pointers or indices to special addresses:
    unsigned short *SP;   // Stack Pointer
    unsigned short *LCL;  // Local segment
    unsigned short *ARG;  // Argument segment
    unsigned short *THIS; // this segment
    unsigned short *THAT; // that segment
    unsigned short *TEMP; // TEMP segment starts at address 5 and goes till 12
    unsigned short *R13;  // Variable Register
    unsigned short *R14;  // Variable Register
    unsigned short *R15;  // Variable Register
} HackMemory;

// Function prototypes
HackMemory initHackMemory();
HackMemoryStatus pushToStack(HackMemory *memory, const char *segment, short value);
short popFromStack(HackMemory *memory, const char *segment);
short add(HackMemory *memory);

#endif // HACK_RAM_H

