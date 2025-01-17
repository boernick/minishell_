/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 14:23:55 by prichugh          #+#    #+#             */
/*   Updated: 2024/12/03 14:23:55 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t g_interrupted = false;

// static void	switch_signal_handler(int signal, __sighandler_t handler)
// {
// 	struct sigaction	sa;


// 	sa.sa_handler = handler;
// 	sigemptyset(&sa.sa_mask);
// 	sigaddset(&sa.sa_mask, signal);
// 	sa.sa_flags = 0;
// 	sigaction(signal, &sa, NULL);
// 	if (signal == SIGINT)
// 		printf("[DEBUG] Signal handler for SIGINT switched\n");
// 	if (signal == SIGQUIT)
// 		printf("[DEBUG] Signal handler for SIGQUIT switched\n");
// }

// Custom SIGINT handler for heredoc
void heredoc_sigint_handler(int signum)
{
    (void)signum;
    g_interrupted = true;
    write(STDOUT_FILENO, "\n", 1);  // Print a newline to maintain shell aesthetics
}

// Set custom signal handlers for heredoc
void setup_heredoc_signals(struct sigaction *prev_sigint, struct sigaction *prev_sigquit) {
    struct sigaction sa;

    // Save current signal handlers
    sigaction(SIGINT, NULL, prev_sigint);
    sigaction(SIGQUIT, NULL, prev_sigquit);

    // Set custom SIGINT handler
    sa.sa_handler = heredoc_sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Ignore SIGQUIT
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

// int	get_tempfile_name(char *tempfile)
// {
// 	int	i;

// 	ft_memcpy(tempfile, ".tmphered000", 13);
// 	i = 1;
// 	while (i < 1000)
// 	{
// 		tempfile[11] = '0' + (i % 10);
// 		tempfile[10] = '0' + ((i / 10) % 10);
// 		tempfile[9] = '0' + (i / 100);
// 		if (access(tempfile, F_OK) != 0)
// 			return (0);
// 		i++;
// 	}
// 	return (-1);
// }

char *generate_temp_filename(void) {
    static int counter = 0; // Static counter to ensure uniqueness
    char *filename = malloc(64); // Allocate space for the filename
	if (!filename)
		return NULL;
	sprintf(filename, "/tmp/heredoc_temp_%d.txt", counter++);
	return filename;
}

// char *generate_temp_filename(void) {
//     static int counter = 0; // Static counter for uniqueness
//     char *filename = malloc(64); // Allocate space for filename
//     if (!filename)
//         return NULL;
//     sprintf(filename, "/tmp/heredoc_temp_%d_%d.txt", getpid(), counter++); // Include PID
//     return filename;
// }

// void cleanup_heredoc(t_cmd *cmd)
// {
//     t_redirect *rdir = cmd->redir;

//     while (rdir)
//     {
//         if (rdir->type == TOKEN_HEREDOC && rdir->file)
//         {
//             unlink(rdir->file); // Delete the temporary heredoc file
//         }
//         rdir = rdir->next;
//     }
// }

		// // // Handles heredoc input, writes to a temporary file, and returns the file path
		// char *create_heredoc(char *delimiter)
		// {
		// 	struct sigaction	prev_sigint;
		// 	struct sigaction	prev_sigquit;
		// 	printf("[DEBUG] Starting heredoc with delimiter: %s\n", delimiter);

		//     char *temp_file = generate_temp_filename();
		//     if (!temp_file)
		//     {
		//         perror("Failed to allocate memory for temp file name");
		//         return NULL;
		//     }
		// 	setup_heredoc_signals(&prev_sigint, &prev_sigquit);
		// 	// switch_signal_handler(SIGINT, SIG_DFL);
		// 	// switch_signal_handler(SIGQUIT, SIG_IGN);
		//     int fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		//     if (fd == -1)
		//     {
		//         perror("Failed to create heredoc file");
		//         free(temp_file);
		//         return NULL;
		//     }

		//     //printf("Debug: Created heredoc file `%s`\n", temp_file);

		//     char *line;
		//     while (1)
		//     {
		// 			line = readline("heredoc> ");
		//         // write(STDOUT_FILENO, "heredoc> ", 9);
		//         // line = get_next_line(STDIN_FILENO);
		// 	if (!line)
		// 	{
		// 		write(STDOUT_FILENO, "\n", 1);
		// 		fprintf(stderr, "bash: warning: here-document at line %d delimited by end-of-file (wanted '%s')\n", __LINE__, delimiter);
		// 		close(fd);
		// 		unlink(temp_file); // Delete the temporary file
		// 		free(temp_file);
		// 		//switch_signal_handlers(&sa_int, &sa_quit, false);
		// 		return NULL; // Return NULL to signal failure
		// 	}

		//         // Remove trailing newline for comparison
		//         size_t len = ft_strlen(line);
		//         if (len > 0 && line[len - 1] == '\n')
		//             line[len - 1] = '\0';

		//         // Check if line matches the delimiter
		//         if (strcmp(line, delimiter) == 0)
		//         {
		//             free(line);
		//             printf("Debug: Delimiter `%s` found, ending heredoc\n", delimiter);
		//             break;
		//         }

		//         // Write line to file
		//         //printf("Debug: Writing line to `%s`: %s\n", temp_file, line);
		//         write(fd, line, ft_strlen(line));
		//         write(fd, "\n", 1); // Add newline
		//         free(line);
		//     }

		//     close(fd);
		// 	// switch_signal_handler(SIGINT, default_sigint_handler);
		//     // switch_signal_handler(SIGQUIT, SIG_IGN);
		//     printf("Debug: Finished writing to heredoc file `%s`\n", temp_file);

		//     return temp_file;
		// }


char *expand_env_variables_heredoc(const char *input);

// char *create_heredoc(char *delimiter)
// {
//     struct sigaction prev_sigint, prev_sigquit;
//     struct sigaction sa;

//     // Set up custom SIGINT and SIGQUIT handlers for heredoc
//     sigaction(SIGINT, NULL, &prev_sigint);
//     sigaction(SIGQUIT, NULL, &prev_sigquit);

//     sa.sa_handler = heredoc_sigint_handler;  // Custom SIGINT handler
//     sigemptyset(&sa.sa_mask);
//     sa.sa_flags = 0;
//     sigaction(SIGINT, &sa, NULL);

//     sa.sa_handler = SIG_IGN;  // Ignore SIGQUIT
//     sigaction(SIGQUIT, &sa, NULL);

//     // Generate a unique temporary file for heredoc
//     char *temp_file = generate_temp_filename();
//     if (!temp_file)
//     {
//         perror("Failed to allocate memory for temp file name");
//         return NULL;
//     }

//     int fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
//     if (fd == -1)
//     {
//         perror("Failed to create heredoc file");
//         free(temp_file);
//         return NULL;
//     }

//     char *line;
//     g_interrupted = false;  // Reset interruption flag
//     while (!g_interrupted) // Stop loop if interrupted
//     {
//         //write(STDOUT_FILENO, "heredoc> ", 9);
//         // line = get_next_line(STDIN_FILENO)
// 		line = readline("heredoc> ");
//         if (!line)
//         {
// 				write(STDOUT_FILENO, "\n", 1);
// 				fprintf(stderr, "bash: warning: here-document at line %d delimited by end-of-file (wanted '%s')\n", __LINE__, delimiter);
// 				close(fd);
// 				unlink(temp_file); // Delete the temporary file
// 				free(temp_file);
// 				//switch_signal_handlers(&sa_int, &sa_quit, false);
// 				break; // Return NULL to signal failure
//         }
// 		//printf("g_interrupted: %d\n", g_interrupted);
//         size_t len = ft_strlen(line);
//         if (len > 0 && line[len - 1] == '\n')
//             line[len - 1] = '\0';

//         if (strcmp(line, delimiter) == 0)
//         {
//             free(line);
//             break;
//         }
// 		printf("line before: %s\n", line);
// 		expand_env_variables_heredoc(line);
// 		printf("line after: %s\n", line);
//         write(fd, line, ft_strlen(line));
//         write(fd, "\n", 1);
//         free(line);
//     }

//     close(fd);

//     if (g_interrupted)  // Cleanup if interrupted
//     {
//         unlink(temp_file);  // Delete temporary file
//         free(temp_file);
//         temp_file = NULL;
// 		//printf("Debug: Heredoc interrupted by SIGINT\n");
// 		return NULL;
//     }

//     return temp_file;
// }

// char *create_heredoc(char *delimiter)
// {
// 	(void)delimiter;

//     char *temp_file = generate_temp_filename();
//     if (!temp_file) {
//         perror("Failed to allocate memory for temp file name");
//         return NULL;
//     }

//     int fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
//     if (fd == -1) {
//         perror("Failed to create heredoc file");
//         free(temp_file);
//         return NULL;
//     }

//     // Signal handling and reading input will be done by `run_heredoc` now.
//     close(fd);
//     return temp_file;
// }


//for testing
void print_temp_files(t_redirect *redir)
{
            printf("Checking contents of file: %s\n", redir->file);


            // Open the file for reading
            int fd = open(redir->file, O_RDONLY);
            if (fd == -1)
            {
                perror("Failed to open heredoc file");
				return;
            }

            // Read and print the file contents
            char buffer[1024];
            ssize_t bytes_read;
            while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
            {
                buffer[bytes_read] = '\0'; // Null-terminate the buffer
                printf("%s", buffer);
            }
            if (bytes_read == -1)
                perror("Error reading heredoc file");

            close(fd);
}
//for testing
void check_temp_files(t_cmd *cmd)
{
    t_redirect *redir = cmd->redir;
    while (redir)
	{

		if (redir->type == TOKEN_HEREDOC && redir->file){
            printf("Checking contents of file: %s\n", cmd->redir->file);

            // Open the file for reading
            int fd = open(cmd->redir->file, O_RDONLY);
            if (fd == -1)
            {
                perror("Failed to open heredoc file");
                cmd = cmd->next;
                continue;
            }

            // Read and print the file contents
            char buffer[1024];
            ssize_t bytes_read;
            while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
            {
                buffer[bytes_read] = '\0'; // Null-terminate the buffer
                printf("%s", buffer);
            }
            if (bytes_read == -1)
                perror("Error reading heredoc file");

            close(fd);
        }
        redir = redir->next;
    }
}

char *create_heredoc(void) {

	char *temp_file = generate_temp_filename();
	if (!temp_file) {
		ft_putstr_fd("Failed to allocate memory for temp file name", STDERR_FILENO);
		return NULL;
	}
	int fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("Failed to create heredoc file", STDERR_FILENO);
		free(temp_file);
		return NULL;
	}
	// Store the file path and delimiter in t_redirect for later use.
	return temp_file;
}

// char *get_env_variable_heredoc(char *var_name, t_parse *data, t_shell *shell)
// {
// 	char	*env_value;
// 	char	*exit_status_str;
// 	t_env	*env_node;
// 	char	*equals_sign;

//     if (ft_strncmp(var_name, "?", 1) == 0)
//     {
//         exit_status_str = ft_itoa(data->exit);

//         // Concatenate the exit status with the rest of `var_name`, if any
//         char *remaining = var_name + 1; // Skip `?`
//         char *result = ft_strjoin(exit_status_str, remaining);

//         free(exit_status_str); // Free the temporary exit status string
//         return result;
//     }
// 	env_node = get_env_lst(shell, var_name);
// 	if (env_node)
// 	{
// 		equals_sign = ft_strchr(env_node->content, '=');
// 		if (equals_sign)
// 			return ft_strdup(equals_sign + 1); // Return the value part only
// 	}
// 	env_value = getenv(var_name);
// 	if (env_value)
// 		return ft_strdup(env_value); // Return an empty string if variable doesn't exist
// 	return ft_strdup(""); // Duplicate the value for safe usage
// }


char *replace_variables_in_heredoc(char *input, t_parse *data, t_shell *shell)
{
    char result[1024] = {0};  // Buffer to store the final result
    int res_index = 0;        // Index for writing to result buffer
    int i = 0;                // Index for reading input
    while (input[i] != '\0')
    {
        if (input[i] == '$') // Expand only outside single quotes
        {
			i++; // Skip the `$`
			if (input[i] == ' ' || !input[i]) // Handle single `$` or `$ $`
			{
				result[res_index++] = '$';
				continue;
			}
			if (input[i] == '$') // Handle `$$` (expand to PID)
			{
				// Expand `$$` into PID
				char *pid_str = get_pid_as_string();
				if (pid_str)
				{
					int j = 0;
					// Copy the PID into the result buffer
					while (pid_str[j] != '\0')
					{
						result[res_index] = pid_str[j];
						res_index++;
						j++;
					}
					free(pid_str);
				}
				i++; // Skip the second `$` after handling `$$`
			}
            char var_name[256] = {0};
            int var_index = 0;

            while (ft_isalnum(input[i]) || input[i] == '_' || (var_index == 0 && input[i] == '?'))
				var_name[var_index++] = input[i++];
            var_name[var_index] = '\0';

            char *var_value = get_env_variable(var_name, data, shell);
            if (!var_value)
                var_value = "";

            strcpy(result + res_index, var_value); // Use custom strcpy if required
            res_index += ft_strlen(var_value);
            free(var_value);
        }
        else
        {
            result[res_index++] = input[i++];
        }
    }
    result[res_index] = '\0';
    return ft_strdup(result);
}
