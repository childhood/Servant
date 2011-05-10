#include <stdio.h>
#include <string.h>
#include "servant.h"
#include "signal_handling.h"
#include "server_actions.h"

extern user_list* servant_users;

void start_servant();

void quit(int signal);

servant_response *send_request_1_svc(servant_request* request, struct svc_req *rqstp);
