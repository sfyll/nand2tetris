#include <stdio.h>
#include <string.h>

#define RAM_SIZE 32768 // 32K 16-bit words
#define STACK_START 256
#define STACK_END 2047

typedef enum { HACK_MEMORY_SUCCESS, HACK_MEMORY_ERROR } HackMemoryStatus;

typedef struct {
  unsigned short
      RAM[RAM_SIZE]; // This can represent the entire RAM. Each entry is 16-bit.

  // You could also explicitly store pointers or indices to special addresses:
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

HackMemory initHackMemory() {
  HackMemory memory;
  memory.SP = &memory.RAM[0];
  memory.LCL = &memory.RAM[1];
  memory.ARG = &memory.RAM[2];
  memory.THIS = &memory.RAM[3];
  memory.THAT = &memory.RAM[4];
  memory.TEMP = &memory.RAM[5];
  memory.R13 = &memory.RAM[13];
  memory.R14 = &memory.RAM[14];
  memory.R15 = &memory.RAM[15];

  *memory.SP = STACK_START;
  *memory.LCL = 1015;
  *memory.ARG = 1400;
  return memory;
}

unsigned short *getSegmentBasePointer(HackMemory *memory, const char *segment) {
  if (strcmp(segment, "local") == 0) {
    return memory->LCL;
  } else if (strcmp(segment, "argument") == 0) {
    return memory->ARG;
  } else if (strcmp(segment, "this") == 0) {
    return memory->THIS;
  } else if (strcmp(segment, "that") == 0) {
    return memory->THAT;
  } else {
    return memory->SP;
  }
}

void updatePointer(unsigned short **pointer, int operation) {
    if(operation == 1) {
        (*pointer)++;
    } else if(operation == -1) {
        (*pointer)--;
    }
}

HackMemoryStatus pushToStack(HackMemory *memory, const char *segment, short value) {
  unsigned short *basePointer = getSegmentBasePointer(memory, segment);
  printf("Updating the pointer %d to value %d\n", memory->RAM[*basePointer], value);
  if (!basePointer) {
    fprintf(stderr, "Error: Invalid memory segment.\n");
    return HACK_MEMORY_ERROR;
  }
  if (*basePointer >= 0 && *basePointer<= STACK_END) {
    memory->RAM[*basePointer] = value;
    updatePointer(&basePointer, 1);
    printf("Updated the pointer %d to value %d\n", *basePointer, value);
    return HACK_MEMORY_SUCCESS;
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", *memory->SP);
    return HACK_MEMORY_ERROR;
  }
}

short popFromStack(HackMemory *memory, const char *segment) {
  unsigned short *basePointer = getSegmentBasePointer(memory, segment);
  printf("Popping the value %d at pointer %d\n", memory->RAM[*basePointer], *basePointer);
  if (!basePointer) {
    fprintf(stderr, "Error: Invalid memory segment.\n");
    return HACK_MEMORY_ERROR;
  }
  if (*basePointer > 0 && *basePointer <= STACK_END) {
    updatePointer(&basePointer, -1);
    return memory->RAM[*basePointer];
    printf("Popped the value %d at pointer %d\n", memory->RAM[*basePointer], *basePointer);
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", *basePointer);
    return 0;
  }
}

short add(HackMemory *memory) {
  if (*memory->SP >= STACK_START) {
    short x = popFromStack(memory, "stack");
    short y = popFromStack(memory, "stack");
    short added_value = x + y;
    pushToStack(memory, "stack", added_value);
    return memory->RAM[*memory->SP];
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", *memory->SP);
    return 0;
  }
}
