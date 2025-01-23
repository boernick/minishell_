/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_mini.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:51:09 by nboer             #+#    #+#             */
/*   Updated: 2025/01/23 14:08:01 by prichugh         ###   ########.fr       */
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
	}
}
