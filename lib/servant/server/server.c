#include "server.h"

#ifdef __APPLE__
    extern int errno;
#else
    extern __thread int errno;
#endif

extern user_list* servant_users;

servant_response *send_request_1_svc(servant_request* request, struct svc_req *rqstp) {
    request_message_t* request_data = NULL;
    response_message_t* response_data = NULL;
    static servant_response* response;
    char* command = NULL;
    
    request_data = disassemble_request(request);

    //request handling
    command = get_command_from_action(request_data->action);
    
    if (!strcmp(command, "DOWNLOAD")) {
        download(request_data, &response_data);
    } else if (!strcmp(command, "UPLOAD")) {
        upload(request_data, &response_data);
    } else if (!strcmp(command, "PING")) {
        ping(request_data, &response_data);
    } else if (!strcmp(command, "LOGIN")) {
        login(request_data, &response_data);
    } else if (!strcmp(command, "REMOVE")) {
        rm(request_data, &response_data);
    } else if (!strcmp(command, "COPY")) {
        cp(request_data, &response_data);
    } else if (!strcmp(command, "MOVE")) {
        mv(request_data, &response_data);
    } else if (!strcmp(command, "MKDIR")) {
        makedir(request_data, &response_data);
    } else if (!strcmp(command, "LIST")) {
        list(request_data, &response_data);
    } else {
        return NULL;
    }
    
    response = assemble_response(response_data);
    
    //free(response_data);
    //free(request_data);
    //free(command);
    
    return response;
}

void start_servant() { 
	trap(SIGINT, quit);
	trap(SIGQUIT, quit);
	trap(SIGTERM, quit);

    if (!file_exists(SERVANT_ACCOUNTS_FILE)) {
		char password[32], confirm_password[32];

		printf("\nLooks like it's the first time you are running servantd. Please, set a password for the root account:\n");
		do {
			printf("\nPassword: ");
			read_password(password);

			printf("\nConfirm password: ");
			read_password(confirm_password);
		} while (strcmp(password, confirm_password));

		if (!new_account("root", MDString(password))) {
			perror("Could not create root account. Quiting...\n");
			exit(1);
		}
	}

	parse_accounts_file(&servant_users, ORDERED_LIST);
}

void quit(int signal) { 
	printf("\n\n");
	switch(signal) {
		case SIGINT:
			printf("Got SIGINT, ");
			break;

		case SIGQUIT:
			printf("Got SIGQUIT, ");
			break;

		case SIGTERM:
			printf("Got SIGTERM, ");
			break;
	}

	printf("finishing...\n");
    update_accounts_file(servant_users);	
	printf("Ok.\n");
	exit(0);
}
