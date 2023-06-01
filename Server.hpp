#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <fcntl.h>

class Server
{
private:
	int _mainSocketFD;
	std::string _pass;
	int _nfds;
	std::vector <struct pollfd> _fds;
public:
	Server ();
	int setup(int port, std::string pass);
	int process();
	void finish();
	int pollIn(int i);
	int newClient();
	int handleMessage(int i);
};

#endif