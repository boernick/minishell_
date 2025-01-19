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
char *get_token_type_name(e_token_type type);

void	classify_empty_tokens(t_parse *data)
{
	t_token	*current_token;

	current_token = data->head;
	while (current_token)
	{
		if (!current_token->value || current_token->value[0] == '\0')
		{
			current_token->type = TOKEN_SKIP;
			if (!current_token->next)
				break;
		}
		// printf("current_token->value: %s\n", current_token->value);
		// printf("current_token->type: %s\n", get_token_type_name(current_token->type));
		current_token = current_token->next;
	}
}

void process_tokens(char *input, t_parse *data, t_shell *shell)
{
	if (input && *input)
		add_history(input);  // add_history causes mem leaks
	reset_parse(data);
	split_tokens(input, data);
	data->exit = shell->last_exit;
	//write(1, "testing1\n", 9);
	if (validate_input(data->head, data) && data->valid_input)
	{
		//write(2, "testing2\n", 9);
		data->valid_input = 1;
		classify_token_types(data, shell);
		replace_env_variables_in_tokens(data->head, data, shell);
		//write(3, "testing3\n", 9);
		//print_tokens(data->head);
		//classify_empty_tokens(data);
		//write(4, "testing4\n", 9);
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
void	tokenize(t_parse *data, t_shell *shell, t_sigaction *sa_int, t_sigaction *sa_quit)
{
	char	*input;

	//setup_signal_handlers();
	if (data->cmd)
	{
		free_command_stack(data->cmd);
		data->cmd = NULL;
	}
	input = readline("MINISHELL>>> "); //readline caues mem leaks
	if (!input) // Ctrl+D sends EOF, readline returns NULL.
		handle_eof(shell, data);
	if (ft_strlen(input) == 0) // Empty input handling
	{
		free(input);
		data->valid_input = 0;
		return; // Simply redisplay prompt
	}
	inside_process_signals(sa_int, sa_quit);
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
