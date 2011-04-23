#include "string_utils.h"

char* chomp(char* line) { 
	char* newline;

	newline = strrchr(line, '\r');
	if (newline) {
		*newline = '\0';
	}

	newline = strrchr(line, '\n');
	if (newline) {
		*newline = '\0';
	}

	return line;
}

void build_string_list(string_list* list, unsigned int size, unsigned int max_length) { 
	*list = (char**) malloc(size*sizeof(char*));

	int i;
	for (i = 0; i < size; i++) {
		(*list)[i] = (char*) malloc(max_length*sizeof(char));
	}
}

void insert(string_list list, char* string, unsigned int position) { 
    strncpy(list[position], string, 32);
	list[sizeof(string) - 1] = '\0';
}

string_list tokenize(char* string, const char* delimiters) { 
    char* position;
	string_list list;
	int i = 0;

	build_string_list(&list, 2, 32);

	position = strtok(string, delimiters);
	while (position != NULL) { 
		insert(list, position, i);
		position = strtok(NULL, delimiters);
		i++;
	}

	return list;

}
