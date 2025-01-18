/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 23:06:21 by nick              #+#    #+#             */
/*   Updated: 2024/11/24 23:59:34 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// print the listed environment variables
int	builtin_env(char **argv, t_shell *shell)
{
	t_env	*lst;

	if (!shell || !shell->env_lst)
		return (EXIT_FAILURE);
	if (argv[1])
	{
		ft_putstr_fd("env: '", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putendl_fd("': No such file or directory", STDERR_FILENO);
		return (127);
	}
	lst = shell->env_lst;
	while (lst)
	{
		ft_printf("%s\n", lst->content);
		lst = lst->next;
	}
	return (EXIT_SUCCESS);
}

// write text to the terminal or to a file
int	builtin_echo(char **argv)
{
	int	i;
	int	toggle;

	toggle = 0;
	i = 1;
	if (argv[i] && !ft_strncmp(argv[i], "-n", 3))
	{
		toggle = 1;
		i++;
	}
	while (argv[i])
	{
		if (!(ft_strncmp(argv[i], "-n", 3)))
		{
			i++;
			continue;
		}
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
				ft_printf(" ");
		i++;
	}
	if (!toggle)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}

// change the current working directory to an absolute or relative path
int	builtin_cd(char **argv, t_shell *shell)
{
	char	*path;
	int		ret;
	t_env	*home;

	if (argv[1] && argv[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else if (argv[1] && argv[1][0] == '-')
		return (invalid_option("cd", argv[1]));
	else if (!argv[1])
	{
		home = get_env_lst(shell, "HOME");
		if (!home || !home->content || ft_strlen(home->content) <= 5)
			return (EXIT_FAILURE);
		path = home->content + 5;
		if (chdir(path) == -1)
			return (EXIT_FAILURE);
	}
	else
		ret = cd_to_path(shell, argv[1]);
	if (getcwd(shell->cwd, PATH_MAX) == NULL)
		ret = EXIT_FAILURE;
	return (ret);
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

// print the present working directory
int	builtin_pwd(char **argv, t_shell *shell)
{
	if (argv[1] && argv[1][0] == '-')
	{
		ft_putstr_fd("minishell: pwd: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putendl_fd(": invalid option", 2);
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(shell->cwd, 1);
	return (EXIT_SUCCESS);
}

// export a new env variable to the list
int	builtin_export(char **argv, t_shell *shell)
{
	char	*pos;
	char	*env;
	int		ret;

	ret = 0;
	if (argv[0] && !argv[1])
		export_lst(shell->env_lst);
	else if (!ft_strncmp(argv[1], "", 2))
		ret = invalid_identifier("export", argv[1]);
	else
	{
		env = argv[1];
		if (!export_is_valid(env))
		{
			ret = invalid_identifier("export", argv[1]);
			return (ret);
		}
		ret = export_check(env);
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
	return (ret);
}

int	export_is_valid(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_') // Must start with a letter or '_'
		return (0);
	i = 1;
	while (str[i]) // Check every character
	{
		if (str[i] == '=') // '=' marks the end of the identifier
			break;
		if (!ft_isalnum(str[i]) && str[i] != '_') // Only letters, numbers, and '_'
			return (0);
		i++;
	}
	return (1);
}

// remove duplicates in the env list to be exported
int	export_deldup(t_shell *shell, char *name)
{
	t_env	*lst;
	int		len;
	int		ret;

	ret = EXIT_SUCCESS;
	lst = shell->env_lst;
	len = 0;
	while (name[len] && name[len] != '=')
		len++;
	while (lst)
	{
		if (lst->content && !(ft_strncmp(lst->content, name, len)))
		{
			ret = env_del(shell, name);
			return (ret);
		}
		lst = lst->next;
	}
	return (ret);
}

// check if export builtin identifiers are valid
int	export_check(char *str)
{
	int		ret;

	ret = EXIT_SUCCESS;
	if (str[0] == '=' || ft_isdigit(str[0]))
		ret = invalid_identifier("export", str);
	return (ret);
}

// remove a listed env variable
int	builtin_unset(char **argv, t_shell *shell)
{
	int	i;
	int	ret;

	ret = EXIT_SUCCESS;
	i = 1;
	if (argv[0] && !argv[1])
		return (ret);
	if (argv[1][0] == '=' && !argv[1][1])
		return (ret);
	else
	{
		while (argv[i])
		{
			if (env_del(shell, argv[i]) == -1)
				ret = EXIT_SUCCESS;
			i++;
		}
	}
	return (ret);
}

// exit minishell with optional exit code
int	builtin_exit(char **argv, t_shell *shell)
{
	if (argv[1] && argv[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else if (argv[1] && !(exit_is_valid(argv[1])))
	{
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		return (2);
	}
	else
	{
		shell->last_exit = EXIT_SUCCESS;
		if (argv[1] && exit_is_valid(argv[1]))
			shell->last_exit = (ft_atoi(argv[1]) % 256 + 256) % 256;
		shell->exit = 1;
		ft_putendl_fd("exit", STDOUT_FILENO);
		return (shell->last_exit);
	}
}

// check if exit code is valid
bool	exit_is_valid(char *pnum)
{
	while (*pnum == ' ' || (*pnum >= 9 && *pnum <= 13))
		pnum++;
	if (*pnum == '-' || *pnum == '+')
		pnum++;
	while (ft_isdigit(*pnum))
		pnum++;
	if (*pnum != '\0')
		return (false);
	return (true);
}

//checks if the given string only consists of numbers
int	check_num(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(ft_isdigit(str[i])))
			return (0);
		i++;
	}
	return (1);
}

// print env variable list in alphabetic order
void	export_lst(t_env *env_lst)
{
	t_env	*lst;
	t_env	*cheapest;
	int		n_printed;
	int		len;

	lst = env_lst;
	len = lst_len(env_lst);
	n_printed = 0;
	while (n_printed < len)
	{
		cheapest = NULL;
		while (lst)
		{
			if ((!cheapest && !lst->export) ||
				(!lst->export && ft_strncmp(lst->content,
					cheapest->content, strlen(lst->content)) < 0))
				cheapest = lst;
			lst = lst->next;
		}
		export_lst_one(cheapest);
		lst = env_lst;
		n_printed++;
	}
	export_reset(env_lst);
}

// reset env linked list for the next export command
void	export_reset(t_env *lst)
{
	while (lst)
	{
		lst->export = 0;
		lst = lst->next;
	}
}

// print one env variable for export builtin
void	export_lst_one(t_env *lst)
{
	int	i;
	int	equal;

	i = 0;
	equal = 0;
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	while (lst->content[i])
	{
		ft_putchar_fd(lst->content[i], STDOUT_FILENO);
		if (lst->content[i] == '=' && !equal++)
			ft_putchar_fd('"', 1);
		i++;
	}
	if (equal)
		ft_putchar_fd('"', STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	lst->export = 1;
}
