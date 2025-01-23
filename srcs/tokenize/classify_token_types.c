/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_ext.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:30:28 by prichugh          #+#    #+#             */
/*   Updated: 2024/11/13 18:30:28 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

void	classify_token_types(t_parse *data, t_shell *shell)
{
	t_token	*curr;
	int		cmd_flag;

	cmd_flag = 0;
	curr = data->head;
	while (curr)
	{
		if (curr->value[0] == '$'
			&& !check_empty_variable(curr, data, shell))
		{
			if (curr->next)
				curr = curr->next;
		}
		if (curr->type == TOKEN_PIPE)
			cmd_flag = 0;
		else if (curr->type == TOKEN_WORD && cmd_flag == 0)
			classify_command_token(curr, &cmd_flag);
		else if (curr->type == TOKEN_REDIR_IN || curr->type == TOKEN_REDIR_OUT
			|| curr->type == TOKEN_REDIR_APPEND || curr->type == TOKEN_HEREDOC)
			classify_file_arg(&curr);
		else if (curr->type == TOKEN_WORD)
			classify_word_arg(curr);
		curr = curr->next;
	}
}

int	check_empty_variable(t_token *current_token, t_parse *data, t_shell *shell)
{
	char	*expnd_var;

	expnd_var = replace_variables_in_heredoc(current_token->value, data, shell);
	if (!expnd_var || expnd_var[0] == '\0')
	{
		current_token->type = TOKEN_SKIP;
		free(expnd_var);
		return (0);
	}
	free(expnd_var);
	return (1);
}

void	classify_command_token(t_token *current_token, int *cmd_flag)
{
	current_token->type = TOKEN_CMD;
	(*cmd_flag)++;
}

void	classify_file_arg(t_token **current_token)
{
	if ((*current_token)->next && (*current_token)->next->type == TOKEN_WORD)
	{
		(*current_token)->next->type = TOKEN_FILE_ARG;
		*current_token = (*current_token)->next;
	}
}

void	classify_word_arg(t_token *current_token)
{
	if (current_token->value[0] == '-' && current_token->value[1] == 'n')
		current_token->type = TOKEN_FLAG_ARG;
	else
		current_token->type = TOKEN_ARG;
}
