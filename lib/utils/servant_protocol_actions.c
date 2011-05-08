#include "servant_protocol_actions.h"

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
    char user[32], pass[32];
    
    int status_flag = STATUS_OK;
    
    if (response == NULL) {
        return;
    }
    if ((*response) != NULL) {
        free((*response));
    }
    (*response) = (response_message_t*)malloc(sizeof(response_message_t));
    
    //possible regex
    regcomp(&arg_pattern, "user:([a-z][a-z0-9]) pass:([^ ]+)", REG_EXTENDED | REG_NEWLINE);      
    arg = get_arg_from_action(request->action); 
    if (!regexec(&arg_pattern, arg, 3, pm, 0)) {
        strncpy(username, arg + pm[1].rm_so, pm[1].rm_eo - pm[1].rm_so);
        user[pm[1].rm_eo - pm[1].rm_so] = '\0';
        
        strncpy(password, arg + pm[2].rm_so, pm[2].rm_eo - pm[2].rm_so);
        pass[pm[2].rm_eo - pm[2].rm_so] = '\0';
    } else {
        status_flag = STATUS_WRONG_ARGS;
    }

    if (status_flag == STATUS_OK) {
        //your implementation here!
        /* include this somewhere with or without content.
         (*response)->content = (char*)malloc(2*sizeof(char));
         strcpy((*response)->content, "");
         (*response)->content_length = 0; 
         */
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
<<<<<<< HEAD
*/}
=======
}
>>>>>>> 8fb7604a94d92a0e4e618b5d6eab5501a3387aa6
