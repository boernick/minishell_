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

//WORKING
//Go trough the list of tokens and correcltly split up WORD_TOKENS
//to be arguments or commands
void classify_token_types(t_parse *data, t_shell *shell)
{
	t_token	*current_token;
	int		i;

	i = 0;
	current_token = data->head;
	while (current_token)
	{
		if (current_token->value[0] == '$')
		{
			char *expanded_var = replace_variables_in_heredoc(current_token->value, data, shell);
			if (!expanded_var || expanded_var[0] == '\0')
			{
				current_token->type = TOKEN_SKIP;
				current_token = current_token->next;
				continue;
			}
			free(expanded_var);
		}
		// Reset i to 0 if we're starting a new command sequence after a pipe
		if (current_token->type == TOKEN_PIPE)
			i = 0;
		// Identify command at the start of each sequence or after a pipe
		// else if (current_token->value[0] == '$')
		// 	current_token->type = TOKEN_ARG;
		else if (current_token->type == TOKEN_WORD && i == 0)
		{
			current_token->type = TOKEN_CMD;
			i++;
		}
		// Mark the following token after any redirection as a file argument
		else if (current_token->type == TOKEN_REDIR_IN ||
				current_token->type == TOKEN_REDIR_OUT ||
				current_token->type == TOKEN_REDIR_APPEND ||
				current_token->type == TOKEN_HEREDOC)
		{
			// Ensure the next token is marked as a file argument if it's a word
			if (current_token->next && current_token->next->type == TOKEN_WORD)
			{
				current_token->next->type = TOKEN_FILE_ARG;
			current_token = current_token->next; // Skip to the file argument
		}
		}
		// Handle other arguments and flags
		else if (current_token->type == TOKEN_WORD)
		{
			if (current_token->value[0] == '-' && current_token->value[1] == 'n')
				current_token->type = TOKEN_FLAG_ARG;
			else
				current_token->type = TOKEN_ARG;
		}
		current_token = current_token->next;
	}
}
