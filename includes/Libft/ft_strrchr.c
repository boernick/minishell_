/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 14:59:12 by nboer             #+#    #+#             */
/*   Updated: 2024/05/22 13:33:46 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int		len;
	char	*p;

	len = ft_strlen(str);
	p = (char *)str + len;
	if ((char)c == '\0')
		return (p);
	while (len-- >= 0)
	{
		if (*p == (char)c)
			return (p);
		p--;
	}
	return (NULL);
}
