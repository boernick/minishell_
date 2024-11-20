/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:18:32 by nboer             #+#    #+#             */
/*   Updated: 2024/11/20 19:44:41 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// prepare exec struct for use
void	exec_init(t_execution *pipex, t_cmd *cmd)
{
	pipex->n_cmds = cmdlst_length(cmd);
	pipex->n_pipes = pipex->n_cmds - 1;
	pipex->infile = STDIN_FILENO;
	pipex->outfile = STDOUT_FILENO;
	pipex->index_pipe = 0;
	pipex->index_cmd = 0;
	pipex->index_prev_pipe = -1;
}

void setup_redirections(t_cmd *cmd)
{
	t_redirect	*redir;
	cmd->fdin = -2;
	cmd->fdout = -2;
	
	if (!cmd->redir)
		return;
	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == TOKEN_REDIR_IN || redir->type == TOKEN_HEREDOC)
			cmd->fdin = handle_file(redir->file, redir->type);
		if (cmd->fdin == -1)
			str_error("setup_redirections(): error reading the file.");
		if (cmd->fdout != -2)
			close(cmd->fdout);
		if (redir->type == TOKEN_REDIR_OUT || redir->type == 
			TOKEN_REDIR_APPEND)
			cmd->fdout = handle_file(redir->file, redir->type);
		redir = redir->next;
	}
	ft_putstr_fd("gets here\n", 2);
}

// prepare exec struct for next call
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

	ft_putstr_fd("creating pipes..\n", 2); //DEBUG
	if (!(pipex->pipe_arr = malloc(sizeof(int *) * pipex->n_pipes + 1)))
		str_error("Malloc failure while creating array of pointers");
	pipex->pipe_arr[0] = NULL;
	i = 0;
	while (i < pipex->n_pipes)
	{
		if (!(pipex->pipe_arr[i] = malloc(sizeof(int) * 2)))
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
	ft_putstr_fd("forking child..\n", 2);
	pid = fork();
	if (pid < 0)
		str_error("Error: false PID");
	return (pid);
}

//redirect STDIN to INFILE, STDOUT to OUTFILE, and between linking pipes 
void	get_fd(t_execution *pipex, t_cmd *cmd)
{
	// ft_putstr_fd("duping fds..\n", 2);
	// ft_putnbr_fd(cmd->fdin, 2);
	// ft_putstr_fd("\n", 2);
	if (cmd->fdin != -2) //if there is a redirection-> overwrite it into the pipex->infile
		pipex->infile = cmd->fdin;	
	if (pipex->index_pipe == 0)
		dup2(pipex->infile, STDIN_FILENO); 
	else
		dup2(pipex->pipe_arr[pipex->index_prev_pipe][0], STDIN_FILENO);


	if (cmd->fdout != -2) //if there is a redirection-> overwrite it into the pipex->outfile
		pipex->outfile = cmd->fdout;	
	if (pipex->index_cmd == pipex->n_cmds - 1)
		dup2(pipex->outfile, STDOUT_FILENO);
	else
		dup2(pipex->pipe_arr[pipex->index_pipe][1], STDOUT_FILENO);
}
// close all file descriptors in the pipe FD array
void	clean_pipes(t_execution *pipex, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!pipex->pipe_arr)
		return;
	while (i < pipex->n_pipes)
	{
		ft_putstr_fd("clean pipe..\n", 2); //DEBUG
		close(pipex->pipe_arr[i][0]);
		close(pipex->pipe_arr[i][1]);
		i++;
	}
	if (cmd->fdin >= 0)
		close(cmd->fdin); //FD ADJUSTMENTS
	if (cmd->fdout >= 0)
		close(cmd->fdout); //FD ADJUSTMENTS
}

// waits for a series of given child processes
void	waitpids(pid_t *pids, int n)
{
	int	i;
	
	i = 0;
	while (i < n)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}
