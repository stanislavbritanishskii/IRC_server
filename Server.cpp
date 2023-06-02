#include "Server.hpp"
#include <poll.h>

Server::Server() {}

int Server::setup(int port, std::string pass)
{
	_mainSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (_mainSocketFD < 0) {
		std::cerr << "socket creation failed: " << strerror(errno) << std::endl;
		return 0;
	}
	std::cout << _mainSocketFD << std::endl;

	if (fcntl(_mainSocketFD, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fnctl function failed");

	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);


	int ret = bind(_mainSocketFD, (struct sockaddr *)&address, sizeof(address));
	if (ret < 0) {
		std::cerr << "bind failed: " << strerror(errno) << std::endl;
		return 0;
	}

	ret = listen(_mainSocketFD, 5);
	if (ret < 0) {
		std::cerr << "listen failed: " << strerror(errno) << std::endl;
		return 0;
	}

	pollfd zero;
	_fds.push_back(zero);
	_fds[0].fd = _mainSocketFD;
	_fds[0].events = POLLIN;

	_nfds = 1;
	return 1;
}

int Server::process()
{
	int ret = poll(&_fds[0], _nfds, -1);
	if (ret == -1) {
		std::cerr << "poll failed: " << strerror(errno) << std::endl;
		return 0;
	}

	for (int i = 0; i < _nfds; i++) {
		send(_fds[i].fd, "sending\n", 8, 0);
		if (_fds[i].revents & POLLIN)
			if (!pollIn(i))
				return 0;
		if (_fds[i].revents & POLLOUT)
		{
			std::cout << "pollout fd " << _fds[i].fd << std::endl;
		}
		if (_fds[i].revents & POLLHUP)
		{
			std::cout << "pollhup fd " << _fds[i].fd << std::endl;
		}
		std::cout << _fds[i].revents << std::endl;
	}
	return 1;
}


void Server::finish()
{
	for (int i = 0; i < _nfds; i++) {
		if (_fds[i].fd != -1) {
			std::cout << "here"<< std::endl;
			close(_fds[i].fd);
			_fds[i].fd = -1;
		}
	}

	close(_mainSocketFD);
}

int Server::pollIn(int i)
{
	if (_fds[i].fd == _mainSocketFD)
		return newClient();
	else
		return handleMessage(i);
}


int Server::newClient()
{
	struct sockaddr_in client_address;
	memset(&client_address, 0, sizeof(client_address)); // in separate function not needed
	socklen_t client_len = sizeof(client_address);
	int client_sockfd = accept(_mainSocketFD, (struct sockaddr *)&client_address, &client_len);
	if (client_sockfd < 0) {
		std::cerr << "accept failed: " << strerror(errno) << std::endl;
		return 0;
	}
	pollfd newPollFD;
	memset (&newPollFD, 0, sizeof(newPollFD));
	_fds.push_back(newPollFD);
	std::cout << "_fds size = " << _fds.size() << "_nfds = " << _nfds << std::endl;
	_fds[_nfds].fd = client_sockfd;
	_fds[_nfds].events = POLLIN;
	_nfds++;
	return 1;
}

int Server::handleMessage(int i)
{
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	int ret = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
	if (ret <= 0)
		return disconnectUser(i, ret);
	else
		return processMessage(buffer, i);
}

int Server::disconnectUser(int i, int ret)
{
	if (ret < 0) {
		std::cerr << "recv failed: " << strerror(errno) << std::endl;
	}
	std::cout << "disconnecting" << std::endl;
	close(_fds[i].fd);
	_fds[i].fd = -1;
	_fds.erase(_fds.begin() + i);

	_nfds--;
	return 1;
}

int Server::processMessage(char *buffer, int i)
{
	std::cout << "Received message: " << buffer << std::endl;
	manager.parseMessage(buffer);

	// Example: Send a response back to the client
	const char* response = "Server received your message\n";
	int ret = send(_fds[i].fd, response, strlen(response), 0);
	std::cout << _fds[i].fd << " " << _fds[i].events <<" " <<_fds[i].revents << std::endl;
	if (ret < 0) {
		std::cerr << "send failed: " << strerror(errno) << std::endl;
	}
	return 1;
}