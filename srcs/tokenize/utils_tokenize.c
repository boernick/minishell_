/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:30:25 by prichugh          #+#    #+#             */
/*   Updated: 2024/11/13 18:30:25 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*trim_whitespace(char *str)
{
	int	start;
	int	end;

	start = 0;
	end = ft_strlen(str) - 1;
	while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n')
		start++;
	while (end >= start && (str[end] == ' '
			|| str[end] == '\t' || str[end] == '\n'))
		end--;
	str[end + 1] = '\0';
	return (&str[start]);
}

void	free_tokens(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head->next;
		if (head->value)
			free(head->value);
		free(head);
		head = temp;
	}
}
