/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 12:50:56 by nboer             #+#    #+#             */
/*   Updated: 2024/05/22 13:33:37 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	char	*p;
	int		len;
	int		i;

	i = 0;
	len = ft_strlen(str);
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
