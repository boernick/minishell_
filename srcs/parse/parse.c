/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:30:01 by prichugh          #+#    #+#             */
/*   Updated: 2024/11/13 18:55:40 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	add_cmd_to_list(t_parse *data, t_cmd *new_cmd)
// {
// 	if (data->head == NULL)
// 		data->head = new_cmd;
// 	else
// 		data->tail->next = new_cmd;
//     data->tail = new_cmd;
// }

// t_cmd	*new_cmd(e_token_type type, char *value)
// {
// 	t_cmd *cmd = (t_cmd *)malloc(sizeof(t_cmd));
// 	if (!cmd)
// 	{
// 		perror("Failed to allocate token");
// 		exit(EXIT_FAILURE);
// 	}
// 	cmd->type = type;
// 	cmd->value = ft_strdup(value);
// 	cmd->next = NULL;
// 	return (cmd);
// }

void	parse(t_parse *data)
{
	t_cmd	cmds;
	t_token	*current_token;

	current_token = data->head;
	while (current_token->next)
	{
		if (current_token->type = TOKEN_CMD)
		{
			while (current_token->type != TOKEN_PIPE )
		}
	}
}
