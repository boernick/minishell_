// test file to fill in structs for execution.

#include "includes/minishell.h"

void calibrate_exec(t_cmd *cmd_struct) {
    cmd_struct->cmd = "cat";
    // Allocating argv as a placeholder with two arguments
    cmd_struct->argv = (char **)malloc(3 * sizeof(char *));
    cmd_struct->argv[0] = "cat";
    cmd_struct->argv[1] = "file.txt";
    cmd_struct->argv[2] = NULL;

    cmd_struct->index = 0;  // Placeholder index
    cmd_struct->is_builtin = false;  // Not a built-in command
    cmd_struct->next = NULL;  // End of linked list
    
    // Placeholder redirection setup
    cmd_struct->redir = (t_redirect *)malloc(sizeof(t_redirect));
    cmd_struct->redir->type = TOKEN_REDIR_OUT;
    cmd_struct->redir->file = "output.txt";
}
