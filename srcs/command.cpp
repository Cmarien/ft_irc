/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 10:07:25 by user42            #+#    #+#             */
/*   Updated: 2022/06/14 18:40:12 by user42           ###   ########.fr       */
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
		if (nickname[i] < 32 || nickname[i] > 126)
			return false;
	return true;
}

std::string    nick(std::string nickname, server & serv, client & cli){
	std::string toRet = ":" + cli.get_prefix() + " NICK :" + nickname;

	(void)serv;
    // Utility?
	clean_string(nickname);

    // CHECK IF CLIENT IS CHANN OP, IF YES, UPDATE IT

	if (nickname.empty())
		return ERR_NONICKNAMEGIVEN(); // EMPTY NICKNAME
	else if (!isValidNickname(nickname))
		return ERR_ERRONEUSNICKNAME(nickname); // UNVALID NICKNAME
	else if (!isNicknameAvailable(nickname, serv.clients))
		return ERR_NICKNAMEINUSE(nickname); // UNAVAILABLE NICKNAME

    //if cli is chann op then send to this cli mode -o old nick +o new nick
	cli.nickname = nickname;
	return toRet;
}
std::string    ping(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
	return "PONG " + args;
}
std::string    pong(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
	return "PING " + args;
}
std::string    join(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
	int index = 0;
	std::string toRet;
	std::cout << toRet[0] << std::endl;
	while (index < 30 && serv.channels[index].name != ""){
		if (serv.channels[index].name.compare(args) == 0){
			serv.channels[index].add_client(cli.client_socket, cli);
			if (serv.channels[index].topic != ""){
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
	if (index >= 30){
		std::cout << "all chann full" << std::endl;
	}
	else{
		serv.channels[index].add_op(cli.client_socket, cli);
		serv.channels[index].name = args;
		if (serv.channels[index].topic != ""){
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
	}
	serv.channels[j].send_to_clients(toRet + "\r\n", cli.client_socket);
	return toRet;
}
std::string    quit(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
	close(cli.client_socket);
	cli.clear_client();
	return args;
}
std::string    privmsg(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
	std::string toRet = ":" + cli.get_prefix() + " PRIVMSG " + args + "\r\n";
	std::string target;
	int index = 0;

	while (args[index] && args[index] != ' '){
		index++;
	}
	target = args.substr(0, index);
	
	index = 0;

	if (target[0] == '#'){
		while (index < 30){
			if (serv.channels[index].name.compare(target) == 0){
				serv.channels[index].send_to_clients(toRet, cli.client_socket);
			}
			index++;
		}
	}
	else
	{
		while (index < 30){
			if (serv.clients[index].nickname.compare(target) == 0){
				send(serv.clients[index].client_socket, toRet.c_str(), toRet.length(), 0);
				break;
			}
			index++;
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

std::string    who(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
	int index = 0;

	while (index < 30){
		if (serv.channels[index].name.compare(args) == 0){
			break;
		}
		index++;
	}
	if (index >= 30){
		return "";
	}
	std::string toRet = ":" + cli.get_prefix() + " 315 " + cli.nickname + " " + get_replies(315, serv.channels[index].get_users(), "", "", "", "", "", "");
	return toRet;
}
