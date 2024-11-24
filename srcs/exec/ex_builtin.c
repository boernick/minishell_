/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:11:28 by nboer             #+#    #+#             */
/*   Updated: 2024/11/24 14:36:21 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// checks if a given cmd argument is a builtin function
int	is_builtin(char **argv)
{
	ft_putstr_fd("checking builtin..\n", 2); //DEBUG
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
	if (!ft_strncmp(argv[1], "exit", 4))
		return (7);
	return (-1);
}

// runs a given builtin function
int	run_builtin(int	n, char **argv, t_shell *shell)
{
	if (n == 1 || n == 2)
		builtin_echo(argv, n);
	else if (n == 3)
		str_error("run builtin_cd");
	else if (n == 4)
		str_error("run builtin_pwd");
	else if (n == 5)
		builtin_unset(argv, shell);
	else if (n == 6)
		builtin_env(shell);
	else if (n == 7)
		builtin_exit(argv, shell);
	return (0);
}
