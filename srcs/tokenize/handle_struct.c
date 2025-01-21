/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_struct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:06:42 by nboer             #+#    #+#             */
/*   Updated: 2025/01/21 16:06:42 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Initializes the fields of the t_data struct for starting new shell instance.
void	struct_init(t_parse *data, t_shell *shell)
{
	shell->exit = 0;
	shell->last_exit = 0;
	data->cmd = NULL;
	data->exit = 0;
	data->head = NULL;
	data->tail = NULL;
	data->buf_index = 0;
	data->in_double_quote = 0;
	data->in_single_quote = 0;
}

// Resets specific fields in t_data struct to prepare processing a new command.
void	reset_parse(t_parse *data)
{
	data->head = NULL;
	data->tail = NULL;
	data->in_double_quote = 0;
	data->in_single_quote = 0;
}
