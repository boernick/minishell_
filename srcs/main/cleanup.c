/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:32:30 by nboer             #+#    #+#             */
/*   Updated: 2025/01/23 00:13:07 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup(t_parse *parse, t_shell *shell)
{
	free_tokens(parse->head);
	free_command_stack(parse->cmd);
	clear_history();
	free_envlst(shell->env_lst);
}
