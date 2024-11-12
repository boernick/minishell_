#include "../../includes/minishell.h"

void handle_sigint(int sig)
{
	(void)sig;
	printf("\nSIGINT detected, exiting...\n");
	clear_history();
	exit(0);
}
