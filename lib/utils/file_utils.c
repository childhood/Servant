#include <stdio.h>
#include "file_utils.h"

int file_exists(char* filename) { 
	FILE* f = fopen(filename, "r");

	if (f != NULL) {
		fclose(f);
		return FILE_EXISTS;
	}

	return FILE_NOT_FOUND;
}

char* read_line(FILE* file) { 
    char* line = (char*) malloc(255*sizeof(char));
	char* read_content;

	read_content = fgets(line, 255*sizeof(char), file);

	if (read_content == NULL) {
		return NULL;
	}

	return chomp(line);
}

void write_line(FILE* f, const char* string) { 
    fprintf(f, "%s\n", string);
}
