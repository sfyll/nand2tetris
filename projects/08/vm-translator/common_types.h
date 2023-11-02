#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stdio.h>

typedef struct {
  FILE *file;
  char filename[256];
  char input_filename[256];
} FileData;

#endif // COMMON_TYPES_H
