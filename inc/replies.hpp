#pragma once

/* 5 REPLIES */
/* 5.1 COMMAND REPLIES */

// 001     RPL_WELCOME
// Returned by the server to indicate the successful registration of a user.
#define RPL_WELCOME(server, nick, user, host) (":" + server + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")

// 002     RPL_YOURHOST
// Returned by the server to provide the client with the server's hostname and version.
#define RPL_YOURHOST(server, nick, servername, ver) (":" + server + " 002 " + nick + " :Your host is " + servername + ", running version " + ver + "\r\n")

// 003     RPL_CREATED
// Returned by the server to provide the client with the date the server was created.
#define RPL_CREATED(server, nick, date) (":" + server + " 003 " + nick + " :This server was created " + date + "\r\n")

// 004     RPL_MYINFO
// Returned by the server to provide the client with the server's information.
#define RPL_MYINFO(server, nick, servername, version, userModes, channelModes) (":" + server + " 004 " + nick + " " + servername + " " + version + " " + userModes + " " + channelModes + "\r\n")







/*
 * 6. REPLIES
 * The following is a list of numeric replies which are generated in response
 * to the commands given above. Each numeric is given with its number, name and
 * reply string. 
 * */

/* 6.1 Error Replies. */


//  401     ERR_NOSUCHNICK
//  Used to indicate the nickname parameter supplied to a
//  command is currently unused.
#define ERR_NOSUCHNICK(server, nick) (":" + server + " 401 " + nick + " :No such nick/channel\r\n")

//  402     ERR_NOSUCHSERVER
//  Used to indicate the server name given currently
//  doesn't exist.
#define ERR_NOSUCHSERVER(server, nick) (":" + server + " 402 " + nick + " :No such server\r\n")

//  403     ERR_NOSUCHCHANNEL
//  Used to indicate the given channel name is invalid.
#define ERR_NOSUCHCHANNEL(server, nick, channel) (":" + server + " 403 " + nick + " " + channel + " :No such channel\r\n")

//  404     ERR_CANNOTSENDTOCHAN
//  Sent to a user who is either (a) not on a channel
//  which is mode +n or (b) not a chanop (or mode +v) on
//  a channel which has mode +m set or where the user is
//  banned and is trying to send a PRIVMSG message to
//  that channel.
#define ERR_CANNOTSENDTOCHAN(server, nick, channel) (":" + server + " 404 " + nick + " " + channel + " :Cannot send to channel\r\n")

//  405     ERR_TOOMANYCHANNELS
//  Sent to a user when they have joined the maximum
//  number of allowed channels and they try to join
//  another channel.
#define ERR_TOOMANYCHANNELS(server, nick, channel) (":" + server + " 405 " + nick + " " + channel + " :You have joined too many channels\r\n")

//  406     ERR_WASNOSUCHNICK
//  Returned by WHOWAS to indicate there is no history
//  information for that nickname.
#define ERR_WASNOSUCHNICK(server, nick, wasNick) (":" + server + " 406 " + nick + " " + wasNick + " :There was no such nickname\r\n")

//  407     ERR_TOOMANYTARGETS
//  Returned to a client which is attempting to send a
//  PRIVMSG/NOTICE using the user@host destination format
//  and for a destination which has several occurrences.
#define ERR_TOOMANYTARGETS(server, nick, target) (":" + server + " 407 " + nick + " " + target + " :Too many targets\r\n")

//  408     ERR_NOSUCHSERVICE
//  Returned to a client which is attempting to send a
//  SERVICE message to a service that does not exist.
#define ERR_NOSUCHSERVICE(server, nick, service) (":" + server + " 408 " + nick + " " + service + " :No such service\r\n")

//  409     ERR_NOORIGIN
//  PING or PONG message missing the originator parameter.
#define ERR_NOORIGIN(server, nick) (":" + server + " 409 " + nick + " :No origin specified\r\n")

//  411     ERR_NORECIPIENT
//  Returned when no recipient is given with a command.
#define ERR_NORECIPIENT(server, nick, command) (":" + server + " 411 " + nick + " :No recipient given (" + command + ")\r\n")

//  412     ERR_NOTEXTTOSEND
//  Returned when no text to send is provided.
#define ERR_NOTEXTTOSEND(server, nick) (":" + server + " 412 " + nick + " :No text to send\r\n")

//  413     ERR_NOTOPLEVEL
//  Returned when a mask specified in a command is
//  not in a valid format.
#define ERR_NOTOPLEVEL(server, nick, mask) (":" + server + " 413 " + nick + " " + mask + " :No toplevel domain specified\r\n")

//  414     ERR_WILDTOPLEVEL
//  Returned when a mask specified in a command contains
//  a wildcard for a toplevel domain.
#define ERR_WILDTOPLEVEL(server, nick, mask) (":" + server + " 414 " + nick + " " + mask + " :Wildcard in toplevel domain\r\n")

//  421     ERR_UNKNOWNCOMMAND
//  Returned to a registered client to indicate that the
//  command sent is unknown by the server.
#define ERR_UNKNOWNCOMMAND(server, nick, command) (":" + server + " 421 " + nick + " " + command + " :Unknown command\r\n")

//  422     ERR_NOMOTD
//  Returned by a server in response to a MOTD message
//  to indicate that the message of the day file was
//  missing.
#define ERR_NOMOTD(server, nick) (":" + server + " 422 " + nick + " :MOTD File is missing\r\n")

//  423     ERR_NOADMININFO
//  Returned by a server in response to an ADMIN message
//  when there is an error in finding the appropriate
//  information.
#define ERR_NOADMININFO(server, nick) (":" + server + " 423 " + nick + " " + server + " :No administrative info available\r\n")

//  424     ERR_FILEERROR
//  Generic error message used to report a file error
//  to the client.
#define ERR_FILEERROR(server, nick, fileop, filename) (":" + server + " 424 " + nick + " :File error doing " + fileop + " on " + filename + "\r\n")

//  431     ERR_NONICKNAMEGIVEN
//  Returned when a nickname parameter expected for a
//  command and isn't found.
#define ERR_NONICKNAMEGIVEN(server, nick) (":" + server + " 431 " + nick + " :No nickname given\r\n")

//  432     ERR_ERRONEUSNICKNAME
//  Returned after receiving a NICK message which contains
//  characters which do not fall within the defined set.
#define ERR_ERRONEUSNICKNAME(server, nick, erroneousNick) (":" + server + " 432 " + nick + " " + erroneousNick + " :Erroneous nickname\r\n")

//  433     ERR_NICKNAMEINUSE
//  Returned when a NICK message is processed that
//  results in an attempt to change to a currently
//  existing nickname.
#define ERR_NICKNAMEINUSE(server, nick, existingNick) (":" + server + " 433 " + nick + " " + existingNick + " :Nickname is already in use\r\n")

//  436     ERR_NICKCOLLISION
//  Returned by a server to a client when it detects a
//  nickname collision (registered nickname by another
//  server).
#define ERR_NICKCOLLISION(server, nick, conflictingNick) (":" + server + " 436 " + nick + " " + conflictingNick + " :Nickname collision KILL\r\n")

//  441     ERR_USERNOTINCHANNEL
//  Returned by the server to indicate that the target
//  user of the command is not on the given channel.
#define ERR_USERNOTINCHANNEL(server, nick, user, channel) (":" + server + " 441 " + nick + " " + user + " " + channel + " :They aren't on that channel\r\n")

//  442     ERR_NOTONCHANNEL
//  Returned by the server whenever a client tries to
//  perform a channel effecting command for which the
//  client isn't a member.
#define ERR_NOTONCHANNEL(server, nick, channel) (":" + server + " 442 " + nick + " " + channel + " :You're not on that channel\r\n")

//  443     ERR_USERONCHANNEL
//  Returned when a client tries to invite a user to a
//  channel they are already on.
#define ERR_USERONCHANNEL(server, nick, user, channel) (":" + server + " 443 " + nick + " " + user + " " + channel + " :is already on channel\r\n")

//  444     ERR_NOLOGIN
//  Returned by the summon after a SUMMON command for a
//  user was unable to be performed since they are not
//  logged in.
#define ERR_NOLOGIN(server, nick, user) (":" + server + " 444 " + nick + " " + user + " :User not logged in\r\n")

//  445     ERR_SUMMONDISABLED
//  Returned as a response to the SUMMON command. Must be
//  returned by any server which doesn't implement it.
#define ERR_SUMMONDISABLED(server, nick) (":" + server + " 445 " + nick + " :SUMMON has been disabled\r\n")

//  446     ERR_USERSDISABLED
//  Returned as a response to the USERS command. Must be
//  returned by any server which does not implement it.
#define ERR_USERSDISABLED(server, nick) (":" + server + " 446 " + nick + " :USERS has been disabled\r\n")

//  451     ERR_NOTREGISTERED
//  Returned by the server to indicate that the client
//  must be registered before the command is accepted.
#define ERR_NOTREGISTERED(server, nick) (":" + server + " 451 " + nick + " :You have not registered\r\n")

//  461     ERR_NEEDMOREPARAMS
//  Returned by the server by any command which requires
//  parameters and is used to indicate the client didn't
//  supply enough parameters.
#define ERR_NEEDMOREPARAMS(server, nick, command) (":" + server + " 461 " + nick + " " + command + " :Not enough parameters\r\n")

//  462     ERR_ALREADYREGISTRED
//  Returned by the server to any link which tries to
//  change part of the registered details (such as
//  password or user details) and to indicate that this
//  is not allowed.
#define ERR_ALREADYREGISTRED(server, nick) (":" + server + " 462 " + nick + " :You may not reregister\r\n")

//  463     ERR_NOPERMFORHOST
//  Returned to a client which attempts to register with
//  a server which does not allow connections from the
//  client's host.
#define ERR_NOPERMFORHOST(server, nick) (":" + server + " 463 " + nick + " :Your host isn't among the privileged\r\n")

//  464     ERR_PASSWDMISMATCH
//  Returned to indicate a failed attempt at registering
//  a connection for which a password was required and
//  was either not given or incorrect.
#define ERR_PASSWDMISMATCH(server, nick) (":" + server + " 464 " + nick + " :Password incorrect\r\n")

//  465     ERR_YOUREBANNEDCREEP
//  Returned after an attempt to connect and register
//  yourself with a server which has been setup to
//  explicitly deny connections to you.
#define ERR_YOUREBANNEDCREEP(server, nick) (":" + server + " 465 " + nick + " :You are banned from this server\r\n")

//  466     ERR_YOUWILLBEBANNED
//  Sent by a server to a user to inform that access to
//  the server will soon be restricted.
#define ERR_YOUWILLBEBANNED(server, nick) (":" + server + " 466 " + nick + " :You will be banned from this server\r\n")

//  467     ERR_KEYSET
//  Returned when the channel key for a channel has
//  already been set.
#define ERR_KEYSET(server, nick, channel) (":" + server + " 467 " + nick + " " + channel + " :Channel key already set\r\n")

//  471     ERR_CHANNELISFULL
//  Returned when attempting to join a channel that is
//  already full.
#define ERR_CHANNELISFULL(server, nick, channel) (":" + server + " 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n")

//  472     ERR_UNKNOWNMODE
//  Returned when a client tries to set a channel mode
//  that does not exist.
#define ERR_UNKNOWNMODE(server, nick, modeChar) (":" + server + " 472 " + nick + " " + modeChar + " :is unknown mode char to me\r\n")

//  473     ERR_INVITEONLYCHAN
//  Returned when attempting to join a channel that is
//  invite-only.
#define ERR_INVITEONLYCHAN(server, nick, channel) (":" + server + " 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n")

//  474     ERR_BANNEDFROMCHAN
//  Returned when attempting to join a channel that the
//  user is banned from.
#define ERR_BANNEDFROMCHAN(server, nick, channel) (":" + server + " 474 " + nick + " " + channel + " :Cannot join channel (+b)\r\n")

//  475     ERR_BADCHANNELKEY
//  Returned when attempting to join a channel with an
//  incorrect key.
#define ERR_BADCHANNELKEY(server, nick, channel) (":" + server + " 475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n")

//  476     ERR_BADCHANMASK
//  Returned when a client tries to join a channel with
//  a mask that does not exist.
#define ERR_BADCHANMASK(server, nick, channel) (":" + server + " 476 " + nick + " " + channel + " :Bad Channel Mask\r\n")

//  477     ERR_NOCHANMODES
//  Returned when a client tries to set channel modes on
//  a channel that does not support modes.
#define ERR_NOCHANMODES(server, nick, channel) (":" + server + " 477 " + nick + " " + channel + " :Channel doesn't support modes\r\n")

//  478     ERR_BANLISTFULL
//  Returned when a client tries to add a ban to a
//  channel but the channel ban list is full.
#define ERR_BANLISTFULL(server, nick, channel, banMask) (":" + server + " 478 " + nick + " " + channel + " " + banMask + " :Channel ban list is full\r\n")

//  481     ERR_NOPRIVILEGES
//  Returned when a client tries to perform an operator
//  command but does not have the necessary privileges.
#define ERR_NOPRIVILEGES(server, nick) (":" + server + " 481 " + nick + " :Permission Denied- You're not an IRC operator\r\n")

//  482     ERR_CHANOPRIVSNEEDED
//  Returned when a client tries to perform a channel
//  operator command but does not have the necessary
//  privileges.
#define ERR_CHANOPRIVSNEEDED(server, nick, channel) (":" + server + " 482 " + nick + " " + channel + " :You're not channel operator\r\n")

//  483     ERR_CANTKILLSERVER
//  Returned when a client tries to kill a server.
#define ERR_CANTKILLSERVER(server, nick) (":" + server + " 483 " + nick + " :You can't kill a server!\r\n")

//  484     ERR_RESTRICTED
//  Returned when a client tries to perform an action
//  that is restricted.
#define ERR_RESTRICTED(server, nick) (":" + server + " 484 " + nick + " :Your connection is restricted!\r\n")

//  485     ERR_UNIQOPPRIVSNEEDED
//  Returned when a client tries to perform a unique
//  operator command but does not have the necessary
//  privileges.
#define ERR_UNIQOPPRIVSNEEDED(server, nick) (":" + server + " 485 " + nick + " :You're not the original channel operator\r\n")

//  491     ERR_NOOPERHOST
//  Returned when a client tries to perform an operator
//  command from a host that is not permitted.
#define ERR_NOOPERHOST(server, nick) (":" + server + " 491 " + nick + " :No O-lines for your host\r\n")

//  501     ERR_UMODEUNKNOWNFLAG
//  Returned when a client tries to set a user mode flag
//  that is not recognized.
#define ERR_UMODEUNKNOWNFLAG(server, nick) (":" + server + " 501 " + nick + " :Unknown MODE flag\r\n")

//  502     ERR_USERSDONTMATCH
//  Returned when a client tries to change a user mode
//  for a user other than themselves.
#define ERR_USERSDONTMATCH(server, nick) (":" + server + " 502 " + nick + " :Cannot change mode for other users\r\n")

// 6.2 Command responses.

//  300     RPL_NONE
//  Dummy reply, not used by servers.
#define RPL_NONE(server, nick) (":" + server + " 300 " + nick + " :Dummy reply\r\n")

//  302     RPL_USERHOST
//  Reply format used by USERHOST to list replies to the
//  query list.
#define RPL_USERHOST(server, nick, reply) (":" + server + " 302 " + nick + " :" + reply + "\r\n")

//  303     RPL_ISON
//  Reply format used by ISON to list replies to the
//  query list.
#define RPL_ISON(server, nick, reply) (":" + server + " 303 " + nick + " :" + reply + "\r\n")

//  301     RPL_AWAY
//  Reply used by AWAY command when the user is away.
#define RPL_AWAY(server, nick, user, message) (":" + server + " 301 " + nick + " " + user + " :" + message + "\r\n")

//  305     RPL_UNAWAY
//  Reply used by AWAY command to indicate the user is
//  no longer marked as being away.
#define RPL_UNAWAY(server, nick) (":" + server + " 305 " + nick + " :You are no longer marked as being away\r\n")

//  306     RPL_NOWAWAY
//  Reply used by AWAY command to indicate the user is
//  now marked as being away.
#define RPL_NOWAWAY(server, nick) (":" + server + " 306 " + nick + " :You have been marked as being away\r\n")

//  311     RPL_WHOISUSER
//  Reply format for WHOIS query to return information
//  about a specific user.
#define RPL_WHOISUSER(server, requester, user, userName, host, realName) (":" + server + " 311 " + requester + " " + user + " " + userName + " " + host + " * :" + realName + "\r\n")

//  312     RPL_WHOISSERVER
//  Reply format for WHOIS query to return information
//  about which server the user is on.
#define RPL_WHOISSERVER(server, requester, user, serverName, serverInfo) (":" + server + " 312 " + requester + " " + user + " " + serverName + " :" + serverInfo + "\r\n")

//  313     RPL_WHOISOPERATOR
//  Reply format for WHOIS query to indicate that the
//  user is an IRC operator.
#define RPL_WHOISOPERATOR(server, requester, user) (":" + server + " 313 " + requester + " " + user + " :is an IRC operator\r\n")

//  317     RPL_WHOISIDLE
//  Reply format for WHOIS query to return idle information
//  of a user.
#define RPL_WHOISIDLE(server, requester, user, idleTime, signonTime) (":" + server + " 317 " + requester + " " + user + " " + idleTime + " " + signonTime + " :seconds idle, signon time\r\n")

//  318     RPL_ENDOFWHOIS
//  Reply format to indicate the end of WHOIS query responses.
#define RPL_ENDOFWHOIS(server, requester, user) (":" + server + " 318 " + requester + " " + user + " :End of /WHOIS list.\r\n")

//  319     RPL_WHOISCHANNELS
//  Reply format for WHOIS query to return the channels
//  a user is on.
#define RPL_WHOISCHANNELS(server, requester, user, channels) (":" + server + " 319 " + requester + " " + user + " :" + channels + "\r\n")

//  314     RPL_WHOWASUSER
//  Reply format for WHOWAS query to return information
//  about a user who no longer exists.
#define RPL_WHOWASUSER(server, requester, user, userName, host, realName) (":" + server + " 314 " + requester + " " + user + " " + userName + " " + host + " * :" + realName + "\r\n")

//  369     RPL_ENDOFWHOWAS
//  Reply format to indicate the end of WHOWAS query responses.
#define RPL_ENDOFWHOWAS(server, requester, user) (":" + server + " 369 " + requester + " " + user + " :End of WHOWAS\r\n")

//  321     RPL_LISTSTART
//  Reply format to indicate the start of a LIST query response.
#define RPL_LISTSTART(server, nick) (":" + server + " 321 " + nick + " Channel :Users  Name\r\n")

//  322     RPL_LIST
//  Reply format to return the details of a channel in
//  response to a LIST query.
#define RPL_LIST(server, nick, channel, numVisible, topic) (":" + server + " 322 " + nick + " " + channel + " " + numVisible + " :" + topic + "\r\n")

//  323     RPL_LISTEND
//  Reply format to indicate the end of LIST query responses.
#define RPL_LISTEND(server, nick) (":" + server + " 323 " + nick + " :End of /LIST\r\n")

//  325     RPL_UNIQOPIS
//  Reply format to return the unique operator of a channel.
#define RPL_UNIQOPIS(server, nick, channel, uniqOp) (":" + server + " 325 " + nick + " " + channel + " " + uniqOp + "\r\n")

//  324     RPL_CHANNELMODEIS
//  Reply format to return the current channel modes.
#define RPL_CHANNELMODEIS(server, nick, channel, modes) (":" + server + " 324 " + nick + " " + channel + " " + modes + "\r\n")

//  331     RPL_NOTOPIC
//  Reply format to indicate that a channel has no topic set.
#define RPL_NOTOPIC(server, nick, channel) (":" + server + " 331 " + nick + " " + channel + " :No topic is set\r\n")

//  332     RPL_TOPIC
//  Reply format to return the topic of a channel.
#define RPL_TOPIC(server, nick, channel, topic) (":" + server + " 332 " + nick + " " + channel + " :" + topic + "\r\n")

//  341     RPL_INVITING
//  Reply format to indicate that an invitation has been
//  sent to a user.
#define RPL_INVITING(server, nick, invitee, channel) (":" + server + " 341 " + nick + " " + invitee + " " + channel + "\r\n")

//  342     RPL_SUMMONING
//  Reply format to indicate that a user is being summoned.
#define RPL_SUMMONING(server, nick, user) (":" + server + " 342 " + nick + " " + user + " :Summoning user to IRC\r\n")

//  346     RPL_INVITELIST
//  Reply format to return an entry in the channel's invite list.
#define RPL_INVITELIST(server, nick, channel, inviteMask) (":" + server + " 346 " + nick + " " + channel + " " + inviteMask + "\r\n")

//  347     RPL_ENDOFINVITELIST
//  Reply format to indicate the end of the channel's invite list.
#define RPL_ENDOFINVITELIST(server, nick, channel) (":" + server + " 347 " + nick + " " + channel + " :End of channel invite list\r\n")

//  348     RPL_EXCEPTLIST
//  Reply format to return an entry in the channel's exception list.
#define RPL_EXCEPTLIST(server, nick, channel, exceptionMask) (":" + server + " 348 " + nick + " " + channel + " " + exceptionMask + "\r\n")

//  349     RPL_ENDOFEXCEPTLIST
//  Reply format to indicate the end of the channel's exception list.
#define RPL_ENDOFEXCEPTLIST(server, nick, channel) (":" + server + " 349 " + nick + " " + channel + " :End of channel exception list\r\n")

//  351     RPL_VERSION
//  Reply format to return the server version.
#define RPL_VERSION(server, nick, version, serverName, comments) (":" + server + " 351 " + nick + " " + version + " " + serverName + " :" + comments + "\r\n")

//  352     RPL_WHOREPLY
//  Reply format to return details about a user in response
//  to a WHO query.
#define RPL_WHOREPLY(server, requester, channel, user, host, serverName, nick, flags, hops, realName) (":" + server + " 352 " + requester + " " + channel + " " + user + " " + host + " " + serverName + " " + nick + " " + flags + " " + hops + " :" + realName + "\r\n")

//  315     RPL_ENDOFWHO
//  Reply format to indicate the end of WHO query responses.
#define RPL_ENDOFWHO(server, requester, name) (":" + server + " 315 " + requester + " " + name + " :End of /WHO list.\r\n")

//  353     RPL_NAMREPLY
//  Reply format to return the list of users on a channel.
#define RPL_NAMREPLY(server, requester, channelType, channel, nickList) (":" + server + " 353 " + requester + " " + channelType + " " + channel + " :" + nickList + "\r\n")

//  366     RPL_ENDOFNAMES
//  Reply format to indicate the end of NAMES query responses.
#define RPL_ENDOFNAMES(server, requester, channel) (":" + server + " 366 " + requester + " " + channel + " :End of /NAMES list.\r\n")

//  364     RPL_LINKS
//  Reply format to return details about a server link.
#define RPL_LINKS(server, requester, mask, serverName, hops, serverInfo) (":" + server + " 364 " + requester + " " + mask + " " + serverName + " " + hops + " :" + serverInfo + "\r\n")

//  365     RPL_ENDOFLINKS
//  Reply format to indicate the end of LINKS query responses.
#define RPL_ENDOFLINKS(server, requester, mask) (":" + server + " 365 " + requester + " " + mask + " :End of /LINKS list.\r\n")

//  367     RPL_BANLIST
//  Reply format to return an entry in the channel's ban list.
#define RPL_BANLIST(server, nick, channel, banMask) (":" + server + " 367 " + nick + " " + channel + " " + banMask + "\r\n")

//  368     RPL_ENDOFBANLIST
//  Reply format to indicate the end of the channel's ban list.
#define RPL_ENDOFBANLIST(server, nick, channel) (":" + server + " 368 " + nick + " " + channel + " :End of channel ban list\r\n")

//  371     RPL_INFO
//  Reply format to return information about the server.
#define RPL_INFO(server, nick, info) (":" + server + " 371 " + nick + " :" + info + "\r\n")

//  374     RPL_ENDOFINFO
//  Reply format to indicate the end of INFO query responses.
#define RPL_ENDOFINFO(server, nick) (":" + server + " 374 " + nick + " :End of /INFO list.\r\n")

//  375     RPL_MOTDSTART
//  Reply format to indicate the start of the Message of the Day.
#define RPL_MOTDSTART(server, nick) (":" + server + " 375 " + nick + " :- " + server + " Message of the day - \r\n")

//  372     RPL_MOTD
//  Reply format to return a line of the Message of the Day.
# define RPL_MOTD(server, nick)(":" + server + " 372 " + nick + " :- ")

//  376     RPL_ENDOFMOTD
//  Reply format to indicate the end of the Message of the Day.
#define RPL_ENDOFMOTD(server, nick) (":" + server + " 376 " + nick + " :End of /MOTD command.\r\n")

//  381     RPL_YOUREOPER
//  Reply format to indicate that a user has successfully
//  become an IRC operator.
#define RPL_YOUREOPER(server, nick) (":" + server + " 381 " + nick + " :You are now an IRC operator\r\n")

//  382     RPL_REHASHING
//  Reply format to indicate that the server is rehashing.
#define RPL_REHASHING(server, nick, configFile) (":" + server + " 382 " + nick + " " + configFile + " :Rehashing\r\n")

//  391     RPL_TIME
//  Reply format to return the local time of the server.
#define RPL_TIME(server, nick, serverName, timeString) (":" + server + " 391 " + nick + " " + serverName + " :" + timeString + "\r\n")

//  392     RPL_USERSSTART
//  Reply format to indicate the start of a USERS query response.
#define RPL_USERSSTART(server, nick) (":" + server + " 392 " + nick + " :UserID   Terminal  Host\r\n")

//  393     RPL_USERS
//  Reply format to return details about a user in response
//  to a USERS query.
#define RPL_USERS(server, nick, userID, terminal, host) (":" + server + " 393 " + nick + " :" + userID + " " + terminal + " " + host + "\r\n")

//  394     RPL_ENDOFUSERS
//  Reply format to indicate the end of USERS query responses.
#define RPL_ENDOFUSERS(server, nick) (":" + server + " 394 " + nick + " :End of users\r\n")

//  395     RPL_NOUSERS
//  Reply format to indicate that there are no users logged in.
#define RPL_NOUSERS(server, nick) (":" + server + " 395 " + nick + " :Nobody logged in\r\n")

//  200     RPL_TRACELINK
//  Reply format for TRACE query to indicate a link in the
//  path to the server.
#define RPL_TRACELINK(server, nick, version, destination, nextServer) (":" + server + " 200 " + nick + " " + version + " " + destination + " " + nextServer + "\r\n")

//  201     RPL_TRACECONNECTING
//  Reply format for TRACE query to indicate an attempt
//  to connect to a server.
#define RPL_TRACECONNECTING(server, nick, classType, serverName) (":" + server + " 201 " + nick + " " + classType + " " + serverName + "\r\n")

//  202     RPL_TRACEHANDSHAKE
//  Reply format for TRACE query to indicate a handshake
//  with a server.
#define RPL_TRACEHANDSHAKE(server, nick, classType, serverName) (":" + server + " 202 " + nick + " " + classType + " " + serverName + "\r\n")

//  203     RPL_TRACEUNKNOWN
//  Reply format for TRACE query to indicate an unknown
//  connection.
#define RPL_TRACEUNKNOWN(server, nick, classType, ipAddress) (":" + server + " 203 " + nick + " " + classType + " " + ipAddress + "\r\n")

//  204     RPL_TRACEOPERATOR
//  Reply format for TRACE query to indicate an operator.
#define RPL_TRACEOPERATOR(server, nick, classType, user) (":" + server + " 204 " + nick + " " + classType + " " + user + "\r\n")

//  205     RPL_TRACEUSER
//  Reply format for TRACE query to indicate a user.
#define RPL_TRACEUSER(server, nick, classType, user) (":" + server + " 205 " + nick + " " + classType + " " + user + "\r\n")

//  206     RPL_TRACESERVER
//  Reply format for TRACE query to indicate a server.
#define RPL_TRACESERVER(server, nick, classType, serverName, hops, info) (":" + server + " 206 " + nick + " " + classType + " " + serverName + " " + hops + " " + info + "\r\n")

//  208     RPL_TRACENEWTYPE
//  Reply format for TRACE query to indicate a new type
//  of connection.
#define RPL_TRACENEWTYPE(server, nick, newType, clientName) (":" + server + " 208 " + nick + " " + newType + " 0 " + clientName + "\r\n")

//  211     RPL_STATSLINKINFO
//  Reply format for STATS query to return link information.
#define RPL_STATSLINKINFO(server, nick, linkName, sendq, sentMsg, sentBytes, recvMsg, recvBytes, timeOpen) (":" + server + " 211 " + nick + " " + linkName + " " + sendq + " " + sentMsg + " " + sentBytes + " " + recvMsg + " " + recvBytes + " " + timeOpen + "\r\n")

//  212     RPL_STATSCOMMANDS
//  Reply format for STATS query to return command statistics.
#define RPL_STATSCOMMANDS(server, nick, command, count) (":" + server + " 212 " + nick + " " + command + " " + count + "\r\n")

//  213     RPL_STATSCLINE
//  Reply format for STATS query to return C/N line information.
#define RPL_STATSCLINE(server, nick, host, name, port, classType) (":" + server + " 213 " + nick + " C " + host + " * " + name + " " + port + " " + classType + "\r\n")

//  214     RPL_STATSNLINE
//  Reply format for STATS query to return N line information.
#define RPL_STATSNLINE(server, nick, host, name, port, classType) (":" + server + " 214 " + nick + " N " + host + " * " + name + " " + port + " " + classType + "\r\n")

//  215     RPL_STATSILINE
//  Reply format for STATS query to return I line information.
#define RPL_STATSILINE(server, nick, host, name, port, classType) (":" + server + " 215 " + nick + " I " + host + " * " + name + " " + port + " " + classType + "\r\n")

//  216     RPL_STATSKLINE
//  Reply format for STATS query to return K line information.
#define RPL_STATSKLINE(server, nick, host, name, port, classType) (":" + server + " 216 " + nick + " K " + host + " * " + name + " " + port + " " + classType + "\r\n")

//  218     RPL_STATSYLINE
//  Reply format for STATS query to return Y line information.
#define RPL_STATSYLINE(server, nick, classType, pingFrequency, connectFrequency, maxSendq) (":" + server + " 218 " + nick + " Y " + classType + " " + pingFrequency + " " + connectFrequency + " " + maxSendq + "\r\n")

//  219     RPL_ENDOFSTATS
//  Reply format to indicate the end of STATS query responses.
#define RPL_ENDOFSTATS(server, nick, statsLetter) (":" + server + " 219 " + nick + " " + statsLetter + " :End of /STATS report.\r\n")

//  241     RPL_STATSLLINE
//  Reply format for STATS query to return L line information.
#define RPL_STATSLLINE(server, nick, host, name, port, classType) (":" + server + " 241 " + nick + " L " + host + " * " + name + " " + port + " " + classType + "\r\n")

//  242     RPL_STATSUPTIME
//  Reply format for STATS query to return server uptime.
#define RPL_STATSUPTIME(server, nick, days, hours, minutes, seconds) (":" + server + " 242 " + nick + " :Server Up " + days + " days " + hours + ":" + minutes + ":" + seconds + "\r\n")

//  243     RPL_STATSOLINE
//  Reply format for STATS query to return O line information.
#define RPL_STATSOLINE(server, nick, host, name, port, classType) (":" + server + " 243 " + nick + " O " + host + " * " + name + " " + port + " " + classType + "\r\n")

//  244     RPL_STATSHLINE
//  Reply format for STATS query to return H line information.
#define RPL_STATSHLINE(server, nick, host, name, port, classType) (":" + server + " 244 " + nick + " H " + host + " * " + name + " " + port + " " + classType + "\r\n")

//  251     RPL_LUSERCLIENT
//  Reply format to return the number of users and servers
//  connected to the IRC network.
#define RPL_LUSERCLIENT(server, nick, message) (":" + server + " 251 " + nick + " :" + message + "\r\n")

//  252     RPL_LUSEROP
//  Reply format to return the number of IRC operators online.
#define RPL_LUSEROP(server, nick, count) (":" + server + " 252 " + nick + " " + count + " :operator(s) online\r\n")

//  253     RPL_LUSERUNKNOWN
//  Reply format to return the number of unknown connections.
#define RPL_LUSERUNKNOWN(server, nick, count) (":" + server + " 253 " + nick + " " + count + " :unknown connection(s)\r\n")

//  254     RPL_LUSERCHANNELS
//  Reply format to return the number of channels formed.
#define RPL_LUSERCHANNELS(server, nick, count) (":" + server + " 254 " + nick + " " + count + " :channels formed\r\n")

//  255     RPL_LUSERME
//  Reply format to return the number of clients and servers
//  on the network.
#define RPL_LUSERME(server, nick, message) (":" + server + " 255 " + nick + " :" + message + "\r\n")

//  256     RPL_ADMINME
//  Reply format to return the administrative information
//  about the server.
#define RPL_ADMINME(server, nick, serverName) (":" + server + " 256 " + nick + " " + serverName + " :Administrative info\r\n")

//  257     RPL_ADMINLOC1
//  Reply format to return the administrative location information.
#define RPL_ADMINLOC1(server, nick, locationInfo) (":" + server + " 257 " + nick + " :" + locationInfo + "\r\n")

//  258     RPL_ADMINLOC2
//  Reply format to return the administrative location information.
#define RPL_ADMINLOC2(server, nick, locationInfo) (":" + server + " 258 " + nick + " :" + locationInfo + "\r\n")

//  259     RPL_ADMINEMAIL
//  Reply format to return the administrative contact email.
#define RPL_ADMINEMAIL(server, nick, email) (":" + server + " 259 " + nick + " :" + email + "\r\n")

//  263     RPL_TRYAGAIN
//  Reply format to indicate that a command is being
//  rate-limited or otherwise delayed.
#define RPL_TRYAGAIN(server, nick, command) (":" + server + " 263 " + nick + " " + command + " :Please wait a while and try again.\r\n")

// MISC

#pragma once

# define RPL_WHO(server, myNick, channel, username, hostname, nick, operator, realname) (":" + server + " 352 " + myNick + " " + channel + " " + username + " " + hostname + " " + server + " "  + nick + " H" + operator + " :0 " + realname + "\r\n")

# define RPL_ENDWHO(server, myNick, channel)(":" + server + " 352 " + myNick + " " + channel + " :End of /WHO list\r\n")

# define RPL_NICK(old_nick, nick) (":" + old_nick + " NICK " + nick + "\r\n")

# define PONG(server) ("PONG " + server + "\r\n")

# define JOIN(nick, user, host, channel) (":" + nick + "!" + user + "@" + host  + " JOIN " + channel + "\r\n")

# define MODE_CHANNELMSGWITHPARAM(channel, mode, param) (":localhost MODE #" + channel + " " + mode + " " + param + "\r\n")

# define PRIVMSG(nick, user, host, channel, msg) (":" + nick + "!" + user + "@" + host + " PRIVMSG " + channel + " :" + msg + "\r\n")

# define PART(nick, user, host, chan, reason) (":" + nick + "!" + user + "@" + host + " PART " + chan + " :" + reason + "\r\n")

# define KICK(nick, user, host, chan, kicked, reason) (":" + nick + "!" + user + "@" + host + " KICK " + chan + " " + kicked + " :" + reason + "\r\n")

# define INVITE(nick, user, host, nickinvite, channel)  (":" + nick + "!" + user + "@" + host + " INVITE " + nickinvite + " " + channel + "\r\n")

# define QUIT(nick, user, host, msg) (nick + "!" + user + "@" + host  + " QUIT " + msg + "\r\n")

# define RPL_WHO(server, myNick, channel, username, hostname, nick, operator, realname) (":" + server + " 352 " + myNick + " " + channel + " " + username + " " + hostname + " " + server + " "  + nick + " H" + operator + " :0 " + realname + "\r\n")

# define RPL_ENDWHO(server, myNick, channel)(":" + server + " 352 " + myNick + " " + channel + " :End of /WHO list\r\n")

# define MODE(nick, user, host, channel, modeSymbol, object) (":" + nick + "!" + user + "@" + host + " MODE " + channel + " " + modeSymbol + " " + object + "\r\n")