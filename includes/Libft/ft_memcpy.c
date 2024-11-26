/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 16:05:07 by nboer             #+#    #+#             */
/*   Updated: 2024/05/22 13:33:23 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*c_dest;
	const char	*c_src;	
	size_t		i;

	c_dest = dest;
	c_src = src;
	i = 0;
	if (!dest && !src)
		return (NULL);
	while (i < n)
	{
		c_dest[i] = c_src[i];
		i++;
	}
	return (dest);
}
/*
#include <stdio.h>
int	main(void)
{
	char src[1] = '\0';
	char dest[1] = '\0';
	ft_memcpy('\0', '\0', 5);
	printf("%s", dest);
	return(0);
}*/
