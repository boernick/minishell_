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

// organizes execution process for minishell
void	exec_mini(t_shell *shell, t_execution *pipex)
{
	pid_t		*pids;
	pid_t		pid_last;

	pid_last = -1;
	exec_init(shell, pipex, pipex->cmd);
	if (pipex->n_pipes > 0)
		create_pipes(pipex);
	pids = malloc(pipex->n_cmds * sizeof(pid_t));
	if (!pids)
		str_error("malloc failed");
	if (pipex->cmd->is_builtin == 1 && pipex->n_cmds == 1)
		run_single_builtin(pipex, shell);
	else
	{
		while (pipex->index_cmd < pipex->n_cmds)
			pid_last = run_single_cmd(shell, pipex, pids);
		clean_pipes(pipex);
		waitpids(pids, pipex->n_cmds, shell, pid_last);
	}
	if (pids != NULL)
		free(pids);
}

//organizes execution process for a single command
pid_t	run_single_cmd(t_shell *shell, t_execution *pipex, pid_t *pids)
{
	int	redir_status;

	redir_status = 0;
	redir_status = setup_redirections(pipex->cmd);
	if (redir_status == 1)
		shell->last_exit = 1;
	pids[pipex->index_cmd] = fork_child();
	if (pids[pipex->index_cmd] == 0)
	{
		if (redir_status == 1)
		{
			clean_pipes(pipex);
			cleanup_child(shell, pipex);
			exit(1);
		}
		run_child_exec(pipex, shell);
	}
	update_exec(pipex);
	return (pids[pipex->index_cmd - 1]);
}

// child process runs builtin or executable
void	run_child_exec(t_execution *pipex, t_shell *shell)
{
	get_fd(pipex, pipex->cmd);
	clean_pipes(pipex);
	close_fd_in_out(pipex->cmd);
	if (pipex->cmd->is_builtin)
		run_builtin(pipex->cmd->argv, shell);
	else
		shell->last_exit = run_ex(pipex->cmd, envlst_to_array(shell));
	cleanup_child(shell, pipex);
	exit(shell->last_exit);
}

// organizes parent process for a single builtin without pipes
void	run_single_builtin(t_execution *pipex, t_shell *shell)
{
	if (setup_redirections(pipex->cmd) == 1)
	{
		shell->last_exit = 1;
		return ;
	}
	get_fd(pipex, pipex->cmd);
	close_fd_in_out(pipex->cmd);
	run_builtin(pipex->cmd->argv, shell);
	update_exec(pipex);
	reset_fds(pipex);
}

int	main(int argc, char **argv, char **envp)
{
	t_execution	pipex;
	t_shell		shell;
	t_parse		parse;
	t_sigaction	sa_int;
	t_sigaction	sa_quit;

	(void) argv;
	if (argc != 1)
		return (invalid_argument_count(1));
	struct_init(&parse, &shell);
	t_env_init(&shell, envp);
	init_signal_handlers(&sa_int, &sa_quit);
	while (shell.exit == 0)
	{
		tokenize(&parse, &shell, &sa_int, &sa_quit);
		if (parse.valid_input)
			run_mini(&parse, &shell, &pipex);
		outside_process_signals(&sa_int, &sa_quit);
		free_tokens(parse.head);
		parse.head = NULL;
	}
	cleanup(&parse, &shell, &pipex);
	return (shell.last_exit);
}
