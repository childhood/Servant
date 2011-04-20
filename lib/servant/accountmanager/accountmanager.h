#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define PASSWORD_MAXLEN 256

void read_password(char* output);

int new_account(char* username, char* password);
