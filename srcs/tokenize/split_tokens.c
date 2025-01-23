/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:27:50 by prichugh          #+#    #+#             */
/*   Updated: 2025/01/23 12:27:50 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_redirection(char *input, t_parse *data, int *i)
{
	handle_buffer(data, TOKEN_WORD);
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token_to_list(data, new_token(TOKEN_REDIR_APPEND, ">>"));
		*i += 2;
	}
	else if (input[*i] == '>')
	{
		add_token_to_list(data, new_token(TOKEN_REDIR_OUT, ">"));
		(*i)++;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token_to_list(data, new_token(TOKEN_HEREDOC, "<<"));
		*i += 2;
	}
	else
	{
		add_token_to_list(data, new_token(TOKEN_REDIR_IN, "<"));
		(*i)++;
	}
}

void	process_pipe(char *input, t_parse *data, int *i)
{
	(void)input;
	handle_buffer(data, TOKEN_WORD);
	add_token_to_list(data, new_token(TOKEN_PIPE, "|"));
	(*i)++;
}

void	process_whitespace(t_parse *data, int *i)
{
	handle_buffer(data, TOKEN_WORD);
	(*i)++;
}

void	process_quote(char *input, t_parse *data, int *i)
{
	if (input[*i] == '\'' && !data->in_double_quote)
	{
		data->buffer[data->buf_index++] = input[*i];
		data->in_single_quote = !data->in_single_quote;
		(*i)++;
	}
	if (input[*i] == '\"' && !data->in_single_quote)
	{
		data->buffer[data->buf_index++] = input[*i];
		data->in_double_quote = !data->in_double_quote;
		(*i)++;
	}
}

void	split_tokens(char *input, t_parse *data)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		if (data->buf_index >= BUFFER_SIZE - 1)
			exit_perror("Buffer overflow detected\n");
		if ((input[i] == '\'' && !data->in_double_quote)
			|| (input[i] == '\"' && !data->in_single_quote))
			process_quote(input, data, &i);
		else if ((input[i] == ' ' || input[i] == '\t' || input[i] == '\n')
			&& !data->in_single_quote && !data->in_double_quote)
			process_whitespace(data, &i);
		else if (input[i] == '|'
			&& !(data->in_single_quote || data->in_double_quote))
			process_pipe(input, data, &i);
		else if ((input[i] == '>' || input[i] == '<')
			&& !(data->in_single_quote || data->in_double_quote))
			process_redirection(input, data, &i);
		else
			data->buffer[data->buf_index++] = input[i++];
	}
	finalize_splitting(data);
}
