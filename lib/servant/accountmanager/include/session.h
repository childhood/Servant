#include <time.h>
#include <sys/time.h>
#include "md5.h"
#include "user.h"

#define TOO_MANY_USERS -1
#define MAX_CONNECTED_USERS 512

typedef struct { 
    servant_user user;
	char* session_key;
	struct timeval last_access;
} connection;

typedef struct { 
	connection* connections;
	unsigned int size;
	unsigned char behavior;
} session_t;

void init_session(session_t** session, unsigned char behavior);

char* generate_session_key(connection c);

servant_bool_t new_connection(session_t* session, connection c);

servant_bool_t touch(session_t* session, char* session_key);

connection* connection_with_key(session_t* session, char* session_key);

struct timeval* last_access(session_t* session, char* session_key);

servant_bool_t close_session(session_t* session, char* session_key);
