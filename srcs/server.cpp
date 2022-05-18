/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:42:17 by user42            #+#    #+#             */
/*   Updated: 2022/05/18 17:39:15 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


void    server::init_server(std::string _port, std::string _password){
    int opt = 1;
    this->port = get_port(_port);
    this->password = _password;
    int addrlen;


    for(int i = 0; i < max_clients; i++){
        this->clients[i].client_socket = 0;
    }

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

        for (int i = 0; i < max_clients; i++){
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
            for (int i = 0; i < max_clients; i++){
                if (this->clients[i].client_socket == 0){
                    this->clients[i].client_socket = new_socket;
                    std::cout << "Adding to list of sockets as " << i << std::endl;
                    break;
                }
            }
        }

        for (int i = 0; i < max_clients; i++){
            sd = this->clients[i].client_socket;

            if (FD_ISSET(sd, &readfds)){
                if ((valread = recv(sd, &buffer, 1024, 0)) == 0){
                    getpeername(sd, (struct sockaddr*)&this->address, (socklen_t*)&addrlen);
                    std::cout << "Client Disconnected, ip: " << inet_ntoa(this->address.sin_addr) << " port: " << ntohs(this->address.sin_port) << std::endl;
                    close(sd);
                    this->clients[i].clear_client();
                }
                else if (this->clients[i].is_registered == false){
                    buffer[valread] = '\0';
                    std::cout << "Buff :" << buffer << std::endl;
                    if(this->clients[i].registr(buffer, *this) == false){
                        std::cout << "Client Registration Failed" << std::endl;
                    }       
                }
                else{
                    buffer[valread] = '\0';
                    std::cout << "BUFFER: " << buffer << std::endl;
                }
            }
        }
    }
}


server::server(){
}

server::~server(){
}