#include "accountmanager.h"

void read_password(char* output) { 
	static struct termios old_term, new_term;
	int i = 0, c;

	tcgetattr(STDIN_FILENO, &old_term);
	new_term = old_term;

	new_term.c_lflag &= ~(ECHO);

	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

	while((c = getchar()) != '\n' && c != EOF && i < PASSWORD_MAXLEN) {
		output[i++] = c;
	}
	output[i] = '\0';

	tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
}

int new_account(char* username, char* password) { 
	FILE *accounts;

	if ((accounts = fopen("data/accounts.servant", "a")) == NULL) {
		perror("Could not open accounts file.\n");
		exit(1);
	}

	if (fprintf(accounts, "%s:%s\n", username, password) < 0) {
		return 0;
	}

	fclose(accounts);

	return 1;
}

void parse_accounts_file(user_list** list, unsigned char behavior, char* accounts_file_name) { 
    FILE* accounts_file = fopen(accounts_file_name, "r");
	servant_user* new_user;
	string_list user;
	char* line;

	initialize(list, behavior);

	while((line = read_line(accounts_file)) != NULL) {
		user = tokenize(line, ":");
		new_user = (servant_user*) malloc(sizeof(servant_user));
		strncpy(new_user->username, user[0], sizeof(new_user->username) - 1); 
		strncpy(new_user->password, user[1], sizeof(new_user->password) - 1); 
		user[0][31] = '\0';
		user[1][31] = '\0';
		add(*list, new_user);
		free(new_user);
	} 

	fclose(accounts_file);
}

void update_accounts_file(user_list* list, char* accounts_file_name) { 
    if (list->dirty) {
		if (file_exists(accounts_file_name)) {
			remove(accounts_file_name);
		}

		FILE* accounts_file = fopen(accounts_file_name, "w");
		char line[64];
		int i;

		for (i = 0; i < list->size; i++) {
			sprintf(line, "%s:%s", list->users[i].username, list->users[i].password);
			write_line(accounts_file, line);
		}

		fclose(accounts_file);
	}
}
