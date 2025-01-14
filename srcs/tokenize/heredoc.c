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


static void	switch_signal_handler(int signal, __sighandler_t handler)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, signal);
	sa.sa_flags = 0;
	sigaction(signal, &sa, NULL);
	if (signal == SIGINT)
		printf("[DEBUG] Signal handler for SIGINT switched\n");
	if (signal == SIGQUIT)
		printf("[DEBUG] Signal handler for SIGQUIT switched\n");
}

char *generate_temp_filename(void) {
    static int counter = 0; // Static counter to ensure uniqueness
    char *filename = malloc(64); // Allocate space for the filename
    if (!filename)
        return NULL;

    sprintf(filename, "/tmp/heredoc_temp_%d.txt", counter++);
    return filename;
}

// Handles heredoc input, writes to a temporary file, and returns the file path
char *create_heredoc(char *delimiter)
{
	printf("[DEBUG] Starting heredoc with delimiter: %s\n", delimiter);

    char *temp_file = generate_temp_filename();
    if (!temp_file)
    {
        perror("Failed to allocate memory for temp file name");
        return NULL;
    }
	switch_signal_handler(SIGINT, SIG_DFL);
	switch_signal_handler(SIGQUIT, SIG_IGN);
    int fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("Failed to create heredoc file");
        free(temp_file);
        return NULL;
    }

    //printf("Debug: Created heredoc file `%s`\n", temp_file);

    char *line;
    while (1)
    {
        write(STDOUT_FILENO, "heredoc> ", 9);
        line = get_next_line(STDIN_FILENO);
	if (!line)
	{
		write(STDOUT_FILENO, "\n", 1);
		fprintf(stderr, "bash: warning: here-document at line %d delimited by end-of-file (wanted '%s')\n", __LINE__, delimiter);
		close(fd);
		unlink(temp_file); // Delete the temporary file
		free(temp_file);
		//switch_signal_handlers(&sa_int, &sa_quit, false);
		return NULL; // Return NULL to signal failure
	}

        // Remove trailing newline for comparison
        size_t len = ft_strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        // Check if line matches the delimiter
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            printf("Debug: Delimiter `%s` found, ending heredoc\n", delimiter);
            break;
        }

        // Write line to file
        //printf("Debug: Writing line to `%s`: %s\n", temp_file, line);
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1); // Add newline
        free(line);
    }

    close(fd);
	// switch_signal_handler(SIGINT, outside_process_signals);
    // switch_signal_handler(SIGQUIT, SIG_IGN);
    printf("Debug: Finished writing to heredoc file `%s`\n", temp_file);

    return temp_file;
}


