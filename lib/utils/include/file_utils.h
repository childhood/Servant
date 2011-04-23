#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_utils.h"

#define FILE_EXISTS 1
#define FILE_NOT_FOUND 0

int file_exists(char* filename);

char* read_line(FILE* file);

void write_line(FILE* f, const char* string);
