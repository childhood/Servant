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
    servant_response* response = NULL;
    char* command = NULL;
    
    request_data = disassemble_request(request);

    //request handling
    command = get_command_from_action(request_data->action);
    if (!strcmp(command, "DOWNLOAD")) {
        download(request_data, &response_data);
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
    }
    
    response = assemble_response(response_data);

    free(response_data);
    free(request_data);
    free(command);
    
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


/*get_response *download_1_svc(get_request *request, struct svc_req *rqstp)
{
    FILE *file;
    static char data[1024];
    int bytes;
    static get_response  result;

    file = fopen(request->path, "r");
    if (file == NULL)  {
        result.errno = errno;
        return &result;
    }

    fseek(file, request->start, SEEK_SET);
    bytes = fread(data, 1, 1024, file);

    result.get_response_u.chunk.data.chunk_val = data;
    result.get_response_u.chunk.data.chunk_len = bytes;
    result.get_response_u.chunk.bytes = bytes;

    result.errno = 0;
    fclose(file);

    return &result;
}

int *upload_1_svc(put_request *request, struct svc_req *rqstp)
{
    FILE *file;
    static int  result;

    file = fopen(request->path, "a");
    if (file == NULL)  {
        result = errno;
        return &result;
    }

    fwrite(request->data.chunk_val, 1, request->bytes, file);
    fclose(file);

    result = 0;
    return &result;
}*/
