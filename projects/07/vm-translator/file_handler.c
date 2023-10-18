#include <libgen.h>
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

FileData openOutputFile(const char *inputFile) {
  FileData fd;
  char outputPath[256];
  
  // Create the output filename by replacing .vm with .asm
  strncpy(outputPath, inputFile, sizeof(outputPath) - 1);
  char *extension = strstr(outputPath, ".vm");
  if (extension) {
    strcpy(extension, ".asm");
  } else {
    printf("Error, Wrong File Extension, should be .vm: %s\n", inputFile);
    fd.file = NULL;
    return fd;
  }

  // Extract just the filename from the path
  char *base = basename(outputPath);
  char *dot = strchr(base, '.');
  if (dot) *dot = '\0'; // Remove the extension
  strncpy(fd.filename, base, sizeof(fd.filename) - 1);

  // Open the file for writing
  fd.file = fopen(outputPath, "w");
  if (!fd.file) {
    printf("Error: Could not create output file %s\n", outputPath);
  }
  
  return fd;
}


void closeFile(FILE *file) {
  if (file) {
    fclose(file);
  }
}