#include <libgen.h>
#include <string.h>
#include "file_handler.h"

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

FileData openOutputFile(const char *inputFile) {
  FileData fd;
  char outputPath[256];

  //needed for submission on Coursera, cries if basename is used
  char tempPath[256];

  // Copy the inputFile into outputPath up to its size minus 1 (to ensure null termination)
  strncpy(outputPath, inputFile, sizeof(outputPath) - 1);
  strncpy(tempPath, outputPath, sizeof(tempPath) - 1);
  outputPath[sizeof(outputPath) - 1] = '\0'; // Ensure null termination

  // Find the .vm extension starting from the end
  char *extensionPos = strrchr(outputPath, '.'); // Get the last dot in the string

  if (extensionPos && strcmp(extensionPos, ".vm") == 0) {
    // Correct extension found, let's replace
    strcpy(extensionPos, ".asm");
  } else {
    // No .vm extension found or the string ends unexpectedly
    printf("Error, Wrong File Extension, should be .vm: %s\n", inputFile);
    fd.file = NULL;
    return fd;
  }

  // Debug: Print the outputPath after replacing the extension
  printf("Debug: outputPath after replacing extension: %s\n", outputPath);

  // Extract just the filename from the path
  char *base = basename(tempPath);

  char *dotInBase = strrchr(base, '.');
    if (dotInBase) {
        *dotInBase = '\0'; // Null-terminate at the dot to remove the extension
    }

  // Debug: Print the base
  printf("Debug: base after basename: %s\n", base);

  strncpy(fd.filename, base, sizeof(fd.filename) - 1);
  fd.filename[sizeof(fd.filename) - 1] = '\0'; // Ensure null termination

  // Open the file for writing
  fd.file = fopen(outputPath, "w");
  if (!fd.file) {
    printf("Error: Could not create output file %s\n", outputPath);
  }

  printf("Ouput file location: %s\n", outputPath);

  return fd;
}



void closeFile(FILE *file) {
  if (file) {
    fclose(file);
  }
}