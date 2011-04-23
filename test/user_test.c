#include <stdio.h>
#include "accountmanager.h"

#define TEST_FILE "test/user_test"

void print_if_has_user(user_list* list, servant_user user) { 
	if (has_user(list, user.username)) {
		printf("User %s is registered.\n");
	} else {
		printf("User %s is not registered.\n");
	}
}

void print_if_has_user_by_username(user_list* list, char* username) { 
	if (has_user(list, username)) {
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
	FILE* test_file = fopen(TEST_FILE, "w");

	servant_user* u1 = (servant_user*) malloc(sizeof(servant_user));
	servant_user* u2 = (servant_user*) malloc(sizeof(servant_user));
	servant_user* u3 = (servant_user*) malloc(sizeof(servant_user));
	servant_user* u4 = (servant_user*) malloc(sizeof(servant_user));
	servant_user* u5 = (servant_user*) malloc(sizeof(servant_user));

	printf("Testing user management. Using ORDERED_LIST behavior.\n");

	write_line(test_file, "user0:3d517fe6ebab7b8cfcf98db6259c8a59");
	write_line(test_file, "user1:24c9e15e52afc47c225b757e7bee1f9d");
	write_line(test_file, "user8:7e58d63b60197ceb55a1c487989a3720");
	write_line(test_file, "user3:92877af70a45fd6a2ed7fe81e1236b78");
	write_line(test_file, "user2:3f02ebe3d7929b091e3d8ccfde2f3bc6");
	write_line(test_file, "user5:0a791842f52a0acfbb3a783378c066b8");

	fclose(test_file);

	printf("Parsing test accounts file...\t");
	parse_accounts_file(&servant_users, ORDERED_LIST, TEST_FILE);
	printf("done.\n");

	print_users(servant_users);
	
	printf("Building some users...\n");

	strncpy(u1->username, "user4", sizeof(u1->username) -1);
	u1->username[31] = '\0';
	strncpy(u1->password, "7668f673d5669995175ef91b5d171945", sizeof(u1->password) - 1);
	u1->password[31] = '\0';

	strncpy(u2->username, "user6", sizeof(u2->username) -1);
	u2->username[31] = '\0';
	strncpy(u2->password, "affec3b64cf90492377a8114c86fc093", sizeof(u2->password) - 1);
	u2->password[31] = '\0';

	strncpy(u3->username, "user10", sizeof(u3->username) -1);
	u3->username[31] = '\0';
	strncpy(u3->password, "990d67a9f94696b1abe2dccf06900322", sizeof(u3->password) - 1);
	u3->password[31] = '\0';

	strncpy(u4->username, "user7", sizeof(u4->username) -1);
	u4->username[31] = '\0';
	strncpy(u4->password, "990d67a9f94696b1abe2dccf06900322", sizeof(u4->password) - 1);
	u4->password[31] = '\0';

	strncpy(u5->username, "user9", sizeof(u5->username) -1);
	u5->username[31] = '\0';
	strncpy(u5->password, "8808a13b854c2563da1a5f6cb2130868", sizeof(u5->password) - 1);
	u5->password[31] = '\0';

	printf("Adding them...\n");

	add(servant_users, u1);
	add(servant_users, u2);
	add(servant_users, u5); 

	print_users(servant_users);

	print_if_has_user(servant_users, *u1);
	print_if_has_user(servant_users, *u2);
	print_if_has_user(servant_users, *u3);
	print_if_has_user(servant_users, *u4);
	print_if_has_user(servant_users, *u5);
	print_if_has_user_by_username(servant_users, "user0");
	print_if_has_user_by_username(servant_users, "user1");
	print_if_has_user_by_username(servant_users, "user8");
	print_if_has_user_by_username(servant_users, "user2");
	print_if_has_user_by_username(servant_users, "user3");
	print_if_has_user_by_username(servant_users, "user5");

	printf("\nRemoving some users...\n");
	remove_user(servant_users, u1);
	remove_user(servant_users, u5);
	remove_user(servant_users, u3);

	print_users(servant_users);

	printf("\n\nUpdating accounts file...\t");
	update_accounts_file(servant_users, TEST_FILE);
	printf("done.\n");

	free(servant_users); 
	remove(TEST_FILE);

	printf("\n\nNow using UNORDERED_LIST behavior.\n");

	test_file = fopen(TEST_FILE, "w");

	write_line(test_file, "user0:3d517fe6ebab7b8cfcf98db6259c8a59");
	write_line(test_file, "user1:24c9e15e52afc47c225b757e7bee1f9d");
	write_line(test_file, "user8:7e58d63b60197ceb55a1c487989a3720");
	write_line(test_file, "user3:92877af70a45fd6a2ed7fe81e1236b78");
	write_line(test_file, "user2:3f02ebe3d7929b091e3d8ccfde2f3bc6");
	write_line(test_file, "user5:0a791842f52a0acfbb3a783378c066b8");

	fclose(test_file);
	
	printf("Parsing test accounts file...\t");
	parse_accounts_file(&servant_users, UNORDERED_LIST, TEST_FILE);
	printf("done.\n");
	
	printf("Building some users...\n");

	strncpy(u1->username, "user4", sizeof(u1->username) -1);
	u1->username[31] = '\0';
	strncpy(u1->password, "7668f673d5669995175ef91b5d171945", sizeof(u1->password) - 1);
	u1->password[31] = '\0';

	strncpy(u2->username, "user6", sizeof(u2->username) -1);
	u2->username[31] = '\0';
	strncpy(u2->password, "affec3b64cf90492377a8114c86fc093", sizeof(u2->password) - 1);
	u2->password[31] = '\0';

	strncpy(u3->username, "user10", sizeof(u3->username) -1);
	u3->username[31] = '\0';
	strncpy(u3->password, "990d67a9f94696b1abe2dccf06900322", sizeof(u3->password) - 1);
	u3->password[31] = '\0';

	strncpy(u4->username, "user7", sizeof(u4->username) -1);
	u4->username[31] = '\0';
	strncpy(u4->password, "990d67a9f94696b1abe2dccf06900322", sizeof(u4->password) - 1);
	u4->password[31] = '\0';

	strncpy(u5->username, "user9", sizeof(u5->username) -1);
	u5->username[31] = '\0';
	strncpy(u5->password, "8808a13b854c2563da1a5f6cb2130868", sizeof(u5->password) - 1);
	u5->password[31] = '\0';

	printf("Adding them...\n");

	add(servant_users, u1);
	add(servant_users, u2);
	add(servant_users, u5); 

	print_users(servant_users);

	print_if_has_user(servant_users, *u1);
	print_if_has_user(servant_users, *u2);
	print_if_has_user(servant_users, *u3);
	print_if_has_user(servant_users, *u4);
	print_if_has_user(servant_users, *u5);
	print_if_has_user_by_username(servant_users, "user0");
	print_if_has_user_by_username(servant_users, "user1");
	print_if_has_user_by_username(servant_users, "user8");
	print_if_has_user_by_username(servant_users, "user2");
	print_if_has_user_by_username(servant_users, "user3");
	print_if_has_user_by_username(servant_users, "user5");

	printf("\nRemoving some users...\n");
	remove_user(servant_users, u1);
	remove_user(servant_users, u5);
	remove_user(servant_users, u3);

	print_users(servant_users); 

	printf("\n\nUpdating accounts file...\t");
	update_accounts_file(servant_users, TEST_FILE);
	printf("done.\n");

	free(servant_users); 
	remove(TEST_FILE); 


	printf("\n\nTests are done. No errors.\n");

	return 0;
}
