/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 17:14:57 by user42            #+#    #+#             */
/*   Updated: 2022/05/18 18:04:34 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"


void    config::edit_config(std::string str){
    int index = 0;
    int tmpdex = 0;
    std::string tmp;
    while (str[index] != '='){
        index++;
    }
    tmp = str.substr(tmpdex, index);
    tmpdex = index + 1;
    while (str[index] != '\n' && str[index]){
        index++;
    }
    if (tmp.compare("name") == 0){
        this->server_name = str.substr(tmpdex, index - tmpdex);
    }
    else if (tmp.compare("version") == 0){
        this->version = str.substr(tmpdex, index - tmpdex);
    }
    else if (tmp.compare("motd") == 0){
        this->motd = str.substr(tmpdex, index - tmpdex);
    }
}

void    config::init_config(std::string buff){
    int index = 0;
    int tmpdex;
    std::string tmp;
    while (buff[index]){
        tmpdex = index;
        while (buff[index] != '\n' && buff[index]){
            index++;
        }
        tmp = buff.substr(tmpdex, index - tmpdex);
        edit_config(tmp);
        index++;
    }
}

config::config(){
    int fd;
    char buff[1025];
    if ((fd = open("config", O_RDONLY)) < 0){
        std::cout << "Error with OPEN" << std::endl;
    }
    read(fd, buff, 1024);
    this->init_config(buff);
}

config::~config()
{
}
