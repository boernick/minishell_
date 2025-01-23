/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:20:52 by prichugh          #+#    #+#             */
/*   Updated: 2025/01/23 12:20:52 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

int	validate_pipe_start(t_token *tokens, t_parse *data)
{
	if (tokens->value[0] == '|')
	{
		syntax_error("|", data);
		return (0);
	}
	return (1);
}

int	validate_operator_sequence(t_token *next, t_parse *data)
{
	if (!next || next->type == TOKEN_PIPE)
	{
		if (next)
			syntax_error(next->value, data);
		else
			syntax_error("newline", data);
		return (0);
	}
	return (1);
}

int	validate_operator_sequence2(t_token *next, t_parse *data, t_shell *shell)
{
	char	*expanded_var;

	if ((next && (next->type == TOKEN_REDIR_IN
				|| next->type == TOKEN_REDIR_OUT
				|| next->type == TOKEN_REDIR_APPEND)))
	{
		syntax_error(next->value, data);
		return (0);
	}
	if (next->value[0] == '$')
	{
		expanded_var = replace_variables_in_heredoc(next->value, data, shell);
		if (!expanded_var || expanded_var[0] == '\0')
		{
			ambiguous_redirect_error(next->value);
			free(expanded_var);
			data->exit = 1;
			return (0);
		}
		free(expanded_var);
	}
	return (1);
}

int	validate_tokens(t_token *current, t_parse *data, t_shell *shell)
{
	if (current->type == TOKEN_PIPE || current->type == TOKEN_REDIR_IN
		|| current->type == TOKEN_REDIR_OUT
		|| current->type == TOKEN_REDIR_APPEND)
	{
		if (!validate_operator_sequence(current->next, data))
			return (0);
	}
	if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
		|| current->type == TOKEN_REDIR_APPEND)
	{
		if (!validate_operator_sequence2(current->next, data, shell))
			return (0);
	}
	if (current->type == TOKEN_HEREDOC
		&& !validate_heredoc(current->next, data))
		return (0);
	return (1);
}

int	validate_input(t_token *tokens, t_parse *data, t_shell *shell)
{
	t_token	*current;
	t_token	*last;

	current = tokens;
	last = tokens;
	if (!current)
		return (1);
	if (!validate_pipe_start(tokens, data))
		return (0);
	while (current)
	{
		if (!validate_tokens(current, data, shell))
			return (0);
		current = current->next;
	}
	while (last && last->next)
		last = last->next;
	if (last->type == TOKEN_PIPE || last->type == TOKEN_REDIR_IN
		|| last->type == TOKEN_REDIR_OUT || last->type == TOKEN_REDIR_APPEND)
	{
		syntax_error("newline", data);
		return (0);
	}
	return (1);
}
