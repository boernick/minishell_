/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 15:05:24 by nboer             #+#    #+#             */
/*   Updated: 2025/01/19 15:14:37 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//exports an environment variable to the env list
void	export_env_var(char *env, t_shell *shell)
{
	char *pos;

	pos = ft_strchr(env, '=');
	export_deldup(shell, env);
	if (!pos)
		env_addback(shell, ft_strjoin(env, "="));
	if (pos)
	{
		env_addback(shell, env);
		pos = NULL;
	}
}

// Extract the part that updates the path check directory, and change it.
int cd_to_path(t_shell *shell, char *arg)
{
	char	*path;
	int		ret;

	path = cd_update_path(shell, arg);
	if (!path)
		return (EXIT_FAILURE);
	if (check_dir(path) == 1)
	{
		free(path);
		return (invalid_filedir_builtin("cd", arg));
	}
	ret = cd_check_error(chdir(path), arg);
	free(path);
	return (ret);
}

// check the directory for potential errors and return exit code.
int	cd_check_error(int err_status, char *dir)
{
	if (err_status == EACCES)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(dir, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
	}
	else if (err_status)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(dir, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	}
	else
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
// format the directory path
char	*cd_update_path(t_shell *shell, char *str)
{
	char	*path;

	if (str[0] == '/')
		path = ft_strdup("");
	else
		path = ft_strjoin(shell->cwd, "/");
	if (!path)
		return (NULL);
	path = ft_strjoin(path, str);
	if (!path)
		return (NULL);
	return (path);
}

//checks if directory path can be opened
int	check_dir(char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (!dir)
		return (EXIT_FAILURE);
	closedir(dir);
	return (EXIT_SUCCESS);
}
