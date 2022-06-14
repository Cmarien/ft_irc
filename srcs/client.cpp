/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 10:07:18 by user42            #+#    #+#             */
/*   Updated: 2022/06/14 17:32:46 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <vector>
#include "server.hpp"
#include "utils.hpp"
#include "replies.hpp"
#include "command.hpp"

client::client(){
	clear_client();
}

client::~client(){
}

bool    client::check_buff(){
	if (this->buffer[this->buffer.length() - 2] == '\r' && this->buffer[this->buffer.length() - 1] == '\n')
		return true;
	return false;
}

void    client::clear_buff(){
	this->buffer = "";
}

void    client::clear_client(){
	this->client_socket = 0;
	this->nickname = "";
	this->username = "";
	this->realname = "";
	this->pass = "";
	this->host = "";
	this->server_name = "";
	this->registration_status = "Unregistered";
	this->is_registered = false;
}

std::string client::get_prefix(){
	return this->nickname + "!" + this->username + "@" + this->server_name;
}

void    client::finish_registration(server &serv){
	std::string toSend;
	std::string nbr = "0";
	std::string nul = "";
	std::string date = "Today";
	std::string umod = "aiwro";
	std::string cmod = "Oovimnptkl";
	this->serv = &serv;
	toSend = ": NICK :" + this->nickname + LINEEND;
	toSend += ":" + get_prefix() + " 001 " + this->nickname + " " + get_replies(1, get_prefix(), nul, nul, nul, nul, nul, nul) + LINEEND;
	toSend += ":" + get_prefix() + " 002 " + this->nickname + " " + get_replies(2, serv._config.version, nul, nul, nul, nul, nul, nul) + LINEEND;
	toSend += ":" + get_prefix() + " 003 " + this->nickname + " " + get_replies(3, date, nul, nul, nul, nul, nul, nul) + LINEEND;
	toSend += ":" + get_prefix() + " 004 " + this->nickname + " " + get_replies(4, serv._config.server_name, serv._config.version, umod, cmod, nul, nul, nul) + LINEEND;
	toSend += ":" + get_prefix() + " 251 " + this->nickname + " " + get_replies(251, nbr, nbr, nbr, nul, nul, nul, nul) + LINEEND;
	toSend += ":" + get_prefix() + " 252 " + this->nickname + " " + get_replies(252, nbr, nul, nul, nul, nul, nul, nul) + LINEEND;
	toSend += ":" + get_prefix() + " 253 " + this->nickname + " " + get_replies(253, nbr, nul, nul, nul, nul, nul, nul) + LINEEND;
	toSend += ":" + get_prefix() + " 254 " + this->nickname + " " + get_replies(254, nbr, nul, nul, nul, nul, nul, nul) + LINEEND;
	toSend += ":" + get_prefix() + " 255 " + this->nickname + " " + get_replies(255, nbr, nbr, nul, nul, nul, nul, nul) + LINEEND;
	toSend += ":" + get_prefix() + " 375 " + this->nickname + " " + get_replies(375, serv._config.server_name, nul, nul, nul, nul, nul, nul) + LINEEND;
	toSend += ":" + get_prefix() + " 372 " + this->nickname + " " + get_replies(372, serv._config.motd, nul, nul, nul, nul, nul, nul) + LINEEND;
	std::cout << toSend << std::endl;
	send(this->client_socket, toSend.c_str(), toSend.length(), 0);
}

bool    client::registr(std::string buffer, server &serv){
	int index = 0;
	int tmpdex = 0;
	std::string tmp = buffer.substr(0, 5);
	if (this->registration_status.compare("Unregistered") == 0){
		if (tmp.compare("CAP L") == 0){
			index = 5;
			while (buffer[index] && buffer[index] != '\n'){
				index++;
			}
			if (buffer[index + 1]){
				index++;
				buffer.erase(0, index);
				return registr(buffer, serv);
			}
		}
		if (tmp.compare("PASS ") == 0){
			index = 5;
			while (buffer[index] && buffer[index] != '\n'){
				index++;
			}
			this->pass = buffer.substr(5, index - 4);
			clean_string(this->pass);
			this->registration_status = "PASS";
			if (buffer[index + 1]){
				index++;
				buffer.erase(0, index);
				return registr(buffer, serv);
			}
		}
	}
	if (this->registration_status.compare("PASS") == 0 || this->registration_status.compare("Unregistered") == 0){
		if (tmp.compare("NICK ") == 0){
			index = 5;
			while (buffer[index] && buffer[index] != '\n')
				index++;
			std::string nickname = buffer.substr(5, index - 4);
			std::cout << "name: '" << nickname << "'\n";
			clean_string(nickname);
			std::cout << "name2: '" << nickname << "'\n";

			if (nickname.empty()) {
				printerr(ERR_NONICKNAMEGIVEN()); // EMPTY NICKNAME
				return false;
			}
			else if (!isValidNickname(nickname)) {
				printerr(ERR_ERRONEUSNICKNAME(nickname)); // UNVALID NICKNAME
				return false;
			}
			else if (!isNicknameAvailable(nickname, serv.clients)) {
				printerr(ERR_NICKNAMEINUSE(nickname)); // UNAVAILABLE NICKNAME
				return false;
			}
			this->nickname = nickname;
			this->registration_status = "NICK";
			if (buffer[index + 1]){
				index++;
				buffer.erase(0, index);
				return registr(buffer, serv);
			}
		}
	}
	if (this->registration_status.compare("NICK") == 0){
		if (tmp.compare("USER ") == 0){
			index = 5;
			tmpdex = index;
			while (buffer[index] && buffer[index] != ' '){
				index++;
			}
			this->username = buffer.substr(tmpdex, index - (tmpdex));
			index++;
			tmpdex = index;
			while (buffer[index] && buffer[index] != ' '){
				index++;
			}
			this->host = buffer.substr(tmpdex, index - (tmpdex));
			index++;
			tmpdex = index;
			while (buffer[index] && buffer[index] != ' '){
				index++;
			}
			this->server_name = buffer.substr(tmpdex, index - (tmpdex));
			index++;
			index++;
			tmpdex = index;
			while (buffer[index] && buffer[index] != '\n'){
				index++;
			}
			this->realname = buffer.substr(tmpdex, index - (tmpdex - 1));
			// std::cout << "USER :" << this->username << "-" << this->host << "-" << this->server_name << "-" << this->realname << std::endl;
			clean_string(this->username);
			this->registration_status = "USER";
			this->is_registered = true;
			finish_registration(serv);
		}
	}
	return true;
}

void	client::remove_mode(char toRemove){
	std::vector<char>::iterator ite = this->modes.end();

	for (std::vector<char>::iterator it = this->modes.begin(); it != ite; it++){
		if (*it == toRemove){
			this->modes.erase(it);
			break;
		}
	}
}

std::string client::get_modes(){
	std::string toRet = "";
	std::vector<char>::const_iterator ite = this->modes.end();

	if (this->modes.size()){
		toRet += "+";
	}
	for (std::vector<char>::const_iterator it = this->modes.begin(); it != ite; it++){
		toRet += *it;
	}
	return toRet;
}

std::string client::add_modes(std::string modestring, __attribute__((unused)) std::string mode_arg){
	int index = 0;
	int err_check = 0;
	std::string toRet;

	if (modestring[0] == '+' || modestring[0] == '-'){
		index = 1;
	}
	
	if (modestring[0] != '-'){
		while (modestring[index] && modestring[index] != '-'){
			if (check_charset(modestring[index], "aiwro")){
				if (check_modes(modestring[index], this->modes)){
					if (modestring[index] != 'o'){
						this->modes.push_back(modestring[index]);
					}
				}
			}
			else{
				err_check = 1;
			}
			index++;
		}
		while (modestring[index]){
			if (check_charset(modestring[index], "aiwro")){
				if (check_modes(modestring[index], this->modes)){
					this->remove_mode(modestring[index]);
				}
			}
			else{
				err_check = 1;
			}
			index++;
		}
	}
	else{
		while (modestring[index] && modestring[index] != '+'){
			if (check_charset(modestring[index], "aiwro")){
				if (check_modes(modestring[index], this->modes) == 0){
					if (modestring[index] != 'r'){
						this->remove_mode(modestring[index]);
					}
				}
			}
			else{
				err_check = 1;
			}
			index++;
		}
		while (modestring[index]){
			if (check_charset(modestring[index], "aiwro")){
				if (check_modes(modestring[index], this->modes)){
					this->modes.push_back(modestring[index]);
				}
			}
			else{
				err_check = 1;
			}
			index++;
		}
	}
	toRet = ":" + this->get_prefix() + " 221 " + this->nickname + " " + get_replies(221, this->get_modes(), "", "", "", "", "", "");
	if (err_check){
		toRet += "\r\n:" + this->get_prefix() + " 501 " + this->nickname + " " + get_replies(501, "", "", "", "", "", "" ,"");
	}
	return toRet;
}