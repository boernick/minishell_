/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:30:01 by prichugh          #+#    #+#             */
/*   Updated: 2024/11/19 16:20:11 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	process_token_skip(t_token **cur_tkn, t_parse *data)
{
	if ((*cur_tkn)->next)
		*cur_tkn = (*cur_tkn)->next;
	else
	{
		if (data->n_cmds > 0)
			return (1);
		data->valid_input = 0;
		return (0);
	}
	return (1);
}

void	process_token_redirection(t_token *cur_tkn, t_parse *data,
		t_cmd **current_cmd)
{
	t_redirect	*redir;

	if (!*current_cmd)
		init_cmd_redir(data, current_cmd);
	redir = (t_redirect *)malloc(sizeof(t_redirect));
	if (!redir)
		malloc_error(sizeof(t_redirect));
	redir->file = ft_strdup(cur_tkn->next->value);
	redir->type = cur_tkn->type;
	redir->next = NULL;
	add_redirection_to_cmd(*current_cmd, redir);
}

int	process_token_heredoc(t_token *cur_tkn, t_parse *data,
		t_cmd **current_cmd, t_shell *shell)
{
	t_redirect	*redir;
	char		*delimeter;

	if (!*current_cmd)
	{
		if (!init_cmd_redir(data, current_cmd))
			return (0);
	}
	redir = (t_redirect *)malloc(sizeof(t_redirect));
	if (!redir)
		malloc_error(sizeof(t_cmd));
	redir->file = create_heredoc();
	redir->type = TOKEN_HEREDOC;
	redir->next = NULL;
	add_redirection_to_cmd(*current_cmd, redir);
	delimeter = ft_strdup(cur_tkn->next->value);
	cur_tkn = cur_tkn->next;
	data->exit = run_heredoc(data, redir, delimeter, shell);
	free(delimeter);
	return (heredoc_exit((*current_cmd)->redir, data, data->exit, shell));
}

void	process_token_pipe(t_parse *data, t_cmd **current_cmd)
{
	data->n_pipes++;
	*current_cmd = NULL;
}

void	parse_tokens(t_parse *data, t_shell *shell)
{
	t_token		*cur_t;
	t_cmd		*current_cmd;

	init_parse_data(data, &cur_t, &current_cmd);
	while (cur_t)
	{
		if (cur_t->type == TOKEN_SKIP && !process_token_skip(&cur_t, data))
			break ;
		if (cur_t->type == TOKEN_CMD)
			add_cmd_to_token(cur_t, data, &current_cmd);
		else if ((cur_t->type == TOKEN_ARG || cur_t->type == TOKEN_FLAG_ARG)
			&& current_cmd)
			add_argument_to_cmd(current_cmd, cur_t->value);
		else if (cur_t->type == TOKEN_REDIR_IN || cur_t->type == TOKEN_REDIR_OUT
			|| cur_t->type == TOKEN_REDIR_APPEND)
			process_token_redirection(cur_t, data, &current_cmd);
		else if (cur_t->type == TOKEN_HEREDOC
			&& (!process_token_heredoc(cur_t, data, &current_cmd, shell)))
			break ;
		else if (cur_t->type == TOKEN_PIPE)
			process_token_pipe(data, &current_cmd);
		cur_t = cur_t->next;
	}
}
