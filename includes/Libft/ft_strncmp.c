/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 15:29:15 by nboer             #+#    #+#             */
/*   Updated: 2024/05/22 13:33:44 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	while ((str1[i] != '\0' || str2[i] != '\0') && i < n)
	{
		j = (unsigned char) str1[i] - (unsigned char) str2[i];
		if (j != 0)
			return (j);
		i++;
	}
	return (0);
}
/*
#include <stdio.h>
int	main(void)
{
	char str[] = "1234";
	char str2[] = "1235";
	int res = ft_strncmp(str, str2, 4);
	printf("%u", res);
	return 0;
}*/
