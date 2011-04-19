build:
	gcc -Wall -Ilib/servant/include -c lib/servant/servant_xdr.c -o bin/servant_xdr.o
	gcc -Wall -Ilib/servant/include -o bin/server lib/servant/server.c lib/servant/servant_svc.c bin/servant_xdr.o
	gcc -Wall -Ilib/servant/include -o bin/client lib/servant/client.c lib/servant/servant_clnt.c bin/servant_xdr.o

clean:
	find -name "*~" | xargs rm -rf
