const CHUNK_LENGTH = 1024;
const FILENAME_LENGTH = 1024;

typedef string filename<FILENAME_LENGTH>;

typedef opaque chunk<CHUNK_LENGTH>;

struct get_request {
	filename path;
	int start;
};

struct put_request {
	filename path;
	chunk data;
	int bytes;	
};

struct _get_response {
	chunk data;
	int bytes;
};

union get_response switch(int errno) {
	case 0:
		_get_response chunk;
	default:
		void;
};

program SERVANT {
	version SERVANTVERSION {
		get_response download(get_request*) = 1;
		int upload(put_request*) = 2;
	} = 1;
} = 43;
