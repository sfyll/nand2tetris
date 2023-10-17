#include "hack_ram.h"
#include "instructions.h"
#include "template.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 10000

// We returns instructions, a pointer to an array of VMInstruction
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
      // we need to make copies cause strtok modifies the token inplace
      currentInstr.arg1 = strdup(strtok(NULL, " \n"));
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
void executeInstructions(VMInstruction *instructions, int lineCount,
                         HackMemory *memory, FILE *outputFile) {
  for (int i = 0; i < lineCount; i++) {
    VMInstruction instr = instructions[i];
    const char *assembly_code = getAssemblyTemplate(&instr);

    switch (instr.commandType) {
    case C_PUSH:
      pushToStack(memory, instr.arg1, instr.arg2);
      writePushAndPopAssembly(assembly_code, &instr.arg2, outputFile);
      break;

    case C_POP:
      popFromStack(memory, instr.arg1, instr.arg2);
      writePushAndPopAssembly(assembly_code, &instr.arg2, outputFile);
      break;

    case C_INSTRUCTION:
      if (strcmp(instr.arg1, "add") == 0) {
        short result = hack_add(memory);
        writePushAndPopAssembly(assembly_code, &instr.arg2, outputFile);
      } else if (strcmp(instr.arg1, "sub") == 0) {
        short result = hack_sub(memory);
        writeSubAssembly(assembly_code, outputFile);
      } else if (strcmp(instr.arg1, "neg") == 0) {
        short result = hack_neg(memory);
        writeNegAssembly(assembly_code, outputFile);
      } else if (strcmp(instr.arg1, "eq") == 0) {
        short result = hack_eq(memory);
        short value_to_pass = (short)i;
        writeEqAssembly(assembly_code, &value_to_pass, outputFile);
      } else if (strcmp(instr.arg1, "gt") == 0) {
        short result = hack_gt(memory);
        short value_to_pass = (short)i;
        writeGtAssembly(assembly_code, &value_to_pass, outputFile);
      } else if (strcmp(instr.arg1, "lt") == 0) {
        short result = hack_lt(memory);
        short value_to_pass = (short)i;
        writeLtAssembly(assembly_code, &value_to_pass, outputFile);
      } else if (strcmp(instr.arg1, "and") == 0) {
        short result = hack_and(memory);
        writeAndAssembly(assembly_code, outputFile);
      } else if (strcmp(instr.arg1, "or") == 0) {
        short result = hack_or(memory);
        writeOrAssembly(assembly_code, outputFile);
      } else if (strcmp(instr.arg1, "not") == 0) {
        short result = hack_not(memory);
        writeNotAssembly(assembly_code, outputFile);
      }
      break;

    default:
      printf("Unknown instruction type.\n");
      break;
    }
    printf("%d, %s, %d\n", instr.commandType, instr.arg1, instr.arg2);
    printHackMemory(*memory);
  }
  writeEndOfFileAssembly(outputFile);
}

FILE *openOutputFile(const char *inputFile) {
  // Create the output filename by replacing .vm with .asm
  char outputFile[256]; // Ensure enough space
  strncpy(outputFile, inputFile, sizeof(outputFile) - 1);
  char *extension = strstr(outputFile, ".vm");
  if (extension) {
    strcpy(extension, ".asm");
  } else {
    printf("Error, Wrong File Extension, should be .vm: %s\n", inputFile);
    return NULL;
  }

  // Open the file for writing
  FILE *file = fopen(outputFile, "w");
  if (!file) {
    printf("Error: Could not create output file %s\n", outputFile);
  }
  return file;
}

void closeFile(FILE *file) {
  if (file) {
    fclose(file);
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

  // Use the utility function to open the output file
  FILE *file = openOutputFile(argv[1]);
  if (!file) {
    return 1; // exit if error occurred
  }

  // Create memory component
  HackMemory memory = initHackMemory();

  // Initialize assembly templates
  initializeTemplates();
  printf("template: %s\n", PUSH_TEMPLATE);
  // Populate Instruction mapper to assembly template
  initializeAssemblyMappings();

  executeInstructions(instructions, lineCount, &memory, file);

  closeFile(file);

  return 0;
}
