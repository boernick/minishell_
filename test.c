// test file to fill in structs for execution.

#include "includes/minishell.h"

void calibrate_exec(t_cmd *cmd_struct) 
{
	// First command in the pipeline
    cmd1->cmd = "cat";
    cmd1->argv = (char **)malloc(3 * sizeof(char *));
    cmd1->argv[0] = "cat";
    cmd1->argv[1] = "file.txt";
    cmd1->argv[2] = NULL;
    cmd1->index = 0;
    cmd1->is_builtin = false;
    cmd1->redir = NULL;  // No redirection for this command
    cmd1->next = cmd2;   // Link to the next command

    // Second command in the pipeline (e.g., `wc -l`)
    cmd2->cmd = "wc";
    cmd2->argv = (char **)malloc(3 * sizeof(char *));
    cmd2->argv[0] = "wc";
    cmd2->argv[1] = "-l";
    cmd2->argv[2] = NULL;
    cmd2->index = 1;
    cmd2->is_builtin = false;
    cmd2->redir = (t_redirect *)malloc(sizeof(t_redirect));
    cmd2->redir->type = TOKEN_REDIR_OUT;  // Redirect output
    cmd2->redir->file = "output.txt";
    cmd2->next = NULL;  // End of the pipeline
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
