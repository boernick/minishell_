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
		if (strchr(lst->content, '='))
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
			continue ;
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
	char	*env;
	int		ret;
	int		i;

	ret = 0;
	if (argv[0] && !argv[1])
		export_lst(shell->env_lst);
	else if (!ft_strncmp(argv[1], "", 2))
		ret = invalid_identifier("export", argv[1]);
	else
	{
		i = 1;
		while (argv[i])
		{
			env = argv[i];
			if (!export_is_valid_str(env))
				ret = invalid_identifier("export", argv[i]);
			if (!ret)
				ret = export_check(env);
			if (!ret)
				export_env_var(env, shell);
			i++;
		}
	}
	return (ret);
}
