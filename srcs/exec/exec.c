/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:18:32 by nboer             #+#    #+#             */
/*   Updated: 2024/11/24 19:50:11 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// prepare exec struct for use
void	exec_init(t_shell *shell, t_execution *pipex, t_cmd *cmd)
{
	pipex->n_cmds = cmdlst_length(cmd);
	pipex->n_pipes = pipex->n_cmds - 1;
	pipex->infile = STDIN_FILENO;
	pipex->outfile = STDOUT_FILENO;
	pipex->index_pipe = 0;
	pipex->index_cmd = 0;
	pipex->index_prev_pipe = -1;
	pipex->start_in = dup(STDIN_FILENO);
	pipex->start_out = dup(STDOUT_FILENO);
	getcwd(shell->cwd, PATH_MAX);
}

//process tokens for all redirections for a single command.
int setup_redirections(t_cmd *cmd)
{
	t_redirect *rdir;

	rdir = cmd->redir;
	cmd->fdin = -2;
	cmd->fdout = -2;
	while (rdir)
	{
		if (rdir->type == TOKEN_REDIR_IN || rdir->type == TOKEN_HEREDOC)
		{
			if (cmd->fdin != -2)
				close(cmd->fdin);
			cmd->fdin = handle_file(rdir->file, rdir->type);
		}
		if (cmd->fdout != -2)
			close(cmd->fdout);
		if (rdir->type == TOKEN_REDIR_OUT || rdir->type == TOKEN_REDIR_APPEND)
			cmd->fdout = handle_file(rdir->file, rdir->type);
		if (cmd->fdin == -1)
			return (invalid_filedir(rdir->file));
		if (cmd->fdout == -1)
			return EXIT_FAILURE;
		rdir = rdir->next;
	}
	return (EXIT_SUCCESS);
}

// reset FDs after execution process.
void	reset_fds(t_execution *pipex)
{
	pipex->infile = STDIN_FILENO;
	pipex->outfile = STDOUT_FILENO;
	dup2(pipex->start_in, STDIN_FILENO);
	dup2(pipex->start_out, STDOUT_FILENO);
}

// prepare exec struct for next call.
void	update_exec(t_execution *pipex)
{
	pipex->index_prev_pipe = pipex->index_pipe;
	pipex->index_pipe++;
	pipex->index_cmd++;
	pipex->cmd = pipex->cmd->next;
}

// create an array of pointers to integers to store the total amount of pipes.
void	create_pipes(t_execution *pipex)
{
	int		i;

	pipex->pipe_arr = malloc(sizeof(int *) * (pipex->n_pipes + 1));
	if (!(pipex->pipe_arr))
		str_error("Malloc failure while creating array of pointers");
	pipex->pipe_arr[0] = NULL;
	i = 0;
	while (i < pipex->n_pipes)
	{
		pipex->pipe_arr[i] = malloc(sizeof(int) * 2);
		if (!(pipex->pipe_arr[i]))
		{
			free_int_array(pipex, i + 1);
			str_error("Malloc failure while creating pipes");
		}
		if (pipe(pipex->pipe_arr[i]) == -1)
		{
			free_int_array(pipex, i + 1);
			str_error("Error creating pipes");
		}
		i++;
	}
	pipex->pipe_arr[i] = NULL;
}

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
	if (cmd->fdin != -2)
		dup2(cmd->fdin, STDIN_FILENO);
	else if (pipex->index_pipe == 0)
	{
		if (pipex->infile != -2)
			dup2(pipex->infile, STDIN_FILENO);
		else
			dup2(pipex->start_in, STDIN_FILENO);
	}
	else
		dup2(pipex->pipe_arr[pipex->index_prev_pipe][0], STDIN_FILENO);
	if (cmd->fdout != -2)
		dup2(cmd->fdout, STDOUT_FILENO);
	else if (pipex->index_cmd == pipex->n_cmds - 1)
	{
		if (pipex->outfile != -2)
			dup2(pipex->outfile, STDOUT_FILENO);
		else
			dup2(pipex->start_out, STDOUT_FILENO);
	}
	else
		dup2(pipex->pipe_arr[pipex->index_pipe][1], STDOUT_FILENO);
}

// close all file descriptors in the pipe FD array
void	clean_pipes(t_execution *pipex, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!pipex->pipe_arr)
		return ;
	while (i < pipex->n_pipes)
	{
		close(pipex->pipe_arr[i][0]);
		close(pipex->pipe_arr[i][1]);
		i++;
	}
	close_fd_in_out(cmd);
	//!!!!
	//cleanup_heredoc(cmd); //causes lots of errors in minishell tester
	//!!!!
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
				status = 1; //this was -1
			else if (WIFEXITED(tmp))
				status = WEXITSTATUS(tmp);
			else if (WTERMSIG(tmp) == SIGQUIT)
				status = 131;
			else if (WTERMSIG(tmp) == SIGINT)
				status = 130;
			else
				status = 0; // this was 1
			// printf("Child %i exit status: %d\n", pids[i], WEXITSTATUS(tmp));
		}
		i++;
	}
	if (pids[i] == pid_last)
		shell->last_exit = status;
}

//close open input and output filedescriptors for a single command
void	close_fd_in_out(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->fdin >= 0)
			close(cmd->fdin);
		if (cmd->fdout >= 0)
			close(cmd->fdout);
	}
}
