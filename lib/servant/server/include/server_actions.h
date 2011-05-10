#ifndef _SERVANT_PROTOCOL_ACTIONS_H_
#define _SERVANT_PROTOCOL_ACTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <sys/stat.h>
#include <ftw.h>

#include "servant_protocol_utils.h"
#include "accountmanager.h"
#include "servant.h"

void ping(request_message_t* request, response_message_t** response);

void download(request_message_t* request, response_message_t** response);

void upload(request_message_t* request, response_message_t** response);

void login(request_message_t* request, response_message_t** response);

void rm(request_message_t* request, response_message_t** response);

void cp(request_message_t* request, response_message_t** response);

void mv(request_message_t* request, response_message_t** response);

void makedir(request_message_t* request, response_message_t** response);

void list(request_message_t* request, response_message_t** response);

#endif
