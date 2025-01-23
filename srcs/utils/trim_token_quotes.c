/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_token_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:21:13 by prichugh          #+#    #+#             */
/*   Updated: 2025/01/23 12:21:13 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

int	find_first_quote(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (i);
		i++;
	}
	return (-1);
}

int	find_matching_quote(char *str, char quote_char, int start)
{
	int	i;

	i = start + 1;
	while (str[i])
	{
		if (str[i] == quote_char)
			return (i);
		i++;
	}
	return (-1);
}

void	trim_and_shift(char *str, int start, int end, int len)
{
	int	i;

	i = start;
	while (i < len)
	{
		str[i] = str[i + 1];
		i++;
	}
	len--;
	i = end - 1;
	while (i < len)
	{
		str[i] = str[i + 1];
		i++;
	}
	str[len - 1] = '\0';
}

void	trim_quotes(char *str)
{
	int		len;
	int		start;
	int		end;
	char	quote_char;

	len = 0;
	start = -1;
	end = -1;
	while (str[len])
		len++;
	start = find_first_quote(str);
	if (start == -1)
		return ;
	quote_char = str[start];
	end = find_matching_quote(str, quote_char, start);
	if (end == -1 || end <= start)
		return ;
	trim_and_shift(str, start, end, len);
}

void	trim_file_quotes(char *str)
{
	int		i;
	int		j;
	char	temp[1024];

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			i++;
			continue ;
		}
		temp[j++] = str[i++];
	}
	temp[j] = '\0';
	i = 0;
	while (temp[i])
	{
		str[i] = temp[i];
		i++;
	}
	str[i] = '\0';
}
