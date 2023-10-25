#ifndef FILE_OPERATIONS_API_H
#define FILE_OPERATIONS_API_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructions.h"
#include "common_types.h"

// Define the size limits and the VMInstruction struct type
#define MAX_LINE_SIZE 1024

// Function Prototypes
VMInstruction *loadFile(const char *filePath, int *lineCount, FileData *fd);
FileData openOutputFile(const char *inputFile);
void closeFile(FILE *file);

#endif // FILE_OPERATIONS_API_H
