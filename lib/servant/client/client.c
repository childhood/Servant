#include <stdio.h>
#include <stdlib.h>
#include "servant.h"

#ifdef __APPLE__
	extern int errno;
#else 
	extern __thread int errno;
#endif

void
download_file(char* host, char* filename) {
	CLIENT* client;
	int file_position;
	char* path;
	get_response* result;
	get_request request;
	FILE *file;

	request.path = filename;
	path = (char*) malloc(FILENAME_LENGTH*sizeof(char));
	//sprintf(path, "output/%s", filename);
	request.start = 0;

	client = clnt_create(host, SERVANT, SERVANTVERSION, "tcp");
	if (client == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}

	file = fopen(path, "w");
	file_position = 0;

	do {
		request.start = file_position;
		result = download_1(&request, client);

		if (result == NULL) {
			clnt_perror(client, host);
			exit(1);
		}

		if (result->errno != 0) {
			errno = result->errno;
			perror("Could not retrieve file...\n");
			exit(1);
		}

		fwrite(result->get_response_u.chunk.data.chunk_val, 1, result->get_response_u.chunk.data.chunk_len, file);
		file_position += result->get_response_u.chunk.bytes;

	} while(result->get_response_u.chunk.bytes == CHUNK_LENGTH);

	fclose(file);
}

int
main (int argc, char *argv[])
{
	char* host;
	char* filename = (char*) malloc(FILENAME_LENGTH*sizeof(char));

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];

	printf(">> ");
	scanf("%s", filename);

	download_file(host, filename);
	
	return 0;
}
