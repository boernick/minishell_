/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 18:25:17 by nboer             #+#    #+#             */
/*   Updated: 2025/01/23 20:28:22 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// fork a child process and return error if PID is false
pid_t	fork_child(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		str_error("Error: false PID");
	return (pid);
}

//organizes filedescriptors for input and output redirection
void	get_fd(t_execution *pipex, t_cmd *cmd)
{
	if (pipex->infile >= 0)
	{
		close(pipex->infile);
		pipex->infile = -2;
	}
	if (cmd->fdin != -2)
		dup2(cmd->fdin, STDIN_FILENO);
	else if (pipex->index_pipe == 0)
		dup2(pipex->start_in, STDIN_FILENO);
	else
		dup2(pipex->pipe_arr[pipex->index_prev_pipe][0], STDIN_FILENO);
	if (pipex->outfile >= 0)
	{
		close(pipex->outfile);
		pipex->outfile = -2;
	}
	if (cmd->fdout != -2)
		dup2(cmd->fdout, STDOUT_FILENO);
	else if (pipex->index_cmd == pipex->n_cmds - 1)
		dup2(pipex->start_out, STDOUT_FILENO);
	else
		dup2(pipex->pipe_arr[pipex->index_pipe][1], STDOUT_FILENO);
}

// close all file descriptors in the pipe FD array
void	clean_pipes(t_execution *pipex)
{
	int	i;

	if (pipex->start_in != -2)
	{
		close(pipex->start_in);
		pipex->start_in = -2;
	}
	if (pipex->start_out != -2)
	{
		close(pipex->start_out);
		pipex->start_out = -2;
	}
	if (pipex->n_pipes == 0)
		return ;
	i = 0;
	while (i < pipex->n_pipes)
	{
		close(pipex->pipe_arr[i][0]);
		close(pipex->pipe_arr[i][1]);
		free(pipex->pipe_arr[i]);
		i++;
	}
	free(pipex->pipe_arr);
}

// waits for a series of given child processes
void	waitpids(pid_t *pids, int n_pids, t_shell *shell, pid_t pid_last)
{
	int	i;
	int	status;
	int	tmp;

	i = 0;
	status = 0;
	while (i < n_pids)
	{
		if (pids[i] != -1)
		{
			if (waitpid(pids[i], &tmp, 0) == -1)
				status = 1;
			else if (WIFEXITED(tmp))
				status = WEXITSTATUS(tmp);
			else if (WTERMSIG(tmp) == SIGQUIT)
				status = 131;
			else if (WTERMSIG(tmp) == SIGINT)
				status = 130;
			else
				status = 0;
		}
		if (pids[i] == pid_last)
			shell->last_exit = status;
		i++;
	}
}

//close open input and output filedescriptors for a single command
void	close_fd_in_out(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->fdin >= 0)
		{
			close(cmd->fdin);
			cmd->fdin = -2;
		}
		if (cmd->fdout >= 0)
		{
			close(cmd->fdout);
			cmd->fdout = -2;
		}
	}
}
