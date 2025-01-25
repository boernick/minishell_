/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:32:30 by nboer             #+#    #+#             */
/*   Updated: 2025/01/25 18:33:02 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_start_fds(t_execution *pipex)
{
	if (pipex->start_in != -2)
	{
		close(pipex->start_in);
		pipex->start_in = -2;	
	}
	if (pipex->start_out != -2)
	{
		close(pipex->start_out);
		pipex->start_out = -2;
	}
}

void	cleanup(t_parse *parse, t_shell *shell, t_execution *pipex)
{
	close_start_fds(pipex);
	free_tokens(parse->head);
	free_command_stack(parse->cmd);
	clear_history();
	free_envlst(shell->env_lst);
}
