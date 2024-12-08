/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:09:43 by nboer             #+#    #+#             */
/*   Updated: 2024/11/24 19:48:55 by nboer            ###   ########.fr       */
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

// Search through bin folder (NOW LINKED LIST) which dir contains PATH 
// environment variable, skips "PATH" in the string and returns it.
char	*get_path_env(char **path_env)
{
	int	i;

	i = 0;
	while (path_env[i])
	{
		if (!(ft_strncmp(path_env[i], "PATH=", 5)))
			return (path_env[i] + 5); 
		i++;
	}
	return (NULL);
}
// join path and run if result can be executed
int	run_ex(t_cmd *cmd, char **path_env)
{
	int		i;
	char	**path_split;
	char	*check_path;

	path_split = ft_split(get_path_env(path_env), ':');
	if (!path_split)
		str_error("path split failure");
	if (!cmd->argv || !cmd->argv[0])
		str_error("no cmd->argv in struct");
	i = 0;
	while (path_split[i])
	{
		check_path = path_join(path_split[i], cmd->argv[0]);
		if (!(access(check_path, X_OK)))
			if (execve(check_path, cmd->argv, path_env) == -1)
				str_error("exec error");
		i++;
		free(check_path);
	}
	free_array(path_split);
	ft_putstr_fd(cmd->argv[0], 2);
	ft_putendl_fd(": command not found", 2);
	return (127);
}
