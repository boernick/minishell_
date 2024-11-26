/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 15:22:10 by nboer             #+#    #+#             */
/*   Updated: 2024/05/22 13:33:18 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *str, size_t n)
{
	unsigned char	*p;

	p = str;
	while (n > 0)
	{
		*p = '\0';
		p++;
		n--;
	}
}
/*
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char str[] = "test";
	ft_bzero(str, 3);
	int i;
	i = 0;
	while (str[i] == '\0')
	{
		printf("%c", 'y');
		i++;
	}
	return (0);
}*/
