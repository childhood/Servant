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
    char username[32];
	char password[32];
} servant_user;

/* typedef _linked_list { 
    servant_user user;
	struct _linked_list* next;
} linked_list; */

typedef struct _user_list { 
	unsigned char behavior;
	unsigned int size;
	servant_user users[MAX_SERVANT_USERS];
} user_list;

void initialize(user_list** list, unsigned char behavior);

bool_t add(user_list* list, servant_user new_user);

bool_t remove_user(user_list* list, servant_user usr);

bool_t remove_by_username(user_list* list, char* username);

bool_t has_user(user_list* list, char* username);
