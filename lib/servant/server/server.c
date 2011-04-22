#include <stdio.h>
#include <string.h>
#include "servant.h"
#include "servant_protocol_utils.h"

#ifdef __APPLE__
    extern int errno;
#else
    extern __thread int errno;
#endif

void ping(request_message_t* request, response_message_t* response) {
    response->content = (char*)malloc(sizeof(char)*1024);  
    servant_request* message = assemble_request(request);
    strcpy(response->content, message->data.chunk_val);
    response->content_length = strlen(response->content);
}

int download(request_message_t* request, response_message_t* response) {
    FILE *file;
    char path[100];
    char str_start[10];
    int start;
    char* arg;
    regex_t arg_pattern;
    regmatch_t pm[3];
    
    regcomp(&arg_pattern, "([0-9]+) ([^ ]+)", REG_EXTENDED | REG_NEWLINE);      
    
    arg = get_arg_from_action(request->action); 
    if (!regexec(&arg_pattern, arg, 3, pm, 0)) {
        //getting the the start point and the path
        strncpy(str_start, arg + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        str_start[pm[1].rm_eo - pm[1].rm_so] = '\0';
        start = atoi(str_start);
        strncpy(path, arg + pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so);
        path[pm[2].rm_eo - pm[2].rm_so] = '\0';
        
        file = fopen(path, "r");
        if (file != NULL)  {
            response->content = (char*)malloc(1024*sizeof(char));
            fseek(file, start, SEEK_SET);
            response->content_length = fread(response->content, 1, 1024, file);        
            fclose(file);
        } else {
            return 1;
        }
    }
    
    return 0;
}

servant_response *send_request_1_svc(servant_request* request, struct svc_req *rqstp) {
    servant_response* response = NULL;
    request_message_t* request_data = NULL;
    response_message_t response_data;
    char* command;
    
    request_data = disassemble_request(request);
    
    //request log 
    //printf("\nREQUEST >>>\n%s\n", request->data.chunk_val);
    //if(request_data != NULL) {
    //    printf("->version:%s\n->action:%s\n->params:%s\n->cont:%s\n<<< REQUEST\n", request_data->version, request_data->action, request_data->params[0], request_data->content);
    //}
    ////
              
    //request handling
    command = get_command_from_action(request_data->action);
    if (!strcmp(command, "DOWNLOAD")) {
        download(request_data, &response_data);
    } else if (!strcmp(command, "PING")) {
        ping(request_data, &response_data);
    }
    //upload()
    //login()
    //delete()
    //copy()
    //move()
    //mkdir()
    ////
    strcpy(response_data.status, "OK"); 
    strcpy(response_data.version, "1");
    
    return assemble_response(&response_data);
}

/*
int *upload_1_svc(put_request *request, struct svc_req *rqstp)
{
    FILE *file;
    static int  result;

    file = fopen(request->path, "a");
    if (file == NULL)  {
        result = errno;
        return &result;
    }

    fwrite(request->data.chunk_val, 1, request->bytes, file);
    fclose(file);

    result = 0;
    return &result;
}*/
