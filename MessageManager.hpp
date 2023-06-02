#ifndef MESSAGEMANAGER_HPP
#define MESSAGEMANAGER_HPP

#include <iostream>
#include "Message.hpp"
#include <sstream>
#include <string>
class MessageManager
{
public:
	MessageManager();
	~MessageManager();
	Message parseMessage(char *buffer, int i);
	int findMessageAction(std::string text);
	void doKick(std::vector<std::string> args, Message &message);
	void doInvite(std::istringstream &iss, Message &message);
};

#endif