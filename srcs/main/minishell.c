/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:53:08 by nboer             #+#    #+#             */
/*   Updated: 2024/11/20 23:56:28 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec_mini(t_shell *shell, t_execution *pipex, char **env)
{
	(void) shell;
	
	pid_t		*pids;
	int			i;

	calibrate_exec(pipex);
	exec_init(pipex, pipex->cmd);
	if (pipex->n_pipes > 0)
		create_pipes(pipex);
	pids = malloc(pipex->n_cmds * sizeof(pid_t));
	if (!pids)
		str_error("malloc failed");
	i = 0;
	while (pipex->index_cmd < pipex->n_cmds)
	{
		setup_redirections(pipex->cmd);
		pids[i] = fork_child();
		if (pids[i++] == 0) //case child
		{
			if (pipex->cmd->is_builtin == 1)
				run_builtin(is_builtin(pipex->cmd->argv), pipex->cmd->argv, shell);
			else
			{
				get_fd(pipex, pipex->cmd); //DUP2 to STDIN/OUT
				clean_pipes(pipex, pipex->cmd); //CLOSING FDS
				run_ex(pipex->cmd, env); //RUN EX
				// exit(EXIT_SUCCESS); // this should be removed?
			}
		}
		else
			update_exec(pipex);
	}
	clean_pipes(pipex, pipex->cmd);
	waitpids(pids, pipex->n_cmds);
}

int	main(int argc, char **argv, char **envp)
{
	t_execution	pipex;
	t_shell		shell;
	t_parse		parse;

	(void) argv;
	struct_init(&parse);
	t_env_init(&shell, envp);
	if (argc != 1)
	{
		printf("\"./minishell\" must be the only argument\n");
		return (0);
	}
	// while (shell.exit == 0) //while no exit signal
	// {
	// 	//handle signals
	// 	if (1) //TO-DO if input is correct
	// tokenize_and_parse(&parse);
	exec_mini(&shell, &pipex, envp);
	// }
	//free t_env()
	return (0);
}
