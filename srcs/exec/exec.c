/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:18:32 by nboer             #+#    #+#             */
/*   Updated: 2024/11/20 18:38:24 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// prepare exec struct for use
void	exec_init(t_execution *pipex, t_cmd *cmd_lst)
{
	// pipex->n_cmds = cmdlst_length(cmd_lst); Prince: commented out because i pass this info to you
	pipex->n_pipes = pipex->n_cmds - 1;
	ft_putstr_fd("number of cmds: ", 2); //DEBUG
	ft_putnbr_fd(pipex->n_cmds, 2); //DEBUG
	ft_putstr_fd("\n", 2); //DEBUG
	setup_redirections(pipex, cmd_lst);
	pipex->index_pipe = 0;
	pipex->index_cmd = 0;
	pipex->index_prev_pipe = -1;

}

void setup_redirections(t_execution *pipex, t_cmd *cmd)
{
	if (access(cmd->redir->file, F_OK) >= 0)
			pipex->infile = handle_file(cmd->argv[1], 6); //CHECK
	pipex->outfile = handle_file(cmd->redir->file, cmd->redir->type);

	/* TO DO - since i have the index of each command in the t_cmd struct i can make it like
	if (cmd->index == 0) CASE: FIRST COMMAND
		check file name + redir type and handle the file
	if (cmd->n_cmds > 1)
		if (cmd->index == pipex->n_cmds - 1) LAST COMMAND
			check if
		else
			check filename + redir type and handle the file
	*/

	// t_cmd *last;

	// pipex->infile = STDIN_FILENO;
	// pipex->outfile = STDOUT_FILENO;

	// if (pipex->n_cmds > 1)
	// 	last = find_cmdlst_index(cmd, pipex->n_cmds - 1);
	// else
	// 	last = cmd;
	// if (cmd->redir && last->redir->type == 6)
	// 	pipex->infile = handle_file(cmd->redir->file, cmd->redir->type);
	// if (last->redir && (last->redir->type == 7 || last->redir->type == 8))
	// 	pipex->outfile = handle_file(last->redir->file, last->redir->type);

}

// prepare exec struct for next call
void	update_exec(t_execution *pipex, t_cmd *cmd_lst)
{
	pipex->index_prev_pipe = pipex->index_pipe;
	pipex->index_pipe++;
	pipex->index_cmd++;
	cmd_lst = cmd_lst->next;
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
void	get_fd(t_execution *pipex)
{
	ft_putstr_fd("getting fds..\n", 2);
	if (pipex->index_pipe == 0)
		dup2(pipex->infile, STDIN_FILENO);
	else
		dup2(pipex->pipe_arr[pipex->index_prev_pipe][0], STDIN_FILENO);
	if (pipex->index_cmd == pipex->n_cmds - 1)
		dup2(pipex->outfile, STDOUT_FILENO);
	else
		dup2(pipex->pipe_arr[pipex->index_pipe][1], STDOUT_FILENO);
}
// close all file descriptors in the pipe FD array
void	clean_pipes(t_execution *pipex)
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
	if (pipex->infile >= 0)
		close(pipex->infile);
	if (pipex->outfile >= 0)
		close(pipex->outfile);
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
