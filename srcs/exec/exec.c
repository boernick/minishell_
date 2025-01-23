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
int	setup_redirections(t_cmd *cmd)
{
	t_redirect	*rdir;

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
			return (EXIT_FAILURE);
		rdir = rdir->next;
	}
	return (EXIT_SUCCESS);
}

// reset FDs after execution process.
void	reset_fds(t_execution *pipex)
{
	if (pipex->infile >= 0)
	{
		close(pipex->infile);
		pipex->infile = -2;
	}
	if (pipex->outfile >= 0)
	{
		close(pipex->outfile);
		pipex->outfile = -2;
	}
	dup2(pipex->start_in, STDIN_FILENO);
	dup2(pipex->start_out, STDOUT_FILENO);
}

// prepare exec struct for next call.
void	update_exec(t_execution *pipex)
{
	close_fd_in_out(pipex->cmd);
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
