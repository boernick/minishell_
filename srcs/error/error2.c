/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 18:23:22 by nboer             #+#    #+#             */
/*   Updated: 2025/01/21 15:24:55 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	invalid_filedir_builtin(char *builtin, char *file)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(builtin, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(file, STDERR_FILENO);
	ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	return (EXIT_FAILURE);
}

int	permission_denied(char *arg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": Permission denied", STDERR_FILENO);
	return (126);
}

int	cmd_not_found(char *arg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": command not found", 2);
	return (127);
}

int	is_a_directory(char *arg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": Is a directory", STDERR_FILENO);
	return (126);
}

int invalid_argument_count(int ret)
{
	ft_putendl_fd("\"./minishell\" must be the only argument", STDERR_FILENO);
	return (ret);
}
