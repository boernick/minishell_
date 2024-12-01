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


// Repeatedly prompts for user input, tokenizes, validates, and processes commands.
// Exits the loop if "exit" or EOF is detected, handling cleanup and history appropriately.
void	tokenize(t_parse *data, t_shell *shell)
{
	char			*input;
	// t_token			*tokens;

	// tokens = data->head;
	 // Set the signal handler for ctrl+c, ctrl+d, and ctr+\"
	//while (1)
	//{
		if (data->cmd)
		{
			free_command_stack(data->cmd);
			data->cmd = NULL;
		}
		input = readline("MINISHELL>>> "); //readline caues mem leaks
		// if (input == NULL) //ADJUST TO SUBJECT
		// {
		// 	printf("EOF detected, exiting...\n");
		// 	//mbreak ;
		// }
		// if (strcmp(input, "exit") == 0) // add my own ft_strcmp
		// {
		// 	free(input);
		// 	break ;
		// }
		if (input && *input)
			add_history(input);  //add_history causes mem leaks
		reset_parse(data);
		split_tokens(input, data);
		data->exit = shell->exit;
		if (validate_input(data->head))
		{
			classify_token_types(data);
			replace_env_variables_in_tokens(data->head, data);
			//print_tokens(data->head); //Only for testing
		}
		free(input);
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
