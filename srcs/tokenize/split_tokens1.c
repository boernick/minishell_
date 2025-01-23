/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:23:42 by prichugh          #+#    #+#             */
/*   Updated: 2025/01/23 12:23:42 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

int	validate_heredoc(t_token *next, t_parse *data)
{
	if (!next || next->type != TOKEN_WORD)
	{
		if (next)
			syntax_error(next->value, data);
		else
			syntax_error("newline", data);
		return (0);
	}
	return (1);
}

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		malloc_error(sizeof(t_token));
	token->type = type;
	token->value = ft_strdup(value);
	token->next = NULL;
	return (token);
}

void	add_token_to_list(t_parse *data, t_token *new_token)
{
	if (data->head == NULL)
		data->head = new_token;
	else
		data->tail->next = new_token;
	data->tail = new_token;
}

void	handle_buffer(t_parse *data, t_token_type token_type)
{
	if (data->buf_index > 0)
	{
		data->buffer[data->buf_index] = '\0';
		add_token_to_list(data, new_token(token_type, data->buffer));
		data->buf_index = 0;
	}
}

void	finalize_splitting(t_parse *data)
{
	handle_buffer(data, TOKEN_WORD);
	if (data->in_single_quote || data->in_double_quote)
	{
		data->valid_input = 0;
		ft_putstr_fd("minishell: syntax error: unclosed quote detected\n",
			STDERR_FILENO);
		free_tokens(data->head);
		data->head = NULL;
		data->tail = NULL;
	}
	else
	{
		data->valid_input = 1;
	}
}

// void	append_character(char *input, t_parse *data, int *i)
// {
// 	data->buffer[data->buf_index++] = input[*i];
// 	(*i)++;
// }
