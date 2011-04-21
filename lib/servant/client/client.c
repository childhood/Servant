#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "servant.h"
#include "servant_protocol_utils.h"

#ifdef __APPLE__
    extern int errno;
#else 
    extern __thread int errno;
#endif

#define SERV_ERROR_FILE 0
#define SERV_ERROR_UPLOAD 1
#define SERV_ERROR_DOWNLOAD 2
    

int download_file(CLIENT *client, char *host, char* filename) {
    
    int file_position;
    servant_response *response;
    FILE *file;
    request_message_t data;
    response_message_t* response_data;
    
    servant_request* request;
    char str_start[20];

    file_position = 0;

    strcpy(data.version, "1");
    strcpy(data.action, "DOWNLOAD"); 
    data.params = (char**)malloc(sizeof(char*));
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;

    char *path = (char *) malloc(200*sizeof(char));
    sprintf(path, "output/%s", filename);

    file = fopen(path, "w");

    //do {
        data.content = (char*)malloc(sizeof(char)*200);
        strcpy(data.content, "Filename:\"");
        strcat(data.content, filename);
        strcat(data.content, "\"Start:");
        sprintf(str_start, "%d", file_position);
        strcat(data.content, str_start);
        data.content_length = strlen(data.content);

        request = assemble_request(&data);

        printf("%s\n", request->data.chunk_val);
        
        
        response = send_request_1(request, client);
        
        if (response== NULL) {
            clnt_perror(client, host);
            //exit(1);
            return SERV_ERROR_DOWNLOAD;
        }

        //if (result->errno != 0) {
        //    errno = result->errno;
            //perror("Could not retrieve file...\n");
            //exit(1);
        //    return SERV_ERROR_DOWNLOAD;
        //}*/

        response_data = disassemble_response(response);
    
        printf("%s\n%s\n%d\n%s\n", response_data->version, response_data->status, response_data->content_length, response_data->content);

        //fwrite(response_data->content, 1, response_data->content_length, file);
        //file_position += response_data->content_length;
        
    //} while(response_data->content_length == CHUNK_LENGTH);

    fclose(file);
}

/*
int upload_file(CLIENT* client, char *host, char *filename) {
    char data[1100];
    int read_bytes;
    int* result;
    
    put_request request;
    FILE* file;
    char* path;
    path = (char*)malloc(FILENAME_LENGTH*sizeof(char));    
    sprintf(path, "output/%s", filename);
    file = fopen(path, "r");

    if (file == NULL) {
        return SERV_ERROR_FILE;
    }
    request.path = filename;

    do {  
        read_bytes = fread(data, 1, CHUNK_LENGTH, file);

        request.bytes = read_bytes;
        request.data.chunk_val = data;
        request.data.chunk_len = read_bytes;        
        
        result = upload_1(&request, client);

        if (result == NULL) {
            //clnt_perror(client, host);
            return SERV_ERROR_UPLOAD;
        }
        
        if (*result != 0) {
            errno = *result;
            //perror(filename);
            return SERV_ERROR_UPLOAD;
        }
    } while(read_bytes == CHUNK_LENGTH);

    free(path);
    fclose(file);
}
*/
char** parse_command(char* command) {
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


void execute_on_server(CLIENT* client, char* host, char** command) {
    if(!strcmp(command[0], "get")) {
        download_file(client, host, command[1]);
    } else if(!strcmp(command[0], "put")) {
        //upload_file(client, host, command[1]);
    }
}

int main (int argc, char* argv[])
{
    char* host;
    int exit_flag = 0;
    char** parsed_command, command[100];
    CLIENT* client;
        

    //Checking number of arguments
    if (argc < 2) {
	printf ("usage: %s server_host\n", argv[0]);
        exit (1);
    }

    //Creating client
    host = argv[1];
    client = clnt_create(host, SERVANT, SERVANTVERSION, "tcp");
    if (client == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

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
