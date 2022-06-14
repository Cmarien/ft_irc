/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 13:38:26 by user42            #+#    #+#             */
/*   Updated: 2022/06/14 18:24:20 by user42           ###   ########.fr       */
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
    std::vector<char> modes;
    int number_clients;

    channel(){this->number_clients = 0;}
    ~channel(){};

    void    remove_op(int sock){
        typename std::map<int, client>::iterator it = this->ops.begin();
        typename std::map<int, client>::iterator ite = this->ops.end();

        while (it != ite){
            if (it->first == sock){
                this->ops.erase(it);
                break;
            }
            it++;
        }
        this->number_clients--;
    }

    void    remove_client(int sock){
        typename std::map<int, client>::iterator it = this->clients.begin();
        typename std::map<int, client>::iterator ite = this->clients.end();

        while (it != ite){
            if (it->first == sock){
                this->clients.erase(it);
                break;
            }
            it++;
        }
        this->number_clients--;
    }

    void	remove_mode(char toRemove){
        std::vector<char>::iterator ite = this->modes.end();

        for (std::vector<char>::iterator it = this->modes.begin(); it != ite; it++){
            if (*it == toRemove){
                this->modes.erase(it);
                break;
            }
        }
    }

    std::string get_modes(){
        std::string toRet = "";
        std::vector<char>::iterator ite = this->modes.end();
        
        if (this->modes.size()){
            toRet += "+";
        }
        for (std::vector<char>::iterator it = this->modes.begin(); it != ite; it++){
            toRet += *it;
        }
        return toRet;
    }

    std::string add_modes(std::string modestring, __attribute__((unused)) std::string mode_arg, client &cli){
        int index = 0;
        int err_check = 0;
        std::string toRet;

        if (modestring[0] == '+' || modestring[0] == '-'){
            index = 1;
        }
        
        if (is_op(mode_arg) == 0 && is_client(mode_arg) == 0){
            toRet = ":" + cli.get_prefix() + " 442 " + cli.nickname + " " + get_replies(442, this->name, "", "", "", "", "", "");
        }

        if (is_op(cli.client_socket) == 0){
            toRet = ":" + cli.get_prefix() + " 482 " + cli.nickname + " " + get_replies(482, this->name, "", "", "", "", "", "");
            return toRet;
        }

        if (modestring[0] != '-'){
            while (modestring[index] && modestring[index] != '-'){
                if (check_charset(modestring[index], "ovimnptkl")){
                    if (check_modes(modestring[index], this->modes)){
                        if (modestring[index] == 'o'){
                            if (is_client(mode_arg)){
                                put_op(mode_arg);
                            }
                            else if (is_op(mode_arg) == 0){
                                if (mode_arg == ""){
                                    toRet = ":" + cli.get_prefix() + " 431 " + cli.nickname + " " + get_replies(431, "", "", "", "", "", "", "");
                                }
                                else{
                                    toRet = ":" + cli.get_prefix() + " 401 " + cli.nickname + " " + get_replies(401, mode_arg, "", "", "", "", "", "");
                                }
                            }
                        }
                    }
                }
                else{
                    err_check = 1;
                }
                index++;
            }
            while (modestring[index]){
                if (check_charset(modestring[index], "ovimnptkl")){
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
                if (check_charset(modestring[index], "Oovimnptkl")){
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
                if (check_charset(modestring[index], "Oovimnptkl")){
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
        toRet = ":" + cli.get_prefix() + " 324 " + cli.nickname + " " + get_replies(324, this->get_modes(), "", "", "", "", "", "");
        if (err_check){
            toRet += "\r\n:" + cli.get_prefix() + " 501 " + cli.nickname + " " + get_replies(501, "", "", "", "", "", "" ,"");
        }
        return toRet;
    }

    void add_client(int sock, client &cli){
        this->clients.insert(std::make_pair(sock, cli));
        this->number_clients++;
    }

    void    add_op(int sock, client &cli){
        this->ops.insert(std::make_pair(sock, cli));
        this->number_clients++;
    }

    void    send_to_clients(std::string toSend, int nosend){
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

    void    put_op(std::string toOp){
        typename std::map<int, client>::iterator it = this->clients.begin();
        while (it != this->clients.end()){
            if (toOp.compare(it->second.nickname) == 0){
                this->ops.insert(std::make_pair(it->first, it->second));
                this->clients.erase(it);
                break;
            }
            it++;
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

    int    is_op(std::string check){
        typename std::map<int, client>::iterator it = this->ops.begin();
        while (it != this->ops.end()){
            if (check.compare(it->second.nickname) == 0){
                return 1;
            }
            it++;
        }
        return 0;
    }

    int is_client(int check){
        typename std::map<int, client>::iterator it = this->clients.begin();
        while (it != this->clients.end()){
            if (check == it->first){
                return 1;
            }
            it++;
        }
        return 0;
    }

    int is_client(std::string check){
        typename std::map<int, client>::iterator it = this->clients.begin();
        while (it != this->clients.end()){
            if (check.compare(it->second.nickname) == 0){
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