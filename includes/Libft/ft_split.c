/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 12:08:42 by nboer             #+#    #+#             */
/*   Updated: 2024/05/22 13:33:30 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	word_count(char *str, char c)
{
	int	toggle;
	int	i;

	toggle = 0;
	i = 0;
	if (!str)
		return (0);
	while (*str)
	{
		if (*str != c && toggle == 0)
		{
			toggle = 1;
			i++;
		}
		if (*str == c)
			toggle = 0;
		str++;
	}
	return (i);
}

static int	ft_find_wordend(char const *str, int pos, char c)
{
	int	i;

	i = 0;
	while (str[pos + i] != '\0' && str[pos + i] != c)
		i++;
	return (i);
}

static char	*ft_write_word(char const *str, int *pos, char c)
{
	char	*word;
	int		wordend;
	int		i;

	i = 0;
	wordend = ft_find_wordend(str, *pos, c);
	word = malloc((wordend + 1) * sizeof (char));
	if (!word)
		return (NULL);
	while (i < wordend)
	{
		word[i] = str[*pos];
		(*pos)++;
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	ft_free_mem(char **list)
{
	int	i;

	i = 0;
	while (list[i] != NULL)
	{
		free(list[i]);
		i++;
	}
	free(list);
}

char	**ft_split(char const *str, char c)
{
	char	**list;
	int		i;
	int		pos;
	int		wordcount;

	i = 0;
	pos = 0;
	wordcount = word_count((char *) str, c);
	list = malloc((wordcount + 1) * sizeof (char *));
	if (!list)
		return (NULL);
	while (i < wordcount)
	{
		while (str[pos] == c)
			pos++;
		list[i] = ft_write_word(str, &pos, c);
		if (list[i] == NULL)
		{
			ft_free_mem(list);
			return (NULL);
		}
		i++;
	}
	list[i] = NULL;
	return (list);
}
