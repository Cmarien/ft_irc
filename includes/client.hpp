/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:09:22 by user42            #+#    #+#             */
/*   Updated: 2022/05/26 10:27:41 by user42           ###   ########.fr       */
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

    std::string nickname;
    std::string username;
    std::string realname;
    std::string pass;
    std::string host;
    std::string server_name;
    std::string registration_status;

    bool        is_registered;

    server *serv;


    bool    registr(std::string buffer, server &serv);
    void    finish_registration(server &serv);
    void    process(std::string);
    std::string get_prefix();
    void    clear_client();

    client();
    ~client();
};

#endif