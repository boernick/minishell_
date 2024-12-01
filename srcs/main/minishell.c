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

void	exec_mini(t_shell *shell, t_execution *pipex)
{
	pid_t		*pids;
	int			i;

	//calibrate_exec(pipex);
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
		if (pipex->cmd->is_builtin == 1)
			run_builtin(do_builtin(pipex->cmd->argv), pipex->cmd->argv, shell); //need to solve redirections for builtins
		pids[i] = fork_child();
		if (pids[i++] == 0)
		{
			get_fd(pipex, pipex->cmd); //DUP2 to STDIN/OUT
			clean_pipes(pipex, pipex->cmd); //CLOSING FDS
			run_ex(pipex->cmd, envlst_to_array(shell)); //RUN EX
		}
		else
			update_exec(pipex);
	}
	// print_lst(shell->env_lst);
	clean_pipes(pipex, pipex->cmd);
	waitpids(pids, pipex->n_cmds);
}

int	main(int argc, char **argv, char **envp)
{
	t_execution	pipex;
	t_shell		shell;
	t_parse		parse;

	(void)	argv;
	if (argc != 1)
	{
		printf("\"./minishell\" must be the only argument\n");
		return (0);
	}
	struct_init(&parse);
	t_env_init(&shell, envp);
	signal(SIGINT, handle_sigint);
	shell.exit = 0;
	while (shell.exit == 0)
	{
		tokenize(&parse, &shell);
		parse_tokens(&parse);
		// print_command_stack(parse.cmd); //DEBUG
		pipex.cmd = parse.cmd;
		print_command_stack(pipex.cmd); //DEBUG
		exec_mini(&shell, &pipex);
		free_tokens(parse.head);
		parse.head = NULL; // Reset tokens to NULL
	}
	ft_putstr_fd("freeing tokens && clearing history..\n", 2);
	free_tokens(parse.head);
	free_command_stack(parse.cmd);
	clear_history();//make sure to use the better one (this vs the one below)
	rl_clear_history();//make sure to use the better one
	//free t_env()
}
