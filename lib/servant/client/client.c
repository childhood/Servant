#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "servant.h"
#include "client.h"

#ifdef __APPLE__
    extern int errno;
#else 
    extern __thread int errno;
#endif

#define SERV_ERROR_FILE 0
#define SERV_ERROR_UPLOAD 1
#define SERV_ERROR_DOWNLOAD 2

int make_ping(CLIENT* client, char* host) {
    request_message_t data;
    servant_request* request;
    servant_response *response;
    response_message_t* response_data;
    
    data.content = (char*)malloc(sizeof(char)*3);
    strcpy(data.content, "");
    data.content_length = 0;
    strcpy(data.version, SERVANT_PROTOCOL_VERSION);
    data.params = (char**)malloc(sizeof(char*));
    
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    data.content = (char*)malloc(sizeof(char)*200);
    strcpy(data.action, "PING");
    request = assemble_request(&data);
    
    response = send_request_1(request, client);
    
    response_data = disassemble_response(response);
    
    printf("-VERSION: %s\n-STATUS: %s\n-CONTENT-LENGTH: %d\n-CONTENT: %s\n", response_data->version, response_data->status, response_data->content_length, response_data->content);
    
    return 0;
}


void print_usage_message() { 
    printf("\
			servant1.0 Copyright (C) 2011 Renato Mascarenhas and Rafael Regis do Prado\n
			Usage: %s <server_address>\n
			This program comes with ABSOLUTELY NO WARRANTY.\n
			This is free software, and you are welcome to redistribute it\n
			under certain conditions; refer to the GPLv3 for details.\n
			");
}

int download_file(CLIENT* client, char* host, char* filename) {
    
    int file_position;
    servant_response *response;
    FILE *file;
    request_message_t data;
    data.content = (char*)malloc(sizeof(char));
    response_message_t* response_data;
    
    servant_request* request;
    char str_start[20];
    
    file_position = 0;
    
    strcpy(data.content, "");
    data.content_length = 0;
    strcpy(data.version, "1");
    data.params = (char**)malloc(sizeof(char*));
    
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    char *path = (char *) malloc(200*sizeof(char));
    sprintf(path, "output/%s", filename);
    
    file = fopen(path, "w");
    
    do {
        data.content = (char*)malloc(sizeof(char)*200);
        strcpy(data.action, "DOWNLOAD ");
        
        sprintf(str_start, "%d ", file_position);
        strcat(data.action, str_start);
        strcat(data.action, filename);
        request = assemble_request(&data);
        
        response = send_request_1(request, client);
        
        response_data = disassemble_response(response);
        
        fwrite(response_data->content, 1, response_data->content_length, file);
        file_position += response_data->content_length;
        
    } while(response_data->content_length == 512);
    
    fclose(file);
    
	return 0;
}

int upload_file(CLIENT* client, char* host, char* filename) {
    servant_response *response;
    FILE *file;
    request_message_t data;
    data.content = (char*)malloc(sizeof(char));
    response_message_t* response_data;
    
    servant_request* request;
    
    strcpy(data.version, "1");
    data.params = (char**)malloc(sizeof(char*));
    
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    char *path = (char *) malloc(200*sizeof(char));
    sprintf(path, "output/%s", filename);
    
    file = fopen(path, "r");
    
    data.content = (char*)malloc(sizeof(char)*1030);
    data.content_length = fread(data.content, 1, 512, file);
    strcpy(data.action, "UPLOAD ");
    strcat(data.action, "N ");
    strcat(data.action, filename);
    
    request = assemble_request(&data);
    response = send_request_1(request, client);
    response_data = disassemble_response(response);
    
    while(data.content_length == 512) {
        
        data.content_length = fread(data.content, 1, 512, file);
        strcpy(data.action, "UPLOAD ");
        strcat(data.action, "A ");
        strcat(data.action, filename); 
        
        request = assemble_request(&data);
        response = send_request_1(request, client);
        response_data = disassemble_response(response);
    }
    
    fclose(file);
    
	return 0;
}

int remove_file(CLIENT* client, char* host, char* filename) {
    servant_response *response;
    request_message_t data;
    data.content = (char*)malloc(sizeof(char));
    response_message_t* response_data;
    
    servant_request* request;
    
    strcpy(data.content, " ");
    data.content_length = 1;
    
    strcpy(data.version, "1");
    
    data.params = (char**)malloc(sizeof(char*));
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    strcpy(data.action, "REMOVE ");
    strcat(data.action, filename);
    
    request = assemble_request(&data);
    response = send_request_1(request, client);
    response_data = disassemble_response(response);
    
    if (!strcmp(response_data->status, STATUS_MESSAGE_OK)) {
        printf("File removed\n");
    } else {
        printf("No file\n");
    }
    
    return 0;
}

int copy_file(CLIENT* client, char* host, char* filename) {
    servant_response *response;
    request_message_t data;
    data.content = (char*)malloc(sizeof(char));
    response_message_t* response_data;
    
    servant_request* request;
    
    strcpy(data.content, " ");
    data.content_length = 1;
    
    strcpy(data.version, "1");
    
    data.params = (char**)malloc(sizeof(char*));
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    strcpy(data.action, "COPY ");
    strcat(data.action, filename);
    
    request = assemble_request(&data);
    response = send_request_1(request, client);
    response_data = disassemble_response(response);
    
    if (!strcmp(response_data->status, STATUS_MESSAGE_OK)) {
        printf("File copied\n");
    } else {
        printf("No file copied\n");
    }
    
    return 0;
}

int move_file(CLIENT* client, char* host, char* filename) {
    servant_response *response;
    request_message_t data;
    data.content = (char*)malloc(sizeof(char));
    response_message_t* response_data;
    
    servant_request* request;
    
    strcpy(data.content, " ");
    data.content_length = 1;
    
    strcpy(data.version, "1");
    
    data.params = (char**)malloc(sizeof(char*));
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    strcpy(data.action, "MOVE ");
    strcat(data.action, filename);
    
    request = assemble_request(&data);
    response = send_request_1(request, client);
    response_data = disassemble_response(response);
    
    if (!strcmp(response_data->status, STATUS_MESSAGE_OK)) {
        printf("File moved\n");
    } else {
        printf("No file moved\n");
    }
    
    return 0;
}

int make_directory(CLIENT* client, char* host, char* filename) {
    servant_response *response;
    request_message_t data;
    data.content = (char*)malloc(sizeof(char));
    response_message_t* response_data;
    
    servant_request* request;
    
    strcpy(data.content, " ");
    data.content_length = 1;
    
    strcpy(data.version, "1");
    
    data.params = (char**)malloc(sizeof(char*));
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    strcpy(data.action, "MKDIR ");
    strcat(data.action, filename);
    
    request = assemble_request(&data);
    response = send_request_1(request, client);
    response_data = disassemble_response(response);
    
    if (!strcmp(response_data->status, STATUS_MESSAGE_OK)) {
        printf("Directory created\n");
    } else {
        printf("No directory created\n");
    }
    
    return 0;
}

servant_bool_t authenticate(CLIENT* client, char* host, char* username, char* password) { 
    request_message_t data;
	servant_request* request;
	servant_response* response;
	response_message_t* response_data;

	data.content = (char*)malloc(sizeof(char)*3);
    strcpy(data.content, "");
    data.content_length = 0;
    strcpy(data.version, SERVANT_PROTOCOL_VERSION);
    data.params = (char**)malloc(sizeof(char*));
    
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    data.content = (char*)malloc(sizeof(char)*200);
    strcpy(data.action, "LOGIN");
    request = assemble_request(&data);
    
    response = send_request_1(request, client);

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
	scanf("%s", username);
	printf("Password :");
	read_password(password);

	registered = authenticate(client, host, username, password);

    do {
        printf(">> ");
        scanf("%[^\n]s", command);
        getchar();

        if (strcmp(command, "exit")) {
            parsed_command = parse_command(command);
            if (parsed_command != NULL) {
                //execute_on_client(parsed_command);
                execute_on_server(client, host, parsed_command);
                free(parsed_command[0]);
                free(parsed_command[1]);        
                free(parsed_command);
            }
        } else {
            exit_flag = 1;
        }        
    } while(!exit_flag);

    return 0;
}
