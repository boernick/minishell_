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


void	process_tokens(char *input, t_parse *data, t_shell *shell)
{
	if (input && *input)
		add_history(input);
	reset_parse(data);
	split_tokens(input, data);
	data->exit = shell->last_exit;
	//print_tokens(data->head); //REMOVE
	if (validate_input(data->head, data, shell) && data->valid_input)
	{
		data->valid_input = 1;
		classify_token_types(data, shell);
		//print_tokens(data->head); //REMOVE
		replace_env_variables_in_tokens(data->head, data, shell);
		//print_tokens(data->head); //REMOVE
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
