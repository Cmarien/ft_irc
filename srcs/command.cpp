/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouduri <rmouduri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 15:09:54 by user42            #+#    #+#             */
/*   Updated: 2022/06/06 19:59:15 by rmouduri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "replies.hpp"

bool isNicknameAvailable(std::string nickname, std::vector<client> clients) {
	for (std::vector<client>::const_iterator cit = clients.begin(), cite = clients.end(); cit != cite; ++cit)
		if (nickname == cit->nickname)
			return false;
	return true;
}

bool isValidNickname(std::string nickname) {
	for (int i = 0; nickname[i]; ++i)
		if (nickname[i] < 32)
			return false;
	return true;
}

std::string    nick(std::string nickname, server & serv, client &  cli) {
	std::string toRet = ":" + cli.get_prefix() + " NICK :" + nickname;

	clean_string(nickname);

	if (nickname.empty())
		return ":" + cli.get_prefix() + " " +  ERR_NONICKNAMEGIVEN(); // EMPTY NICKNAME
	else if (!isValidNickname(nickname))
		return ":" + cli.get_prefix() + " " +  ERR_ERRONEUSNICKNAME(nickname); // UNVALID NICKNAME
	else if (!isNicknameAvailable(nickname, serv.clients))
		return ":" + cli.get_prefix() + " " + ERR_NICKNAMEINUSE(nickname); // UNAVAILABLE NICKNAME

	cli.nickname = nickname;
	for (int i = 0; i < 30; i++){
		if (serv.channels[i].is_client(cli.client_socket)){
		serv.channels[i].remove_client(cli.client_socket);
		serv.channels[i].add_client(cli.client_socket, cli);
	}
	else if (serv.channels[i].is_op(cli.client_socket)){
		serv.channels[i].remove_op(cli.client_socket);
		serv.channels[i].add_op(cli.client_socket, cli);
	}
	}
	return toRet;
}
std::string    ping(std::string args, __attribute__((unused)) server & serv, __attribute__((unused)) client & cli) {
	if (args.empty()){
		return ERR_NEEDMOREPARAMS("PONG");
	}
	return "PONG " + args;
}
std::string    pong(std::string args, __attribute__((unused)) server & serv, __attribute__((unused)) client & cli) {
	return "" + args;
}
std::string    join(std::string args, __attribute__((unused)) server & serv, __attribute__((unused)) client & cli) {
	int index = 0;
	std::string toRet;
	if (args.empty()){
		return ERR_NEEDMOREPARAMS("JOIN");
	}
	while (index < 30 && serv.channels[index].name != "") {
		if (serv.channels[index].name.compare(args) == 0) {
			if (cli.chan_number >= 5){
				return ":" + cli.get_prefix() + " 473 " + cli.nickname + " " + ERR_TOOMANYCHANNELS(args);
			}
			if (serv.channels[index].is_banned(cli.client_socket)){
				return ":" + cli.get_prefix() + " 473 " + cli.nickname + " " + ERR_BANNEDFROMCHAN(args);
			}
			if (serv.channels[index].number_clients >= 30){
				return ":" + cli.get_prefix() + " 473 " + cli.nickname + " " + ERR_CHANNELISFULL(args);
			}
			if (serv.channels[index].invite_only == true && !(serv.channels[index].is_invited(cli.nickname))){
				return ":" + cli.get_prefix() + " 473 " + cli.nickname + " " + ERR_INVITEONLYCHAN(args);
			}
			serv.channels[index].add_client(cli.client_socket, cli);
			if (serv.channels[index].topic != "") {
				toRet += ":" + cli.get_prefix() + " 332 " + cli.nickname + " " + get_replies(332, serv.channels[index].name, serv.channels[index].topic, "", "", "","","") + LINEEND;
			}
			toRet += ":" + cli.get_prefix() + " 353 " + cli.nickname + " " + get_replies(353, serv.channels[index].get_modes(), serv.channels[index].name, serv.channels[index].get_nicks("@"), "", "", "", "") + LINEEND;
			toRet += ":" + cli.get_prefix() + " 366 " + cli.nickname + " " + get_replies(366, serv.channels[index].name, "", "", "", "", "", "") + LINEEND;
			toRet += ":" + cli.get_prefix() + " JOIN :" + serv.channels[index].name;
			serv.channels[index].send_to_clients(":" + cli.get_prefix() + " JOIN " + serv.channels[index].name + "\r\n", cli.client_socket);
			return toRet;
		}
		index++;
	}
	if (index >= 30) {
		return ERR_NOSUCHCHANNEL(args);
	}
	else {
		if (cli.chan_number >= 5){
				return ERR_TOOMANYCHANNELS(args);
		}
		serv.channels[index].add_op(cli.client_socket, cli);
		serv.channels[index].name = args;
		if (serv.channels[index].topic != "") {
			toRet += ":" + cli.get_prefix() + " 332 " + cli.nickname + " " + get_replies(332, serv.channels[index].name, serv.channels[index].topic, "", "", "","","") + LINEEND;
		}
		toRet += ":" + cli.get_prefix() + " 353 " + cli.nickname + " " + get_replies(353, serv.channels[index].get_modes(), serv.channels[index].name, serv.channels[index].get_nicks("@"), "", "", "", "") + LINEEND;
		toRet += ":" + cli.get_prefix() + " 366 " + cli.nickname + " " + get_replies(366, serv.channels[index].name, "", "", "", "", "", "") + LINEEND;
		toRet += ":" + cli.get_prefix() + " JOIN :" + serv.channels[index].name;
	}
	return toRet;
}
std::string    part(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
	std::string toRet = ":" + cli.get_prefix() + " PART " + args;
	std::string target;
	std::string reason = "";
	int j;
	int index = 0;

	if (args.empty()){
		return ERR_NEEDMOREPARAMS("PART");
	}
	while (args[index] && args[index] != ' '){
        index++;
    }
    target = args.substr(0, index);
	if (args[index]){
    	args.erase(0, index + 1);
	}
	index = 0;
	if (args[index] == ':'){
		while (args[index]){
			index++;
		}
		reason = args.substr(0, index);
	}
	for (int i = 0; i < 30; i++){
		if (target.compare(serv.channels[i].name) == 0){
				j = i;
				break;
			}
			if (i == 30){
				return ":" + cli.get_prefix() + " 403 " + cli.nickname + " " + get_replies(403, target, "", "", "", "", "", "");
			}
			j = i;
	}
	if (serv.channels[j].is_client(cli.client_socket)){
		serv.channels[j].remove_client(cli.client_socket);
	}
	else if (serv.channels[j].is_op(cli.client_socket)){
		serv.channels[j].remove_op(cli.client_socket);
	}
	else{
		return ":" + cli.get_prefix() + " 442 " + cli.nickname + " " + get_replies(442, target, "", "", "", "", "", "");
	}
	if (serv.channels[j].number_clients == 0){
			serv.channels[j].name = "";
			serv.channels[j].modes.clear();
			serv.channels[j].banlist.clear();
			serv.channels[j].invitelist.clear();
			serv.channels[j].invite_only = false;
	}
	serv.channels[j].send_to_clients(toRet + "\r\n", cli.client_socket);
	return toRet;
}
std::string    quit(std::string args, __attribute__((unused)) server & serv, client & cli) {
	cli.quit(args);
	close(cli.client_socket);
	cli.clear_client();
	return "";
}
std::string    privmsg(std::string args, __attribute__((unused)) server & serv, __attribute__((unused)) client & cli) {
	std::string toRet = ":" + cli.get_prefix() + " PRIVMSG " + args + "\r\n";
	std::string target;
	int index = 0;

	while (args[index] && args[index] != ' ') {
		index++;
	}
	target = args.substr(0, index);
	
	index = 0;

	if (target[0] == '#') {
		while (index < 30) {
			if (serv.channels[index].name.compare(target) == 0) {
				serv.channels[index].send_to_clients(toRet, cli.client_socket);
			}
			index++;
		}
	}
	else
	{
		std::vector<client>::iterator ite = serv.clients.end();
		for (std::vector<client>::iterator it = serv.clients.begin(); it != ite; it++){
			if (it->nickname.compare(target) == 0){
				send(serv.clients[index].client_socket, toRet.c_str(), toRet.length(), 0);
				break;
			}
		}
	}
	return "";
}

std::string    mode(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
	std::string target;
    std::string tags;
    std::string options;
    std::vector<client>::const_iterator it = serv.clients.begin();
    std::vector<client>::const_iterator ite = serv.clients.end();
    
    int index = 0;
	int j;
    while (args[index] && args[index] != ' '){
        index++;
    }
    target = args.substr(0, index);
	if (target[0] != '#'){
		if (target.compare(cli.nickname)){
			while (it != ite){
				if (target.compare(it->nickname) == 0){
					return ":" + cli.get_prefix() + " 401 " + cli.nickname + " " + get_replies(401, target, "", "", "", "", "", "");
				}
				it++;
			}
			return ":" + cli.get_prefix() + " 502 " + cli.nickname + " " + get_replies(502, "", "", "", "", "", "", "");
		}
		if (!(args[index])){
			return ":" + cli.get_prefix() + " 221 " + cli.nickname + " " + get_replies(221, cli.get_modes(), "", "", "", "", "", "");
		}
    }
	else{
		for (int i = 0; i < 30; i++){
			if (target.compare(serv.channels[i].name) == 0){
				j = i;
				break;
			}
			if (i == 30){
				return ":" + cli.get_prefix() + " 403 " + cli.nickname + " " + get_replies(403, target, "", "", "", "", "", "");
			}
			j = i;
		}
		if (!(args[index])){
			return ":" + cli.get_prefix() + " 324 " + cli.nickname + " " + get_replies(324, target, serv.channels[j].get_modes(), "", "", "", "", "");
		}
	}
    args.erase(0, index + 1);

    index = 0;
    while (args[index] && args[index] != ' '){
        index++;
    }
    tags = args.substr(0, index);
    if (args[index]){
        args.erase(0, index + 1);
		index = 0;
        while (args[index]){
            index++;
        }
		options = args.substr(0, index);
    }

	if (target[0] != '#'){
		return cli.add_modes(tags, options);
	}
	return serv.channels[j].add_modes(tags, options, cli);
}

std::string    who(std::string args, __attribute__((unused)) server & serv, __attribute__((unused)) client & cli) {
	int index = 0;

	while (index < 30) {
		if (serv.channels[index].name.compare(args) == 0) {
			break;
		}
		index++;
	}
	if (index >= 30) {
		return "";
	}
	std::string toRet = ":" + cli.get_prefix() + " 315 " + cli.nickname + " " + get_replies(315, serv.channels[index].get_users(), "", "", "", "", "", "");
	return toRet;
}

std::string    oper(std::string args, __attribute__((unused)) server & serv, __attribute__((unused)) client & cli) {
	std::string name;
	std::string pass;
	std::string toRet;
	int index = 0;

	if (args.empty()){
		return ERR_NEEDMOREPARAMS("OPER");
	}
	while (args[index] && args[index] != ' '){
		index++;
	}
	name = args.substr(0, index);
	args.erase(0, index + 1);
	index = 0;
	if (args.empty()){
		return ERR_NEEDMOREPARAMS("OPER");
	}
	while (args[index]){
		index++;
	}
	pass = args.substr(0, index);
	if (name.compare(cli.serv->_config.oper_name) == 0 && name.compare(cli.serv->_config.oper_pass) == 0){
		cli.is_op = true;
		cli.modes.push_back('o');
		toRet = ":" + cli.get_prefix() + " 381 " + cli.nickname + " " + get_replies(381, "", "", "", "", "", "", "") + LINEEND;
		toRet += ":" + cli.get_prefix() + " 221 " + cli.nickname + " " + get_replies(221, cli.get_modes(), "", "", "", "", "", "");
	}
	else{
		toRet = ERR_PASSWDMISMATCH();
	}
	return toRet;
}

std::string    kick(std::string args, __attribute__((unused)) server & serv, __attribute__((unused)) client & cli) {
	std::string chan;
	std::string nick;
	std::string reason;
	int index = 0;

	if (args.empty()){
		return ERR_NEEDMOREPARAMS("KICK");
	}
	while (args[index] && args[index] != ' '){
		index++;
	}
	chan = args.substr(0, index);
	args.erase(0, index + 1);
	index = 0;
	if (args.empty()){
		return ERR_NEEDMOREPARAMS("KICK");
	}
	while (args[index] && args[index] != ' '){
		index++;
	}
	nick = args.substr(0, index);
	args.erase(0, index  + 2);
	index = 0;
	if (args.empty()){
		reason = "because i can";
	}
	else{
		while (args[index]){
			index++;
		}
		reason = args.substr(0, index);
		index = 0;
	}
	std::cout << "NICK :" << nick << "CHAN :" << chan << "REASON :" << reason << std::endl;
	while (index < 30){
		if (serv.channels[index].name.compare(chan) == 0){
			if (serv.channels[index].is_op(cli.client_socket) == 0){
				if (serv.channels[index].is_client(cli.client_socket) == 0){
					return ERR_NOTONCHANNEL(chan);
				}
				return ERR_CHANOPRIVSNEEDED(chan);
			}
			if (serv.channels[index].is_client(nick) == 0 && serv.channels[index].is_op(nick) == 0){
				return ERR_USERNOTINCHANNEL(nick, chan);
			}
			serv.channels[index].kick_client(nick);
			return ":" + cli.get_prefix() + " KICK " + chan + " " + nick + " :" + reason;
		}
	}
	return ERR_NOSUCHCHANNEL(chan);
}

std::string    invite(std::string args, __attribute__((unused)) server & serv, __attribute__((unused)) client & cli) {
	std::string toRet;
	std::string chan;
	std::string nick;
	int index = 0;

	if (args.empty()){
		return ":" + cli.get_prefix() + " 461 " + cli.nickname + " " + ERR_NEEDMOREPARAMS("INVITE");
	}
	while (args[index] && args[index] != ' '){
		index++;
	}
	nick = args.substr(0, index);
	args.erase(0, index + 1);
	index = 0;
	if (args.empty()){
		return ":" + cli.get_prefix() + " 461 " + cli.nickname + " " + ERR_NEEDMOREPARAMS("INVITE");
	}
	while (args[index]){
		index++;
	}
	chan = args.substr(0, index);
	index = 0;

	while (index < 30){
		if (serv.channels[index].name.compare(chan) == 0){
			if (serv.channels[index].is_op(cli.client_socket) == 0){
				if (serv.channels[index].is_client(cli.client_socket) == 0){
					return ":" + cli.get_prefix() + " 442 " + cli.nickname + " " + ERR_NOTONCHANNEL(chan);
				}
				return ":" + cli.get_prefix() + " 482 " + cli.nickname + " " + ERR_CHANOPRIVSNEEDED(chan);
			}
			if (serv.channels[index].is_client(nick) || serv.channels[index].is_op(nick)){
				return ":" + cli.get_prefix() + " 443 " + cli.nickname + " " + ERR_USERONCHANNEL(nick, chan);
			}
			serv.channels[index].invitelist.push_back(nick);
			return ":" + cli.get_prefix() + " " + nick + " " + chan;
		}
		index++;
	}
	return ":" + cli.get_prefix() + " 403 " + cli.nickname + " " + ERR_NOSUCHCHANNEL(chan);
}

std::string    notice(std::string args, __attribute__((unused)) server & serv, __attribute__((unused)) client & cli) {
	std::string toRet = ":" + cli.get_prefix() + " NOTICE " + args + "\r\n";
	std::string target;
	int index = 0;

	while (args[index] && args[index] != ' ') {
		index++;
	}
	target = args.substr(0, index);
	
	index = 0;

	if (target[0] == '#') {
		while (index < 30) {
			if (serv.channels[index].name.compare(target) == 0) {
				serv.channels[index].send_to_clients(toRet, cli.client_socket);
			}
			index++;
		}
	}
	else
	{
		while (index < 30) {
			if (serv.clients[index].nickname.compare(target) == 0) {
				send(serv.clients[index].client_socket, toRet.c_str(), toRet.length(), 0);
				break;
			}
			index++;
		}
	}
	return "";
}