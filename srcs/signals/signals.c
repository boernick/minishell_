/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:39:20 by prichugh          #+#    #+#             */
/*   Updated: 2025/01/23 12:39:20 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

void	setup_signal_handlers(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	process_running_sigint_handler(int signum)
{
	(void)signum;
	ft_putendl_fd("", STDOUT_FILENO);
}

void	handle_sigquit(int signum)
{
	(void)signum;
	ft_putendl_fd("Quit", STDOUT_FILENO);
}

void	outside_process_signals(t_sigact *sa_int, t_sigact *sa_quit)
{
	sa_int->sa_handler = setup_signal_handlers;
	sigaction(SIGINT, sa_int, NULL);
	sa_quit->sa_handler = SIG_IGN;
	sigaction(SIGQUIT, sa_quit, NULL);
}

void	inside_process_signals(t_sigact *sa_int, t_sigact *sa_quit)
{
	sa_int->sa_handler = process_running_sigint_handler;
	sigaction(SIGINT, sa_int, NULL);
	sa_quit->sa_handler = handle_sigquit;
	sigaction(SIGQUIT, sa_quit, NULL);
}
