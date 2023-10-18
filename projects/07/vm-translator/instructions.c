#include "instructions.h"
#include <string.h>
#include <stdio.h>

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
    {C_INSTRUCTION, "add", NULL}, {C_INSTRUCTION, "sub", NULL},
    {C_INSTRUCTION, "neg", NULL}, {C_INSTRUCTION, "eq", NULL},
    {C_INSTRUCTION, "gt", NULL},  {C_INSTRUCTION, "lt", NULL},
    {C_INSTRUCTION, "and", NULL}, {C_INSTRUCTION, "or", NULL},
    {C_INSTRUCTION, "not", NULL}, {C_PUSH, "local", NULL},
    {C_PUSH, "argument", NULL},   {C_PUSH, "this", NULL},
    {C_PUSH, "that", NULL},       {C_PUSH, "constant", NULL},
    {C_PUSH, "temp", NULL},       {C_PUSH, "pointer", NULL},
    {C_PUSH, "static", NULL},     {C_POP, "local", NULL},
    {C_POP, "argument", NULL},    {C_POP, "this", NULL},
    {C_POP, "that", NULL},        {C_POP, "temp", NULL},
    {C_POP, "pointer", NULL},     {C_POP, "static", NULL},
};

void initializeAssemblyMappings(void) {
  assemblyMappings[0].assembly_instructions = ADD_TEMPLATE;
  assemblyMappings[1].assembly_instructions = SUB_TEMPLATE;
  assemblyMappings[2].assembly_instructions = NEG_TEMPLATE;
  assemblyMappings[3].assembly_instructions = EQ_TEMPLATE;
  assemblyMappings[4].assembly_instructions = GT_TEMPLATE;
  assemblyMappings[5].assembly_instructions = LT_TEMPLATE;
  assemblyMappings[6].assembly_instructions = AND_TEMPLATE;
  assemblyMappings[7].assembly_instructions = OR_TEMPLATE;
  assemblyMappings[8].assembly_instructions = NOT_TEMPLATE;
  assemblyMappings[9].assembly_instructions = PUSH_MAIN;
  assemblyMappings[10].assembly_instructions = PUSH_MAIN;
  assemblyMappings[11].assembly_instructions = PUSH_MAIN;
  assemblyMappings[12].assembly_instructions = PUSH_MAIN;
  assemblyMappings[13].assembly_instructions = PUSH_CONSTANT;
  assemblyMappings[14].assembly_instructions = PUSH_TEMP;
  assemblyMappings[15].assembly_instructions = PUSH_POINTER;
  assemblyMappings[16].assembly_instructions = PUSH_STATIC;
  assemblyMappings[17].assembly_instructions = POP_MAIN;
  assemblyMappings[18].assembly_instructions = POP_MAIN;
  assemblyMappings[19].assembly_instructions = POP_MAIN;
  assemblyMappings[20].assembly_instructions = POP_MAIN;
  assemblyMappings[21].assembly_instructions = POP_TEMP;
  assemblyMappings[22].assembly_instructions = POP_POINTER;
  assemblyMappings[23].assembly_instructions = POP_STATIC;
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
        if (strcmp(assemblyMappings[i].arg1, instruction->arg1) == 0) {
          return assemblyMappings[i].assembly_instructions;
        }
    }
  }
  return NULL;
}

