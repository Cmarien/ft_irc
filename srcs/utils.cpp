/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:35:28 by user42            #+#    #+#             */
/*   Updated: 2022/05/19 16:59:52 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void    clean_string(std::string &str){
    if (str[str.length() - 1] == '\n'){
        str = str.substr(0, str.length() -2);
    }
    if (str[str.length() - 1] == '\r'){
        str = str.substr(0, str.length() -2);
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