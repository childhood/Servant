#include "session.h"

void init_session(session_t** session, unsigned char behavior) {
	*session = (session_t*) malloc(sizeof(session_t));
    (*session)->connections = (connection*) malloc(MAX_CONNECTED_USERS*sizeof(connection));
	(*session)->behavior = behavior;
	(*session)->size = 0;
}

char* generate_session_key(connection c) { 
    char* buffer = (char*) malloc(512*sizeof(char));
	struct tm* timeinfo;
	time_t raw_time;
	char time_now[80];

	char salt[128];
	int i;

	time(&raw_time);
	timeinfo = localtime(&raw_time);
	strftime(time_now, 80, "%c", timeinfo);

	srand(time(NULL));
	for (i = 0; i < 128; ++i) {
		salt[i] = (rand() % 32) + 94; // ascii table: from 32 to 126
	}
	
	strncpy(buffer, c.user.username, strlen(c.user.username));
	strcat(buffer, c.user.password);
	strcat(buffer, time_now);
	strcat(buffer, salt);

	return MDString(buffer);
}

void set_connection_at(unsigned int index, session_t* session, connection c) {
    session->connections[index] = c;
}

void shift_sessions(session_t* session, unsigned int index, unsigned char direction) {
    unsigned int i;

    switch (direction) {
    case RIGHT:
        for (i = session->size; i > index; i--) {
            session->connections[i] = session->connections[i-1];
        }
        break;

    case LEFT:
        for (i = index+1; i < session->size; i++) {
            session->connections[i-1] = session->connections[i];
        }
        break;
    }
}

void append_connection(session_t* session, connection c) {
    set_connection_at(session->size++, session, c);
}

void session_in_order(session_t* session, connection c) {
    unsigned int i;

    for (i = 0; i < session->size && strcmp((session->connections[i]).session_key, c.session_key) < 0; ++i);

    shift_sessions(session, i, RIGHT);
    set_connection_at(i, session, c);
    session->size++;
}

servant_bool_t new_connection(session_t* session, connection c) {
	c.session_key = generate_session_key(c);
	gettimeofday(&(c.last_access), NULL);

    if (session->size >= MAX_CONNECTED_USERS) {
        return TOO_MANY_USERS;
    }

    if (session->behavior == ORDERED_LIST) {
        session_in_order(session, c);
    } else {
        append_connection(session, c);
    }

	printf("Registered user %s with session key %s\n", c.user.username, c.session_key);

    return SERVANT_TRUE;
}

int session_sequential_session_search(session_t* session, char* session_key) {
    int i;

    for (i = 0; i < session->size && strcmp((session->connections[i]).session_key, session_key); ++i);

    if (i == session->size) {
        return USER_NOT_FOUND;
    }

    return i;
}

int session_binary_session_search(session_t* session, char* session_key) {
    int min, max, mid;
    min = 0;
    max = session->size - 1;

    while (min <= max) {
        mid = (min+max)/2;
        if (strcmp((session->connections[mid]).session_key, session_key) < 0) {
            min = mid + 1;
        } else if (strcmp((session->connections[mid]).session_key, session_key) > 0) {
            max = mid - 1;
        } else {
            return mid;
        }
    }

    return USER_NOT_FOUND;
}

int session_search(session_t* session, char* username) {
    if (session->behavior == ORDERED_LIST) {
        return session_binary_session_search(session, username);
    }

    return session_sequential_session_search(session, username);

}

connection* connection_with_key(session_t* session, char* session_key) { 
    int index;

	index = session_search(session, session_key);

	if (index == USER_NOT_FOUND) {
		return NULL;
	}

	return &(session->connections[index]);
}

servant_bool_t close_session(session_t* session, char* session_key) {
    int index;

    index = session_search(session, session_key);

    if (index == USER_NOT_FOUND) {
        return SERVANT_FALSE;
    }

    shift_sessions(session, index, LEFT);
    session->size--;

    return SERVANT_TRUE;
}

servant_bool_t touch(session_t* session, char* session_key) { 
    int index;

	index = session_search(session, session_key);

	if (index == USER_NOT_FOUND) {
		return SERVANT_FALSE;
	}

	gettimeofday(last_access(session, session_key), NULL);

	return SERVANT_TRUE;

}

struct timeval* last_access(session_t* session, char* session_key) { 
    int index;

	index = session_search(session, session_key);

	if (index == USER_NOT_FOUND) {
		return NULL;
	}

	return &(session->connections[index].last_access);
}
