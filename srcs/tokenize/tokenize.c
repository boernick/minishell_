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

void process_tokens(char *input, t_parse *data, t_shell *shell)
{
	if (!input) // Ctrl+D sends EOF, readline returns NULL.
		handle_eof();
	if (input && *input)
		add_history(input);  // add_history causes mem leaks
	reset_parse(data);
	split_tokens(input, data);
	data->exit = shell->last_exit;
	if (validate_input(data->head, data) && data->valid_input)
	{
		data->valid_input = 1;
		classify_token_types(data);
		//print_tokens(data->head);
		replace_env_variables_in_tokens(data->head, data);
		//print_tokens(data->head);
		free(input);
	}
	else
	{
		shell->last_exit = data->exit;
		data->valid_input = 0;
		reset_parse(data);
		free(input);
	}
}

// Repeatedly prompts for user input, tokenizes, validates, and processes commands.
// Exits the loop if "exit" or EOF is detected, handling cleanup and history appropriately.
void	tokenize(t_parse *data, t_shell *shell)
{
	char	*input;

	setup_signal_handlers();
	if (data->cmd)
	{
		free_command_stack(data->cmd);
		data->cmd = NULL;
	}
	input = readline("MINISHELL>>> "); //readline caues mem leaks
	process_tokens(input, data, shell);
}

//void	tokenize_and_parse()
//{
	//initialize main data struct to hold tokens and other info
	//initialize signal handlers for the various signals
	//start of infinite while loop
	//use readline to retrieve user input
	//handle various bad input per subject
	//if input is exit then call exit program function
	//if there is in fact legal input add it to history
	//call tokenize function to tokenize input into a linked list inside main data struct
	//if token have been succesfully created, call syntax check function to verify correct grammar
	//then call parse function which dettermine which execute functions to call
	//free the input and tokens
	//clear history
//}
