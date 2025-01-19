/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 15:11:29 by nboer             #+#    #+#             */
/*   Updated: 2025/01/19 15:11:56 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
