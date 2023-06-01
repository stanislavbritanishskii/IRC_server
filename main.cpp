#include <iostream>
#include <cstring>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>

#define PORT 8080
#define MAX_EVENTS 5

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		std::cerr << "socket creation failed: " << strerror(errno) << std::endl;
		return -1;
	}
	std::cout << sockfd << std::endl;

	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);

	int ret = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
	if (ret < 0) {
		std::cerr << "bind failed: " << strerror(errno) << std::endl;
		return -1;
	}

	ret = listen(sockfd, 5);
	if (ret < 0) {
		std::cerr << "listen failed: " << strerror(errno) << std::endl;
		return -1;
	}

	struct pollfd fds[MAX_EVENTS];
	memset(fds, 0, sizeof(fds));
	fds[0].fd = sockfd;
	fds[0].events = POLLIN;

	int nfds = 1;
	while (true) {
		int ret = poll(fds, nfds, -1);
		if (ret == -1) {
			std::cerr << "poll failed: " << strerror(errno) << std::endl;
			break;
		}

		for (int i = 0; i < nfds; i++) {
			if (fds[i].revents & POLLIN) {
				if (fds[i].fd == sockfd) {
					struct sockaddr_in client_address;
					memset(&client_address, 0, sizeof(client_address));
					socklen_t client_len = sizeof(client_address);
					int client_sockfd = accept(sockfd, (struct sockaddr *)&client_address, &client_len);
					if (client_sockfd < 0) {
						std::cerr << "accept failed: " << strerror(errno) << std::endl;
						break;
					}

					fds[nfds].fd = client_sockfd;
					fds[nfds].events = POLLIN;
					nfds++;
				}
				else {
					char buffer[1024];
					memset(buffer, 0, sizeof(buffer));
					int ret = recv(fds[i].fd, buffer, sizeof(buffer), 0);
					if (ret <= 0) {
						if (ret < 0) {
							std::cerr << "recv failed: " << strerror(errno) << std::endl;
						}
						close(fds[i].fd);
						fds[i].fd = -1;

						// Shift remaining clients to fill the gap
						for (int j = i; j < nfds - 1; j++) {
							fds[j] = fds[j + 1];
						}

						nfds--;
					}
					else {
						std::cout << "Received message: " << buffer << std::endl;

						// Example: Send a response back to the client
						const char* response = "Server received your message";
						ret = send(fds[i].fd, response, strlen(response), 0);
						if (ret < 0) {
							std::cerr << "send failed: " << strerror(errno) << std::endl;
						}
					}
				}
			}
		}
	}

	// Close remaining client connections
	for (int i = 0; i < nfds; i++) {
		if (fds[i].fd != -1) {
			close(fds[i].fd);
		}
	}

	close(sockfd);

	return 0;
}
