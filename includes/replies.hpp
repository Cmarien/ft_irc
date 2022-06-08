/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 10:09:00 by user42            #+#    #+#             */
/*   Updated: 2022/06/02 10:09:14 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

# include<string>

std::string get_replies(unsigned int rpl, std::string arg1, std::string arg2, std::string arg3, std::string arg4, std::string arg5, std::string arg6, std::string arg7);

std::string RPL_WELCOME(std::string prefix);
std::string RPL_YOURHOST(std::string servername, std::string version);
std::string RPL_CREATED(std::string date);
std::string RPL_MYINFO(std::string servername, std::string version, std::string usermodes, std::string channelmodes);
std::string RPL_BOUNCE(std::string server, std::string port);
std::string RPL_TRACELINK(std::string version, std::string dest, std::string server);
std::string RPL_TRACECONNECTING(std::string class_, std::string server);
std::string RPL_TRACEHANDSHAKE(std::string class_, std::string server);
std::string RPL_TRACEUNKNOWN(std::string class_, std::string ip);
std::string RPL_TRACEOPERATOR(std::string class_, std::string nick);
std::string RPL_TRACEUSER(std::string class_, std::string nick);
std::string RPL_TRACESERVER(std::string class_, std::string int1, std::string int2, std::string server, std::string nickname, std::string username, std::string host);
std::string RPL_TRACENEWTYPE(std::string newtype, std::string client);
std::string RPL_TRACELOG(std::string logfile, std::string debug);
std::string RPL_STATSLINKINFO(std::string linkname, std::string sendq, std::string smessage, std::string sbytes, std::string rmessage, std::string rbytes, std::string time);
std::string RPL_STATSCOMMANDS(std::string command, std::string count);
std::string RPL_STATSCLINE(std::string host, std::string name, std::string port, std::string class_);
std::string RPL_STATSNLINE(std::string host, std::string name, std::string port, std::string class_);
std::string RPL_STATSILINE(std::string host1, std::string host2, std::string port, std::string class_);
std::string RPL_STATSKLINE(std::string host, std::string username, std::string port, std::string class_);
std::string RPL_STATSYLINE(std::string class_, std::string ping_freq, std::string connect_freq, std::string max_sendq);
std::string RPL_ENDOFSTATS(std::string stats_letter);
std::string RPL_STATSLLINE(std::string hostmask, std::string servername, std::string maxdepth);
std::string RPL_STATSUPTIME();
std::string RPL_STATSOLINE(std::string hostmask, std::string name);
std::string RPL_STATSHLINE(std::string hostmask, std::string servername);
std::string RPL_UMODEIS(std::string user_mode);
std::string RPL_LUSERCLIENT(std::string int1, std::string int2, std::string int3);
std::string RPL_LUSEROP(std::string int_);
std::string RPL_LUSERUNKNOWN(std::string int_);
std::string RPL_LUSERCHANNELS(std::string int_);
std::string RPL_LUSERME(std::string int1, std::string int2);
std::string RPL_ADMINME(std::string server);
std::string RPL_ADMINLOC1(std::string admin_info);
std::string RPL_ADMINLOC2(std::string admin_info);
std::string RPL_ADMINEMAIL(std::string admin_info);
std::string RPL_NONE();
std::string RPL_USERHOST(std::string replies);
std::string RPL_ISON(std::string nicks);
std::string RPL_AWAY(std::string nick, std::string message);
std::string RPL_UNAWAY();
std::string RPL_NOWAWAY();
std::string RPL_WHOISUSER(std::string nick, std::string user, std::string host, std::string realname);
std::string RPL_WHOISSERVER(std::string nick, std::string server, std::string server_info);
std::string RPL_WHOISOPERATOR(std::string nick);
std::string RPL_WHOISIDLE(std::string nick, std::string int_);
std::string RPL_ENDOFWHOIS(std::string nick);
std::string RPL_WHOISCHANNELS(std::string nick, std::string channels);
std::string RPL_WHOWASUSER(std::string nick, std::string user, std::string host, std::string real_name);
std::string RPL_ENDOFWHOWAS(std::string nick);
std::string RPL_LISTSTART();
std::string RPL_LIST(std::string channel, std::string visible, std::string topic);
std::string RPL_LISTEND();
std::string RPL_CHANNELMODEIS(std::string channel, std::string mode, std::string mode_param);
std::string RPL_NOTOPIC(std::string channel);
std::string RPL_TOPIC(std::string channel, std::string topic);
std::string RPL_INVITING(std::string channel, std::string nick);
std::string RPL_SUMMONING(std::string user);
std::string RPL_VERSION(std::string version, std::string server, std::string comments);
std::string RPL_WHOREPLY(std::string channel, std::string user, std::string host, std::string server, std::string nick, std::string state, std::string realname);
std::string RPL_ENDOFWHO(std::string name);
std::string RPL_NAMREPLY(std::string mod, std::string channel, std::string nicks);
std::string RPL_ENDOFNAMES(std::string channel);
std::string RPL_LINKS(std::string mask, std::string server, std::string hopcount, std::string serverinfo);
std::string RPL_ENDOFLINKS(std::string mask);
std::string RPL_BANLIST(std::string channel, std::string banid);
std::string RPL_ENDOFBANLIST(std::string channel);
std::string RPL_INFO(std::string str);
std::string RPL_ENDOFINFO();
std::string RPL_MOTDSTART(std::string server);
std::string RPL_MOTD(std::string text);
std::string RPL_ENDOFMOTD();
std::string RPL_YOUREOPER();
std::string RPL_REHASHING(std::string config_file);
std::string RPL_TIME(std::string server, std::string local_time);
std::string RPL_USERSTART();
std::string RPL_USERS();
std::string RPL_ENDOFUSERS();
std::string RPL_NOUSERS();

std::string ERR_NOSUCHNICK(std::string nickname);
std::string ERR_NOSUCHSERVER(std::string server);
std::string ERR_NOSUCHCHANNEL(std::string channel);
std::string ERR_CANNOTSENDTOCHAN(std::string channel);
std::string ERR_TOOMANYCHANNELS(std::string channel);
std::string ERR_WASNOSUCHNICK(std::string nickname);
std::string ERR_TOOMANYTARGETS(std::string target);
std::string ERR_NOORIGIN();
std::string ERR_NORECIPIENT(std::string command);
std::string ERR_NOTESTTOSEND();
std::string ERR_NOTOPLEVEL(std::string mask);
std::string ERR_WILDTOPLEVEL(std::string mask);
std::string ERR_UNKNOWNCOMMAND(std::string command);
std::string ERR_NOMOTD();
std::string ERR_NOADMININFO(std::string server);
std::string ERR_FILEERROR(std::string file_op, std::string file);
std::string ERR_NONICKNAMEGIVEN();
std::string ERR_ERRONEUSNICKNAME(std::string nickname);
std::string ERR_NICKNAMEINUSE(std::string nickname);
std::string ERR_NICKCOLLISION(std::string nickname);
std::string ERR_USERNOTINCHANNEL(std::string nick, std::string channel);
std::string ERR_NOTONCHANNEL(std::string channel);
std::string ERR_USERONCHANNEL(std::string user, std::string channel);
std::string ERR_NOLOGIN(std::string user);
std::string ERR_SUMMONDISABLED();
std::string ERR_USERSDISABLED();
std::string ERR_NOTREGISTERED();
std::string ERR_NEEDMOREPARAMS(std::string command);
std::string ERR_ALREADYREGISTRED();
std::string ERR_PASSWDMISMATCH();
std::string ERR_YOUREBANNEDCREEP();
std::string ERR_KEYSET(std::string channel);
std::string ERR_CHANNELISFULL(std::string channel);
std::string ERR_UNKNOWNMODE(std::string char_);
std::string ERR_INVITEONLYCHAN(std::string channel);
std::string ERR_BANNEDFROMCHAN(std::string channel);
std::string ERR_BADCHANNELKEY(std::string channel);
std::string ERR_NOPRIVILEGES();
std::string ERR_CHANOPRIVSNEEDED(std::string channel);
std::string ERR_CANTKILLSERVER();
std::string ERR_NOOPERHOST();
std::string ERR_UMODEUNKNOWNFLAG();
std::string ERR_USERSDONTMATCH();


# endif 