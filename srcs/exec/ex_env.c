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

//joins and formats to a correct path 
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
	char	*checkp;

	if (ft_strchr(cmd->argv[0], '/'))
		return (run_path(cmd, path_env));
	path_split = ft_split(get_path_env(path_env), ':');
	if (!path_split)
		str_error("path split failure");
	if (!cmd->argv || !cmd->argv[0])
		str_error("no cmd->argv in struct");
	i = 0;
	while (path_split[i])
	{
		checkp = path_join(path_split[i++], cmd->argv[0]);
		if (!access(checkp, X_OK) && execve(checkp, cmd->argv, path_env) == -1)
		{
			free(checkp);
			break ;
		}
		free(checkp);
	}
	free_array(path_env);
	free_array(path_split);
	return (cmd_not_found(cmd->argv[0]));
}

//runs given path directly from prompt and prints errors
int	run_path(t_cmd *cmd, char **path_env)
{
	if (!(check_dir(cmd->argv[0])))
	{
		free_array(path_env);
		return (is_a_directory(cmd->argv[0]));
	}
	if (access(cmd->argv[0], X_OK) == 0)
	{
		if (execve(cmd->argv[0], cmd->argv, path_env) == -1)
		{
			free_array(path_env);
			if (errno == EACCES)
				return (permission_denied(cmd->argv[0]));
			str_error("minishell: error: execve failed");
		}
	}
	else if (errno == EACCES)
	{
		free_array(path_env);
		return (permission_denied(cmd->argv[0]));
	}
	free_array(path_env);
	invalid_filedir(cmd->argv[0]);
	return (127);
}
