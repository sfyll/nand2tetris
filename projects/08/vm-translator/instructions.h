#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "template.h"

//  TRANSLATING TEXT TO VM INSTRUCTIONS

typedef enum {
    C_INSTRUCTION,
    C_POP,
    C_PUSH,
    BRANCHING,
    INVALID_TYPE
} CommandType;

typedef struct {
  CommandType commandType;
  char *arg1; //Arithmetic Instruction Or Segment or Branching cmd
  short arg2; //segment index
  char *arg3; //label for branching command
} VMInstruction;

typedef struct {
  char *command;
  CommandType type;
} CommandMap;

extern CommandMap commands[];
//The below is for easy printing and debugging
extern const char *CommandTypeStrings[];

 //Sentinel values from not yet supported conmmands
static const CommandMap INVALID_COMMAND = {NULL, INVALID_TYPE};

// TRANSLATING VM INSTRUCTIONS TO HACK-MACHINE ASSEMBLY

typedef struct {
    CommandType type;
    const char *arg1;  // NULL for C_PUSH and C_POP
    const char *assembly_instructions;
} AssemblyMap;

extern AssemblyMap assemblyMappings[];

void initializeAssemblyMappings(void); 
CommandMap getCommandType(const char *command);
void printInstructions(VMInstruction *instructions, int lineCount);
const char* getAssemblyTemplate(VMInstruction *instructions);

#endif

