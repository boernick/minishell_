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

	exec_init(shell, pipex, pipex->cmd);
	// shell->last_exit = run_heredoc(pipex);
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
		clean_pipes(pipex, pipex->cmd);
		waitpids(pids, pipex->n_cmds, shell, pid_last);
		//ft_printf("last exit at the end of exec: %i\n", shell->last_exit);
	}
}
//organizes execution process for a single command
pid_t	run_single_cmd(t_shell *shell, t_execution *pipex, pid_t *pids)
{
	int	i;
	int	redir_status;

	i = pipex->index_cmd;
	redir_status = 0;
	redir_status = setup_redirections(pipex->cmd);
	if (redir_status == 1)
		shell->last_exit = 1;
	pids[i] = fork_child();
	if (pids[i++] == 0 && redir_status != 1)
		run_child_exec(pipex, shell);
	else
		update_exec(pipex);
	return (pids[i]);
}

// child process runs builtin or executable
void	run_child_exec(t_execution *pipex, t_shell *shell)
{
	get_fd(pipex, pipex->cmd); //DUP2 TO STDIN
	clean_pipes(pipex, pipex->cmd); //CLOSING FDS
	if (pipex->cmd->is_builtin)
		run_builtin(pipex->cmd->argv, shell); // exit code?
	else
		shell->last_exit = run_ex(pipex->cmd, envlst_to_array(shell));
	exit(shell->last_exit); //added to stop infinite loop when command not found
}

// organizes parent process for a single builtin without pipes
void	run_single_builtin(t_execution *pipex, t_shell *shell)
{
	if (setup_redirections(pipex->cmd) == 1)
	{
		shell->last_exit = 1;
		return;
	}
	get_fd(pipex, pipex->cmd); //DUP2 to STDIN/OUT
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

	(void)	argv;
	if (argc != 1)
	{
		printf("\"./minishell\" must be the only argument\n");
		return (0);
	}
	struct_init(&parse);
	t_env_init(&shell, envp);
	shell.exit = 0;
	while (shell.exit == 0)
    {
        tokenize(&parse, &shell);
        if (parse.valid_input)
        {
            parse_tokens(&parse);
            pipex.cmd = parse.cmd;
            exec_mini(&shell, &pipex);
        }
        free_tokens(parse.head);
        parse.head = NULL;
		//printf("exit status: %i\n", shell.last_exit);
    }
	free_tokens(parse.head);
	free_command_stack(parse.cmd);
	clear_history();//make sure to use the better one (this vs the one below)
	rl_clear_history();//make sure to use the better one
	free_envlst(shell.env_lst);
	//printf("last exit, on exit: %i\n", shell.last_exit);
	return (shell.last_exit);
}
