/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 14:46:16 by nboer             #+#    #+#             */
/*   Updated: 2025/01/12 14:48:02 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *str, int len)
{
	char	*p;
	int		i;

	i = 0;
	p = (char *) malloc((len + 1) * sizeof(char));
	if (p != NULL)
	{
		while (i < len)
		{
			p[i] = str[i];
			i++;
		}
		p[i] = '\0';
	}
	return (p);
}
