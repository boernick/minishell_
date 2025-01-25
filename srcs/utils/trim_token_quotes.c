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

void	trim_quotes_handler(char *input, t_parse *data, int *i)
{
	if (input[*i] == '\'' && !data->in_double_quote)
	{
		data->in_single_quote = !data->in_single_quote;
		(*i)++;
	}
	else if (input[*i] == '\"' && !data->in_single_quote)
	{
		data->in_double_quote = !data->in_double_quote;
		(*i)++;
	}
}

void	trim_quotes(char *str, t_parse *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	data->in_double_quote = 0;
	data->in_single_quote = 0;
	while (str[i] != '\0')
	{
		if ((str[i] == '\'' && !data->in_double_quote)
			|| (str[i] == '\"' && !data->in_single_quote))
			trim_quotes_handler(str, data, &i);
		else
		{
			str[j] = str[i];
			j++;
			i++;
		}
	}
	str[j] = '\0';
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
