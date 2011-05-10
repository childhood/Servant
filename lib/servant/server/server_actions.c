#include "server_actions.h"

extern user_list* servant_users;
//extern session_t* session;

void ping(request_message_t* request, response_message_t** response) {
    if (response == NULL) {
        return;
    }
        
    if ((*response) != NULL) {
        free((*response));
    }
    (*response) = (response_message_t*)malloc(sizeof(response_message_t));
    
    (*response)->content = (char*)malloc(sizeof(char)*1024);  
    
    servant_request* message = assemble_request(request);
    strcpy((*response)->content, message->data.chunk_val);
    (*response)->content_length = strlen((*response)->content);
    
    set_status_message((*response), STATUS_OK); 
    set_protocol_version(*response);
}

void download(request_message_t* request, response_message_t** response) {
    /* path and starting point of the file to be sent */
    char path[100];
    char str_start[10];
    int start;
    
    /* regex to extract download action args */
    regex_t arg_pattern;
    regmatch_t pm[3];
    char* arg;
    
    FILE *file;
    int status_flag = STATUS_OK;
    
    if (response == NULL) {
        return;
    }
    if ((*response) != NULL) {
        free((*response));
    }
    (*response) = (response_message_t*)malloc(sizeof(response_message_t));
    
    /* extraction of the the the start point and the path for download action */
    regcomp(&arg_pattern, "([0-9]+) ([^ ]+)", REG_EXTENDED | REG_NEWLINE);      
    arg = get_arg_from_action(request->action); 
    if (!regexec(&arg_pattern, arg, 3, pm, 0)) {
        strncpy(str_start, arg + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        str_start[pm[1].rm_eo - pm[1].rm_so] = '\0';
        start = atoi(str_start);
        strncpy(path, arg + pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so);
        path[pm[2].rm_eo - pm[2].rm_so] = '\0';
    } else {
        status_flag = STATUS_WRONG_ARGS;
    }
    
    /* storing the chunk in content field */
    if (status_flag == STATUS_OK) {
        file = fopen(path, "r");
        if (file != NULL)  {
            (*response)->content = (char*)malloc(1024*sizeof(char));
            fseek(file, start, SEEK_SET);
            (*response)->content_length = fread((*response)->content, 1, 512, file);        
            fclose(file);
        } else {
            status_flag = STATUS_NO_FILE;
        }
    }
    
    set_status_message((*response), status_flag); 
    set_protocol_version(*response);
    
    //free(arg);
}

void upload(request_message_t* request, response_message_t** response) { 
    // path and starting point of the file to be sent 
    char path[100];
    char option[2];
    
    // regex to extract download action args 
    regex_t arg_pattern;
    regmatch_t pm[3];
    char* arg;
    
    FILE *file;
    int status_flag = STATUS_OK;
    
    if (response == NULL) {
        return;
    }
    if ((*response) != NULL) {
        free((*response));
    }
    (*response) = (response_message_t*)malloc(sizeof(response_message_t));
    
    regcomp(&arg_pattern, "(A|N) ([^ ]+)", REG_EXTENDED | REG_NEWLINE);      
    arg = get_arg_from_action(request->action); 
    if (!regexec(&arg_pattern, arg, 3, pm, 0)) {
        strncpy(option, arg + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        option[pm[1].rm_eo - pm[1].rm_so] = '\0';
        
        strncpy(path, arg + pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so);
        path[pm[2].rm_eo - pm[2].rm_so] = '\0';
    } else {
        status_flag = STATUS_WRONG_ARGS;
    }
    
    if (status_flag == STATUS_OK) {
        if (option[0] == 'N') {
            file = fopen(path, "w");
        } else if (option[0] == 'A') {
            file = fopen(path, "a");
        }
        if (file != NULL)  {
            fwrite(request->content, 1, request->content_length, file);
            fclose(file);          
            (*response)->content = (char*)malloc(2*sizeof(char));
            strcpy((*response)->content, "");
            (*response)->content_length = 0;
        } else {
            status_flag = STATUS_NO_FILE;
        }
    }
    
    set_status_message((*response), status_flag); 
    set_protocol_version(*response);
}

void login(request_message_t* request, response_message_t** response) {
    // regex to extract download action args 
    regex_t arg_pattern;
    regmatch_t pm[3];
	char* arg;
    char password[32], username[32];
    
    int status_flag = STATUS_OK;
    
    if (response == NULL) {
        return;
    }
    if ((*response) != NULL) {
        free((*response));
    }
    (*response) = (response_message_t*)malloc(sizeof(response_message_t));

    //possible regex
	regcomp(&arg_pattern, "username:([^ ]+) password:([^ ]+)", REG_EXTENDED | REG_NEWLINE);      
    arg = get_arg_from_action(request->action); 
    if (!regexec(&arg_pattern, arg, 3, pm, 0)) {
		strncpy(username, arg + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        username[pm[1].rm_eo - pm[1].rm_so] = '\0';
        
        strncpy(password, arg + pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so);
        password[pm[2].rm_eo - pm[2].rm_so] = '\0';

    } else {
        status_flag = STATUS_WRONG_ARGS;
    }
    
	if (status_flag == STATUS_OK) {
         (*response)->content = (char*)malloc(32*sizeof(char));
		 if (registered(servant_users, username, password)) {
			connection c;
			c.user = get_user(servant_users, username, password);
			new_connection(session, c);
			strcpy((*response)->content, "OK");
		 } else {
			 strcpy((*response)->content, "ACCESS DENIED");
		 }

         (*response)->content_length = sizeof((*response)->content); 
    }
    
    set_status_message((*response), status_flag); 
    set_protocol_version(*response);
}
                                                            
void rm(request_message_t* request, response_message_t** response) {
    // path and starting point of the file to be sent
    char path[100];
    
    // regex to extract download action args
    regex_t arg_pattern;
    regmatch_t pm[3];
    char* arg;
    
    int status_flag = STATUS_OK;
    
    if (response == NULL) {
        return;
    }
    if ((*response) != NULL) {
        free((*response));
    }
    (*response) = (response_message_t*)malloc(sizeof(response_message_t));
    
    regcomp(&arg_pattern, "([a-z][a-z0-9]*)", REG_EXTENDED | REG_NEWLINE);      
    arg = get_arg_from_action(request->action); 
    if (!regexec(&arg_pattern, arg, 3, pm, 0)) {       
        strncpy(path, arg + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        path[pm[1].rm_eo - pm[1].rm_so] = '\0';
    } else {
        status_flag = STATUS_WRONG_ARGS;
    }
    (*response)->content = (char*)malloc(2*sizeof(char));
    strcpy((*response)->content, "");
    (*response)->content_length = 0;    
    if (status_flag == STATUS_OK) {
        if (remove(path) != 0) {
            status_flag = STATUS_NO_FILE;
        }
    }
    
    set_status_message((*response), status_flag); 
    set_protocol_version(*response);
}

void cp(request_message_t* request, response_message_t** response) {
    // path and starting point of the file to be sent
    char path_origin[100], path_destiny[100];
    
    // regex to extract download action args
    regex_t arg_pattern;
    regmatch_t pm[3];
    char* arg;
    
    int read_data;
    char* content;
    
    FILE *file_origin, *file_destiny;
    int status_flag = STATUS_OK;
    
    if (response == NULL) {
        return;
    }
    if ((*response) != NULL) {
        free((*response));
    }
    (*response) = (response_message_t*)malloc(sizeof(response_message_t));
    
    regcomp(&arg_pattern, "([^ ]+) ([^ ]+)", REG_EXTENDED | REG_NEWLINE);      
    arg = get_arg_from_action(request->action); 
    if (!regexec(&arg_pattern, arg, 3, pm, 0)) {
        strncpy(path_origin, arg + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        path_origin[pm[1].rm_eo - pm[1].rm_so] = '\0';
        
        strncpy(path_destiny, arg + pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so);
        path_destiny[pm[2].rm_eo - pm[2].rm_so] = '\0';
    } else {
        status_flag = STATUS_WRONG_ARGS;
    }
    
    (*response)->content = (char*)malloc(2*sizeof(char));
    strcpy((*response)->content, "");
    (*response)->content_length = 0;
    if (status_flag == STATUS_OK) {
        file_destiny = fopen(path_destiny, "w");
        file_origin = fopen(path_origin, "r");
        
        if (file_origin != NULL && file_destiny != NULL)  {
            content = (char*)malloc(1024*sizeof(char));
            do {
                read_data = fread(content, 1, 1024, file_origin);
                fwrite(content, 1, read_data, file_destiny);
            } while (read_data == 1024);
            fclose(file_destiny);          
            fclose(file_origin);            
            free(content);
        } else {
            status_flag = STATUS_NO_FILE;
        }
    }
    
    set_status_message((*response), status_flag); 
    set_protocol_version(*response);    
}

void mv(request_message_t* request, response_message_t** response) {
    // path and starting point of the file to be sent
    char path_origin[100], path_destiny[100];
    
    // regex to extract download action args
    regex_t arg_pattern;
    regmatch_t pm[3];
    char* arg;
    
    int read_data;
    char* content;
    
    FILE *file_origin, *file_destiny;
    int status_flag = STATUS_OK;
    
    if (response == NULL) {
        return;
    }
    if ((*response) != NULL) {
        free((*response));
    }
    (*response) = (response_message_t*)malloc(sizeof(response_message_t));
    
    regcomp(&arg_pattern, "([^ ]+) ([^ ]+)", REG_EXTENDED | REG_NEWLINE);      
    arg = get_arg_from_action(request->action); 
    if (!regexec(&arg_pattern, arg, 3, pm, 0)) {
        strncpy(path_origin, arg + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        path_origin[pm[1].rm_eo - pm[1].rm_so] = '\0';
        
        strncpy(path_destiny, arg + pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so);
        path_destiny[pm[2].rm_eo - pm[2].rm_so] = '\0';
    } else {
        status_flag = STATUS_WRONG_ARGS;
    }
    
    (*response)->content = (char*)malloc(2*sizeof(char));
    strcpy((*response)->content, "");
    (*response)->content_length = 0;    
    if (status_flag == STATUS_OK) {
        file_destiny = fopen(path_destiny, "w");
        file_origin = fopen(path_origin, "r");
        
        if (file_origin != NULL && file_destiny != NULL)  {
            content = (char*)malloc(1024*sizeof(char));
            do {
                read_data = fread(content, 1, 1024, file_origin);
                fwrite(content, 1, read_data, file_destiny);
            } while (read_data == 1024);
            fclose(file_origin);
            fclose(file_destiny);
            remove(path_origin);
            free(content);
        } else {
            status_flag = STATUS_NO_FILE;
        }
    }
    
    set_status_message((*response), status_flag); 
    set_protocol_version(*response);  
}


void makedir(request_message_t* request, response_message_t** response) {
    // path and starting point of the file to be sent 
    char path[100];
    
    // regex to extract download action args 
    regex_t arg_pattern;
    regmatch_t pm[3];
    char* arg;
        
    int status_flag = STATUS_OK;
    
    if (response == NULL) {
        return;
    }
    if ((*response) != NULL) {
        free((*response));
    }
    (*response) = (response_message_t*)malloc(sizeof(response_message_t));
    
    regcomp(&arg_pattern, "([^ ]+)", REG_EXTENDED | REG_NEWLINE);      
    arg = get_arg_from_action(request->action); 
    if (!regexec(&arg_pattern, arg, 3, pm, 0)) {
        strncpy(path, arg + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        path[pm[1].rm_eo - pm[1].rm_so] = '\0';
    } else {
        status_flag = STATUS_WRONG_ARGS;
    }
    (*response)->content = (char*)malloc(2*sizeof(char));
    strcpy((*response)->content, "");
    (*response)->content_length = 0;
    if (status_flag == STATUS_OK) {
        if (mkdir(path, S_IRWXU | S_IRWXG) != 0) {
            status_flag = STATUS_NO_FILE;
        }
    }
    set_status_message((*response), status_flag); 
    set_protocol_version(*response);   
}

/*char __list_of_files[10000];

int generate_tree(const char *path, const struct stat *status, int type, struct FTW* description) {
    int i; 
    char *last_part;
    
    if (strcmp(path, ".")) {        
        for(i = 0; i < description->level; i++) {
            strcat(__list_of_files, "  |");
        }
        strcat(__list_of_files, "-");
        last_part = strrchr(path, '/');
        if(last_part != NULL) {
            strcat(__list_of_files, last_part+1);
        } else {
            strcat(__list_of_files, path);
        }
        strcat(__list_of_files, "\n");
    }
    return 0;
}*/


/*void list(request_message_t* request, response_message_t** response) {
    // path and starting point of the file to be sent 
    char path[100];
    char str_seq[2];
    char sequence = 0;
    
    // regex to extract download action args 
    regex_t arg_pattern;
    regmatch_t pm[3];
    char* arg;
    
    int status_flag = STATUS_OK;
    
    if (response == NULL) {
        return;
    }
    if ((*response) != NULL) {
        free((*response));
    }
    (*response) = (response_message_t*)malloc(sizeof(response_message_t));
    
    regcomp(&arg_pattern, "([0-9]+) ([^ ]+)", REG_EXTENDED | REG_NEWLINE);      
    arg = get_arg_from_action(request->action); 
    if (!regexec(&arg_pattern, arg, 3, pm, 0)) {
        strncpy(str_seq, arg + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        str_seq[pm[1].rm_eo - pm[1].rm_so] = '\0';
        sequence = atoi(str_seq);
        
        strncpy(path, arg + pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so);
        path[pm[2].rm_eo - pm[2].rm_so] = '\0';
    } else {
        status_flag = STATUS_WRONG_ARGS;
    }
    
    if (status_flag == STATUS_OK) {
        strcpy(__list_of_files, "");
        
        nftw(path, generate_tree, 1, 0);
        
        (*response)->content = (char*)malloc(1000*sizeof(char));
        
        strncpy((*response)->content, __list_of_files + sequence*1000, 1000);
        (*response)->content[1000] = '\0';

        (*response)->content_length = strlen((*response)->content);
    }
    
    set_status_message((*response), status_flag); 
    set_protocol_version(*response);
    
    
    printf("%d\n", (*response)->content_length);
}*/
