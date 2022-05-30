/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 13:38:26 by user42            #+#    #+#             */
/*   Updated: 2022/05/30 11:19:54 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <iterator>
#include <utility>
#include <iostream>
#include <sys/socket.h>
#include "utils.hpp"

template<class client>
class channel
{
private:
    
    
public:

    std::map<int, client> clients;
    std::map<int, client> ops;
    std::string name;
    std::string topic;
    std::string mods;
    int number_clients;

    channel(){};
    ~channel(){};

    void add_client(int sock, client &cli){
        this->clients.insert(std::make_pair(sock, cli));
    }

    void    add_op(int sock, client &cli){
        this->ops.insert(std::make_pair(sock, cli));
    }

    void    send_to_clients(std::string toSend, int nosend){
        // std::string tmp;
        std::cout << "TOSEND: " << std::endl;
        typename std::map<int, client>::iterator it = this->clients.begin();
        typename std::map<int, client>::iterator it2 = this->ops.begin();
        while (it != this->clients.end()){
            if (it->first != nosend){
                send(it->first, toSend.c_str(), toSend.length(), 0);
            }
            it++;
        }
        while (it2 != this->ops.end()){
            if (it2->first != nosend){
                send(it2->first, toSend.c_str(), toSend.length(), 0);
            }
            it2++;
        }
    }

    int    is_op(int check){
        typename std::map<int, client>::iterator it = this->ops.begin();
        while (it != this->ops.end()){
            if (check == it->first){
                return 1;
            }
            it++;
        }
        return 0;
    }

    std::string get_nicks(std::string op){
        std::string toRet = "";
        typename std::map<int, client>::iterator it = this->ops.begin();
        typename std::map<int, client>::iterator it2 = this->clients.begin();
        while (it != this->ops.end()){
            toRet += op + it->second.nickname;
            it++;
            if (it != this->ops.end() || it2 != this->clients.end()){
                toRet += " ";
            }
        }
        while (it2 != this->clients.end()){
            toRet += it2->second.nickname;
            it2++;
            if (it2 != this->clients.end()){
                toRet += " ";
            }
        }
        return toRet;
    }

    std::string get_users(){
        std::string toRet = "";
        typename std::map<int, client>::iterator it = this->ops.begin();
        typename std::map<int, client>::iterator it2 = this->clients.begin();
        while (it != this->ops.end()){
            toRet += it->second.username;
            it++;
            if (it != this->ops.end()){
                toRet += " ";
            }
        }
        while (it2 != this->clients.end()){
            toRet += it2->second.username;
            it2++;
            if (it2 != this->clients.end()){
                toRet += " ";
            }
        }
        return toRet;
    }
};
#endif