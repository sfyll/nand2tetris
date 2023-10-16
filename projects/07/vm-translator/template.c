#include "template.h"

const char *PUSH_TEMPLATE;
const char *POP_TEMPLATE;
const char *ADD_TEMPLATE;
const char *SUB_TEMPLATE;
const char *NEG_TEMPLATE;
const char *EQ_TEMPLATE;
const char *GT_TEMPLATE;
const char *LT_TEMPLATE;
const char *AND_TEMPLATE;
const char *OR_TEMPLATE;
const char *NOT_TEMPLATE;
const char *FILE_END;

void initializeTemplates(void) {
  PUSH_TEMPLATE = "@%d\n"
                  "D=A\n"
                  "@SP\n"
                  "A=M\n"
                  "M=D\n"
                  "@SP\n"
                  "M=M+1\n";

  POP_TEMPLATE = "@%d\n"
                 "A=M-1\n"
                 "D=A\n"
                 "@SP\n"
                 "A=M\n"
                 "M=D\n";

  ADD_TEMPLATE = "@SP\n"
                 "M=M-1\n"
                 "A=M\n"
                 "D=M\n"
                 "A=A-1\n"
                 "M=D+M\n";

  SUB_TEMPLATE = "@SP\n"
                 "M=M-1\n"
                 "A=M\n"
                 "D=M\n"
                 "A=A-1\n"
                 "M=M-D\n";

  NEG_TEMPLATE = "D=0\n"
                 "@SP\n"
                 "A=M-1\n"
                 "M=D-M\n";

  EQ_TEMPLATE = "@SP\n"
                "M=M-1\n"
                "A=M\n"
                "D=M\n"
                "A=A-1\n"
                "D=M-D\n"
                "@ISEQUAL%d\n"
                "D;JEQ\n"
                "@NOTEQUAL%d\n"
                "D;JNE\n"
                "(ISEQUAL%d)\n"
                "@SP\n"
                "A=M-1\n"
                "M=-1\n"
                "@EQEND%d\n"
                "0;JMP\n"
                "(NOTEQUAL%d)\n"
                "@SP\n"
                "A=M-1\n"
                "M=0\n"
                "@EQEND%d\n"
                "0;JMP\n"
                "(EQEND%d)\n";

  GT_TEMPLATE = "@SP\n"
                "M=M-1\n"
                "A=M\n"
                "D=M\n"
                "A=A-1\n"
                "D=M-D\n"
                "@ISGREATER_GT%d\n"
                "D;JGT\n"
                "@ISLESS_GT%d\n"
                "D;JLE\n"
                "(ISGREATER_GT%d)\n"
                "@SP\n"
                "A=M-1\n"
                "M=-1\n"
                "@GTEND%d\n"
                "0;JMP\n"
                "(ISLESS_GT%d)\n"
                "@SP\n"
                "A=M-1\n"
                "M=0\n"
                "@GTEND%d\n"
                "0;JMP\n"
                "(GTEND%d)\n";

  LT_TEMPLATE = "@SP\n"
                "M=M-1\n"
                "A=M\n"
                "D=M\n"
                "A=A-1\n"
                "D=M-D\n"
                "@ISGREATER_LT%d\n"
                "D;JGE\n"
                "@ISLESS_LT%d\n"
                "D;JLT\n"
                "(ISGREATER_LT%d)\n"
                "@SP\n"
                "A=M-1\n"
                "M=0\n"
                "@LTEND%d\n"
                "0;JMP\n"
                "(ISLESS_LT%d)\n"
                "@SP\n"
                "A=M-1\n"
                "M=-1\n"
                "@LTEND%d\n"
                "0;JMP\n"
                "(LTEND%d)\n";
  AND_TEMPLATE = "@SP\n"
                 "M=M-1\n"
                 "A=M\n"
                 "D=M\n"
                 "A=A-1\n"
                 "M=M&D\n";

  OR_TEMPLATE = "@SP\n"
                "M=M-1\n"
                "A=M\n"
                "D=M\n"
                "A=A-1\n"
                "M=M|D\n";

  NOT_TEMPLATE = "@SP\n"
                 "M=M-1\n"
                 "M=!M\n";

  FILE_END = "(END)\n"
             "@END\n"
             "0;JMP\n";
}

void writePushAndPopAssembly(const char *assembly_template, short *value,
                             FILE *outputFile) {
  static char assembly[256]; // Static so it persists after function return
  snprintf(assembly, sizeof(assembly), assembly_template, *value);
  fprintf(outputFile, "%s\n", assembly); // Write to the output file
}

void writeSubAssembly(const char *assembly_template, FILE *outputFile) {
  fprintf(outputFile, "%s\n", assembly_template); // Write to the output file
}

void writeNegAssembly(const char *assembly_template, FILE *outputFile) {
  fprintf(outputFile, "%s\n", assembly_template); // Write to the output file
}

void writeAndAssembly(const char *assembly_template, FILE *outputFile) {
  fprintf(outputFile, "%s\n", assembly_template); // Write to the output file
}

void writeOrAssembly(const char *assembly_template, FILE *outputFile) {
  fprintf(outputFile, "%s\n", assembly_template); // Write to the output file
}

void writeNotAssembly(const char *assembly_template, FILE *outputFile) {
  fprintf(outputFile, "%s\n", assembly_template); // Write to the output file
}

void writeEqAssembly(const char *assembly_template, short *value,
                     FILE *outputFile) {
  static char assembly[256]; // Static so it persists after function return
  snprintf(assembly, sizeof(assembly), assembly_template, *value, *value,
           *value, *value, *value, *value, *value);
  fprintf(outputFile, "%s\n", assembly); // Write to the output file
}

void writeGtAssembly(const char *assembly_template, short *value,
                     FILE *outputFile) {
  static char assembly[256]; // Static so it persists after function return
  snprintf(assembly, sizeof(assembly), assembly_template, *value, *value,
           *value, *value, *value, *value, *value);
  fprintf(outputFile, "%s\n", assembly); // Write to the output file
}

void writeLtAssembly(const char *assembly_template, short *value,
                     FILE *outputFile) {
  static char assembly[256]; // Static so it persists after function return
  snprintf(assembly, sizeof(assembly), assembly_template, *value, *value,
           *value, *value, *value, *value, *value);
  fprintf(outputFile, "%s\n", assembly); // Write to the output file
}

void writeEndOfFileAssembly(FILE *outputFile) {
  fprintf(outputFile, "%s\n", FILE_END); // Write to the output file
}
