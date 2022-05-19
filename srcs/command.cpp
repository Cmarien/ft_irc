/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 15:09:54 by user42            #+#    #+#             */
/*   Updated: 2022/05/19 17:25:44 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

std::string    nick(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    return args;
}
std::string    ping(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    return "PONG " + args;
}
std::string    pong(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    return "PING " + args;
}
std::string    join(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    return args;
}
std::string    part(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    return args;
}
std::string    quit(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    close(cli.client_socket);
    cli.clear_client();
    return args;
}
std::string    privmsg(std::string args, __attribute__((unused)) server &serv, __attribute__((unused)) client& cli){
    return args;
}
