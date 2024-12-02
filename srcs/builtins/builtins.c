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

int	builtin_env(t_shell *shell)
{
	t_env	*lst;

	if (!shell || !shell->env_lst)
		return (EXIT_FAILURE);
	lst = shell->env_lst;
	while (lst)
	{
		ft_printf("%s\n", lst->content);
		lst = lst->next;
	}
	return (EXIT_SUCCESS);
}

// write text to the terminal or to a file
int	builtin_echo(char **argv) //finished
{
	int	i;
	int	toggle;

	toggle = 0;
	i = 1;
	if (argv[i] && argv[i][0] == '-' && ft_strncmp(argv[i], "-n", 3) != 0)
	{
		ft_putstr_fd("echo: invalid option --", STDERR_FILENO);
		return (1);
	}
	if (argv[i] && !ft_strncmp(argv[i], "-n", 3))
	{
		toggle = 1;
		i++;
	}
	while (argv[i])
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!toggle)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}

int	builtin_cd(char **argv, t_shell *shell) //not yet finished
{
	// t_env	*env;
	// t_env	*pwd;
	// env = shell->env_lst;

	if (argv[1] && argv[2])
		ft_putstr_fd("Error: too many arguments", STDERR_FILENO);
	if (argv[1] && argv[1][0] == '-')
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putendl_fd(": invalid option", 2);
		return (EXIT_FAILURE);
	}
	getcwd(shell->cwd, PATH_MAX);

	// if (argv[2] == NULL)
	// 	pwd = get_env_lst(env, "PWD");
	// 	ft_strlcpy();
	// while

		// if (cd cannot change the specified directory -> means doesnt exist or no permissions)
		// error handling and the working directory remains unchanged
	return (EXIT_SUCCESS);
}

int	builtin_pwd(char **argv, t_shell *shell) //finished
{
	if (argv[1] && argv[1][0] == '-')
	{
		ft_putendl_fd("Error: no options integrated for builtin", 2);
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(shell->cwd, 1);
	return (EXIT_SUCCESS);
}

int	builtin_export(char **argv, t_shell *shell)
{
	char	*pos;
	char	*env;

	if (argv[0] && !argv[1])
	{
		export_lst(shell->env_lst);
		return (EXIT_SUCCESS);
	}
	env = argv[1];
	if (env[0] == '=')
	{
		ft_putendl_fd("export: '=': not a valid identifier", 2);
		return (1);
	}
	pos = ft_strchr(env, '=');
	if (!pos)
		ft_putendl_fd("= sign not found", 2); // need to add export <NAME> without '='
	if (pos)
	{
		env_addback(shell, argv[1]); 
		pos = NULL;
	}
	return (EXIT_SUCCESS);
}

int	builtin_unset(char **argv, t_shell *shell)
{
	int	i;
	int	ret;

	ret = 0;
	i = 1;
	if (!argv[1])
	{
		ft_putendl_fd("Error: no VAR provided", STDERR_FILENO);
		ret = 1;
	}
	while (argv[i])
	{
		if (env_del(shell, argv[i]) == -1)
		{
			ft_putstr_fd("unset: ", STDERR_FILENO);
			ft_putstr_fd(argv[i], STDERR_FILENO);
			ft_putendl_fd(": no such variable", STDERR_FILENO);
			ret = 1;
		}
		i++;
	}
	if (ret == 0)
		ft_putendl_fd("ENV variables succesfully deleted", STDOUT_FILENO);
	return (ret);
}

int	builtin_exit(char **argv, t_shell *shell)
{
	ft_putstr_fd("running exit..\n", 2);
	if (argv[1] && argv[1][0] == '-')
	{
		ft_putendl_fd("Error: no options integrated for builtin", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (argv[1] && !(check_num(argv[1])))
	{
		ft_putendl_fd("exit: ", 2);
		ft_putendl_fd(argv[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		return (2);
	}
	else
	{
		ft_putendl_fd("exit", 1);
		shell->exit = 1;
	}
	return (0);
}

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

void	export_reset(t_env *lst)
{
	while (lst)
	{
		lst->export = 0;
		lst = lst->next;
	}
}

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
