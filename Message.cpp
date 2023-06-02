#include "Message.hpp"

const std::vector<std::string> ACTIONS()
{
	std::vector<std::string> actions;
	actions.push_back("PASS");
	actions.push_back("NICK");
	actions.push_back("QUIT");
	actions.push_back("USER");
	actions.push_back("JOIN");
	actions.push_back("PRIVMSG");
	actions.push_back("NOTICE");
	actions.push_back("PING");
	actions.push_back("KICK");
	actions.push_back("CAP");
	actions.push_back("WHO");
	actions.push_back("PART");
	actions.push_back("LIST");
	actions.push_back("MODE");
	actions.push_back("TOPIC");
	actions.push_back("INVITE");

	return actions;
}
