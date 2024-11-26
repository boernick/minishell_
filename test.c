// test file to fill in structs for execution.

#include "includes/minishell.h"

void calibrate_exec(t_execution *pipex)
{
    t_cmd *cmd1 = (t_cmd *)malloc(sizeof(t_cmd));
    if (!cmd1)
        str_error("Malloc failed for cmd1");

    // First command in the pipeline
    cmd1->cmd = "cat";
    cmd1->argv = (char **)malloc(3 * sizeof(char *));
    if (!cmd1->argv)
        str_error("Malloc failed for cmd1->argv");
    cmd1->argv[0] = "cat";
    cmd1->argv[1] = "README.txt";
    cmd1->argv[2] = NULL;
    cmd1->index = 0;
    cmd1->is_builtin = false;
    cmd1->redir = NULL;
    cmd1->next = NULL;

    // Second command in the pipeline
    t_cmd *cmd2 = (t_cmd *)malloc(sizeof(t_cmd));
    if (!cmd2)
        str_error("Malloc failed for cmd2");
    cmd2->cmd = "wc";
    cmd2->argv = (char **)malloc(3 * sizeof(char *));
    if (!cmd2->argv)
        str_error("Malloc failed for cmd2->argv");
    cmd2->argv[0] = "wc";
    cmd2->argv[1] = "-l";
    cmd2->argv[2] = NULL;
    cmd2->index = 1;
    cmd2->is_builtin = false;

    // Adding multiple redirections to cmd2
    t_redirect *redir_out = (t_redirect *)malloc(sizeof(t_redirect));
    if (!redir_out)
        str_error("Malloc failed for redir_out");
    redir_out->type = TOKEN_REDIR_OUT;
    redir_out->file = strdup("output.txt");
    if (!redir_out->file)
        str_error("Malloc failed for redir_out->file");

    t_redirect *redir_in = (t_redirect *)malloc(sizeof(t_redirect));
    if (!redir_in)
        str_error("Malloc failed for redir_in");
    redir_in->type = TOKEN_REDIR_IN;
    redir_in->file = strdup("input_override.txt");
    if (!redir_in->file)
        str_error("Malloc failed for redir_in->file");

    // Link redirections
    redir_in->next = redir_out;
    redir_out->next = NULL;
    cmd2->redir = redir_in;

    cmd2->next = NULL;
    cmd1->next = cmd2;

    // Attach command list to execution struct
    pipex->cmd = cmd1;
}


    /****OPTION 3: TESTED AND WORKING - PIPE BUT NO REDIRECTIONS */
    // // Allocate and initialize the first command
    // t_cmd *cmd1 = (t_cmd *)malloc(sizeof(t_cmd));
    // if (!cmd1)
    //     str_error("Malloc failed for cmd1");

    // cmd1->cmd = "cat";
    // cmd1->argv = (char **)malloc(3 * sizeof(char *));
    // if (!cmd1->argv)
    //     str_error("Malloc failed for cmd1->argv");

    // cmd1->argv[0] = "cat";
    // cmd1->argv[1] = "file.txt";
    // cmd1->argv[2] = NULL;
    // cmd1->index = 0;
    // cmd1->is_builtin = false;
    // cmd1->redir = NULL;
    // cmd1->next = NULL;

    // // Allocate and initialize the second command
    // t_cmd *cmd2 = (t_cmd *)malloc(sizeof(t_cmd));
    // if (!cmd2)
    //     str_error("Malloc failed for cmd2");

    // cmd2->cmd = "wc";
    // cmd2->argv = (char **)malloc(3 * sizeof(char *));
    // if (!cmd2->argv)
    //     str_error("Malloc failed for cmd2->argv");

    // cmd2->argv[0] = "wc";
    // cmd2->argv[1] = "-l";
    // cmd2->argv[2] = NULL;
    // cmd2->index = 1;
    // cmd2->is_builtin = false;
    // cmd2->redir = NULL;
    // cmd2->next = NULL;

    // // Link the commands
    // cmd1->next = cmd2;

    // // Assign the command list to the execution structure
    // pipex->cmd = cmd1;


// /****OPTION 1 - TESTED AND WORKING*****/
//     cmd_struct->cmd = "cat";
//     // Allocating argv as a placeholder with two arguments
//     cmd_struct->argv = (char **)malloc(3 * sizeof(char *));
//     cmd_struct->argv[0] = "cat";
//     cmd_struct->argv[1] = "file.txt";
//     cmd_struct->argv[2] = NULL;

//     cmd_struct->index = 0;  // Placeholder index
//     cmd_struct->is_builtin = false;  // Not a built-in command
//     cmd_struct->next = NULL;  // End of linked list
    
//     // Placeholder redirection setup
//     cmd_struct->redir = (t_redirect *)malloc(sizeof(t_redirect));
//     cmd_struct->redir->type = TOKEN_REDIR_OUT;
//     cmd_struct->redir->file = "out.txt";
// }
