#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "user.h"
#include "file_utils.h"

#define PASSWORD_MAXLEN 256

void read_password(char* output);

int new_account(char* username, char* password);

void parse_accounts_file(user_list** list, unsigned char behavior, char* accounts_file_name);

void update_accounts_file(user_list* list, char* accounts_file_name);
