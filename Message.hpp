#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <vector>
#include <string>
#include <algorithm>
enum Actions
{
	PASS,
	NICK,
	QUIT,
	USER,
	JOIN,
	PRIVMSG,
	NOTICE, //
	PING, //
	KICK,
	CAP,
	WHO, //
	PART, //
	LIST, //
	MODE,
	TOPIC,
	INVITE
};

extern const std::vector<std::string> ACTIONS();

struct Message
{
	int user_id;
	std::string message;
	std::string targetNick;
	std::string targetChat;
	int action;
};

#endif // MESSAGE_HPP
