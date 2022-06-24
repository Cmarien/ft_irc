/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouduri <rmouduri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:35:28 by user42            #+#    #+#             */
/*   Updated: 2022/06/06 16:43:53 by rmouduri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "utils.hpp"

bool    check_charset(char toCheck, std::string charset){
    int index = 0;

    while (charset[index]){
        if (toCheck == charset[index]){
            return true;
        }
        index++;
    }
    return false;
}

bool    check_modes(char toCheck, std::vector<char> modes){
    std::vector<char>::const_iterator ite = modes.end();

    for (std::vector<char>::const_iterator it = modes.begin(); it != ite; it++){
        if (*it == toCheck){
            return false;
        }
    }
    return true;
}

void    clean_string(std::string &str){
    if (str[str.length() - 1] == '\n'){
        str = str.substr(0, str.length() -1);
    }
    if (str[str.length() - 1] == '\r'){
        str = str.substr(0, str.length() -1);
    }
}

int get_port(std::string str){
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

void printerr(std::string str) {
	std::cerr << str << std::endl;
}