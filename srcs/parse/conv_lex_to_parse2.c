/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_lex_to_parse2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:39:26 by prichugh          #+#    #+#             */
/*   Updated: 2025/01/23 12:39:26 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

int	init_cmd_redir(t_parse *data, t_cmd **current_cmd)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		malloc_error(sizeof(t_cmd));
	new_cmd->cmd = NULL;
	new_cmd->argv = NULL;
	new_cmd->index = data->n_cmds;
	new_cmd->is_builtin = false;
	new_cmd->redir = NULL;
	new_cmd->next = NULL;
	add_cmd_to_list(data, new_cmd);
	*current_cmd = new_cmd;
	data->n_cmds++;
	return (1);
}

void	init_cmd_cmd(t_cmd **current_cmd, t_parse *data, char *cmd_value)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		malloc_error(sizeof(t_cmd));
	new_cmd->cmd = ft_strdup(cmd_value);
	new_cmd->argv = (char **)malloc(sizeof(char *) * 2);
	if (!new_cmd->argv)
		malloc_error(sizeof(char *) * 2);
	new_cmd->argv[0] = ft_strdup(cmd_value);
	new_cmd->argv[1] = NULL;
	new_cmd->index = data->n_cmds;
	new_cmd->is_builtin = is_builtin_(new_cmd->cmd);
	new_cmd->redir = NULL;
	new_cmd->next = NULL;
	add_cmd_to_list(data, new_cmd);
	*current_cmd = new_cmd;
	data->n_cmds++;
}

bool	is_builtin_(char *cmd)
{
	if (!cmd)
		return (false);
	return (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "cd", 3) == 0
		|| ft_strncmp(cmd, "pwd", 4) == 0
		|| ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0
		|| ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0);
}

void	free_redir_stack(t_redirect *redir)
{
	t_redirect	*temp_redir;

	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC && redir->file != NULL)
			unlink(redir->file);
		if (redir->file)
			free(redir->file);
		temp_redir = redir;
		redir = redir->next;
		free(temp_redir);
	}
}

void	free_command_stack(t_cmd *cmd_stack)
{
	t_cmd	*temp;
	int		i;

	while (cmd_stack)
	{
		temp = cmd_stack;
		if (cmd_stack->cmd)
			free(cmd_stack->cmd);
		if (cmd_stack->argv)
		{
			i = 0;
			while (cmd_stack->argv[i])
			{
				free(cmd_stack->argv[i]);
				i++;
			}
			free(cmd_stack->argv);
		}
		if (cmd_stack->redir)
			free_redir_stack(cmd_stack->redir);
		cmd_stack = cmd_stack->next;
		free(temp);
	}
}
