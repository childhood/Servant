#include <stdlib.h>
#include <string.h>

typedef char** string_list;

char* chomp(char* line);

void build_string_list(string_list* list, unsigned int size, unsigned int max_length);

void insert(string_list list, char* string, unsigned int position);

string_list tokenize(char* string, const char* delimiters);
