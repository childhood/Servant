#include "client_actions.h"

int make_ping(CLIENT* client, char* host) {
    request_message_t data;
    servant_request* request;
    servant_response *response;
    response_message_t* response_data;
    
    data.content = (char*)malloc(sizeof(char)*3);
    strcpy(data.content, "");
    data.content_length = 0;
    strcpy(data.version, SERVANT_PROTOCOL_VERSION);
    data.params = (char**)malloc(sizeof(char*));
    
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    data.content = (char*)malloc(sizeof(char)*200);
    strcpy(data.action, "PING");
    request = assemble_request(&data);
    
    response = send_request_1(request, client);
    
    response_data = disassemble_response(response);
    
    printf("-VERSION: %s\n-STATUS: %s\n-CONTENT-LENGTH: %d\n-CONTENT: %s\n", response_data->version, response_data->status, response_data->content_length, response_data->content);
    
    return 0;
}

int download_file(CLIENT* client, char* host, char* filename) {
    
    int file_position;
    servant_response *response;
    FILE *file;
    request_message_t data;
    data.content = (char*)malloc(sizeof(char));
    response_message_t* response_data;
    
    servant_request* request;
    char str_start[20];
    
    file_position = 0;
    
    strcpy(data.content, "");
    data.content_length = 0;
    strcpy(data.version, "1");
    data.params = (char**)malloc(sizeof(char*));
    
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    char *path = (char *) malloc(200*sizeof(char));
    sprintf(path, "output/%s", filename);
    
    file = fopen(path, "w");
    
    do {
        data.content = (char*)malloc(sizeof(char)*200);
        strcpy(data.action, "DOWNLOAD ");
        
        sprintf(str_start, "%d ", file_position);
        strcat(data.action, str_start);
        strcat(data.action, filename);
        request = assemble_request(&data);
        
        response = send_request_1(request, client);
        
        response_data = disassemble_response(response);
        
        fwrite(response_data->content, 1, response_data->content_length, file);
        file_position += response_data->content_length;
        
    } while(response_data->content_length == 512);
    
    fclose(file);
    
	return 0;
}

int upload_file(CLIENT* client, char* host, char* filename) {
    servant_response *response;
    FILE *file;
    request_message_t data;
    data.content = (char*)malloc(sizeof(char));
    response_message_t* response_data;
    
    servant_request* request;
    
    strcpy(data.version, "1");
    data.params = (char**)malloc(sizeof(char*));
    
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    char *path = (char *) malloc(200*sizeof(char));
    sprintf(path, "output/%s", filename);
    
    file = fopen(path, "r");
    
    data.content = (char*)malloc(sizeof(char)*1030);
    data.content_length = fread(data.content, 1, 512, file);
    strcpy(data.action, "UPLOAD ");
    strcat(data.action, "N ");
    strcat(data.action, filename);
    
    request = assemble_request(&data);
    response = send_request_1(request, client);
    response_data = disassemble_response(response);
    
    while(data.content_length == 512) {
        
        data.content_length = fread(data.content, 1, 512, file);
        strcpy(data.action, "UPLOAD ");
        strcat(data.action, "A ");
        strcat(data.action, filename); 
        
        request = assemble_request(&data);
        response = send_request_1(request, client);
        response_data = disassemble_response(response);
    }
    
    fclose(file);
    
	return 0;
}

int remove_file(CLIENT* client, char* host, char* filename) {
    servant_response *response;
    request_message_t data;
    data.content = (char*)malloc(sizeof(char));
    response_message_t* response_data;
    
    servant_request* request;
    
    strcpy(data.content, " ");
    data.content_length = 1;
    
    strcpy(data.version, "1");
    
    data.params = (char**)malloc(sizeof(char*));
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    strcpy(data.action, "REMOVE ");
    strcat(data.action, filename);
    
    request = assemble_request(&data);
    response = send_request_1(request, client);
    response_data = disassemble_response(response);
    
    if (!strcmp(response_data->status, STATUS_MESSAGE_OK)) {
        printf("File removed\n");
    } else {
        printf("No file\n");
    }
    
    return 0;
}

int copy_file(CLIENT* client, char* host, char* filename) {
    servant_response *response;
    request_message_t data;
    data.content = (char*)malloc(sizeof(char));
    response_message_t* response_data;
    
    servant_request* request;
    
    strcpy(data.content, " ");
    data.content_length = 1;
    
    strcpy(data.version, "1");
    
    data.params = (char**)malloc(sizeof(char*));
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    strcpy(data.action, "COPY ");
    strcat(data.action, filename);
    
    request = assemble_request(&data);
    response = send_request_1(request, client);
    response_data = disassemble_response(response);
    
    if (!strcmp(response_data->status, STATUS_MESSAGE_OK)) {
        printf("File copied\n");
    } else {
        printf("No file copied\n");
    }
    
    return 0;
}

int move_file(CLIENT* client, char* host, char* filename) {
    servant_response *response;
    request_message_t data;
    data.content = (char*)malloc(sizeof(char));
    response_message_t* response_data;
    
    servant_request* request;
    
    strcpy(data.content, " ");
    data.content_length = 1;
    
    strcpy(data.version, "1");
    
    data.params = (char**)malloc(sizeof(char*));
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    strcpy(data.action, "MOVE ");
    strcat(data.action, filename);
    
    request = assemble_request(&data);
    response = send_request_1(request, client);
    response_data = disassemble_response(response);
    
    if (!strcmp(response_data->status, STATUS_MESSAGE_OK)) {
        printf("File moved\n");
    } else {
        printf("No file moved\n");
    }
    
    return 0;
}

int make_directory(CLIENT* client, char* host, char* dirname) {
    servant_response *response;
    request_message_t data;
    data.content = (char*)malloc(sizeof(char));
    response_message_t* response_data;
    
    servant_request* request;
    
    strcpy(data.content, " ");
    data.content_length = 1;
    
    strcpy(data.version, "1");
    
    data.params = (char**)malloc(sizeof(char*));
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    strcpy(data.action, "MKDIR ");
    strcat(data.action, dirname);
    
    request = assemble_request(&data);
    response = send_request_1(request, client);
    response_data = disassemble_response(response);
    
    if (!strcmp(response_data->status, STATUS_MESSAGE_OK)) {
        printf("Directory created\n");
    } else {
        printf("No directory created\n");
    }
    
    return 0;
}

servant_bool_t authenticate(CLIENT* client, char* host, char* username, char* password) { 
    request_message_t data;
	servant_request* request;
	servant_response* response;
	response_message_t* response_data;

	data.content = (char*)malloc(sizeof(char)*3);
    strcpy(data.content, "");
    data.content_length = 0;
    strcpy(data.version, SERVANT_PROTOCOL_VERSION);
    data.params = (char**)malloc(sizeof(char*));
    
    data.params[0] = (char*)malloc(sizeof(char)*50);
    strcpy(data.params[0], "DEFAULT");
    data.n_params = 1;
    
    data.content = (char*)malloc(sizeof(char)*200);
    sprintf(data.action, "LOGIN username:%s password:%s", username, MDString(password));
    request = assemble_request(&data);
    
    response = send_request_1(request, client);

	response_data = disassemble_response(response);

	if (!strcmp(response_data->status, STATUS_MESSAGE_OK) && !strcmp(response_data->content, "OK")) {
		return SERVANT_TRUE;
	}

	return SERVANT_FALSE;

}
