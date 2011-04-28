#ifndef _SERVANT_PROTOCOL_UTILS_H_
#define _SERVANT_PROTOCOL_UTILS_H_

#include "servant.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define STATUS_OK 0
#define STATUS_WRONG_ARGS 1
#define STATUS_NO_FILE 2

#define STATUS_MESSAGE_OK "OK"
#define STATUS_MESSAGE_NO_FILE "NOFILE"
#define STATUS_MESSAGE_WRONG_ARGS "WRONGARGS"

#define SERVANT_PROTOCOL_VERSION "1"

typedef struct {
    char version[5];
    char action[20];
    int n_params;    
    char** params;
    int content_length;
    char* content;
} request_message_t;

typedef struct {
    char version[5];
    char status[20];
    int content_length;
    char* content;
} response_message_t;

servant_request* assemble_request(request_message_t* message);

request_message_t* disassemble_request(servant_request* request);

servant_response* assemble_response(response_message_t* message);

response_message_t* disassemble_response(servant_response* request);

char* get_command_from_action(char* action);

char* get_arg_from_action(char* action);

void set_status_message(response_message_t* response, int status);

void set_protocol_version(response_message_t* response);

#endif
