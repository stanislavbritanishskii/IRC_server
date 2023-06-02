#include "MessageManager.hpp"


MessageManager::MessageManager() {}

MessageManager::~MessageManager() {}

int MessageManager::parseMessage(char *buffer)
{
	std::cout << "[";
	for (int i = 0; buffer[i]; i++)
	{
		if (std::isprint(buffer[i]))
			std::cout << buffer[i];
		else
			std::cout << "\\" << (int)buffer[i];
	}
	std::cout << "]" << std::endl;
	return 1;
}
