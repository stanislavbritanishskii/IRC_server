//#include <iostream>
//#include <cstring>
//#include <unistd.h>
//#include <poll.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <cerrno>
//#include <cstdio>
#include <sys/signal.h>

#include "Server.hpp"

#define PORT 8080
#define MAX_EVENTS 5

// check change

void finita(Server *s, int exitt)
{
	static Server *_s = NULL;
	if (!_s)
		_s = s;
	if (exitt)
	{
		_s->finish();
		exit(1);
	}
}

void fatality(int sig)
{
	finita(NULL, 1);
}

int main()
{
	Server server;
	finita(&server, 0);
	signal(SIGINT, fatality);
	if (!server.setup(PORT, "password"))
		return 1;
	int running = 1;

	while (running)
		running = server.process();

	server.finish();
	return 0;
}
