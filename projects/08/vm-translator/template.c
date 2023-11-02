#include <string.h>

#include "template.h"

const char *INIT;

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

const char *LABEL_TEMPLATE;
const char *IF_GOTO_TEMPLATE;
const char *GOTO_TEMPLATE;

const char *FUNCTION_CALL_TEMPLATE;
const char *FUNCTION_RETURN_TEMPLATE;

void initializeTemplates(void) {
  INIT = "@256\n"
         "D=A\n"
         "@SP\n"
         "M=D\n";

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

  PUSH_CONSTANT =
      "@%d\n"
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

  // n is initialized to Argument[0] by the test script
  LABEL_TEMPLATE = "(%s)\n";

  IF_GOTO_TEMPLATE = "@ARG\n"
                     "A=M\n"
                     "D=M\n"
                     "@SP\n"
                     "M=M-1\n" // consume that stack level
                     "@%s\n"
                     "D;JNE\n";

  GOTO_TEMPLATE = "@%s\n0;JMP\n";

  FUNCTION_CALL_TEMPLATE = // Generate a unique label for returnAddress.
                           // Typically, it is a combination of the function
                           // name and a unique counter.
      // We'll use a placeholder %s for the return address, which you can
      // replace when generating the actual assembly.
      "// push return address\n"
      "@%s\n"
      "D=A\n"
      "@SP\n"
      "A=M\n"
      "M=D\n"
      "@SP\n"
      "M=M+1\n"

      "//Push LCL\n"
      "@LCL\n"
      "D=M\n"
      "@SP\n"
      "A=M\n"
      "M=D\n"
      "@SP\n"
      "M=M+1\n"

      "//push ARG \n"
      "@ARG\n"
      "D=M\n"
      "@SP\n"
      "A=M\n"
      "M=D\n"
      "@SP\n"
      "M=M+1\n"

      "//push THIS \n"
      "@THIS\n"
      "D=M\n"
      "@SP\n"
      "A=M\n"
      "M=D\n"
      "@SP\n"
      "M=M+1\n"

      "//push THAT \n"
      "@THAT\n"
      "D=M\n"
      "@SP\n"
      "A=M\n"
      "M=D\n"
      "@SP\n"
      "M=M+1\n"

      "//ARG = SP-5-nArgs \n"
      "@SP\n"
      "D=M\n"
      "@%d\n" // Placeholder for 5 + nArgs
      "D=D-A\n"
      "@ARG\n"
      "M=D\n"

      "//LCL = SP \n"
      "@SP\n"
      "D=M\n"
      "@LCL\n"
      "M=D\n"

      "//goto f \n"
      "@%s\n" // Placeholder for function name "f"
      "0;JMP\n"

      "//Inject the return address label into the code \n"
      "(%s)\n";

  FUNCTION_RETURN_TEMPLATE = 
      "//frame = LCL \n"
      "@LCL\n"
      "D=M\n"
      "@frame\n"
      "M=D\n"

      "//retAddr = *(frame-5) \n"
      "@5\n"
      "D=D-A\n"
      "A=D\n"
      "D=M\n"
      "@retAddr\n"
      "M=D\n"

      "//*ARG = pop()\n"
      "@SP\n"
      "M=M-1\n"
      "A=M\n"
      "D=M\n"
      "@ARG\n"
      "A=M\n"
      "M=D\n"

      "//SP = ARG+1 \n"
      "@ARG\n"
      "D=M+1\n"
      "@SP\n"
      "M=D\n"

      "//THAT = *(frame-1) \n"
      "@frame\n"
      "D=M\n"
      "@1\n"
      "D=D-A\n"
      "A=D\n"
      "D=M\n"
      "@THAT\n"
      "M=D\n"

      "//THIS = *(frame-2)\n"
      "@frame\n"
      "D=M\n"
      "@2\n"
      "D=D-A\n"
      "A=D\n"
      "D=M\n"
      "@THIS\n"
      "M=D\n"

      "//ARG = *(frame-3) \n"
      "@frame\n"
      "D=M\n"
      "@3\n"
      "D=D-A\n"
      "A=D\n"
      "D=M\n"
      "@ARG\n"
      "M=D\n"

      "//LCL = *(frame-4) \n"
      "@frame\n"
      "D=M\n"
      "@4\n"
      "D=D-A\n"
      "A=D\n"
      "D=M\n"
      "@LCL\n"
      "M=D\n"

      "//goto retAddr \n"
      "@retAddr\n"
      "A=M\n"
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

void writeInitAssembly(FileData outputFile) {
    printf("INIT %s\n", INIT);
    fprintf(outputFile.file, "%s\n", INIT);
    writeFunctionCallAssembly(FUNCTION_CALL_TEMPLATE, "Sys.init", 0, 0, outputFile);
}

void writePushAssembly(const char *assembly_template, char *segment,
                       short *address, FileData outputFile) {
  static char assembly[256]; // Static so it persists after function return
  if (strcmp(segment, "local") == 0 || strcmp(segment, "argument") == 0 ||
      strcmp(segment, "this") == 0 || strcmp(segment, "that") == 0) {
    char *mapped_segment = segment_mapping(segment);
    snprintf(assembly, sizeof(assembly), assembly_template, *address,
             mapped_segment);
  } else if (strcmp(segment, "constant") == 0 || strcmp(segment, "temp") == 0) {
    snprintf(assembly, sizeof(assembly), assembly_template, *address);
  } else if (strcmp(segment, "pointer") == 0) {
    char *memory_idx = (*address == 0) ? "THIS" : "THAT";
    snprintf(assembly, sizeof(assembly), assembly_template, memory_idx);
  } else if (strcmp(segment, "static") == 0) {
    snprintf(assembly, sizeof(assembly), assembly_template, outputFile.input_filename,
             *address);
  }
  fprintf(outputFile.file, "%s\n", assembly); // Write to the output file
}

void writePopAssembly(const char *assembly_template, char *segment,
                      short *address, FileData outputFile) {
  static char assembly[256]; // Static so it persists after function return
  if (strcmp(segment, "local") == 0 || strcmp(segment, "argument") == 0 ||
      strcmp(segment, "this") == 0 || strcmp(segment, "that") == 0) {
    char *mapped_segment = segment_mapping(segment);
    snprintf(assembly, sizeof(assembly), assembly_template, *address,
             mapped_segment);
  } else if (strcmp(segment, "temp") == 0) {
    snprintf(assembly, sizeof(assembly), assembly_template, *address);
  } else if (strcmp(segment, "pointer") == 0) {
    char *memory_idx = (*address == 0) ? "THIS" : "THAT";
    snprintf(assembly, sizeof(assembly), assembly_template, memory_idx);
  } else if (strcmp(segment, "static") == 0) {
    snprintf(assembly, sizeof(assembly), assembly_template, outputFile.input_filename,
             *address);
  }
  fprintf(outputFile.file, "%s\n", assembly); // Write to the output file
}

void writeAddAssembly(const char *assembly_template, FileData outputFile) {
  fprintf(outputFile.file, "%s\n",
          assembly_template); // Write to the output file
}

void writeSubAssembly(const char *assembly_template, FileData outputFile) {
  fprintf(outputFile.file, "%s\n",
          assembly_template); // Write to the output file
}

void writeNegAssembly(const char *assembly_template, FileData outputFile) {
  fprintf(outputFile.file, "%s\n",
          assembly_template); // Write to the output file
}

void writeAndAssembly(const char *assembly_template, FileData outputFile) {
  fprintf(outputFile.file, "%s\n",
          assembly_template); // Write to the output file
}

void writeOrAssembly(const char *assembly_template, FileData outputFile) {
  fprintf(outputFile.file, "%s\n",
          assembly_template); // Write to the output file
}

void writeNotAssembly(const char *assembly_template, FileData outputFile) {
  fprintf(outputFile.file, "%s\n",
          assembly_template); // Write to the output file
}

void writeEqAssembly(const char *assembly_template, short *value,
                     FileData outputFile) {
  static char assembly[256]; // Static so it persists after function return
  snprintf(assembly, sizeof(assembly), assembly_template, *value, *value,
           *value, *value, *value, *value, *value);
  fprintf(outputFile.file, "%s\n", assembly); // Write to the output file
}

void writeGtAssembly(const char *assembly_template, short *value,
                     FileData outputFile) {
  static char assembly[256]; // Static so it persists after function return
  snprintf(assembly, sizeof(assembly), assembly_template, *value, *value,
           *value, *value, *value, *value, *value);
  fprintf(outputFile.file, "%s\n", assembly); // Write to the output file
}

void writeLtAssembly(const char *assembly_template, short *value,
                     FileData outputFile) {
  static char assembly[256]; // Static so it persists after function return
  snprintf(assembly, sizeof(assembly), assembly_template, *value, *value,
           *value, *value, *value, *value, *value);
  fprintf(outputFile.file, "%s\n", assembly); // Write to the output file
}

void writeEndOfFileAssembly(FileData outputFile) {
  fprintf(outputFile.file, "%s\n", FILE_END); // Write to the output file
}

void writeLabelAssembly(const char *assembly_template, char *label,
                        FileData outputFile) {
  static char assembly[256]; // Static so it persists after function return
  // oddly enough carraige return \n characters gets added before the
  // parenthesis which ultimately means that what comes after gets pre-prended
  // to the string presently, that's the escape parenthesis.
  char *ptr = strchr(label, '\r');
  if (ptr) {
    *ptr = '\0';
  }
  snprintf(assembly, sizeof(assembly), assembly_template, label);
  fprintf(outputFile.file, "%s\n",
          assembly); // Write to the output file
}

void writeGotoAssembly(const char *assembly_template, char *label,
                       FileData outputFile) {
  static char assembly[256]; // Static so it persists after function return
  char *ptr = strchr(label, '\r');
  if (ptr) {
    *ptr = '\0';
  }
  snprintf(assembly, sizeof(assembly), assembly_template, label);
  fprintf(outputFile.file, "%s\n",
          assembly); // Write to the output file
}

void writeIfGotoAssembly(const char *assembly_template, char *label,
                         FileData outputFile) {
  static char assembly[256]; // Static so it persists after function return
  char *ptr = strchr(label, '\r');
  if (ptr) {
    *ptr = '\0';
  }
  snprintf(assembly, sizeof(assembly), assembly_template, label);
  fprintf(outputFile.file, "%s\n",
          assembly); // Write to the output file
}

void writeFunctionLabelAssembly(const char *assembly_template, char *label,
                                int nArgs, FileData outputFile) {
  static char assembly[512]; // Static so it persists after function return
  // oddly enough carraige return \n characters gets added before the
  // parenthesis which ultimately means that what comes after gets pre-prended
  // to the string presently, that's the escape parenthesis.
  char *ptr = strchr(label, '\r');
  if (ptr) {
    *ptr = '\0';
  }
  snprintf(assembly, sizeof(assembly), assembly_template, label);
  fprintf(outputFile.file, "%s\n",
          assembly); // Write to the output filename
  // Push 0 onto the stack 'nArgs' times
  for (int i = 0; i < nArgs; i++) {
    fprintf(outputFile.file, "@0\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
  }
}

void writeFunctionReturnAssembly(const char *assembly_template,
                                 FileData outputFile) {
  fprintf(outputFile.file, "%s\n",
          assembly_template); // Write to the output file
}

void writeFunctionCallAssembly(const char *assembly_template, char *label,
                               int nArgs, int functionCallCounter,
                               FileData outputFile) {
  static char placeholder[512];
  static char assembly[512]; // Static so it persists after function return
  snprintf(placeholder, sizeof(placeholder), "%s$ret.%d",
           label, functionCallCounter);
  snprintf(assembly, sizeof(assembly), assembly_template, 
           placeholder, 5+nArgs, label, placeholder);
  fprintf(outputFile.file, "%s\n",
          assembly); // Write to the output file
}
