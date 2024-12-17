/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 14:53:03 by nboer             #+#    #+#             */
/*   Updated: 2024/12/17 20:55:26 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	invalid_identifier(char *builtin, char *arg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(builtin, STDERR_FILENO);
	ft_putstr_fd(": '", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (EXIT_FAILURE);
}
