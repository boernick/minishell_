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
	if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC) // || HEREDOC
		fd = open(filename, O_RDONLY);
	else if (type == TOKEN_REDIR_OUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == TOKEN_REDIR_APPEND)
		fd = open(filename, O_WRONLY | O_CREAT |O_APPEND, 0644);
	else
		str_error("wrong type argument to handle file\n");
	return (fd);
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

//loop through array and free all values
void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

t_cmd	*find_cmdlst_index(t_cmd *cmd_lst, int n)
{
	t_cmd	*lst;

	lst = cmd_lst;
	while (n >= 0)
	{
		lst = lst->next;
		n--;
	}
	return (lst);
}

int	cmdlst_length(t_cmd *cmd_lst)
{
	t_cmd	*lst;
	int		i;

	i = 0;
	lst = cmd_lst;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

// prints the content of every t_env with newline
void	print_lst(t_env *lst)
{
	while (lst)
	{
		ft_putstr_fd(lst->content, 2);
		ft_putstr_fd("\n", 2);
		lst = lst->next;
	}
}
