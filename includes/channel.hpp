/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouduri <rmouduri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 13:38:26 by user42            #+#    #+#             */
/*   Updated: 2022/06/06 19:15:24 by rmouduri         ###   ########.fr       */
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
    std::vector<int> banlist;
    std::vector<std::string> invitelist;
    int number_clients;

    bool    invite_only;

    channel(){
        this->number_clients = 0;
        this->invite_only = false;
    }
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

    void    remove_client(std::string name){
        typename std::map<int, client>::iterator it = this->clients.begin();
        typename std::map<int, client>::iterator ite = this->clients.end();

        while (it != ite){
            if (it->second.nickname.compare(name) == 0){
                this->clients.erase(it);
                break;
            }
            it++;
        }
        this->number_clients--;
    }

    void    kick_client(std::string name){
        typename std::map<int, client>::iterator it = this->clients.begin();
        typename std::map<int, client>::iterator ite = this->clients.end();
        std::string toRet = ":";

        while (it != ite){
            if (it->second.nickname.compare(name) == 0){
                toRet += it->second.get_prefix() + " PART " + this->name + "\r\n";
                std::cout << toRet << std::endl;
                send(it->first, toRet.c_str(), toRet.length(), 0);
                this->clients.erase(it);
                this->number_clients--;
                return ;
            }
            it++;
        }
    }

    void    ban_client(std::string name){
        typename std::map<int, client>::iterator it = this->clients.begin();
        typename std::map<int, client>::iterator ite = this->clients.end();
        std::string toRet = ":";

        while (it != ite){
            if (it->second.nickname.compare(name) == 0){
                toRet += it->second.get_prefix() + " PART " + this->name + "\r\n";
                std::cout << toRet << std::endl;
                this->banlist.push_back(it->first);
                send(it->first, toRet.c_str(), toRet.length(), 0);
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
                if (check_charset(modestring[index], "ovimnptklb")){
                    if (check_modes(modestring[index], this->modes)){
                        if (modestring[index] == 'o'){
                            if (is_client(mode_arg)){
                                put_op(mode_arg);
                            }
                            else if (is_op(mode_arg) == 0){
                                if (mode_arg == ""){
                                    return ":" + cli.get_prefix() + " 431 " + cli.nickname + " " + get_replies(431, "", "", "", "", "", "", "");
                                }
                                else{
                                    return ":" + cli.get_prefix() + " 401 " + cli.nickname + " " + get_replies(401, mode_arg, "", "", "", "", "", "");
                                }
                            }
                        }
                        else if (modestring[index] == 'b'){
                            std::cout << mode_arg << std::endl;
                            if (is_client(mode_arg)){
                                ban_client(mode_arg);
                            }
                            else if (is_op(mode_arg) == 0){
                                // return ERR_USERNOTINCHANNEL(mode_arg, this->name);
                                return ":" + cli.get_prefix() + " 441 " + cli.nickname + " " + get_replies(443, mode_arg, this->name, "", "", "", "", "");
                            }
                        }
                        else if (modestring[index] == 'i'){
                            if (this->invite_only == false){
                                this->invite_only = true;
                                this->modes.push_back('i');
                            }
                        }
                        else{
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
                if (check_charset(modestring[index], "ovimnptkbl")){
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
                if (check_charset(modestring[index], "Oovimnbptkl")){
                    if (check_modes(modestring[index], this->modes) == 0){
                        if (modestring[index] != 'b'){
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
                        if (modestring[index] == 'o'){
                            if (is_client(mode_arg)){
                                put_op(mode_arg);
                            }
                            else if (is_op(mode_arg) == 0){
                                if (mode_arg == ""){
                                    return ":" + cli.get_prefix() + " 431 " + cli.nickname + " " + get_replies(431, "", "", "", "", "", "", "");
                                }
                                else{
                                    return ":" + cli.get_prefix() + " 401 " + cli.nickname + " " + get_replies(401, mode_arg, "", "", "", "", "", "");
                                }
                            }
                        }
                        else if (modestring[index] == 'b'){
                            if (is_client(mode_arg)){
                                ban_client(mode_arg);
                            }
                            else if (is_op(mode_arg) == 0){
                                // return ERR_USERNOTINCHANNEL(mode_arg, this->name);
                                return ":" + cli.get_prefix() + " 441 " + cli.nickname + " " + get_replies(443, mode_arg, this->name, "", "", "", "", "");
                            }
                        }
                        else if (modestring[index] == 'i'){
                            if (this->invite_only == false){
                                this->invite_only = true;
                                this->modes.push_back('i');
                            }
                        }
                        else{
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

    void    de_op(std::string   deop){
        typename std::map<int, client>::iterator it = this->ops.begin();
        while (it != this->ops.end()){
            if (deop.compare(it->second.nickname) == 0){
                this->clients.insert(std::make_pair(it->first, it->second));
                this->ops.erase(it);
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
                return check;
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

    int is_invited(std::string check){
        std::vector<std::string>::iterator ite = invitelist.end();

        for (std::vector<std::string>::iterator it = invitelist.begin(); it != ite; it++){
            if (it->compare(check) == 0){
                return 1;
            }
        }
        return 0;
    }

    int is_banned(int check){
        std::vector<int>::iterator ite = banlist.end();

        for (std::vector<int>::iterator it = banlist.begin(); it != ite; it++){
            if (*it == check){
                return 1;
            }
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