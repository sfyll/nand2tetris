#include <string.h>

#include "template.h"

const char *PUSH_MAIN;
const char *PUSH_CONSTANT;
const char *PUSH_POINTER;
const char *PUSH_STATIC;
const char *PUSH_TEMP;

const char *POP_MAIN;
const char *POP_POINTER;
const char *POP_STATIC;
const char *POP_TEMP;

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

  PUSH_MAIN = "@%d\n"
              "D=A\n"
              "@%s\n"
              "D=D+M\n"
              "@addr\n"
              "M=D\n"
              "A=M\n"
              "D=M\n"
              "@SP\n"
              "A=M\n"
              "M=D\n"
              "@SP\n"
              "M=M+1\n";

  PUSH_CONSTANT = "@%d\n"
                  "D=A\n"
                  "@SP\n"
                  "A=M\n"
                  "M=D\n"
                  "@SP\n"
                  "M=M+1\n";

  PUSH_TEMP = "@%d\n"
              "D=A\n"
              "@5\n"
              "D=D+A\n"
              "A=D\n"
              "D=M\n"
              "@SP\n"
              "A=M\n"
              "M=D\n"
              "@SP\n"
              "M=M+1\n";

  PUSH_POINTER = "@%s\n"
                 "D=M\n"
                 "@SP\n"
                 "A=M\n"
                 "M=D\n"
                 "@SP\n"
                 "M=M+1\n";

  PUSH_STATIC = "@%s.%d\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n";

  POP_MAIN = "@%d\n"
             "D=A\n"
             "@%s\n"
             "D=D+M\n"
             "@addr\n"
             "M=D\n"
             "@SP\n"
             "M=M-1\n"
             "A=M\n"
             "D=M\n"
             "@addr\n"
             "A=M\n"
             "M=D\n";

  POP_TEMP = "@%d\n"
             "D=A\n"
             "@5\n"
             "D=D+A\n"
             "@addr\n"
             "M=D\n"
             "@SP\n"
             "M=M-1\n"
             "A=M\n"
             "D=M\n"
             "@addr\n"
             "A=M\n"
             "M=D\n";

  POP_POINTER = "@SP\n"
                "M=M-1\n"
                "A=M\n"
                "D=M\n"
                "@%s\n"
                "M=D\n";

  POP_STATIC = "@SP\n"
               "M=M-1\n"
               "A=M\n"
               "D=M\n"
               "@%s.%d\n"
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
                 "A=M-1\n"
                 "M=!M\n";

  FILE_END = "(END)\n"
             "@END\n"
             "0;JMP\n";
}

char *segment_mapping(char *segment) {
  if (strcmp(segment, "local") == 0)
    return "LCL";
  if (strcmp(segment, "argument") == 0)
    return "ARG";
  if (strcmp(segment, "this") == 0)
    return "THIS";
  if (strcmp(segment, "that") == 0)
    return "THAT";
  return NULL;
}

void writePushAssembly(const char *assembly_template, char *segment, short *address,
                             FileData outputFile) {
  static char assembly[256]; // Static so it persists after function return
  if (strcmp(segment, "local") == 0 || strcmp(segment, "argument") == 0 ||
      strcmp(segment, "this") == 0 || strcmp(segment, "that") == 0) {
      char *mapped_segment = segment_mapping(segment);
      snprintf(assembly, sizeof(assembly), assembly_template, *address, mapped_segment);
  } else if (strcmp(segment, "constant") == 0 || strcmp(segment, "temp") == 0) {
      snprintf(assembly, sizeof(assembly), assembly_template, *address);
  } else if (strcmp(segment, "pointer") == 0) {
    char *memory_idx = (*address == 0) ? "THIS" : "THAT";
    snprintf(assembly, sizeof(assembly), assembly_template, memory_idx);
  } else if (strcmp(segment, "static") == 0) {
    snprintf(assembly, sizeof(assembly), assembly_template, outputFile.filename, *address);
  }
  fprintf(outputFile.file,"%s\n", assembly); // Write to the output file
}

void writePopAssembly(const char *assembly_template, char *segment, short *address,
                             FileData outputFile) {
  static char assembly[256]; // Static so it persists after function return
  if (strcmp(segment, "local") == 0 || strcmp(segment, "argument") == 0 ||
      strcmp(segment, "this") == 0 || strcmp(segment, "that") == 0) {
      char *mapped_segment = segment_mapping(segment);
      snprintf(assembly, sizeof(assembly), assembly_template, *address, mapped_segment);
  } else if (strcmp(segment, "temp") == 0) {
      snprintf(assembly, sizeof(assembly), assembly_template, *address);
  } else if (strcmp(segment, "pointer") == 0) {
    char *memory_idx = (*address == 0) ? "THIS" : "THAT";
    snprintf(assembly, sizeof(assembly), assembly_template, memory_idx);
  } else if (strcmp(segment, "static") == 0) {
    snprintf(assembly, sizeof(assembly), assembly_template, outputFile.filename, *address);
  }
  fprintf(outputFile.file,"%s\n", assembly); // Write to the output file
}

void writeAddAssembly(const char *assembly_template, FileData outputFile) {
  fprintf(outputFile.file,"%s\n", assembly_template); // Write to the output file
}

void writeSubAssembly(const char *assembly_template, FileData outputFile) {
  fprintf(outputFile.file,"%s\n", assembly_template); // Write to the output file
}

void writeNegAssembly(const char *assembly_template, FileData outputFile) {
  fprintf(outputFile.file,"%s\n", assembly_template); // Write to the output file
}

void writeAndAssembly(const char *assembly_template, FileData outputFile) {
  fprintf(outputFile.file,"%s\n", assembly_template); // Write to the output file
}

void writeOrAssembly(const char *assembly_template, FileData outputFile) {
  fprintf(outputFile.file,"%s\n", assembly_template); // Write to the output file
}

void writeNotAssembly(const char *assembly_template, FileData outputFile) {
  fprintf(outputFile.file,"%s\n", assembly_template); // Write to the output file
}

void writeEqAssembly(const char *assembly_template, short *value,
                     FileData outputFile) {
  static char assembly[256]; // Static so it persists after function return
  snprintf(assembly, sizeof(assembly), assembly_template, *value, *value,
           *value, *value, *value, *value, *value);
  fprintf(outputFile.file,"%s\n", assembly); // Write to the output file
}

void writeGtAssembly(const char *assembly_template, short *value,
                     FileData outputFile) {
  static char assembly[256]; // Static so it persists after function return
  snprintf(assembly, sizeof(assembly), assembly_template, *value, *value,
           *value, *value, *value, *value, *value);
  fprintf(outputFile.file,"%s\n", assembly); // Write to the output file
}

void writeLtAssembly(const char *assembly_template, short *value,
                     FileData outputFile) {
  static char assembly[256]; // Static so it persists after function return
  snprintf(assembly, sizeof(assembly), assembly_template, *value, *value,
           *value, *value, *value, *value, *value);
  fprintf(outputFile.file,"%s\n", assembly); // Write to the output file
}

void writeEndOfFileAssembly(FileData outputFile) {
  fprintf(outputFile.file,"%s\n", FILE_END); // Write to the output file
}
