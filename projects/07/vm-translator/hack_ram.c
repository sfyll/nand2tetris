#include <stdio.h>
#include <string.h>

#define RAM_SIZE 32768 // 32K 16-bit words
#define STACK_START 256
#define STACK_END 2047

typedef enum { HACK_MEMORY_SUCCESS, HACK_MEMORY_ERROR } HackMemoryStatus;

typedef struct {
  short
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
  memory.SP = (unsigned short *)&memory.RAM[0];
  memory.LCL = (unsigned short *)&memory.RAM[1];
  memory.ARG = (unsigned short *)&memory.RAM[2];
  memory.THIS =(unsigned short *)&memory.RAM[3];
  memory.THAT =(unsigned short *)&memory.RAM[4];
  memory.TEMP =(unsigned short *)&memory.RAM[5];
  memory.R13 =(unsigned short *)&memory.RAM[13];
  memory.R14 =(unsigned short *)&memory.RAM[14];
  memory.R15 =(unsigned short *)&memory.RAM[15];

  *memory.SP = STACK_START;
  *memory.LCL = 1015;
  *memory.ARG = 1400;
  *memory.THIS = 1650;
  *memory.THAT = 1800;
  return memory;
}

void printHackMemory(HackMemory memory) {
  printf("Stack: ");
  for (int i = 0; i < 10; i++) {
    printf("%d ", memory.RAM[STACK_START + i]);
  }
  printf("\n");

  unsigned short *segments[] = {memory.LCL, memory.ARG, memory.THIS,
                                memory.THAT, memory.TEMP};
  const char *segmentNames[] = {"LCL", "ARG", "THIS", "THAT", "TEMP"};

  for (int i = 0; i < 5; i++) {
    printf("%s: ", segmentNames[i]);
    for (int j = 0; j < 9; j++) {
      if (strcmp(segmentNames[i], "TEMP") == 0) {
        // temp allocated in place
        printf("%d ", memory.RAM[5+ j]);
      } else {
        printf("%d ", memory.RAM[*segments[i] + j]);
      }
    }
    printf("\n");
  }
}
unsigned short *getSegmentBasePointer(HackMemory *memory, const char *segment,
                                      unsigned short pointer_value) {
  unsigned short *basePointer;

  if (strcmp(segment, "local") == 0) {
    basePointer = memory->LCL;
  } else if (strcmp(segment, "argument") == 0) {
    basePointer = memory->ARG;
  } else if (strcmp(segment, "this") == 0) {
    basePointer = memory->THIS;
  } else if (strcmp(segment, "that") == 0) {
    basePointer = memory->THAT;
  } else if (strcmp(segment, "temp") == 0) {
      //hacky way, TEMP starts at RAM[5]
      static unsigned short constant_five = 5; 
      basePointer = &constant_five;
  } else if (strcmp(segment, "pointer") == 0) {
    if (pointer_value == 0) {
      basePointer = memory->THIS;
    } else if (pointer_value == 1) {
      basePointer = memory->THAT;
    } else {
      return NULL;
    }
  } else {
    basePointer = memory->SP;
  }

  return basePointer;
}

void updatePointer(unsigned short *pointer, int operation,
                   const char *segment) {
  if (strcmp(segment, "constant") == 0) {
    if (operation == 1) {
      (*pointer)++;
    } else if (operation == -1) {
      (*pointer)--;
    }
  }
}
HackMemoryStatus pushToStack(HackMemory *memory, const char *segment,
                             short value) {
  // Value is either a constant to push to the stack
  // Or an index i that needs to be added to our base segment pointer
  unsigned short *basePointer = getSegmentBasePointer(memory, segment, value);
  if (!basePointer) {
    fprintf(stderr, "Error: Invalid memory segment.\n");
    return HACK_MEMORY_ERROR;
  }
  if (*basePointer >= 0 && *basePointer <= STACK_END) {
    if ((strcmp(segment, "constant") == 0) || (strcmp(segment, "stack") == 0)) {
      // Directly push value to the stack if segment is "constant"
      memory->RAM[*basePointer] = value;
    } else {
      // Push the value stored at address (computed using base pointer) to the
      // stack for other segments
      if (strcmp(segment, "pointer") == 0) {
        unsigned short address = *basePointer;
        memory->RAM[address] = memory->RAM[*memory->SP];
      } else {
        unsigned short address = *basePointer + value;
        memory->RAM[*memory->SP] = memory->RAM[address];
      }
    }
    (*memory->SP)++;
    return HACK_MEMORY_SUCCESS;
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", *memory->SP);
    return HACK_MEMORY_ERROR;
  }
}

short popFromStack(HackMemory *memory, const char *segment,
                   unsigned short pointer_index) {
  unsigned short *basePointer =
      getSegmentBasePointer(memory, segment, pointer_index);
  if (!basePointer) {
    fprintf(stderr, "Error: Invalid memory segment.\n");
    return HACK_MEMORY_ERROR;
  }
  (*memory->SP)--;
  unsigned short address = *basePointer + pointer_index;
  if (address >= 0 && address <= STACK_END) {
    memory->RAM[address] = memory->RAM[*memory->SP];
    return memory->RAM[*memory->SP];
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", address);
    return 0;
  }
}

void popTopTwoFromStack(HackMemory *memory, short *x, short *y) {
  *y = popFromStack(memory, "stack", 0);
  *x = popFromStack(memory, "stack", 0);
}

short hack_add(HackMemory *memory) {
  if (*memory->SP >= STACK_START) {
    short x, y;
    popTopTwoFromStack(memory, &x, &y);
    short added_value = x + y;
    pushToStack(memory, "stack", added_value);
    return memory->RAM[*memory->SP];
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", *memory->SP);
    return 0;
  }
}

short hack_sub(HackMemory *memory) {
  if (*memory->SP >= STACK_START) {
    short x, y;
    popTopTwoFromStack(memory, &x, &y);
    short result = x - y;
    printf("result: %d\n", result);
    pushToStack(memory, "stack", result);
    return memory->RAM[*memory->SP];
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", *memory->SP);
    return 0;
  }
}

short hack_neg(HackMemory *memory) {
  if (*memory->SP >= STACK_START) {
    short x = popFromStack(memory, "stack", 0);
    short result = -x;
    pushToStack(memory, "stack", result);
    return memory->RAM[*memory->SP];
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", *memory->SP);
    return 0;
  }
}

short hack_eq(HackMemory *memory) {
  if (*memory->SP >= STACK_START) {
    short x, y;
    popTopTwoFromStack(memory, &x, &y);
    short result = (x == y) ? -1 : 0;
    pushToStack(memory, "stack", result);
    return memory->RAM[*memory->SP];
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", *memory->SP);
    return 0;
  }
}

short hack_lt(HackMemory *memory) {
  if (*memory->SP >= STACK_START) {
    short x, y;
    popTopTwoFromStack(memory, &x, &y);
    short result = (x < y) ? -1 : 0;
    pushToStack(memory, "stack", result);
    return memory->RAM[*memory->SP];
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", *memory->SP);
    return 0;
  }
}

short hack_gt(HackMemory *memory) {
  if (*memory->SP >= STACK_START) {
    short x, y;
    popTopTwoFromStack(memory, &x, &y);
    short result = (x > y) ? -1 : 0;
    pushToStack(memory, "stack", result);
    return memory->RAM[*memory->SP];
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", *memory->SP);
    return 0;
  }
}

short hack_and(HackMemory *memory) {
  if (*memory->SP >= STACK_START) {
    short x, y;
    popTopTwoFromStack(memory, &x, &y);
    short result = x & y;
    pushToStack(memory, "stack", result);
    return memory->RAM[*memory->SP];
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", *memory->SP);
    return 0;
  }
}

short hack_or(HackMemory *memory) {
  if (*memory->SP >= STACK_START) {
    short x, y;
    popTopTwoFromStack(memory, &x, &y);
    short result = x | y;
    pushToStack(memory, "stack", result);
    return memory->RAM[*memory->SP];
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", *memory->SP);
    return 0;
  }
}

short hack_not(HackMemory *memory) {
  if (*memory->SP >= STACK_START) {
    short x = popFromStack(memory, "stack", 0);
    short result = ~x;
    pushToStack(memory, "stack", result);
    return memory->RAM[*memory->SP];
  } else {
    fprintf(stderr, "Error: Memory Pointer out-of-bond: %d\n", *memory->SP);
    return 0;
  }
}
