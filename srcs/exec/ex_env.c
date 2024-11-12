/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:09:43 by nboer             #+#    #+#             */
/*   Updated: 2024/11/12 23:41:34 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*path_join(char *path_split, char *cmd_arg)
{
	char	*temp;
	char	*joined_path;

	temp = ft_strjoin(path_split, "/");
	if (!temp)
		return (NULL);
	joined_path = ft_strjoin(temp, cmd_arg);
	if (!joined_path)
		return (NULL);
	free(temp);
	return (joined_path);
}

// Search through bin folder (NOW LINKED LIST) which dir contains PATH environment variable, 
// skips "PATH" in the string and returns it.
char	*get_path_env(char **path_env)
{
	int i = 0;
	while (path_env[i]) // TO DO THIS WILL BECOME LINKED LIST
	{
		if (!(ft_strncmp(path_env[i], "PATH=", 5)))
			return (path_env[i] + 5); 
		i++;
	}
	return (NULL);
}
// join path and run if result can be executed
void	run_ex(char *arg, char **path_env)
{
	int		i;
	char	**path_split;
	char	*check_path;
	char	**cmd_arg;

	path_split = ft_split(get_path_env(path_env), ':');
	// if (!pathsplit)
	cmd_arg = ft_split(arg, ' ');
	// if (!cmd_arg)!!
	i = 0;
	while (path_split[i])
	{
		check_path = path_join(path_split[i], cmd_arg[0]);
		if (!(access(check_path, X_OK)))
			if (execve(check_path, cmd_arg, path_env) == -1)
				str_error("exec error");
		i++;
		free(check_path);
	}
	free_array(cmd_arg);
	free_array(path_split);
	str_error("cmd not found\n");
}
