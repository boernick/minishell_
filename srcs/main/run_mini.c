/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_mini.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:51:09 by nboer             #+#    #+#             */
/*   Updated: 2025/01/21 16:05:54 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	run_mini(t_parse *parse, t_shell *shell, t_execution *pipex)
{
	parse_tokens(parse, shell);
	//print_command_stack(pipex.cmd); // REMOVE
	//check_temp_files(parse.cmd); // REMOVE
	if (parse->valid_input)
	{
		pipex->cmd = parse->cmd;
		exec_mini(shell, pipex);
	}
}
