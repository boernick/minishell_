/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 19:32:55 by nick              #+#    #+#             */
/*   Updated: 2025/01/06 23:07:34 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int run_heredoc(t_execution *pipex, t_shell *shell)
{
	t_cmd *cmd;

	cmd = pipex->cmd;
	if (!cmd)
		return (EXIT_FAILURE);
	while (cmd)
	{
		if (cmd->redir && cmd->redir->type == TOKEN_HEREDOC)
			fork_heredoc(shell, cmd);
		cmd = cmd->next;
	}
	return 0;
}

int	fork_heredoc(t_shell *shell, t_cmd *cmd)
{
	pid_t pid;
	int status;
	
	pid = fork();
	if (pid < 0)
		str_error("failed to create heredoc");
	else if (pid == 0)
		return (read_heredoc(cmd->redir->file));
	else
	{
		//handle signals
		if (waitpid(pid, &status, 0) == -1)
			str_error("Error waiting for for heredoc");
		if (WIFEXITED(status))
			shell->last_exit = WEXITSTATUS(status);
		else
			shell->last_exit = WTERMSIG(status) + 128;
		//handle signals.
	}
}

int	read_heredoc(t_cmd *cmd)
{
	int	fd;
	int	read_heredoc;
	
	read_heredoc = 1;
	//signal handling
	fd = open(cmd->redir->file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (EXIT_FAILURE);
	while (read_heredoc)
		read_heredoc = read_line_heredoc(cmd->redir->file, fd);
	close(fd);
	exit (0);
	
}

int	read_line_heredoc(char *file, int fd)
{
	return (EXIT_SUCCESS);
}

void	cleanup_heredoc(t_cmd *cmd_p)
{
	t_cmd	*cmd;
	
	cmd = cmd_p;

	while (cmd)
	{
		if (cmd->redir->type == TOKEN_HEREDOC)
			if (cmd->redir->file != NULL)
				unlink(cmd->redir->file);
		cmd = cmd->next;
	}
}