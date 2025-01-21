/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_and_parse.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:30:11 by prichugh          #+#    #+#             */
/*   Updated: 2024/11/13 18:30:11 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


//for testing
char	*get_token_type_name(t_token_type type);

// void	classify_empty_tokens(t_parse *data)
// {
// 	t_token	*current_token;

// 	current_token = data->head;
// 	while (current_token)
// 	{
// 		if (!current_token->value || current_token->value[0] == '\0')
// 		{
// 			current_token->type = TOKEN_SKIP;
// 			if (!current_token->next)
// 				break;
// 		}
// 		// printf("current_token->value: %s\n", current_token->value);
// 		// printf("current_token->type: %s\n", get_token_type_name(current_token->type));
// 		current_token = current_token->next;
// 	}
// }

void	process_tokens(char *input, t_parse *data, t_shell *shell)
{
	if (input && *input)
		add_history(input);
	reset_parse(data);
	split_tokens(input, data);
	data->exit = shell->last_exit;
	if (validate_input(data->head, data, shell) && data->valid_input)
	{
		data->valid_input = 1;
		classify_token_types(data, shell);
		replace_env_variables_in_tokens(data->head, data, shell);
	}
	else
	{
		shell->last_exit = data->exit;
		data->valid_input = 0;
		reset_parse(data);
	}
	free(input);
}

// Repeatedly prompts for user input, tokenizes, validates, and processes commands.
// Exits the loop if "exit" or EOF is detected, handling cleanup and history appropriately.
void	tokenize(t_parse *data, t_shell *shell,
		t_sigaction *sa_int, t_sigaction *sa_quit)
{
	char	*input;

	if (data->cmd)
	{
		free_command_stack(data->cmd);
		data->cmd = NULL;
	}
	input = readline("MINISHELL>>> ");
	if (!input)
		handle_eof(shell, data);
	if (ft_strlen(input) == 0)
	{
		free(input);
		data->valid_input = 0;
		return ;
	}
	inside_process_signals(sa_int, sa_quit);
	process_tokens(input, data, shell);
}
