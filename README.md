# What is
This software makes use of parameters. It works only on Unix. You specify a port and hostname when you launch the client and in another terminal you specify the same port of client for establishing a connection between them.

# How it works
For compilation use the command of UNIX `make`. It will found the Makefile and it will compiled the files `client.c` and `server.c`.


Then launch the software followed by parameters.
```sh
./server -p 6090
```

and then the client
```sh
./client -p 6090 -h 127.0.0.1 -s Cooper -n Adam -SN
```

WARNING: launch the server before and then the client because you will have a error: Connection refused.


### Version
1.0.0