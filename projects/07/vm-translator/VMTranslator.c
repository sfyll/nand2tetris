#include "hack_ram.h"
#include "instructions.h"
#include "template.h"
#include "file_handler.h"

void executeInstructions(VMInstruction *instructions, int lineCount,
                         HackMemory *memory, FileData outputFile) {
  for (int i = 0; i < lineCount; i++) {
    VMInstruction instr = instructions[i];
    printf("%d, %s, %d\n", instr.commandType, instr.arg1, instr.arg2);
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
        short result = hack_add(memory);
        writeAddAssembly(assembly_code, outputFile);
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
    printHackMemory(*memory);
  }
  writeEndOfFileAssembly(outputFile);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <file_path>\n", argv[0]);
    return 1;
  }

  int lineCount = 0;
  VMInstruction *instructions = loadFile(argv[1], &lineCount);

  // Use the utility function to open the output file
  FileData file = openOutputFile(argv[1]);
  if (!file.file) {
    return 1; // exit if error occurred
  }

  // Create memory component
  HackMemory memory = initHackMemory();

  // Initialize assembly templates
  initializeTemplates();
  // Populate Instruction mapper to assembly template
  initializeAssemblyMappings();

  executeInstructions(instructions, lineCount, &memory, file);

  closeFile(file.file);

  return 0;
}
