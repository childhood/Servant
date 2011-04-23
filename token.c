#include <stdio.h>
#include <string.h>

#include "string_utils.h"
#include "file_utils.h"

#define TEST_FILE "test_file"

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

int main(int argc, char* argv[]) { 
    FILE* f = fopen(TEST_FILE, "r");
	char* line;

	string_list user;
	//build_string_list(&users, 2, 32);

	while ((line = read_line(f)) != NULL) {
		user = tokenize(line, ":");
		printf("Username: %s\nPassword: %s\n\n", user[0], user[1]);
	}

	fclose(f);

	return 0;
}
