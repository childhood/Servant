#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "servant.h"

#ifdef __APPLE__
    extern int errno;
#else 
    extern __thread int errno;
#endif

void download_file(char *host, char *filename) {
    CLIENT *client;
    int file_position;
    //char *path;
    get_response *result;
    get_request request;
    FILE *file;

    request.path = filename;
    char *path = (char *) malloc(FILENAME_LENGTH*sizeof(char));
    sprintf(path, "output/%s", filename);
    request.start = 0;

    client = clnt_create(host, SERVANT, SERVANTVERSION, "tcp");
    if (client == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    file = fopen(path, "w");
    file_position = 0;

    do {
        request.start = file_position;
        result = download_1(&request, client);

        if (result == NULL) {
            clnt_perror(client, host);
            exit(1);
        }

        if (result->errno != 0) {
            errno = result->errno;
            perror("Could not retrieve file...\n");
            exit(1);
        }

        fwrite(result->get_response_u.chunk.data.chunk_val, 1, result->get_response_u.chunk.data.chunk_len, file);
        file_position += result->get_response_u.chunk.bytes;
        
    } while(result->get_response_u.chunk.bytes == CHUNK_LENGTH);

    fclose(file);
}

void upload_file(char *host, char *filename) {
    CLIENT *client;
    char data[1100];
    int read_bytes;
    int *result;
    put_request request;
    FILE *file;
    char *path = (char *) malloc(FILENAME_LENGTH*sizeof(char));

    client = clnt_create(host, SERVANT, SERVANTVERSION, "tcp");
    if (client == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
    sprintf(path, "output/%s", filename);
    file = fopen(path, "r");

    if (file == NULL) {
        return;
    }
    request.path = filename;

    do {  
        read_bytes = fread(data, 1, CHUNK_LENGTH, file);

        request.bytes = read_bytes;
        request.data.chunk_val = data;
        request.data.chunk_len = read_bytes;        
        
        result = upload_1(&request, client);

        if (result == NULL) {
            clnt_perror(client, host);
            exit(1);
        }
        
        if (*result != 0) {
            errno = *result;
            perror(filename);
            exit(1);
        }
    } while(read_bytes == CHUNK_LENGTH);

    fclose(file);
}

char **parse_command(char* command) {
    regex_t regex_command;
    regmatch_t pm[10];
    char **parsed_data;
     
    regcomp(&regex_command, "([a-z]+) ([^\n]*)?", REG_EXTENDED);
    if (!regexec(&regex_command, command, 10, pm, 0)) {
        parsed_data = (char**)malloc(sizeof(char*)*2);
        parsed_data[0] = (char*)malloc(sizeof(char)*20);
        parsed_data[1] = (char*)malloc(sizeof(char)*100);

        strncpy(parsed_data[0], command + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        parsed_data[1][pm[1].rm_eo - pm[1].rm_so] = '\0';
        strncpy(parsed_data[1], command + pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so);
        parsed_data[1][pm[2].rm_eo - pm[2].rm_so] = '\0';
    } else {
        parsed_data = NULL;
    }
    return parsed_data;    
}


void execute_on_server(char* host, char** command) {
    if(!strcmp(command[0], "get")) {
        download_file(host, command[1]);
    } else if(!strcmp(command[0], "put")) {
        upload_file(host, command[1]);
    }
}


int main (int argc, char *argv[])
{
    char *host;
    int exit_flag = 0;
    char **parsed_command, command[100];

    if (argc < 2) {
	printf ("usage: %s server_host\n", argv[0]);
        exit (1);
    }
    host = argv[1];

    //printf(">> ");
    //scanf("%s", filename);

    do {
        printf(">> ");
        scanf("%[^\n]s", command);
        getchar();

        if (strcmp(command, "exit")) {
            parsed_command = parse_command(command);
            
            if (parsed_command != NULL) {
                //executar comando (execute_on_client(parsed_command));          
                execute_on_server(host, parsed_command);
        
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
