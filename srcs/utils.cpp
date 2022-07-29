/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 20:13:24 by mababou           #+#    #+#             */
/*   Updated: 2022/07/29 16:58:40 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// ERROR HANDLING
void handle_error(const char * msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

// LIBFT EQUIVALENT
void ft_memset(void *ptr, int data, size_t len)
{
	char *tmp = (char *)ptr;

	for (size_t i = 0; i < len; ++i)
		tmp[i] = data;
}
