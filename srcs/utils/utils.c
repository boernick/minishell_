/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 22:00:29 by nick              #+#    #+#             */
/*   Updated: 2024/11/24 19:52:26 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// opens files in read/write/append permissions based on a given int type
int	handle_file(char *filename, int type)
{
	int	fd;

	fd = 0;
	if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
		fd = open(filename, O_RDONLY);
	else if (type == TOKEN_REDIR_OUT)
	{
		if (check_folder(filename) != EXIT_SUCCESS)
		{
			invalid_filedir(filename);
			return (-1);
		}
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (type == TOKEN_REDIR_APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		if (errno == EACCES)
			ft_putendl_fd("minishell: Permission denied", STDERR_FILENO);
	return (fd);
}

// check if directory of the file name exists
int	check_folder(char *filename)
{
	char	*last_dash;
	char	*folder_path;

	last_dash = (ft_strrchr(filename, '/'));
	if (last_dash)
	{
		folder_path = ft_strndup(filename, last_dash - filename);
		if (access(folder_path, F_OK) == -1)
		{
			free(folder_path);
			return (EXIT_FAILURE);
		}
		free(folder_path);
	}
	return (EXIT_SUCCESS);
}

//loops through t_env struct and frees all allocated memory
void	free_envlst(t_env *lst)
{
	t_env	*temp;

	temp = lst;
	while (lst)
	{
		temp = lst;
		lst = lst->next;
		if (temp->content)
			free(temp->content);
		free(temp);
	}
}

// free the pipe_array from the last index that was allocated in memory
void	free_int_array(t_execution *pipex, int i)
{
	int		**array;

	array = pipex->pipe_arr;
	if (!array)
		return ;
	while (i >= 0)
	{
		if (array[i])
			free(array[i]);
		i--;
	}
	free(array);
}

void	reset_expand_var(t_expand_var *expand)
{
	expand->var_index = 0;
	ft_memset(expand->var_name, 0, sizeof(expand->var_name));
}
