/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:41:43 by prichugh          #+#    #+#             */
/*   Updated: 2025/01/23 12:41:43 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_perror(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int	malloc_error(size_t n)
{
	ft_putstr_fd("minishell: malloc: cannot allocate ", STDERR_FILENO);
	ft_putnbr_fd(n, STDERR_FILENO);
	ft_putendl_fd(" bytes (or more)", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

int	ambiguous_redirect_error(char *token)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
	return (1);
}

int	heredoc_eof_warning(int line_number, char *delimeter, t_parse *data)
{
	data->valid_input = 0;
	ft_putstr_fd("minishell: warning: here-document at line ", STDERR_FILENO);
	ft_putnbr_fd(line_number, STDERR_FILENO);
	ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(delimeter, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
	return (0);
}
