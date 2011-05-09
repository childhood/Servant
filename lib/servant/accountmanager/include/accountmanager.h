#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "user.h"
#include "file_utils.h"

#define PASSWORD_MAXLEN 256

#define SERVANT_ACCOUNTS_FILE "data/accounts.servant"

user_list* servant_users;

void read_password(char* output);

int new_account(char* username, char* password);

void parse_accounts_file(user_list** list, unsigned char behavior);

void update_accounts_file(user_list* list);
