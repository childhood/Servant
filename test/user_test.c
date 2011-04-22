#include <stdio.h>
#include "user.h"

void print_if_has_user(user_list* list, servant_user user) { 
	if (has_user(list, user.username)) {
		printf("User %s is registred.\n");
	} else {
		printf("User %s is not registered.\n");
	}
}

void print_users(user_list* list) { 
	int i;

	printf("Registered users:\n");
	for (i = 0; i < list->size; i++) {
		printf("%s:%s\n", list->users[i].username, list->users[i].password);
	}
}

int main(int argc, char* argv[]) { 
	user_list* servant_users;

	printf("Testing user management. Using ORDERED_LIST behavior.\n");
	initialize(&servant_users, ORDERED_LIST);
	servant_user u1, u2, u3, u4, u5, u6;

	strncpy(u1.username, "user1", sizeof(u1.username) -1);
	u1.username[31] = '\0';
	strncpy(u1.password, "password1", sizeof(u1.password) - 1);
	u1.password[31] = '\0';

	strncpy(u2.username, "user2", sizeof(u2.username) -1);
	u2.username[31] = '\0';
	strncpy(u2.password, "password2", sizeof(u2.password) - 1);
	u2.password[31] = '\0';

	strncpy(u3.username, "user3", sizeof(u3.username) -1);
	u3.username[31] = '\0';
	strncpy(u3.password, "password3", sizeof(u3.password) - 1);
	u3.password[31] = '\0';

	strncpy(u4.username, "user4", sizeof(u4.username) -1);
	u4.username[31] = '\0';
	strncpy(u4.password, "password4", sizeof(u4.password) - 1);
	u4.password[31] = '\0';

	strncpy(u5.username, "user5", sizeof(u5.username) -1);
	u5.username[31] = '\0';
	strncpy(u5.password, "password5", sizeof(u5.password) - 1);
	u5.password[31] = '\0';

	strncpy(u6.username, "user6", sizeof(u6.username) -1);
	u6.username[31] = '\0';
	strncpy(u6.password, "password6", sizeof(u6.password) - 1);
	u6.password[31] = '\0';


	add(servant_users, u4);
	add(servant_users, u2);
	add(servant_users, u3);
	add(servant_users, u1);
	add(servant_users, u5); 

	print_users(servant_users);

	print_if_has_user(servant_users, u1);
	print_if_has_user(servant_users, u3);
	print_if_has_user(servant_users, u5);
	print_if_has_user(servant_users, u6);

	printf("\nRemoving users 1, 3 and 5...\n");
	remove_user(servant_users, u1);
	remove_user(servant_users, u5);
	remove_user(servant_users, u3);

	print_users(servant_users);

	free(servant_users); 

	printf("\n\nNow using UNORDERED_LIST behavior.\n");
	initialize(&servant_users, UNORDERED_LIST);

	strncpy(u1.username, "user1", sizeof(u1.username) -1);
	u1.username[31] = '\0';
	strncpy(u1.password, "password1", sizeof(u1.password) - 1);
	u1.password[31] = '\0';

	strncpy(u2.username, "user2", sizeof(u2.username) -1);
	u2.username[31] = '\0';
	strncpy(u2.password, "password2", sizeof(u2.password) - 1);
	u2.password[31] = '\0';

	strncpy(u3.username, "user3", sizeof(u3.username) -1);
	u3.username[31] = '\0';
	strncpy(u3.password, "password3", sizeof(u3.password) - 1);
	u3.password[31] = '\0';

	strncpy(u4.username, "user4", sizeof(u4.username) -1);
	u4.username[31] = '\0';
	strncpy(u4.password, "password4", sizeof(u4.password) - 1);
	u4.password[31] = '\0';

	strncpy(u5.username, "user5", sizeof(u5.username) -1);
	u5.username[31] = '\0';
	strncpy(u5.password, "password5", sizeof(u5.password) - 1);
	u5.password[31] = '\0';

	strncpy(u6.username, "user6", sizeof(u6.username) -1);
	u6.username[31] = '\0';
	strncpy(u6.password, "password6", sizeof(u6.password) - 1);
	u6.password[31] = '\0';


	add(servant_users, u4);
	add(servant_users, u2);
	add(servant_users, u3);
	add(servant_users, u1);
	add(servant_users, u5); 

	print_users(servant_users);

	print_if_has_user(servant_users, u1);
	print_if_has_user(servant_users, u3);
	print_if_has_user(servant_users, u5);
	print_if_has_user(servant_users, u6);

	printf("\nRemoving users 1, 3 and 5...\n");
	remove_user(servant_users, u1);
	remove_user(servant_users, u5);
	remove_user(servant_users, u3);

	print_users(servant_users);

	free(servant_users); 


	printf("\n\nTests are done. No errors.\n");

	return 0;
}
