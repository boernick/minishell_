/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_lex_to_parse.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:40:07 by prichugh          #+#    #+#             */
/*   Updated: 2025/01/23 12:40:07 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_cmd_to_list(t_parse *data, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!new_cmd)
		return ;
	if (data->cmd == NULL)
		data->cmd = new_cmd;
	else
	{
		current = data->cmd;
		while (current->next)
			current = current->next;
		current->next = new_cmd;
	}
}

void	add_argument_to_cmd(t_cmd *cmd, char *arg)
{
	int		i;
	int		j;
	char	**new_argv;

	i = 0;
	while (cmd->argv && cmd->argv[i])
		i++;
	new_argv = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_argv)
		malloc_error(sizeof(char *));
	j = 0;
	while (j < i)
	{
		new_argv[j] = cmd->argv[j];
		j++;
	}
	new_argv[i] = ft_strdup(arg);
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

void	add_redirection_to_cmd(t_cmd *cmd, t_redirect *new_redir)
{
	t_redirect	*current;

	if (!cmd->redir)
		cmd->redir = new_redir;
	else
	{
		current = cmd->redir;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
}

void	add_cmd_to_token(t_token *cur_tkn, t_parse *data,
			t_cmd **current_cmd)
{
	if (!*current_cmd)
		init_cmd_cmd(current_cmd, data, cur_tkn->value);
	else if ((*current_cmd)->cmd == NULL)
	{
		(*current_cmd)->cmd = ft_strdup(cur_tkn->value);
		(*current_cmd)->argv = (char **)malloc(sizeof(char *) * 2);
		if (!(*current_cmd)->argv)
			malloc_error(sizeof(char *));
		(*current_cmd)->argv[0] = ft_strdup(cur_tkn->value);
		(*current_cmd)->argv[1] = NULL;
		(*current_cmd)->is_builtin = is_builtin_((*current_cmd)->cmd);
	}
}

int	heredoc_exit(t_redirect *redir, t_parse *data,
		int exit_code, t_shell *shell)
{
	if (exit_code == 130)
	{
		data->valid_input = 0;
		shell->last_exit = 130;
		if (access(redir->file, F_OK) == 0 && exit_code == 130)
			unlink(redir->file);
		return (0);
	}
	return (1);
}
