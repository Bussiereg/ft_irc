#pragma once

# define ERR_PASSWDMISMATCH "464 :Password incorrect\r\n"

# define RPL_NICK(old_nick, nick) (":" + old_nick + " NICK " + nick + "\r\n")

# define ERR_NEEDMOREPARAMS(cmd) ("461 " + cmd + " :Not enough parameters\r\n")

# define RPL_WELCOME(nick, user, host) ("001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")

/* # define PONG(server) ("PONG :" + server + "\r\n") */ // Emili's version
# define PONG(server) ("PONG " + server + "\r\n") // the manual recommand without the :

# define ERR_NOORIGIN(cmd) (cmd + ":No origin specified\r\n")

# define JOIN(nick, user, host, channel) (":" + nick + "!" + user + "@" + host  + " JOIN " + channel + "\r\n")

# define ERR_NONICKNAMEGIVEN(server) (":" + server + " 431 * :No nickname given\r\n")

# define ERR_NICKNAMEINUSE(nick) ("433 " + nick + " :Nickname already in use\r\n")
# define ERR_NICKNAMEISUSE(nick) ("433 NICK :" + nick + "\r\n")

# define ERR_NOTONCHANNEL(server, channel) (":" + server + " 442 " + channel + " :You're not on that channel\r\n")

# define RPL_TOPIC(nick, user, host, channel, topic) ("332 " + nick + "!" + user + "@" + host + " " + channel + " :" + topic + "\r\n")

# define ERR_CHANOPRIVSNEEDED(channel) ("482 " + channel + " :You're not channel operator\r\n")

# define RPL_NOTOPIC(nick, user, host, channel) ("331 " + nick + "!" + user + "@" + host + " " + channel + " :No topic is set\r\n")

# define ERR_CHANNELISFULL(nick, channel) ("471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n")

# define MODE_CHANNELMSGWITHPARAM(channel, mode, param) (":localhost MODE #" + channel + " " + mode + " " + param + "\r\n")

# define PRIVMSG(nick, user, host, channel, msg) (":" + nick + "!" + user + "@" + host + " PRIVMSG " + channel + " :" + msg + "\r\n")

# define ERR_NORECIPIENT(server, nick, command) (":" + server + " 411 " + nick + " :No recipient given(" + command + ")\r\n")

# define ERR_NOTEXTTOSEND(server, nick) (":" + server + " 412 " + nick + " :No text to send\r\n")

# define PART(nick, user, host, chan) (":" + nick + "!" + user + "@" + host + " PART " + chan + "\r\n")

# define PART_REASON(nick, user, host, chan, reason) (":" + nick + "!" + user + "@" + host + " PART " + chan + " :" + reason + "\r\n")

# define ERR_USERNOTINCHANNEL(nick, chan) ("441 " + nick + " " + chan + ": They aren’t on that channel\r\n")

# define RPL_CHANNELMODEIS(server, nick, channel, mode) (":" + server + " 324 " + nick + " " + channel + " " + mode + "\r\n")

# define ERR_UNKNOWNMODE(server, nick, mmode) (":" + server + " 472 " + nick + " " + mmode + " :is unknown mode char to me\r\n")

# define ERR_UNKNOWNCOMMAND(server, nick, command) (":" + server + " 421 " + nick + " " + command + " :Unknown command\r\n")

# define ERR_ERRONEUSNICKNAME(nick) ("432 " + nick + " :Erroneous nickname\r\n")

# define RPL_YOUREOPER(nick) ("381 " + nick + " :You are now an IRC operator\r\n")

# define KICK(nick, user, host, chan, kicked, reason) (":" + nick + "!" + user + "@" + host + " KICK " + chan + " " + kicked + " :" + reason + "\r\n")

# define INVITE(nick, user, host, nickinvite, channel)  (":" + nick + "!" + user + "@" + host + " INVITE " + nickinvite + " " + channel + "\r\n")

# define ERR_NOSUCHNICK(server, sender, receiver) (":" + server + " 401 " + sender + " " + receiver + " :No such nick/channel\r\n")
# define ERR_NOSUCHNICK2(server, nick) (":" + server + " 401 " + nick + " :No such nick/channel\r\n")

# define ERR_USERONCHANNEL(nick, invitenick, chan) ("443 " + nick + " " + invitenick + " " + chan + ":is already on channel\r\n")

# define ERR_INVITEONLYCHAN(nick, chan) ("473 " + nick + " " + chan + " :Cannot join channel (+i)\r\n")

# define QUIT(nick, user, host) (nick + "!" + user + "@" + host  + " QUIT\r\n")

# define QUIT_REASON(nick, user, host, msg) (nick + "!" + user + "@" + host  + " QUIT :" + msg + "\r\n")

# define ERR_BADCHANNELKEY(nick, chan) ("475 " + nick + " " + chan + " :Cannot join channel (+k)\r\n")

# define ERR_BADCHANMASK(nick, chan) ("476 " + nick + " " + chan + " :Bad Channel Mask\r\n")
														// 353
# define RPL_NAMREPLY(server, nick, channel, nickList) (":" + server + " 353 " + nick + " = " + channel + " :" + nickList + "\r\n")

# define RPL_ENDOFNAMES(server, nick, channel) (":" + server + " 366 " + nick + " " + channel + " :End of NAMES list\r\n")

# define ERR_NOSUCHCHANNEL(channel) ("403 " + channel + " :No such channel\r\n")

# define RPL_WHP(server, myNick, channel, username, hostname, nick, operator, realname) (":" + server + " 352 " + myNick + " " + channel + " " + username + " " + hostname + " " + server + " "  + nick + " H" + operator + " :0 " + realname + "\r\n")

# define RPL_ENDWHO(server, myNick, channel)(":" + server + " 352 " + myNick + " " + channel + " :End of /WHO list\r\n")

# define RPL_INVITING(server, channel, nick)(":" + server + " 341 " + channel + " " + nick + "\r\n")