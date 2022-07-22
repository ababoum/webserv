/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 20:07:54 by mababou           #+#    #+#             */
/*   Updated: 2022/07/22 20:46:28 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // For sockaddr_in
#include <arpa/inet.h> // For inet_addr
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <iostream>

#define MAX_CONNECTIONS	1024

typedef struct sockaddr_in t_sockaddr_in;


// errors
void handle_error(const char * msg);

// Libft Equivalent
void ft_memset(void *ptr, int data, size_t len);


#endif