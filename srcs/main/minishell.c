/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:53:08 by nboer             #+#    #+#             */
/*   Updated: 2024/11/24 18:23:19 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec_mini(t_shell *shell, t_execution *pipex, char **env)
{
	pid_t		*pids;
	int			i;

	(void) env; //prince: unused parameter ‘env’
	calibrate_exec(pipex);
	exec_init(shell, pipex, pipex->cmd);
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
				// run_builtin(is_builtin(pipex->cmd->argv), pipex->cmd->argv, shell); //comented out for testing
			else
			{
				get_fd(pipex, pipex->cmd); //DUP2 to STDIN/OUT
				clean_pipes(pipex, pipex->cmd); //CLOSING FDS
				run_ex(pipex->cmd, env); //RUN EX
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
	//currently made it a inifinite loop cuz shell has no memmber named exit
	//while (shell.exit == 0) //while no exit signal
	while (1)
	{
		if (parse.cmd)
		{
			free_command_stack(parse.cmd);
			parse.cmd = NULL;
		}
		tokenize_and_parse(&parse);
		pipex.cmd = parse.cmd;
		// Print the command stack for debugging
		print_command_stack(pipex.cmd);
		//exec_mini(&shell, &pipex, envp);
		free_tokens(parse.head);
		parse.head = NULL; // Reset tokens to NULL
	}
	free_tokens(parse.head);
	clear_history();//make sure toe use the better one (this vs the one below)
	rl_clear_history();//make sure to use the better one
	exec_mini(&shell, &pipex, envp);
	//free t_env()
}
