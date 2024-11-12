/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:18:32 by nboer             #+#    #+#             */
/*   Updated: 2024/11/12 23:41:40 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// prepare exec struct for use
void	exec_init(t_execution *pipex, int argc, char **argv)
{
	if (0)
		pipex->infile = handle_file(argv[1], 0); // TO-DO BUT ONLY IF THE FIRST ARGUMENT IS A FILE!
	// if last argument should be a file PRINCE
	if (access(argv[argc-1], F_OK) >= 0)
		pipex->outfile = handle_file(argv[argc - 1], 1);
	pipex->n_cmds = 1;										// OVERWRITE FROM PRINCE'S STRUCT
	pipex->n_pipes = pipex->n_cmds - 1;							// OVERWRITE FROM PRINCES STRUCT
	pipex->index_pipe = 0;
	pipex->index_cmd = 0;
	pipex->index_prev_pipe = -1;
}
// prepare exec struct for next call
void	update_exec(t_execution *pipex)
{
	pipex->index_prev_pipe = pipex->index_pipe;
	pipex->index_pipe++;
	pipex->index_cmd++;
}

// create an array of pointers to integers to store the total amount of pipes.
void	create_pipes(t_execution *pipex)
{
	int		i;

	if (pipex->n_pipes <= 0)
		return;
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
	
	pid = fork();
	if (pid < 0)
		str_error("Error: false PID");
	return (pid);
}

//redirect STDIN to INFILE, STDOUT to OUTFILE, and between linking pipes 
void	get_fd(t_execution *pipex)
{
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
