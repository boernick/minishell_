/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 18:33:31 by nboer             #+#    #+#             */
/*   Updated: 2024/05/22 13:10:55 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_length_dest(char *dest, size_t size)
{
	size_t	i;

	i = 0;
	while (dest[i] != '\0')
		i++;
	if (i < size)
		return (i);
	else
		return (size);
}

static size_t	get_length_src(const char *src)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
		i++;
	return (i);
}

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	l_dest;
	size_t	l_src;
	size_t	j;

	j = 0;
	l_dest = get_length_dest(dest, size);
	l_src = get_length_src(src);
	if (size > l_dest)
	{
		while (src[j] != '\0' && ((l_dest + j) < (size - 1)))
		{
			dest[l_dest + j] = src[j];
			j++;
		}
		dest[l_dest + j] = '\0';
	}
	return (l_dest + l_src);
}
/*
int	 main(void)
{
	#include <stdio.h> 
	char dest[5] = "test";
	char src[5] = "";
	size_t result = ft_strlcat(dest, src, 12);
	printf("Result: %li\n", result);
	return (0);
}
*/
