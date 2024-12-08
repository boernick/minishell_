/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:30:41 by prichugh          #+#    #+#             */
/*   Updated: 2024/12/04 15:30:41 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *set_line(char *line_buffer)
{
	ssize_t	i;
	char	*left_c;

	i = 0;
	while (line_buffer[i] != '\n' && line_buffer[i] != '\0')
		i++;
	if (line_buffer[i] == 0 || line_buffer[i] == 0)
		return (NULL);
	left_c = ft_substr(line_buffer, i + 1, ft_strlen(line_buffer) - i);
	if (left_c == 0)
	{
		free(left_c);
		left_c = NULL;
	}
	line_buffer[i + 1] = 0;
	return (left_c);
}

static char *fill_line_buffer(int fd, char *left_c, char *buffer)
{
	ssize_t		bytes_read;
	char		*temp;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE); //checking if reading from file worked
		if (bytes_read == -1)
		{
			free (left_c);
			return (NULL);
		}
		else if (bytes_read == 0)
			break;
		buffer[bytes_read] = 0;
		if (left_c == NULL)
			left_c = ft_strdup("");
		temp = left_c;
		left_c = ft_strjoin(temp, buffer);
		free(temp);
		temp = NULL;
		if (ft_strchr(buffer, '\n'))
			break;
	}
	return (left_c);
}

char *get_next_line(int fd)
{
	char		*buffer;
	char		*line;
	static char	*left_c;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1)); //allocate memory for buffer and checking if it worked
	if (!buffer)
		return(NULL);
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0) //checking if file descriptor is a valid value
	{
		free(left_c);
		free(buffer);
		left_c = NULL;
		buffer = NULL;
		return (NULL);
	}
	line =  fill_line_buffer(fd, left_c, buffer); //Call function to start reading from file
	free(buffer);
	buffer = NULL;
	if (line == NULL)
		return (NULL);
	left_c = set_line(line);
	return (line);
}
