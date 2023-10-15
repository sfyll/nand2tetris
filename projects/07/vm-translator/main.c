#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hack_ram.h"

typedef enum {
    C_INSTRUCTION,
    C_POP,
    C_PUSH,
    INVALID_TYPE
} CommandType;

typedef struct {
  CommandType commandType;
  char *arg1;
  int arg2;
} VMInstruction;

typedef struct {
  char *command;
  CommandType type;
} CommandMap;

CommandMap commands[] = {
    {"add", C_INSTRUCTION}, {"sub", C_INSTRUCTION}, {"neg", C_INSTRUCTION},
    {"eq", C_INSTRUCTION},  {"gt", C_INSTRUCTION},  {"lt", C_INSTRUCTION},
    {"and", C_INSTRUCTION}, {"or", C_INSTRUCTION},  {"not", C_INSTRUCTION},
    {"push", C_PUSH},       {"pop", C_POP},
};

//The below is for easy printing and debugging
const char *CommandTypeStrings[] = {
    "C_INSTRUCTION",
    "C_POP",
    "C_PUSH",
    "INVALID_TYPE"
};

//Sentinel values from not yet supported conmmands
static const CommandMap INVALID_COMMAND = {NULL, INVALID_TYPE};


CommandMap getCommandType(const char *command) {
  for (int i = 0; i < sizeof(commands) / sizeof(CommandMap); i++) {
    if (strcmp(commands[i].command, command) == 0) {
      return commands[i];
    }
  }
  return INVALID_COMMAND; // Not found
}

void printInstructions(VMInstruction *instructions, int lineCount) {
    for (int i = 0; i < lineCount; i++) {
        printf("Instruction %d:\n", i + 1);
        printf("\tCommand Type: %s\n", CommandTypeStrings[instructions[i].commandType]);
        
        if (instructions[i].commandType == C_INSTRUCTION) {
            printf("\tArg1: %s\n", instructions[i].arg1 ? instructions[i].arg1 : "N/A");
            printf("\tArg2: N/A\n");
        } else if ((instructions[i].commandType == C_PUSH) ||
            (instructions[i].commandType == C_POP)) {
            printf("\tArg1: %s\n", instructions[i].arg1 ? instructions[i].arg1 : "N/A");
            printf("\tArg2: %d\n", instructions[i].arg2);
        }
    }
}

#define MAX_LINE_SIZE 10000

//We returns instructions, a pointer to an array of VMInstruction
VMInstruction *loadFile(const char *filePath, int *lineCount) {
  FILE *file = fopen(filePath, "r");
  if (file == NULL) {
    perror("Error opening file");
    return NULL;
  }

  VMInstruction *instructions = NULL;
  char buffer[MAX_LINE_SIZE];

  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    char *token;
    int wordCount = 0;

    token = strtok(buffer, " \t\r\n");

    // Ignore empty or comment lines
    if (!token || token[0] == '/')
      continue;

    CommandMap commandType = getCommandType(token);
    if (commandType.type == INVALID_COMMAND.type) {
      fprintf(stderr, "Error: Unrecognized command in line: %d\n", *lineCount);
      fprintf(stderr, "Line in question: %s\n", buffer);
      fprintf(stderr, "Token in question: %s\n", token);
      exit(EXIT_FAILURE);
    }

    VMInstruction currentInstr;
    currentInstr.commandType = commandType.type;

    if (currentInstr.commandType == C_INSTRUCTION) {
      currentInstr.arg1 = commandType.command;
      currentInstr.arg2 = 0;
    } else if ((currentInstr.commandType == C_PUSH) ||
               (currentInstr.commandType == C_POP)) {
      currentInstr.arg1 = strtok(NULL, " \n");
      currentInstr.arg2 = atoi(strtok(
          NULL, " \n")); // Get the next token for arg2 and convert to int
    }
    instructions =
        realloc(instructions, (*lineCount + 1) * sizeof(VMInstruction));
    instructions[*lineCount] = currentInstr;
    (*lineCount)++;
  }

  fclose(file);
  return instructions;
}

void executeInstructions(VMInstruction *instructions, int lineCount, HackMemory *memory) {
    for (int i = 0; i < lineCount; i++) {
        VMInstruction instr = instructions[i];
        
        switch (instr.commandType) {
            case C_PUSH:
                pushToStack(memory, instr.arg1, instr.arg2);
                break;

            case C_POP:
                popFromStack(memory, instr.arg1);
                break;

            case C_INSTRUCTION:
                if (strcmp(instr.arg1, "add") == 0) {
                    add(memory);
                }
                // You can add more cases here for sub, neg, etc.
                break;

            default:
                printf("Unknown instruction type.\n");
                break;
        }
    }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <file_path>\n", argv[0]);
    return 1;
  }

  int lineCount = 0;
  VMInstruction *instructions = loadFile(argv[1], &lineCount);

  printInstructions(instructions, lineCount);

  HackMemory memory = initHackMemory();

  executeInstructions(instructions, lineCount, &memory);
  return 0;
}

