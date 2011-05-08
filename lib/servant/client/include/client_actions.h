#include "servant.h"
#include "servant_protocol_utils.h"
#include "accountmanager.h"
//#include "boolean.h"

#define SERV_ERROR_FILE 0
#define SERV_ERROR_UPLOAD 1
#define SERV_ERROR_DOWNLOAD 2

int make_ping(CLIENT* client, char* host);

int download_file(CLIENT* client, char* host, char* filename);

int upload_file(CLIENT* client, char* host, char* filename);

int remove_file(CLIENT* client, char* host, char* filename);

int copy_file(CLIENT* client, char* host, char* filename);

int move_file(CLIENT* client, char* host, char* filename);

int make_directory(CLIENT* client, char* host, char* dirname);

servant_bool_t authenticate(CLIENT* client, char* host, char* username, char* password);
