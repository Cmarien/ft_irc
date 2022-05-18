/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:35:28 by user42            #+#    #+#             */
/*   Updated: 2022/05/18 17:18:24 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void    clean_string(std::string &str){
    if (str[str.length() - 1] == '\n'){
        str.pop_back();
    }
    if (str[str.length() - 1] == '\r'){
        str.pop_back();
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