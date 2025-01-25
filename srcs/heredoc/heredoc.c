/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:19:38 by prichugh          #+#    #+#             */
/*   Updated: 2025/01/23 12:19:38 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	reverse_str(char *str, int start, int end)
{
	char	tmp_char;

	while (start < end)
	{
		tmp_char = str[start];
		str[start] = str[end];
		str[end] = tmp_char;
		start++;
		end--;
	}
}

void	int_to_str(int num, char *str, int *i)
{
	int	tmp;
	int	start;
	int	end;

	if (num == 0)
	{
		str[*i] = '0';
		(*i)++;
		return ;
	}
	tmp = *i;
	while (num > 0)
	{
		str[tmp] = (num % 10) + '0';
		num /= 10;
		tmp++;
	}
	start = *i;
	end = tmp - 1;
	reverse_str(str, start, end);
	*i = tmp;
}

char	*generate_temp_filename(void)
{
	static int	counter = 0;
	char		*filename;
	int			i;

	filename = malloc(32);
	if (!filename)
		malloc_error(sizeof(char *));
	filename[0] = '/';
	filename[1] = 't';
	filename[2] = 'm';
	filename[3] = 'p';
	filename[4] = '/';
	filename[5] = 'h';
	filename[6] = 'd';
	filename[7] = '_';
	i = 8;
	int_to_str(counter, filename, &i);
	filename[i++] = '.';
	filename[i++] = 't';
	filename[i++] = 'x';
	filename[i++] = 't';
	filename[i] = '\0';
	counter++;
	return (filename);
}

char	*create_heredoc(void)
{
	char	*temp_file;
	int		fd;

	temp_file = generate_temp_filename();
	if (!temp_file)
	{
		ft_putstr_fd("Failed to allocate memory for temp file name",
			STDERR_FILENO);
		return (NULL);
	}
	fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("Failed to create heredoc file", STDERR_FILENO);
		free(temp_file);
		return (NULL);
	}
	return (temp_file);
}

void	cleanup_heredoc(t_cmd *cmd_p)
{
	t_cmd	*cmd;

	cmd = cmd_p;
	while (cmd)
	{
		if (cmd->redir->type == TOKEN_HEREDOC)
			if (cmd->redir->file != NULL)
				unlink(cmd->redir->file);
	}
}
