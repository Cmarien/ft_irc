/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:31:39 by user42            #+#    #+#             */
/*   Updated: 2022/05/18 14:32:15 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>

class command
{
private:
public:
    command(){}
    ~command(){}
    // void    exec_command(std::string);
    std::string get_replies(unsigned int rpl, std::string arg1, std::string arg2, std::string arg3, std::string arg4, std::string arg5, std::string arg6, std::string arg7);

// void    command::exec_command(std::string comm){
// ;
// }
};

#endif