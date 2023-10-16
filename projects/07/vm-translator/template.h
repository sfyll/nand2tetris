#ifndef TEMPLATE_H 
#define TEMPLATE_H

#include <stdio.h>

extern const char *PUSH_TEMPLATE;
extern const char *POP_TEMPLATE;
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

void writePushAndPopAssembly(const char *assembly_template, short *value, FILE *outputFile);
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

