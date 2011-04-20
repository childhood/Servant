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
