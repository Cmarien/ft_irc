/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 15:51:27 by user42            #+#    #+#             */
/*   Updated: 2022/05/30 14:10:33 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "server.hpp"

int quit_check(char *buffer){
    std::string tmp = buffer;
    return tmp.compare("/quit\r\n");
}

int main(int ac, char **av){
    if (ac != 3){
        std::cout << "Wrond number of args" << std::endl;
        exit(EXIT_FAILURE);
    }
    server server;
    server.init_server(av[1], av[2]);
    std::cout << "FINI ???" << std::endl;
    return 0;
}