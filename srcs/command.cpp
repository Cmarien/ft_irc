/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 15:09:54 by user42            #+#    #+#             */
/*   Updated: 2022/05/26 13:27:07 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"


std::string    nick(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    std::string toRet = ":" + cli.get_prefix() + " NICK :" + args;
    cli.nickname = args;
    return toRet;
}
std::string    ping(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    return "PONG " + args;
}
std::string    pong(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    return "PING " + args;
}
std::string    join(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    int index = 0;
    std::string toRet;
    std::cout << toRet[0] << std::endl;
    while (index < 30 && serv.channels[index].name != ""){
        if (serv.channels[index].name.compare(args) == 0){
            serv.channels[index].add_client(cli.client_socket, cli);
            if (serv.channels[index].topic != ""){
                toRet += ":" + cli.get_prefix() + " 332 " + cli.nickname + " " + get_replies(332, serv.channels[index].name, serv.channels[index].topic, "", "", "","","") + LINEEND;
            }
            toRet += ":" + cli.get_prefix() + " 353 " + cli.nickname + " " + get_replies(353, serv.channels[index].mods, serv.channels[index].name, serv.channels[index].get_nicks("@"), "", "", "", "") + LINEEND;
            toRet += ":" + cli.get_prefix() + " 366 " + cli.nickname + " " + get_replies(366, serv.channels[index].name, "", "", "", "", "", "") + LINEEND;
            toRet += ":" + cli.get_prefix() + " JOIN :" + serv.channels[index].name;
            return toRet;
        }
        index++;
    }
    if (index >= 30){
        std::cout << "all chann full" << std::endl;
    }
    else{
        serv.channels[index].add_op(cli.client_socket, cli);
        serv.channels[index].name = args;
        if (serv.channels[index].topic != ""){
            toRet += ":" + cli.get_prefix() + " 332 " + cli.nickname + " " + get_replies(332, serv.channels[index].name, serv.channels[index].topic, "", "", "","","") + LINEEND;
        }
        toRet += ":" + cli.get_prefix() + " 353 " + cli.nickname + " " + get_replies(353, serv.channels[index].mods, serv.channels[index].name, serv.channels[index].get_nicks("@"), "", "", "", "") + LINEEND;
        toRet += ":" + cli.get_prefix() + " 366 " + cli.nickname + " " + get_replies(366, serv.channels[index].name, "", "", "", "", "", "") + LINEEND;
        toRet += ":" + cli.get_prefix() + " JOIN :" + serv.channels[index].name;
    }
    return toRet;
}
std::string    part(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    std::string toRet = ":" + cli.get_prefix() + " PART " + args;
    return toRet;
}
std::string    quit(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    close(cli.client_socket);
    cli.clear_client();
    return args;
}
std::string    privmsg(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    std::string toRet = ":" + cli.get_prefix() + " PRIVMSG " + args;
    int index = 0;

    while (index < 30){
        if (serv.channels[index].name.compare("#test") == 0){
            serv.channels[index].send_to_clients(toRet);
            break;
        }
        index++;
    }
    return toRet;
}

std::string    mode(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    int index = 0;

    while (index < 30){
        if (serv.channels[index].name.compare(args) == 0){
            break;
        }
        index++;
    }
    if (index >= 30){
        return "";
    }
    std::string toRet = ":" + cli.get_prefix() + " 324 " + cli.nickname + " " + get_replies(324, serv.channels[index].name, serv.channels[index].mods, "", "", "","","");
    return toRet;
}

std::string    who(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    int index = 0;

    while (index < 30){
        if (serv.channels[index].name.compare(args) == 0){
            break;
        }
        index++;
    }
    if (index >= 30){
        return "";
    }
    std::string toRet = ":" + cli.get_prefix() + " 315 " + cli.nickname + " " + get_replies(315, serv.channels[index].get_users(), "", "", "", "", "", "");
    return toRet;
}
