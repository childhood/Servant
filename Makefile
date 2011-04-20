build: clean include xdr server client

include:
	gcc -Wall -Ilib/utils/include -c lib/utils/md5.c -o bin/md5.o
	gcc -Wall -Ilib/utils/include -c lib/utils/file_utils.c -o bin/file_utils.o
	gcc -Wall -c lib/servant/accountmanager/accountmanager.c -o bin/accountmanager.o

xdr:
	gcc -Wall -Ilib/servant/include -c lib/servant/servant_xdr.c -o bin/servant_xdr.o

server:
	gcc -Wall -Ilib/servant/include -Ilib/utils/include -Ilib/servant/accountmanager \
		-o bin/servantd lib/servant/server/server.c lib/servant/server/servant_svc.c bin/*.o

client:
	gcc -Wall -Ilib/servant/include -o bin/servant lib/servant/client/client.c lib/servant/client/servant_clnt.c bin/*.o

clean:
	find -name "*~" | xargs rm -rf
	rm -f bin/*
