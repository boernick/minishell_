/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:11:28 by nboer             #+#    #+#             */
/*   Updated: 2024/11/12 23:42:38 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// checks if a given cmd argument is a builtin function
int	is_builtin(t_execution *pipex, char **argv)
{
	(void) pipex;
	if (!(ft_strncmp(argv[1], "echo", 4)))
	{
		if (!(ft_strncmp(argv[2], "-n", 2)))
			return (2);
		return (1);
	} 
	if (!ft_strncmp(argv[1], "cd", 2))
		return (3);
	if (!ft_strncmp(argv[1], "pwd", 3))
		return (4);
	if (!ft_strncmp(argv[1], "unset", 5))
		return (5);
	if (!ft_strncmp(argv[1], "env", 3))
		return (6);
	return (-1);
}

// runs a given builtin function
int	run_builtin(int	n, char **argv, t_data *shell)
{
	if (n == 1 || n == 2)
		builtin_echo(argv, n);
	else if (n == 3)
		str_error("run builtin_cd");
	else if (n == 4)
		str_error("run builtin_pwd");
	else if (n == 5)
		str_error("run builtin_unset");
	else if (n == 6)
		builtin_env(shell);
	return (0);
}
