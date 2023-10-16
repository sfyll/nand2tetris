#include "instructions.h"
#include "template.h"
#include <string.h>

CommandMap commands[] = {
    {"add", C_INSTRUCTION}, {"sub", C_INSTRUCTION}, {"neg", C_INSTRUCTION},
    {"eq", C_INSTRUCTION},  {"gt", C_INSTRUCTION},  {"lt", C_INSTRUCTION},
    {"and", C_INSTRUCTION}, {"or", C_INSTRUCTION},  {"not", C_INSTRUCTION},
    {"push", C_PUSH},       {"pop", C_POP},
};

// The below is for easy printing and debugging
const char *CommandTypeStrings[] = {"C_INSTRUCTION", "C_POP", "C_PUSH",
                                    "INVALID_TYPE"};

AssemblyMap assemblyMappings[] = {
    {C_PUSH, "push", NULL},
    {C_POP, "pop", NULL},
    {C_INSTRUCTION, "add", NULL},
    {C_INSTRUCTION, "sub", NULL},
    {C_INSTRUCTION, "neg", NULL},
    {C_INSTRUCTION, "eq", NULL},
    {C_INSTRUCTION, "gt", NULL},
    {C_INSTRUCTION, "lt", NULL},
    {C_INSTRUCTION, "and", NULL},
    {C_INSTRUCTION, "or", NULL},
    {C_INSTRUCTION, "not", NULL},
};

void initializeAssemblyMappings(void) {
  assemblyMappings[0].assembly_instructions = PUSH_TEMPLATE;
  assemblyMappings[1].assembly_instructions = POP_TEMPLATE;
  assemblyMappings[2].assembly_instructions = ADD_TEMPLATE;
  assemblyMappings[3].assembly_instructions = SUB_TEMPLATE;
  assemblyMappings[4].assembly_instructions = NEG_TEMPLATE;
  assemblyMappings[5].assembly_instructions = EQ_TEMPLATE;
  assemblyMappings[6].assembly_instructions = GT_TEMPLATE;
  assemblyMappings[7].assembly_instructions = LT_TEMPLATE;
  assemblyMappings[8].assembly_instructions = AND_TEMPLATE;
  assemblyMappings[9].assembly_instructions = OR_TEMPLATE;
  assemblyMappings[10].assembly_instructions = NOT_TEMPLATE;
}

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
    printf("\tCommand Type: %s\n",
           CommandTypeStrings[instructions[i].commandType]);

    if (instructions[i].commandType == C_INSTRUCTION) {
      printf("\tArg1: %s\n",
             instructions[i].arg1 ? instructions[i].arg1 : "N/A");
      printf("\tArg2: N/A\n");
    } else if ((instructions[i].commandType == C_PUSH) ||
               (instructions[i].commandType == C_POP)) {
      printf("\tArg1: %s\n",
             instructions[i].arg1 ? instructions[i].arg1 : "N/A");
      printf("\tArg2: %d\n", instructions[i].arg2);
    }
  }
}
const char *getAssemblyTemplate(VMInstruction *instruction) {
  for (int i = 0; i < sizeof(assemblyMappings) / sizeof(AssemblyMap); i++) {
    if (assemblyMappings[i].type == instruction->commandType) {
      // If the instruction is of type C_INSTRUCTION, match the exact instruction
      if (instruction->commandType == C_INSTRUCTION) {
        if (strcmp(assemblyMappings[i].arg1, instruction->arg1) == 0) {
          return assemblyMappings[i].assembly_instructions;
        }
      } else {
        return assemblyMappings[i].assembly_instructions;
      }
    }
  }
  return NULL;
}

