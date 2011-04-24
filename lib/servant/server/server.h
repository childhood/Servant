#include "accountmanager.h"
#include "signal_handling.h"
#include "servant_protocol_utils.h"
#include "md5.h"

user_list* servant_users;

void start_servant();

void quit(int signal);
