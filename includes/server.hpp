/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:41:46 by user42            #+#    #+#             */
/*   Updated: 2022/05/19 17:07:00 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define max_clients 30

#include "config.hpp"
#include <fcntl.h>

class server;

#include "client.hpp"
#include "command.hpp"



class server
{
private:
    int port;
    int addrlen;
    int master_socket;
    std::string password;
    client clients[30];
    struct sockaddr_in address;
    std::string     user_cmd[7];
    std::string    (*f[7])(std::string, server&, client&);

public:
    void    init_server(std::string _port, std::string _password);
    void    process(std::string buffer, client&);
    void    run_server();
    config  _config;
    server();
    ~server();
};

#endif