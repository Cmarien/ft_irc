/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 10:06:59 by user42            #+#    #+#             */
/*   Updated: 2022/06/02 10:08:10 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits>
#include <errno.h>
#include <stdlib.h>
#include "server.hpp"

 void    server::process(std::string buffer, client &cli){
	size_t index = buffer.find(' ', 0);
	std::string tmp;
	if (index == std::numeric_limits<size_t>::max()){
		std::cout << "space not found" << std::endl;
	}
	std::string cmd = buffer.substr(0, index);
	std::string args = buffer.substr(index + 1, buffer.size() - (index + 3));
	for (int i = 0; i < 9; i++){
		if (cmd.compare(this->user_cmd[i]) == 0){
			tmp = this->f[i](args, *this, cli) + LINEEND;
			std::cout << "sending: " << tmp << std::endl;
			if (tmp.compare("")){
				send(cli.client_socket, tmp.c_str(), tmp.length(), 0);
			}
		}
	}
 }

void    server::init_server(std::string _port, std::string _password){
	int opt = 1;
	this->port = get_port(_port);
	this->password = _password;

	if ((this->master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		std::cout << "Failed to create socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	if (setsockopt(this->master_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char*)&opt, sizeof(opt)) < 0){
		std::cout << "Failed to set socket options. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	
	if (fcntl(this->master_socket, F_SETFL, O_NONBLOCK) < 0){
		std::cout << "Failed to fcntl" << std::endl;
		exit(EXIT_FAILURE);
	}

	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(this->port);

	if (bind(this->master_socket, (struct sockaddr *)&this->address, sizeof(this->address)) < 0){
		std::cout << "Failed to bind. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(this->master_socket, this->address.sin_port) < 0){
		std::cout << "Failed to listen. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	addrlen = sizeof(this->address);
	std::cout << "Waiting for connections ..." << std::endl;
	this->run_server();
}

void    server::run_server(){
	fd_set readfds;
	int max_sd;
	int sd;
	int activity;
	int new_socket;
	int valread;
	char buffer[1025];


	while (1){
		FD_ZERO(&readfds);
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		for (size_t i = 0; i < this->clients.size(); i++){
			sd = this->clients[i].client_socket;
			if (sd > 0){
				FD_SET(sd, &readfds);
			}
			if (sd > max_sd){
				max_sd = sd;
			}
		}

		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		if ((activity < 0) && (errno != EINTR)){
			std::cout << "Failed to select." << std::endl;
		}

		if (FD_ISSET(master_socket, &readfds)){
			if ((new_socket = accept(master_socket, (struct sockaddr *)&this->address, (socklen_t*)&addrlen)) < 0){
				std::cout << "Failed to accept. errno: " << errno << std::endl;
				exit(EXIT_FAILURE);
			}

			std::cout << "New connection, socket fd is " << new_socket << ", ip is: " << inet_ntoa(this->address.sin_addr) << ", port: " << ntohs(this->address.sin_port) << std::endl;
			client newClient;
			newClient.client_socket = new_socket;

			std::cout << "Adding to list of sockets as " << this->clients.size() - 1 << std::endl;
			this->clients.push_back(newClient);
		}
		
		for (std::vector<client>::iterator it = clients.begin(), ite = clients.end(); it != ite; ++it) {
			sd = it->client_socket;

			if (FD_ISSET(sd, &readfds)){
				if ((valread = recv(sd, &buffer, 1024, 0)) == 0){
					getpeername(sd, (struct sockaddr*)&this->address, (socklen_t*)&addrlen);
					std::cout << "Client Disconnected, ip: " << inet_ntoa(this->address.sin_addr) << " port: " << ntohs(this->address.sin_port) << std::endl;
					close(sd);
					this->clients.erase(it);
				}
				else if (it->is_registered == false){
					buffer[valread] = '\0';
					std::cout << "Buff :" << buffer << std::endl;
					it->buffer += buffer;
					if (it->check_buff()){
						if(it->registr(it->buffer, *this) == false){
							std::cout << "Client Registration Failed" << std::endl;
						}
						it->clear_buff();
					}
				}
				else{
					buffer[valread] = '\0';
					std::cout << "BUFFER: " << buffer << std::endl;
					it->buffer += buffer;
					if (it->check_buff()){
						this->process(it->buffer, *it);
						it->clear_buff();
					}
				}
			}
		}
	}
}


server::server(){
	this->user_cmd[0] = "NICK";
	this->user_cmd[1] = "PING";
	this->user_cmd[2] = "PONG";
	this->user_cmd[3] = "JOIN";
	this->user_cmd[4] = "PART";
	this->user_cmd[5] = "QUIT";
	this->user_cmd[6] = "PRIVMSG";
	this->user_cmd[7] = "MODE";
	this->user_cmd[8] = "WHO";
 
	this->f[0] = &nick;
	this->f[1] = &ping;
	this->f[2] = &pong;
	this->f[3] = &join;
	this->f[4] = &part;
	this->f[5] = &quit;
	this->f[6] = &privmsg;
	this->f[7] = &mode;
	this->f[8] = &who;
	// this->user_cmd[7] = "OPER";
	// this->user_cmd[8] = "TOPIC";
	// this->user_cmd[9] = "NAMES";
	// this->user_cmd[10] = "LIST";
	// this->user_cmd[11] = "INVITE";
}

server::~server(){
}