/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouduri <rmouduri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:09:22 by user42            #+#    #+#             */
/*   Updated: 2022/06/06 19:07:33 by rmouduri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include "utils.hpp"

class client;

#include "server.hpp"

 
#define LINEEND "\r\n"

class client
{
private:
public:
	int client_socket;
	int	chan_number;

	std::string nickname;
	std::string username;
	std::string realname;
	std::string pass;
	std::string host;
	std::string server_name;
	std::string registration_status;
	std::string buffer;
    std::vector<char> modes;

	bool		is_registered;
	bool		is_op;

	server *serv;


	bool	registr(std::string buffer, server &serv);
	bool	check_buff();
	void	finish_registration(server &serv);
	void	process(std::string);
	std::string get_prefix();
	void	clear_client();
	void	clear_buff();
	void	remove_mode(char);
	void	quit(std::string);
    std::string get_modes();
    std::string    add_modes(std::string modestring, std::string mode_arg);

	client();
	~client();
};

#endif