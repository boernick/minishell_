/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:04:33 by nboer             #+#    #+#             */
/*   Updated: 2024/05/22 13:11:23 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	ft_charinstr(char c, const char *set)
{
	size_t		i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*p;
	size_t	i;
	size_t	front;
	size_t	end;

	front = 0;
	i = 0;
	end = ft_strlen(s1);
	while (s1[front] && ft_charinstr(s1[front], set))
		front++;
	if (front != end)
	{
		while (s1[end - 1] && ft_charinstr(s1[end - 1], set))
			end--;
	}
	p = malloc((end - front + 1) * sizeof(char));
	if (!p)
		return (NULL);
	while (end > front)
		p[i++] = s1[front++];
	p[i] = '\0';
	return (p);
}
