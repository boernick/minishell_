/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 19:32:55 by nick              #+#    #+#             */
/*   Updated: 2024/12/26 20:33:31 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	run_heredoc(t_execution *pipex)
{
	t_cmd	*cmd;

	cmd = pipex->cmd;
	while (cmd)
	{
		if (cmd->redir->type == TOKEN_HEREDOC)
			handle_heredoc(cmd);
		cmd = cmd->next;
	}
}

void	handle_heredoc(t_cmd *cmd)
{
	//handle the heredoc
	(void) cmd;
}