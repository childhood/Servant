#include "servant_protocol_utils.h"

servant_request* assemble_request(request_message_t* data) {
    servant_request* request;
    char content_length[10];
    char* message = (char*)malloc(sizeof(char)*CHUNK_LENGTH);
    int i;

    sprintf(message, "SERVANT/%s|Action:%s|Params:", data->version, data->action);
    for(i = 0; i < data->n_params - 1; i++) {
        strcat(message, data->params[i]);
        strcat(message, ",");
    }

    strcat(message, data->params[data->n_params - 1]);
    strcat(message, "|Content-length:");
    sprintf(content_length, "%d", data->content_length);
    strcat(message, content_length);
    strcat(message, "|Content:");
    strcat(message, data->content);


    request = (servant_request*)malloc(sizeof(servant_request)); 

    request->data.chunk_val = message;
    request->data.chunk_len = strlen(message); 

    return request;
}

request_message_t* disassemble_request(servant_request* request) {
    request_message_t* message = NULL;
    regex_t request_pattern;
    regmatch_t pm[10];
    char content_length[5];

    
    regcomp(&request_pattern, "^SERVANT/([0-9]+)[|]Action:(.+)[|]Params:([a-zA-Z0-9]*)[|]Content-length:([0-9]+)[|]Content:(.*)$", REG_EXTENDED);
    
    
    if (!regexec(&request_pattern, request->data.chunk_val, 10, pm, 0)) {
        message = (request_message_t*) malloc(sizeof(request_message_t));

        strncpy(message->version, request->data.chunk_val + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        message->version[pm[1].rm_eo - pm[1].rm_so] = '\0';

        strncpy(message->action, request->data.chunk_val + pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so);
        message->action[pm[2].rm_eo - pm[2].rm_so] = '\0';

        message->params = (char**) malloc(sizeof(char*));
        message->params[0] = (char*) malloc(50*sizeof(char));
        strncpy(message->params[0], request->data.chunk_val + pm[3].rm_so, pm[3].rm_eo - pm[3].rm_so);

        message->params[0][pm[3].rm_eo - pm[3].rm_so] = '\0';

        message->n_params = 1;

        strncpy(content_length, request->data.chunk_val + pm[4].rm_so, pm[4].rm_eo - pm[4].rm_so);
        content_length[pm[4].rm_eo - pm[4].rm_so] = '\0';
        message->content_length = atoi(content_length);

        message->content = (char*) malloc(sizeof(char)*1050);
        strncpy(message->content, request->data.chunk_val + pm[5].rm_so, pm[5].rm_eo - pm[5].rm_so);
        message->content[pm[5].rm_eo - pm[5].rm_so] = '\0';   
    }
    
    return message;
}

servant_response* assemble_response(response_message_t* data) {
    servant_response* response;
    char* message = (char*)malloc(sizeof(char)*CHUNK_LENGTH);
    
    sprintf(message, "SERVANT/%s|Status:%s|Content-length:%d|Content:%s", data->version, data->status, data->content_length, data->content);
    
    response = (servant_response*)malloc(sizeof(servant_response)); 
    
    response->data.chunk_val = message;
    response->data.chunk_len = strlen(message); 
    
    return response;    
}

response_message_t* disassemble_response(servant_response* response) {
    response_message_t* message = NULL;
    regex_t response_pattern;
    regmatch_t pm[10];
    char content_length[5];


    
    regcomp(&response_pattern, "^SERVANT/([0-9]+)[|]Status:([a-zA-Z0-9]+)[|]Content-length:([0-9]+)[|]Content:(.*)$", REG_EXTENDED);//| REG_NEWLINE);
    
    
    if (!regexec(&response_pattern, response->data.chunk_val, 5, pm, 0)) {
        message = (response_message_t*) malloc(sizeof(response_message_t));
        
        strncpy(message->version, response->data.chunk_val + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        message->version[pm[1].rm_eo - pm[1].rm_so] = '\0';
        
        strncpy(message->status, response->data.chunk_val + pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so);
        message->status[pm[2].rm_eo - pm[2].rm_so] = '\0';
        
        strncpy(content_length, response->data.chunk_val + pm[3].rm_so, pm[3].rm_eo - pm[3].rm_so);
        content_length[pm[3].rm_eo - pm[3].rm_so] = '\0';
        message->content_length = atoi(content_length);
        
        message->content = (char*) malloc(sizeof(char)*1050);
        strncpy(message->content, response->data.chunk_val + pm[4].rm_so, pm[4].rm_eo - pm[4].rm_so);
        message->content[pm[4].rm_eo - pm[4].rm_so] = '\0';
    }

    return message;
}

char* get_command_from_action(char* action) {
    char* command = NULL;
    regex_t action_pattern;
    regmatch_t pm[3];
    
    regcomp(&action_pattern, "([A-Z]+)(.*)?", REG_EXTENDED | REG_NEWLINE);
    
    if (!regexec(&action_pattern, action, 3, pm, 0)) {
        command = (char*) malloc(20*sizeof(char));
        strncpy(command, action + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        command[pm[1].rm_eo - pm[1].rm_so] = '\0';
    }
    
    return command;
}

char* get_arg_from_action(char* action) {
    char* arg = NULL;
    regex_t action_pattern;
    regmatch_t pm[3];   
    
    regcomp(&action_pattern, "[A-Z]+(.*)?", REG_EXTENDED | REG_NEWLINE);
    
    if (!regexec(&action_pattern, action, 3, pm, 0)) {
        arg = (char*) malloc(200*sizeof(char));
        strncpy(arg, action + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        arg[pm[1].rm_eo - pm[1].rm_so] = '\0';
    }
    
    return arg;
}