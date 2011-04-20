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
