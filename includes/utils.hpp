/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouduri <rmouduri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:16:00 by user42            #+#    #+#             */
/*   Updated: 2022/05/30 18:35:40 by rmouduri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>

void    clean_string(std::string &str);
int     get_port(std::string str);
bool    check_charset(char, std::string);
bool    check_modes(char, std::vector<char>);
std::string get_replies(unsigned int rpl, std::string arg1, std::string arg2, std::string arg3, std::string arg4, std::string arg5, std::string arg6, std::string arg7);
void printerr(std::string str);

#endif