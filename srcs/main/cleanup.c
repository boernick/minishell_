/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:32:30 by nboer             #+#    #+#             */
/*   Updated: 2025/01/21 15:36:35 by nboer            ###   ########.fr       */
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
