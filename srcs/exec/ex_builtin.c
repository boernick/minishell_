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

// runs a given builtin function and returns last exit code
void	run_builtin(char **argv, t_shell *shell)
{
	if (!ft_strncmp(argv[0], "echo", 4))
		shell->last_exit = builtin_echo(argv);
	else if (!ft_strncmp(argv[0], "cd", 2))
		shell->last_exit = builtin_cd(argv, shell);
	else if (!ft_strncmp(argv[0], "pwd", 3))
		shell->last_exit = builtin_pwd(argv, shell);
	else if (!ft_strncmp(argv[0], "unset", 5))
		shell->last_exit = builtin_unset(argv, shell);
	else if (!ft_strncmp(argv[0], "env", 3))
		shell->last_exit = builtin_env(argv, shell);
	else if (!ft_strncmp(argv[0], "exit", 4))
		shell->last_exit = builtin_exit(argv, shell);
	else if (!ft_strncmp(argv[0], "export", 6))
		shell->last_exit = builtin_export(argv, shell);
}
