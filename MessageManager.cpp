#include "MessageManager.hpp"


MessageManager::MessageManager() {}

MessageManager::~MessageManager() {}

int MessageManager::findMessageAction(std::string text)
{
	for (int i = 0; i < ACTIONS().size(); i++)
		if (ACTIONS()[i] == text)
			return i;
	return -1;
}

//std::istringstream iss(string)

void MessageManager::doKick(std::vector<std::string> args, Message &message)
{
	std::cout << "args size = " << args.size()<<std::endl;
	if (args.size() < 3 || args[1][0] != '#' || !std::isprint(args[1][1]))
	{
		message.targetChat = "";
		message.targetNick = "";
	}
	else
	{
		message.targetChat = args[1].substr(1, -1);
		message.targetNick = args[2];
	}
}

Message MessageManager::parseMessage(char *buffer, int i)
{
	std::vector<std::string> args;

	Message message;
	std::string str(buffer);
	std::istringstream iss(str);
	std::string token;

	while (iss >> token)
		args.push_back(token);

//	std::cout << token << std::endl;

	message.action = findMessageAction(args[0]);
	message.user_id = i;

	if (message.action == KICK)
		doKick(args,message);
	std::cout << "here2" << std::endl;


//	std::cout << message.targetChat << std::endl;


//	std::cout << "[";
//	for (int i = 0; buffer[i]; i++)
//	{
//		if (std::isprint(buffer[i]))
//			std::cout << buffer[i];
//		else
//			std::cout << "\\" << (int)buffer[i];
//	}
//	std::cout << "]" << std::endl;
	return message;
}
