#include "servant.h"

#ifdef __APPLE__
	extern int errno;
#else
	extern __thread int errno;
#endif

get_response *
download_1_svc(get_request *request, struct svc_req *rqstp)
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

int *
upload_1_svc(put_request *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}