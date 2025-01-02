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
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

void handle_sigint(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);           // Move to a new line
    rl_replace_line("", 0);                 // Clear the current input
    rl_on_new_line();                       // Inform readline of a new prompt
    rl_redisplay();                         // Refresh the display
}

void handle_sigquit(int sig)
{
    (void)sig;
    // You can either clear the current line or reset the shell prompt
    rl_replace_line("", 0);         // Clear the current input
   	rl_on_new_line();               // Inform readline of a new prompt
    rl_redisplay();                 // Refresh the display to show the correct prompt
}


void setup_signal_handlers()
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    // SIGINT
    sa_int.sa_handler = handle_sigint;
    sa_int.sa_flags = SA_RESTART;           // Automatically restart interrupted syscalls
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);

    // SIGQUIT
    sa_quit.sa_handler = handle_sigquit;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void handle_eof()
{
	printf("exit\n");
	exit(0);
}

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
		setup_signal_handlers();
		if (data->cmd)
		{
			free_command_stack(data->cmd);
			data->cmd = NULL;
		}
		input = readline("MINISHELL>>> "); //readline caues mem leaks
		if (!input) // Ctrl+D sends EOF, readline returns NULL.
			handle_eof();
		// if (input == NULL) //ADJUST TO SUBJECT
		// {
		// 	printf("EOF detected, exiting...\n");
		// 	//break ;
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
		data->exit = shell->last_exit;
		if (validate_input(data->head))
		{
			classify_token_types(data);
			replace_env_variables_in_tokens(data->head, data);
			//print_tokens(data->head); //Only for testing
		}
		else
			reset_parse(data);
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
