#include "../../includes/minishell.h"

void handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);           // Move to a new line
	rl_replace_line("", 0);                 // Clear the current input
	rl_on_new_line();                       // Inform readline of a new prompt
	rl_redisplay();                         // Refresh the display
}

void handle_sigquit(int sig)
{
	(void) sig;
	rl_on_new_line();
	rl_redisplay();
	return ;
}


void setup_signal_handlers(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	// SIGINT
	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = SA_RESTART;           // Automatically restart interrupted syscalls
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	// SIGQUIT
	sa_quit.sa_handler = handle_sigquit;
	sa_quit.sa_flags = SA_RESTART;          // Same behavior as SIGINT
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void handle_eof(void)
{
	printf("exit\n");
	//add function to free all memory
	exit(0);
}
