#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "client_actions.h"

#ifdef __APPLE__
    extern int errno;
#else 
    extern __thread int errno;
#endif

void print_usage_message() { 
    printf("\
servant1.0 Copyright (C) 2011 Renato Mascarenhas and Rafael Regis do Prado\n\
Usage: servant <server_address>\n\
This program comes with ABSOLUTELY NO WARRANTY.\n\
This is free software, and you are welcome to redistribute it\n\
under certain conditions; refer to the GPLv3 for details.\n\
");
}

char** parse_command(char* command) {
    regex_t regex_command;
    regmatch_t pm[10];
    char **parsed_data;
    
    regcomp(&regex_command, "([a-z]+)( ([^\n]*))?", REG_EXTENDED|REG_NEWLINE);
    if (!regexec(&regex_command, command, 10, pm, 0)) {
        parsed_data = (char**)malloc(sizeof(char*)*2);
        parsed_data[0] = (char*)malloc(sizeof(char)*20);
        parsed_data[1] = (char*)malloc(sizeof(char)*100);

        strncpy(parsed_data[0], command + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        parsed_data[0][pm[1].rm_eo - pm[1].rm_so] = '\0';
        strncpy(parsed_data[1], command + pm[3].rm_so, pm[3].rm_eo - pm[3].rm_so);
        parsed_data[1][pm[3].rm_eo - pm[3].rm_so] = '\0';
    } else {
        parsed_data = NULL;
    }
    return parsed_data;    
}

void execute_on_server(CLIENT* client, char* host, char** command) {
    if(!strcmp(command[0], "download")) {
        download_file(client, host, command[1]);
    } else if(!strcmp(command[0], "upload")) {
        upload_file(client, host, command[1]);
    } else if(!strcmp(command[0], "ping")) {
        make_ping(client, host);
    } else if(!strcmp(command[0], "remove")) {
        remove_file(client, host, command[1]);
    } else if(!strcmp(command[0], "copy")) {
        copy_file(client, host, command[1]);
    } else if(!strcmp(command[0], "move")) {
        move_file(client, host, command[1]);
    } else if(!strcmp(command[0], "mkdir")) {
        make_directory(client, host, command[1]);
    } else {
		printf("servant: %s: command not found.\n", command[0]);
	}
}

int main (int argc, char* argv[])
{
    char* host;
	char username[32], password[32];
    int exit_flag = 0;
    char** parsed_command, command[100];
    CLIENT* client;
	servant_bool_t registered;
        

    //Checking number of arguments
    if (argc < 2) {
	   	print_usage_message(); 
        exit (1);
    }

    //Creating client
    host = argv[1];
    client = clnt_create(host, SERVANT, SERVANTVERSION, "tcp");
    if (client == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

	printf("Username: ");
	scanf("%[^\n]s", username);
	getchar();
	printf("Password: ");
	read_password(password);

	registered = authenticate(client, host, username, password);
	printf("\n");

    if (registered) {
		
		do {
			printf(">> ");
			scanf("%[^\n]s", command);
			getchar();

			if (strcmp(command, "exit") && strcmp(command, "quit")) {
				parsed_command = parse_command(command);
				if (parsed_command != NULL) {
					execute_on_server(client, host, parsed_command);
					free(parsed_command[0]);
					free(parsed_command[1]);        
					free(parsed_command);
				}
			} else {
				exit_flag = 1;
			}        
		} while(!exit_flag);
	} else {
		printf("Access denied for user %s on server %s.\n", username, host);
	}

    return 0;
}
