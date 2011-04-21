#include "servant_protocol_utils.h"

servant_request* assemble_request(request_message_t* data) {
    servant_request* request;
    char content_length[10];
    char* message = (char*)malloc(sizeof(char)*CHUNK_LENGTH);
    int i;

    sprintf(message, "SERVANT/%s\nAction:%s\nParams:", data->version, data->action);
    for(i = 0; i < data->n_params - 1; i++) {
        strcat(message, data->params[i]);
        strcat(message, ",");
    }

    strcat(message, data->params[data->n_params - 1]);
    strcat(message, "\nContent-length:");
    sprintf(content_length, "%d", data->content_length);
    strcat(message, content_length);
    strcat(message, "\nContent:");
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

    
    regcomp(&request_pattern, "SERVANT/([0-9]+)\nAction:([a-zA-Z0-9]+)\nParams:([a-zA-Z0-9,.]*)\nContent-length:([0-9]+)\nContent:(.*)", REG_EXTENDED);
    
    if (!regexec(&request_pattern, request->data.chunk_val, 5, pm, 0)) {
        request_message_t* message = (request_message_t*) malloc(sizeof(request_message_t));

        strncpy(message->version, request->data.chunk_val + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        message->version[pm[1].rm_eo - pm[1].rm_so] = '\0';

        strncpy(message->action, request->data.chunk_val + pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so);
        message->action[pm[2].rm_eo - pm[2].rm_so] = '\0';
        
        message->params = (char**) malloc(sizeof(char*));
        message->params[0] = (char*) malloc(50*sizeof(char));
        strncpy(message->params[0], request->data.chunk_val + pm[3].rm_so, pm[3].rm_eo - pm[3].rm_so);
        message->content[pm[3].rm_eo - pm[3].rm_so] = '\0';
        
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
    
    sprintf(message, "SERVANT/%s\nStatus:%s\nContent-length:%d\nContent:%s", data->version, data->status, data->content_length, data->content);
    
    response = (servant_response*)malloc(sizeof(servant_response)); 
    
    response->data.chunk_val = message;
    response->data.chunk_len = strlen(message); 
    
    return response;    
}

response_message_t* disassemble_response(servant_response* request) {
    response_message_t* message = NULL;
    regex_t request_pattern;
    regmatch_t pm[10];
    
    char content_length[5];
    
    
    regcomp(&request_pattern, "SERVANT/([0-9]+)\nStatus:([a-zA-Z0-9]+)\nContent-length:([0-9]+)\nContent:(.*)", REG_EXTENDED);
    
    if (!regexec(&request_pattern, request->data.chunk_val, 5, pm, 0)) {
        response_message_t* message = (response_message_t*) malloc(sizeof(response_message_t));
        
        strncpy(message->version, request->data.chunk_val + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        message->version[pm[1].rm_eo - pm[1].rm_so] = '\0';
        
        strncpy(message->status, request->data.chunk_val + pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so);
        message->status[pm[2].rm_eo - pm[2].rm_so] = '\0';
        
        strncpy(content_length, request->data.chunk_val + pm[3].rm_so, pm[3].rm_eo - pm[3].rm_so);
        content_length[pm[3].rm_eo - pm[3].rm_so] = '\0';
        message->content_length = atoi(content_length);
        
        message->content = (char*) malloc(sizeof(char)*1050);
        strncpy(message->content, request->data.chunk_val + pm[4].rm_so, pm[4].rm_eo - pm[4].rm_so);
        message->content[pm[4].rm_eo - pm[4].rm_so] = '\0';        
    }

    return message;
}
