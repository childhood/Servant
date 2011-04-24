Servant
=======

Servant is a simple file server for distributed systems using [RPC Protocol][0].
RPC related code is generated using rpcgen.

Context
-------

Servant was initally developed as part of Distributed Systems course at University of São Paulo.

Current state
-------------

Servant has just started being developed. A protocol is about to come, and user management
functionality will merge to the master branch soon.

Test it!
--------

You can help us testing Servant. Download the code, and just
	$ make
to generate the server and the client files.

Start the server:
	$ ./bin/servantd

Start the client
	$ ./bin/servant 127.0.0.1
changing the IP address above if you started the server in a different machine.

Contributing
------------

We are not accepting pull request by now. We have to finish the first release and hand in
to the teacher first, of course.

In some weeks, pull requests will be gracefully considered.

Authors
-------
* [Renato Mascarenhas][1]
* [Rafael Regis do Prado][2]

[0]: http://www.ietf.org/rfc/rfc1831.txt
[1]: http://github.com/rmascarenhas
[2]: http://github.com/rafaelregis
