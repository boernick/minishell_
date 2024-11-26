/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:11:28 by nboer             #+#    #+#             */
/*   Updated: 2024/11/24 19:41:03 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// checks if a given cmd argument is a builtin function
int	is_builtin(char **argv)
{
	ft_putstr_fd("checking builtin..\n", 2); //DEBUG
	if (!(ft_strncmp(argv[1], "echo", 4)))
		return (1);
	if (!ft_strncmp(argv[1], "cd", 2))
		return (2);
	if (!ft_strncmp(argv[1], "pwd", 3))
		return (3);
	if (!ft_strncmp(argv[1], "unset", 5))
		return (4);
	if (!ft_strncmp(argv[1], "env", 3))
		return (5);
	if (!ft_strncmp(argv[1], "exit", 4))
		return (6);
	return (-1);
}

// runs a given builtin function and returns last exit code
int	run_builtin(int n, char **argv, t_shell *shell)
{
	if (n == 1)
		shell->last_exit = builtin_echo(argv);
	else if (n == 2)
		shell->last_exit = builtin_cd(argv, shell);
	else if (n == 3)
		shell->last_exit = builtin_pwd(argv, shell);
	else if (n == 4)
		shell->last_exit = builtin_unset(argv, shell);
	else if (n == 5)
		shell->last_exit = builtin_env(shell);
	else if (n == 6)
		shell->last_exit = builtin_exit(argv);
	return (0);
}
