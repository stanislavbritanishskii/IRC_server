#ifndef MESSAGEMANAGER_HPP
#define MESSAGEMANAGER_HPP

#include <iostream>

class MessageManager
{
public:
	MessageManager();
	~MessageManager();
	int parseMessage(char *buffer);
};

#endif