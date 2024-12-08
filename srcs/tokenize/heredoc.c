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
    char *temp_file = generate_temp_filename();
    if (!temp_file)
    {
        perror("Failed to allocate memory for temp file name");
        return NULL;
    }

    int fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("Failed to create heredoc file");
        free(temp_file);
        return NULL;
    }

    printf("Debug: Created heredoc file `%s`\n", temp_file);

    char *line;
    while (1)
    {
        printf("heredoc> ");
        line = get_next_line(STDIN_FILENO);
        if (!line)
        {
            printf("Debug: Unexpected EOF while looking for `%s`\n", delimiter);
            break;
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
        printf("Debug: Writing line to `%s`: %s\n", temp_file, line);
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1); // Add newline
        free(line);
    }

    close(fd);
    printf("Debug: Finished writing to heredoc file `%s`\n", temp_file);

    return temp_file;
}
