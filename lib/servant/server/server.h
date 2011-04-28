#include <stdio.h>
#include <string.h>
#include "servant.h"
#include "accountmanager.h"
#include "signal_handling.h"
#include "servant_protocol_utils.h"
#include "servant_protocol_actions.h"
#include "md5.h"

user_list* servant_users;

void start_servant();

void quit(int signal);

servant_response *send_request_1_svc(servant_request* request, struct svc_req *rqstp);
