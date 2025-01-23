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

void	eset_expand_var(t_expand_var *expand)
{
	expand->var_index = 0;
	ft_memset(expand->var_name, 0, sizeof(expand->var_name));
}

void	init_parse_data(t_parse *data, t_token **cur_tkn, t_cmd **current_cmd)
{
	*cur_tkn = data->head;
	*current_cmd = NULL;
	data->n_cmds = 0;
	data->n_pipes = 0;
}

void	init_expand_var(t_expand_var *expand, char *input)
{
	if (!expand)
		malloc_error(sizeof(t_expand_var));
	expand->i = 0;
	expand->res_index = 0;
	expand->var_index = 0;
	ft_memset(expand->result, 0, sizeof(expand->result));
	ft_memset(expand->var_name, 0, sizeof(expand->var_name));
	expand->input = input;
}
