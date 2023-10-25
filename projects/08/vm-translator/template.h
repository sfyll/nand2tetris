#ifndef TEMPLATE_H 
#define TEMPLATE_H

#include <stdio.h>
#include "common_types.h"

extern const char *INIT;

extern const char *PUSH_MAIN;
extern const char *PUSH_CONSTANT;
extern const char *PUSH_POINTER;
extern const char *PUSH_STATIC;
extern const char *PUSH_TEMP;

extern const char *POP_MAIN;
extern const char *POP_POINTER;
extern const char *POP_STATIC;
extern const char *POP_TEMP;

extern const char *ADD_TEMPLATE;
extern const char *SUB_TEMPLATE;
extern const char *NEG_TEMPLATE;
extern const char *EQ_TEMPLATE;
extern const char *GT_TEMPLATE;
extern const char *LT_TEMPLATE;
extern const char *AND_TEMPLATE;
extern const char *OR_TEMPLATE;
extern const char *NOT_TEMPLATE;
extern const char *FILE_END;

extern const char *LABEL_TEMPLATE;
extern const char *IF_GOTO_TEMPLATE;
extern const char *GOTO_TEMPLATE;

extern const char *FUNCTION_CALL_TEMPLATE;
extern const char *FUNCTION_RETURN_TEMPLATE;

void initializeTemplates(void);

void writeInitAssembly(FileData outputFile);
void writePushAssembly(const char *assembly_template, char *segment, short *address, FileData outputFile);
void writePopAssembly(const char *assembly_template, char *segment, short *address, FileData outputFile);
void writeAddAssembly(const char *assembly_template, FileData outputFile);
void writeSubAssembly(const char *assembly_template, FileData outputFile);
void writeNegAssembly(const char *assembly_template, FileData outputFile);
void writeAndAssembly(const char *assembly_template, FileData outputFile);
void writeOrAssembly(const char *assembly_template, FileData outputFile);
void writeNotAssembly(const char *assembly_template, FileData outputFile);
void writeEqAssembly(const char *assembly_template, short *value, FileData outputFile);
void writeGtAssembly(const char *assembly_template, short *value, FileData outputFile);
void writeLtAssembly(const char *assembly_template, short *value, FileData outputFile);
void writeEndOfFileAssembly(FileData outputFile);
void writeLabelAssembly(const char *assembly_template, char *label, FileData outputFile);
void writeGotoAssembly(const char *assembly_template, char *label, FileData outputFile);
void writeIfGotoAssembly(const char *assembly_template, char *label, FileData outputFile);
void writeFunctionLabelAssembly(const char *assembly_template, char *label, int nArgs, FileData outputFile);
void writeFunctionReturnAssembly(const char *assembly_template, FileData outputFile);
void writeFunctionCallAssembly(const char *assembly_template, char *label, int nArgs, int functionCallCounter, FileData outputFile);
#endif

