#include "../../includes/minishell.h"


// Repeatedly prompts for user input, tokenizes, validates, and processes commands.
// Exits the loop if "exit" or EOF is detected, handling cleanup and history appropriately.
void	start_program(t_parse *data)
{
	char			*input;
	// t_token			*tokens;

	// tokens = data->head;
	signal(SIGINT, handle_sigint); // Set the signal handler for ctrl+c, ctrl+d, and ctr+\"
	while (1)
	{
		input = readline(">>> "); //readline caues mem leaks
		if (input == NULL) //ADJUST TO SUBJECT
		{
			printf("EOF detected, exiting...\n");
			break ;
		}
		if (strcmp(input, "exit") == 0) // add my own ft_strcmp
		{
			free(input);
			break ;
		}
		if (input && *input)
			add_history(input);  //add_history causes mem leaks
		reset_parse(data);
		tokenize(input, data);
		if (validate_input(data->head))
		{
			classify_token_types(data); //NEXT STEPS
			replace_env_variables_in_tokens(data->head, data);
			print_tokens(data->head);
			free_tokens(data->head);
		}
		free(input);
	}
	clear_history();//make sure toe use the better one (this vs next line)
	rl_clear_history();//make sure to use the better one
}

//void	start_program()
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
