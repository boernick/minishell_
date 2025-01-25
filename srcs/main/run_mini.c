/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_mini.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:51:09 by nboer             #+#    #+#             */
/*   Updated: 2025/01/25 18:31:23 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	run_mini(t_parse *parse, t_shell *shell, t_execution *pipex)
{
	parse_tokens(parse, shell);
	if (parse->valid_input && parse->cmd)
	{
		pipex->cmd = parse->cmd;
		exec_mini(shell, pipex);
		close_start_fds(pipex);
	}
}
