#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) { 
    struct ifaddrs* ifAddr = NULL;
	struct ifaddrs* ifa = NULL;
	void* tmpAddr = NULL;

	getifaddrs(&ifAddr);

	for (ifa = ifAddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr->sa_family == AF_INET) {
			tmpAddr = &((struct sockaddr_in*) ifa->ifa_addr)->sin_addr;
			char addr[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddr, addr, INET_ADDRSTRLEN);
			printf("%s address - %s\n", ifa->ifa_name, addr);
		} else {
			tmpAddr = &((struct sockaddr_in*) ifa->ifa_addr)->sin_addr;
			char addr[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, tmpAddr, addr, INET6_ADDRSTRLEN);
			printf("%s address - %s\n", ifa->ifa_name, addr);
		}
	}

	if (ifAddr != NULL) {
		free(ifAddr);
	}

	return 0;
}
