#ifndef TEMPLATE_H 
#define TEMPLATE_H

#include <stdio.h>
#include "common_types.h"

const char *PUSH_MAIN;
const char *PUSH_CONSTANT;
const char *PUSH_POINTER;
const char *PUSH_STATIC;
const char *PUSH_TEMP;

const char *POP_MAIN;
const char *POP_POINTER;
const char *POP_STATIC;
const char *POP_TEMP;

const char *PUSH_TEMPLATE;
const char *POP_TEMPLATE;
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

void initializeTemplates(void);

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
#endif

