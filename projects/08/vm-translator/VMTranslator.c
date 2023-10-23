#include <dirent.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common_types.h"
#include "file_handler.h"
#include "hack_ram.h"
#include "instructions.h"
#include "template.h"

void executeInstructions(VMInstruction *instructions, int lineCount,
                         HackMemory *memory, FileData outputFile) {
  for (int i = 0; i < lineCount; i++) {
    VMInstruction instr = instructions[i];
    printf("RIGHT HERE: %d, %s, %d\n", instr.commandType, instr.arg1, instr.arg2);
    const char *assembly_code = getAssemblyTemplate(&instr);

    switch (instr.commandType) {
    case C_PUSH:
      pushToStack(memory, instr.arg1, instr.arg2);
      writePushAssembly(assembly_code, instr.arg1, &instr.arg2, outputFile);
      break;

    case C_POP:
      popFromStack(memory, instr.arg1, instr.arg2);
      writePopAssembly(assembly_code, instr.arg1, &instr.arg2, outputFile);
      break;

    case C_INSTRUCTION:
      if (strcmp(instr.arg1, "add") == 0) {
        hack_add(memory);
        writeAddAssembly(assembly_code, outputFile);
      } else if (strcmp(instr.arg1, "sub") == 0) {
        hack_sub(memory);
        writeSubAssembly(assembly_code, outputFile);
      } else if (strcmp(instr.arg1, "neg") == 0) {
        hack_neg(memory);
        writeNegAssembly(assembly_code, outputFile);
      } else if (strcmp(instr.arg1, "eq") == 0) {
        hack_eq(memory);
        short value_to_pass = (short)i;
        writeEqAssembly(assembly_code, &value_to_pass, outputFile);
      } else if (strcmp(instr.arg1, "gt") == 0) {
        hack_gt(memory);
        short value_to_pass = (short)i;
        writeGtAssembly(assembly_code, &value_to_pass, outputFile);
      } else if (strcmp(instr.arg1, "lt") == 0) {
        hack_lt(memory);
        short value_to_pass = (short)i;
        writeLtAssembly(assembly_code, &value_to_pass, outputFile);
      } else if (strcmp(instr.arg1, "and") == 0) {
        hack_and(memory);
        writeAndAssembly(assembly_code, outputFile);
      } else if (strcmp(instr.arg1, "or") == 0) {
        hack_or(memory);
        writeOrAssembly(assembly_code, outputFile);
      } else if (strcmp(instr.arg1, "not") == 0) {
        hack_not(memory);
        writeNotAssembly(assembly_code, outputFile);
      }
      break;

    case BRANCHING:
      if (strcmp(instr.arg1, "label") == 0) {
        writeLabelAssembly(assembly_code, instr.arg3, outputFile);
      } else if (strcmp(instr.arg1, "goto") == 0) {
        writeGotoAssembly(assembly_code, instr.arg3, outputFile);
      } else if (strcmp(instr.arg1, "if-goto") == 0) {
        writeIfGotoAssembly(assembly_code, instr.arg3, outputFile);
      }
      break;

    case FUNCTION:
      if (strcmp(instr.arg1, "function") == 0) {
        writeFunctionLabelAssembly(assembly_code, instr.arg3, instr.arg2, outputFile);
      } else if (strcmp(instr.arg1, "return") == 0) {
        writeFunctionReturnAssembly(assembly_code, outputFile);
      }
      break;

    default:
      printf("Unknown instruction type.\n");
      break;
    }
    printHackMemory(*memory);
  }
  writeEndOfFileAssembly(outputFile);
}

void processFile(const char *filePath, FileData file) {
  int lineCount = 0;
  printf("%s\n", filePath);
  VMInstruction *instructions = loadFile(filePath, &lineCount);
  printf("UP\n");
  
  // Create memory component
  HackMemory memory = initHackMemory();

  printf("UP\n");
  // Initialize assembly templates
  initializeTemplates();
  // Populate Instruction mapper to assembly template
  initializeAssemblyMappings();

  executeInstructions(instructions, lineCount, &memory, file);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <file_path_or_dir>\n", argv[0]);
    return 1;
  }

  struct stat path_stat;
  stat(argv[1], &path_stat);

  FileData file;
  char outputPath[256] = {0};

  if (S_ISDIR(path_stat.st_mode)) {
    snprintf(outputPath, sizeof(outputPath), "%s.vm", basename(argv[1]));
    file = openOutputFile(outputPath);

    if (!file.file) {
      return 1; // exit if error occurred
    }
    DIR *dir;
    struct dirent *entry;

    dir = opendir(argv[1]);
    if (!dir) {
      printf("Error opening directory.\n");
      return 1;
    }
    // First pass: Process Sys.vm
    char sysVmPath[512];
    snprintf(sysVmPath, sizeof(sysVmPath), "%s/Sys.vm", argv[1]);
    processFile(sysVmPath, file);

    // Second pass: Process all other .vm files
    while ((entry = readdir(dir)) != NULL) {
      char fileInDir[512];
      snprintf(fileInDir, sizeof(fileInDir), "%s/%s", argv[1], entry->d_name);
      if (strstr(fileInDir, ".vm") && strcmp(entry->d_name, "Sys.vm")) {
        processFile(fileInDir, file);
      }
    }

    closedir(dir);
  } else if (S_ISREG(path_stat.st_mode)) {
    file = openOutputFile(argv[1]);
    if (!file.file) {
      return 1; // exit if error occurred
    }
    printf("%s\n", file.filename);
    processFile(argv[1], file);
  } else {
    printf("Unknown file type.\n");
    return 1;
  }

  return 0;
}
