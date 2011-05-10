#include <string.h>
#include <stdlib.h>

#include "boolean.h"

#define ORDERED_LIST 0
#define UNORDERED_LIST 1
#define LEFT 2
#define RIGHT 3

#define USER_NOT_FOUND -1

#define MAX_SERVANT_USERS 500

typedef struct _servant_user {
    char username[33];
	char password[33];
} servant_user;

typedef struct _user_list { 
	unsigned char behavior;
	servant_bool_t dirty; // different from accounts file?
	unsigned int size;
	servant_user users[MAX_SERVANT_USERS];
} user_list;

void initialize(user_list** list, unsigned char behavior);

servant_bool_t add(user_list* list, servant_user* new_user);

servant_bool_t remove_user(user_list* list, servant_user* user);

servant_bool_t remove_by_username(user_list* list, char* username);

servant_bool_t registered(user_list* list, char* username, char* password);

servant_user get_user(user_list* list, char* username, char* password);

servant_bool_t has_user(user_list* list, char* username);
