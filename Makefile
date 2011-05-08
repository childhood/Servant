.PHONY: test

build: clean include xdr server client

include:
	gcc -Wall -Ilib/utils/include -c lib/utils/md5.c -o bin/md5.o
	gcc -Wall -Ilib/utils/include -c lib/utils/string_utils.c -o bin/string_utils.o
	gcc -Wall -Ilib/utils/include -c lib/utils/file_utils.c -o bin/file_utils.o
	gcc -Wall -Ilib/utils/include -c lib/utils/signal_handling.c -o bin/signal_handling.o
	gcc -Wall -Ilib/utils/include -Ilib/servant/include -c lib/utils/servant_protocol_utils.c -o bin/servant_protocol_utils.o
	gcc -Wall -Ilib/utils/include -Ilib/servant/accountmanager/include -c lib/servant/accountmanager/user.c -o bin/user.o
	gcc -Wall -Ilib/servant/accountmanager/include -Ilib/utils/include -c lib/servant/accountmanager/accountmanager.c -o bin/accountmanager.o

xdr:
	gcc -Wall -Ilib/servant/include -c lib/servant/servant_xdr.c -o bin/servant_xdr.o

server:
	gcc -Wall -Ilib/utils/include/ -Ilib/servant/server/include -Ilib/servant/accountmanager/include -Ilib/servant/include -c -o bin/server_actions.o lib/servant/server/server_actions.c
	gcc -Wall -Ilib/servant/include -Ilib/utils/include -Ilib/servant/server/include -Ilib/servant/accountmanager/include \
		-o bin/servantd lib/servant/server/server.c lib/servant/server/servant_svc.c bin/*.o

client:
	gcc -Wall -Ilib/servant/client/include -Ilib/servant/include -Ilib/servant/accountmanager/include -Ilib/utils/include -c -o bin/client_actions.o lib/servant/client/client_actions.c 
	gcc -Wall -Ilib/servant/include -Ilib/utils/include -Ilib/servant/client/include -Ilib/servant/accountmanager/include -o bin/servant lib/servant/client/client.c lib/servant/client/servant_clnt.c bin/*.o

clean:
	@find -name "*~" | xargs rm -rf
	@rm -f bin/*

test:
	gcc -Ilib/servant/accountmanager/include -Ilib/utils/include test/user_test.c bin/file_utils.o bin/accountmanager.o bin/string_utils.o  bin/user.o -o bin/user_test
	./bin/user_test
