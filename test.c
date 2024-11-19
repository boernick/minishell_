// test file to fill in structs for execution.

#include "includes/minishell.h"

void calibrate_exec(t_cmd *cmd_struct)
{
	// First command in the pipeline
	cmd_struct->cmd = "cat";
	cmd_struct->argv = (char **)malloc(3 * sizeof(char *));
	cmd_struct->argv[0] = "cat";
	cmd_struct->argv[1] = "file.txt";
	cmd_struct->argv[2] = NULL;
	cmd_struct->index = 0;
	cmd_struct->is_builtin = false;
	cmd_struct->redir = NULL;

	// Allocate and initialize the second command in the pipeline
	t_cmd *cmd2 = (t_cmd *)malloc(sizeof(t_cmd));
	cmd2->cmd = "wc";
	cmd2->argv = (char **)malloc(3 * sizeof(char *));
	cmd2->argv[0] = "wc";
	cmd2->argv[1] = "-l";
	cmd2->argv[2] = NULL;
	cmd2->index = 1;
	cmd2->is_builtin = false;
	cmd2->redir = (t_redirect *)malloc(sizeof(t_redirect));
	cmd2->redir->type = TOKEN_REDIR_OUT; // Redirect output
	cmd2->redir->file = "output.txt";
	cmd2->next = NULL;

	// Link the first command to the second
	cmd_struct->next = cmd2;
}

/****OPTION 1 - TESTED AND WORKING*****/
    // cmd_struct->cmd = "cat";
    // // Allocating argv as a placeholder with two arguments
    // cmd_struct->argv = (char **)malloc(3 * sizeof(char *));
    // cmd_struct->argv[0] = "cat";
    // cmd_struct->argv[1] = "file.txt";
    // cmd_struct->argv[2] = NULL;

    // cmd_struct->index = 0;  // Placeholder index
    // cmd_struct->is_builtin = false;  // Not a built-in command
    // cmd_struct->next = NULL;  // End of linked list
    
    // // Placeholder redirection setup
    // cmd_struct->redir = (t_redirect *)malloc(sizeof(t_redirect));
    // cmd_struct->redir->type = TOKEN_REDIR_OUT;
    // cmd_struct->redir->file = "output.txt";
