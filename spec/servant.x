const CHUNK_LENGTH = 1500;

typedef opaque chunk<CHUNK_LENGTH>;

struct servant_request {
    chunk data;
};

struct servant_response {
    chunk data;
};

program SERVANT {
    version SERVANTVERSION {
        servant_response send_request(servant_request*) = 1;
    } = 1;
} = 43;
