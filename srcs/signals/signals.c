#include "../../includes/minishell.h"

void	handle_eof(t_shell *shell, t_parse *parse)
{
	printf("exit\n");
	free_tokens(parse->head);
	free_command_stack(parse->cmd);
	free_envlst(shell->env_lst);
	clear_history();
	exit(0);
}

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
