#ifndef TEMPLATE_H 
#define TEMPLATE_H

#include <stdio.h>

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
extern const char *POP_MAIN;
extern const char *POP_POINTER;
extern const char *POP_STATIC;
extern const char *POP_TEMP;
extern const char *PUSH_MAIN;
extern const char *PUSH_CONSTANT;
extern const char *PUSH_POINTER;
extern const char *PUSH_STATIC;
extern const char *PUSH_TEMP;

void initializeTemplates(void);

void writePushAssembly(const char *assembly_template, char *segment, short *address, FILE *outputFile);
void writePopAssembly(const char *assembly_template, char *segment, short *address, FILE *outputFile);
void writeAddAssembly(const char *assembly_template, FILE *outputFile);
void writeSubAssembly(const char *assembly_template, FILE *outputFile);
void writeNegAssembly(const char *assembly_template, FILE *outputFile);
void writeAndAssembly(const char *assembly_template, FILE *outputFile);
void writeOrAssembly(const char *assembly_template, FILE *outputFile);
void writeNotAssembly(const char *assembly_template, FILE *outputFile);
void writeEqAssembly(const char *assembly_template, short *value, FILE *outputFile);
void writeGtAssembly(const char *assembly_template, short *value, FILE *outputFile);
void writeLtAssembly(const char *assembly_template, short *value, FILE *outputFile);
void writeEndOfFileAssembly(FILE *outputFile);
#endif

