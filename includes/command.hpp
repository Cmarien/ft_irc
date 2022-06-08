/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:31:39 by user42            #+#    #+#             */
/*   Updated: 2022/06/02 10:09:15 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include "server.hpp"

    std::string    nick(std::string args, server &serv, client&);
    std::string    ping(std::string args, server &serv, client&);
    std::string    pong(std::string args, server &serv, client&);
    std::string    join(std::string args, server &serv, client&);
    std::string    part(std::string args, server &serv, client&);
    std::string    quit(std::string args, server &serv, client&);
    std::string    privmsg(std::string args, server &serv, client&);
    std::string    mode(std::string args, server &serv, client&);
    std::string    who(std::string args, server &serv, client&);

	bool isNicknameAvailable(std::string nickname, std::vector<client> clients);
	bool isValidNickname(std::string nickname);

#endif