typedef struct s_redirect {
    char *type;  // Type of redirection (e.g., ">", "<")
    char *file;  // File associated with the redirection
    struct s_redirect *next;
} t_redirect;

typedef struct s_cmd {
    char *cmd;
    char **argv;
    int index;
    bool is_builtin;
    struct s_cmd *next;
    t_redirect *redirections;
} t_cmd;

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
    cmd_struct->redirections = (t_redirect *)malloc(sizeof(t_redirect));
    cmd_struct->redirections->type = ">";
    cmd_struct->redirections->file = "output.txt";
    cmd_struct->redirections->next = NULL;
}
