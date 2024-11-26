/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 17:34:49 by nboer             #+#    #+#             */
/*   Updated: 2024/05/22 13:33:24 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*c_dest;
	const char	*c_src;
	size_t		i;

	c_dest = dest;
	c_src = src;
	if (c_src < c_dest)
	{
		while (n > 0)
		{
			c_dest[n - 1] = c_src[n - 1];
			n--;
		}
	}
	else if (c_src > c_dest)
	{
		i = 0;
		while (i < n)
		{
			c_dest[i] = c_src[i];
			i++;
		}
	}
	return (dest);
}
/*
#include <stdio.h>
int	main(void)
{
	char src[] = "";
	char dest[20];
	ft_memmove(dest, src, 3);
	printf("%s", dest);
	return(0);
}*/
