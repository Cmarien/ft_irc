/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:41:46 by user42            #+#    #+#             */
/*   Updated: 2022/05/18 17:36:12 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define max_clients 30

#include "config.hpp"
#include <fcntl.h>

class server;

#include "client.hpp"


class server
{
private:
    int port;
    int addrlen;
    int master_socket;
    std::string password;
    client clients[30];
    struct sockaddr_in address;


public:
    void    init_server(std::string _port, std::string _password);
    void    run_server();
    config  _config;
    server();
    ~server();
};

#endif