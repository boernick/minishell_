 #include "../../includes/minishell.h"

void handle_eof(void)
{
	printf("exit\n");
	//add function to free all memory
	exit(0);
}

void	setup_signal_handlers(int signum)
{
	// (void)signum;
	// ft_putendl_fd("", STDOUT_FILENO);
	// printf("Redisplaying prompt\n");
	// rl_on_new_line();
	// rl_replace_line("", 1);
	// rl_redisplay();
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);           // Move to a new line
	rl_replace_line("", 0);                 // Clear the current input
	rl_on_new_line();                       // Inform readline of a new prompt
	rl_redisplay();                         // Refresh the display
}

void	process_running_sigint_handler(int signum)
{
	ft_putendl_fd("", STDOUT_FILENO);
	(void)signum;
}

void	handle_sigquit(int signum)
{
	ft_putendl_fd("Quit", STDOUT_FILENO);
	(void)signum;
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
		//printf("switch_signal_handlers: signal standard running\n");
		sa_int->sa_handler = setup_signal_handlers;
		sigaction(SIGINT, sa_int, NULL);
		sa_quit->sa_handler = SIG_IGN;
		sigaction(SIGQUIT, sa_quit, NULL);
}

void	inside_process_signals(t_sigact *sa_int, t_sigact *sa_quit)
{
		//printf("switch_signal_handlers: signal standard running\n");
		sa_int->sa_handler = process_running_sigint_handler;
		sigaction(SIGINT, sa_int, NULL);
		sa_quit->sa_handler = handle_sigquit;
		sigaction(SIGQUIT, sa_quit, NULL);
}

// void handle_sigint(int sig)
// {
// 	(void)sig;
// 	write(STDOUT_FILENO, "\n", 1);           // Move to a new line
// 	rl_replace_line("", 0);                 // Clear the current input
// 	rl_on_new_line();                       // Inform readline of a new prompt
// 	rl_redisplay();                         // Refresh the display
// }

// void handle_sigquit(int sig)
// {
// 	(void) sig;
// 	rl_on_new_line();
// 	rl_redisplay();
// 	return ;
// }


// void setup_signal_handlers(void)
// {
// 	struct sigaction sa_int;
// 	struct sigaction sa_quit;

// 	// SIGINT
// 	sa_int.sa_handler = handle_sigint;
// 	sa_int.sa_flags = SA_RESTART;           // Automatically restart interrupted syscalls
// 	sigemptyset(&sa_int.sa_mask);
// 	sigaction(SIGINT, &sa_int, NULL);

// 	// SIGQUIT
// 	sa_quit.sa_handler = handle_sigquit;
// 	sa_quit.sa_flags = SA_RESTART;          // Same behavior as SIGINT
// 	sigemptyset(&sa_quit.sa_mask);
// 	sigaction(SIGQUIT, &sa_quit, NULL);
// }

// void	switch_signal_handlers(t_sigact *sa_int, t_sigact *sa_quit, bool pr)
// {
// 	if (pr)
// 	{
// 		//printf("switch_signal_handlers: process running\n");
// 		sa_int->sa_handler = process_running_sigint_handler;
// 		sigaction(SIGINT, sa_int, NULL);
// 		sa_quit->sa_handler = handle_sigquit;
// 		sigaction(SIGQUIT, sa_quit, NULL);
// 	}
// 	else
// 	{
// 		//printf("switch_signal_handlers: signal standard running\n");
// 		sa_int->sa_handler = setup_signal_handlers;
// 		sigaction(SIGINT, sa_int, NULL);
// 		sa_quit->sa_handler = SIG_IGN;
// 		sigaction(SIGQUIT, sa_quit, NULL);
// 	}
// }
