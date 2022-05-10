/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 15:51:27 by user42            #+#    #+#             */
/*   Updated: 2022/05/03 13:29:45 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <fcntl.h>

struct client{
    int client_socket;
    std::string nickname;
    std::string username;
    std::string pass;
    bool        is_registered = false;
    std::string registration_status = "Unregistered";
};

void    finish_registration(client cli){
    std::string toSend;
    toSend = ":Welcome to the test Network, " + cli.nickname + "\r\n";
    toSend += ":Your host is irctest, running version 0.1\r\n";
    toSend += ":This server was created 03/05/2022;1:09\r\n";
    toSend += "irctest 0.1\r\n";
    toSend += ":There are 0 users and 0 invisible on 0 servers\r\n";
    toSend += "0 :operator(s) online\r\n";
    toSend += "0 :unknown(s) connection\r\n";
    toSend += "0 :channels formed\r\n";
    toSend += ":I have 0 clients and 0 servers\r\n";
    toSend += "BIEVENUE\r\n";
    send(cli.client_socket, toSend.c_str(), toSend.length(), 0);
}

void    check_receive(std::string buffer, client cli){
    std::string tmp = buffer.substr(0, 5);

    if (tmp.compare("PASS ") == 0){
        std::cout << "PASS FOUND" << std::endl;
        cli.pass = buffer.substr(5, buffer.length());
    }
}

void    clean_string(std::string &str){
    if (str[str.length() - 1] == '\n'){
        str.pop_back();
    }
    if (str[str.length() - 1] == '\r'){
        str.pop_back();
    }
}

bool    register_client(client &cli, std::string buffer){
    int index = 0;
    std::string tmp = buffer.substr(0, 5);
    if (cli.registration_status.compare("Unregistered") == 0){
        if (tmp.compare("CAP L") == 0){
            index = 5;
            while (buffer[index] && buffer[index] != '\n'){
                index++;
            }
            if (buffer[index + 1]){
                index++;
                buffer.erase(0, index);
                register_client(cli, buffer);
            }
        }
        if (tmp.compare("PASS ") == 0){
            index = 5;
            while (buffer[index] && buffer[index] != '\n'){
                index++;
            }
            cli.pass = buffer.substr(5, index - 4);
            clean_string(cli.pass);
            cli.registration_status = "PASS";
            if (buffer[index + 1]){
                index++;
                buffer.erase(0, index);
                register_client(cli, buffer);
            }
        }
    }
    if (cli.registration_status.compare("PASS") == 0 || cli.registration_status.compare("Unregistered") == 0){
        if (tmp.compare("NICK ") == 0){
            index = 5;
            while (buffer[index] && buffer[index] != '\n'){
                index++;
            }
            cli.nickname = buffer.substr(5, index - 4);
            clean_string(cli.nickname);
            cli.registration_status = "NICK";
            if (buffer[index + 1]){
                index++;
                buffer.erase(0, index);
                register_client(cli, buffer);
            }
        }
    }
    if (cli.registration_status.compare("NICK") == 0){
        if (tmp.compare("USER ") == 0){
            index = 5;
            while (buffer[index] && buffer[index] != '\n'){
                index++;
            }
            cli.username = buffer.substr(5, index - 4);
            clean_string(cli.username);
            cli.registration_status = "USER";
            cli.is_registered = true;
            finish_registration(cli);
        }
    }
    return true;
}

int quit_check(char *buffer){
    std::string tmp = buffer;
    return tmp.compare("/quit\r\n");
}

int get_port(char *str){
    int nb = 0;
    int index = 0;

    while (str[index]){
        if (str[index] < '0' || str[index] > '9'){
            return -1;
        }
        nb = nb*10 + str[index] - '0';
        index++;
    }
    return nb;
}

std::string get_buff_val(char *buffer){
    std::string tmp = buffer;
    tmp.resize(tmp.size() -2);
    return tmp;
}


//CONFIG IRC SERVEUR
//
//
/////
//
//
//
////
///
///
///
///
///
///
///
///
///
///

int main(int ac, char **av){
    if (ac != 2){
        std::cout << "Wrond number of args" << std::endl;
        exit(EXIT_FAILURE);
    }
    int port  = get_port(av[1]);
    int opt = 1;
    client client[30];
    int master_socket, addrlen, new_socket, max_clients = 30, activity, i, valread, sd;
    int max_sd;
    struct sockaddr_in address;

    char buffer[1025];
    std::string message = "Welcome on the IRC server\n";

    fd_set readfds;

    for(int i = 0; i < max_clients; i++){
        client[i].client_socket = 0;
    }

    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char*)&opt, sizeof(opt)) < 0){
        std::cout << "Failed to set socket options. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    
    if (fcntl(master_socket, F_SETFL, O_NONBLOCK) < 0){
        std::cout << "Failed to fcntl" << std::endl;
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0){
        std::cout << "Failed to bind. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(master_socket, address.sin_port) < 0){
        std::cout << "Failed to listen. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    std::cout << "Waiting for connections ..." << std::endl;

    while (1){
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for (int i = 0; i < max_clients; i++){
            sd = client[i].client_socket;
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
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
                std::cout << "Failed to accept. errno: " << errno << std::endl;
                exit(EXIT_FAILURE);
            }
            std::cout << "New connection, socket fd is " << new_socket << ", ip is: " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << std::endl;
            for (int i = 0; i < max_clients; i++){
                if (client[i].client_socket == 0){
                    client[i].client_socket = new_socket;
                    // // client[i].nickname = get_buff_val(buffer);
                    // valread = recv(client[i].client_socket, buffer, 1024, 0);
                    // std::cout << buffer;
                    // send(client[i].client_socket, ":user42!user42@salty-VirtualBox PONG :user42\r\n", 46, 0);
                    // // client[i].username = get_buff_val(buffer);
                    std::cout << "Adding to list of sockets as " << i << std::endl;
                    break;
                }
            }
        }

        for (int i = 0; i < max_clients; i++){
            sd = client[i].client_socket;

            if (FD_ISSET(sd, &readfds)){
                if ((valread = recv(sd, &buffer, 1024, 0)) == 0){
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    std::cout << "Client Disconnected, ip: " << inet_ntoa(address.sin_addr) << " port: " << ntohs(address.sin_port) << std::endl;
                    close(sd);
                    client[i].client_socket = 0;
                }
                else if (client[i].is_registered == false){
                    buffer[valread] = '\0';
                    if(register_client(client[i], buffer) == false){
                        std::cout << "Client Registration Failed" << std::endl;
                    }       
                }
                else{
                    buffer[valread] = '\0';
                    std::cout << "BUFFER: " << std::endl;
                    if (!(quit_check(buffer))){
                        close(sd);
                        client[i].client_socket = 0;
                        for (int j = 0; j < max_clients; j++){
                            if (client[j].client_socket > 0){
                                // send(client[j].client_socket, "Guest Disconnected", 18, 0);
                            }
                        }
                    }
                    check_receive(buffer, client[i]);
                    for(int j = 0; j < max_clients; j++){
                        if (client[j].client_socket > 0){
                            // send(client[j].client_socket, client[i].nickname.c_str(), client[i].nickname.length(), 0);
                            // send(client[j].client_socket, ": ", 2, 0);
                            // send(client[j].client_socket, buffer, strlen(buffer), 0);
                        }
                    }
                }
            }
        }
    }
    return 0;
}


// int main(int ac, char **av){
//     if (ac != 2){
//         std::cout << "Wrond number of args" << std::endl;
//         exit(EXIT_FAILURE);
//     }
//     pollfd socks[2];
//     int port  = get_port(av[1]);
//     socks[0].fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (socks[0].fd == -1){
//         std::cout << "Failed to create socket. errno: " << errno << std::endl;
//         exit(EXIT_FAILURE);
//     }
//     std::cout << "SOCKETED" << std::endl;
// 	sockaddr_in sockaddr;
//     sockaddr.sin_family = AF_INET;
//     sockaddr.sin_addr.s_addr = INADDR_ANY;
//     sockaddr.sin_port = htons(port);
//     if (bind(socks[0].fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0){
//         std::cout << "Failed to bind to port " << port << ", errno: " << errno << std::endl;
//         exit(EXIT_FAILURE);
//     }
//     std::cout << "BINDED" << std::endl;
//     if (listen(socks[0].fd, 10) < 0){
//         std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
//         exit(EXIT_FAILURE);
//     }
//     std::cout << "LISTENED" << std::endl;
//     auto addrlen = sizeof(sockaddr);
//     int connection = accept(socks[0].fd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
//     if (connection < 0){
//         std::cout << "Failed to grab connection. errno: " << errno << std::endl;
//         exit(EXIT_FAILURE);
//     }
//     std::cout << "CONNECTED" << std::endl;
//     std::string message;
//     while (1){
//         message = get_message(connection);
//         std::cout << "The message was: " << message;
//         std::string ret = "User X: " + message;
//         send(connection, ret.c_str(), ret.size(), MSG_DONTWAIT);
//         if (!(message.compare("/quit\n"))){
//             break;
//         }
//     }
//     std::string response = "Good talking to you \n";
//     send(connection, response.c_str(), response.size(), 0);
//     close(connection);
//     close(socks[0].fd);
//     return 0;
// }