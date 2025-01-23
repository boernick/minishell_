/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:38:53 by prichugh          #+#    #+#             */
/*   Updated: 2025/01/23 12:38:53 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

void	init_signal_handlers(t_sigaction *sa_int, t_sigaction *sa_quit)
{
	(void)sa_quit;
	sigemptyset(&sa_int->sa_mask);
	sigaddset(&sa_int->sa_mask, SIGINT);
	sa_int->sa_flags = SA_RESTART;
	sa_int->sa_handler = setup_signal_handlers;
	sigaction(SIGINT, sa_int, NULL);
	sigemptyset(&sa_quit->sa_mask);
	sigaddset(&sa_quit->sa_mask, SIGQUIT);
	sa_quit->sa_flags = SA_RESTART;
	sa_quit->sa_handler = SIG_IGN;
	sigaction(SIGQUIT, sa_quit, NULL);
}

void	heredoc_sigint_handler(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
}

void	setup_heredoc_signals(struct sigaction *prev_sigint,
		struct sigaction *prev_sigquit)
{
	struct sigaction	sa;

	sigaction(SIGINT, NULL, prev_sigint);
	sigaction(SIGQUIT, NULL, prev_sigquit);
	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
