/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouduri <rmouduri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:20:14 by user42            #+#    #+#             */
/*   Updated: 2022/06/08 18:43:45 by rmouduri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <sstream>
#include "server.hpp"
#include "utils.hpp"
#include "replies.hpp"
#include "command.hpp"

client::client() {
	clear_client();
}

client::~client() {
}

bool    client::check_buff() {
	if (/*this->buffer[this->buffer.length() - 2] == '\r' && */this->buffer[this->buffer.length() - 1] == '\n')
		return true;
	return false;
}

void    client::clear_buff() {
	this->buffer = "";
}

void    client::clear_client() {
	this->client_socket = 0;
	this->chan_number = 0;
	this->nickname = "";
	this->username = "";
	this->realname = "";
	this->pass = "";
	this->host = "";
	this->server_name = "";
	this->registration_status = "Unregistered";
	this->is_registered = false;
	this->is_op = false;
}

std::string client::get_prefix() {
	return this->nickname + "!" + this->username + "@" + this->server_name;
}

void    client::finish_registration(server & serv) {
	std::string toSend;
	std::string nul = "";
	std::string nbr = static_cast<std::ostringstream *>( &(std::ostringstream() << serv.connectedUsers) )->str();
	this->serv = &serv;

	toSend = ": NICK :" + this->nickname + LINEEND;
	toSend += ":" + get_prefix() + " 001 " + this->nickname + " " + get_replies(1, get_prefix(), nul, nul, nul, nul, nul, nul) + LINEEND;
	toSend += ":" + get_prefix() + " 002 " + this->nickname + " " + get_replies(2, serv._config.version, nul, nul, nul, nul, nul, nul) + LINEEND;
	toSend += ":" + get_prefix() + " 003 " + this->nickname + " " + get_replies(3, serv.date, nul, nul, nul, nul, nul, nul) + LINEEND;
	toSend += ":" + get_prefix() + " 004 " + this->nickname + " " + get_replies(4, serv._config.server_name, serv._config.version, serv._config.umod, serv._config.cmod, nul, nul, nul) + LINEEND;
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

bool    client::registr(std::string buffer, server & serv) {
	int index = 0;
	int tmpdex = 0;
	std::string tmp;
	clean_string(buffer);
	if (buffer.length() >= 5){
		tmp = buffer.substr(0, 5);
	}
	else {
		if (!(buffer.compare("PASS") && buffer.compare("NICK") && buffer.compare("USER"))){
			this->serv->sendToUser(this->client_socket, ERR_NEEDMOREPARAMS(buffer) + "\r\n");
		}
		return false;
	}
	if (this->registration_status.compare("Unregistered") == 0) {
		if (tmp.compare("CAP L") == 0) {
			index = 5;
			while (buffer[index] && buffer[index] != '\n') {
				index++;
			}
			if (buffer[index + 1]) {
				index++;
				buffer.erase(0, index);
				return registr(buffer, serv);
			}
		}
		if (tmp.compare("PASS ") == 0) {
			index = 5;
			while (buffer[index] && buffer[index] != '\n') {
				index++;
			}
			this->pass = buffer.substr(5, index - 4);
			std::cout << pass << std::endl;
			clean_string(this->pass);
			if (this->pass.compare(serv.getPassword())) {
				serv.sendToUser(this->client_socket, ":" + this->get_prefix() + " 464 " + this->nickname + " " + get_replies(464, "", "", "", "", "", "", "") + "\r\n");
				return false;
			}
			this->registration_status = "PASS";
			if (buffer[index + 1]) {
				index++;
				buffer.erase(0, index);
				return registr(buffer, serv);
			}
		}
		else {
			printerr(ERR_PASSWDMISMATCH());
			serv.sendToUser(this->client_socket, ERR_PASSWDMISMATCH());
			return false;
		}
	}
	if (this->registration_status.compare("PASS") == 0 || this->registration_status.compare("Unregistered") == 0) {
		if (tmp.compare("NICK ") == 0) {
			index = 5;
			while (buffer[index] && buffer[index] != '\n')
				index++;
			std::string nickname = buffer.substr(5, index - 4);
			clean_string(nickname);

			if (nickname.empty()) {
				printerr(ERR_NONICKNAMEGIVEN()); // EMPTY NICKNAME
				serv.sendToUser(this->client_socket, ERR_NONICKNAMEGIVEN());
				return false;
			}
			else if (!isValidNickname(nickname)) {
				printerr(ERR_ERRONEUSNICKNAME(nickname)); // UNVALID NICKNAME
				serv.sendToUser(this->client_socket, ERR_ERRONEUSNICKNAME(nickname));
				return false;
			}
			else {
				std::string newName = nickname;
				int i = 0;
				while (!isNicknameAvailable(newName, serv.clients)) {
					newName = nickname + "_" + static_cast<std::ostringstream *>( &(std::ostringstream() << ++i) )->str();
				}
				std::cerr << "Final name is: " << newName << std::endl;
				this->nickname = newName;
				this->registration_status = "NICK";
				if (buffer[index + 1]) {
					index++;
					buffer.erase(0, index);
					return registr(buffer, serv);
				}	
			}
		}
	}
	if (this->registration_status.compare("NICK") == 0) {
		if (tmp.compare("USER ") == 0) {
			index = 5;
			tmpdex = index;
			while (buffer[index] && buffer[index] != ' ') {
				index++;
			}
			this->username = buffer.substr(tmpdex, index - (tmpdex));
			if (buffer[index] == ' ' && buffer[index + 1]){
				index++;
			}
			else{
				this->serv->sendToUser(this->client_socket, ERR_NEEDMOREPARAMS("USER") + "\r\n");
				return false;
			}
			tmpdex = index;
			while (buffer[index] && buffer[index] != ' ') {
				index++;
			}
			this->host = buffer.substr(tmpdex, index - (tmpdex));
			if (buffer[index] == ' ' && buffer[index + 1]){
				index++;
			}
			else{
				this->serv->sendToUser(this->client_socket, ERR_NEEDMOREPARAMS("USER") + "\r\n");
				return false;
			}
			tmpdex = index;
			while (buffer[index] && buffer[index] != ' ') {
				index++;
			}
			this->server_name = buffer.substr(tmpdex, index - (tmpdex));
			if (buffer[index] == ' ' && buffer[index + 1]){
				index++;
				index++;
			}
			else{
				this->serv->sendToUser(this->client_socket, ERR_NEEDMOREPARAMS("USER") + "\r\n");
				return false;
			}
			tmpdex = index;
			while (buffer[index] && buffer[index] != '\n') {
				index++;
			}
			this->realname = buffer.substr(tmpdex, index - (tmpdex - 1));
			// std::cout << "USER :" << this->username << "-" << this->host << "-" << this->server_name << "-" << this->realname << std::endl;
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
	}
	toRet = ":" + this->get_prefix() + " 221 " + this->nickname + " " + get_replies(221, this->get_modes(), "", "", "", "", "", "");
	if (err_check){
		toRet += "\r\n:" + this->get_prefix() + " 501 " + this->nickname + " " + get_replies(501, "", "", "", "", "", "" ,"");
	}
	return toRet;
}

void	client::quit(std::string reason){
	int index = 0;
	std::string toSend = "ERROR :" + reason + LINEEND;

	while (index < 30){
		if (this->serv->channels[index].is_client(this->client_socket) || this->serv->channels[index].is_op(this->client_socket)){
			this->serv->channels[index].send_to_clients(":" + this->get_prefix() + " QUIT " + reason + LINEEND, this->client_socket);
		}
		index++;
	}
}