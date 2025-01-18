/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 19:32:55 by nick              #+#    #+#             */
/*   Updated: 2025/01/09 10:31:17 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


static void	switch_signal_handler(int signal, __sighandler_t handler)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, signal);
	sa.sa_flags = 0;
	sigaction(signal, &sa, NULL);
	// if (signal == SIGINT)
	// 	write(1, "[DEBUG] Signal handler for SIGINT switched\n", 43);
	// if (signal == SIGQUIT)
	// 	write(1, "[DEBUG] Signal handler for SIGQUIT switched\n", 44);
}

int run_heredoc(t_parse *data, t_cmd *cmd, char *delimeter)
{
	//t_cmd *cmd;

	//cmd = data->cmd;
	if (!cmd)
		return (EXIT_FAILURE);
	//while (cmd)
	//{
		if (cmd->redir && cmd->redir->type == TOKEN_HEREDOC)
			return (fork_heredoc(data, cmd, delimeter));
		cmd = cmd->next;
	//}
	return (EXIT_SUCCESS);
}

int	fork_heredoc(t_parse *data, t_cmd *cmd, char *delimeter)
{
	pid_t pid;
	int status;
	int ret;

	pid = fork();
	if (pid < 0)
		str_error("failed to create heredoc");
	else if (pid == 0)
		return (read_heredoc(data, cmd, delimeter));
	else
	{
		switch_signal_handler(SIGQUIT, SIG_IGN);
		if (waitpid(pid, &status, 0) == -1)
			str_error("Error waiting for for heredoc");
		if (WIFEXITED(status))
			ret = WEXITSTATUS(status);
		else
			ret = WTERMSIG(status) + 128;
		switch_signal_handler(SIGQUIT, handle_sigquit);
	}
	return (ret);
}

int	read_heredoc(t_parse *data, t_cmd *cmd, char *delimeter)
{
	int	fd;
	int	read_heredoc;

	switch_signal_handler(SIGINT, SIG_DFL);
	switch_signal_handler(SIGQUIT, SIG_IGN);
	read_heredoc = 1;
	fd = open(cmd->redir->file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (EXIT_FAILURE);
	while (read_heredoc == 1)
		read_heredoc = read_line_heredoc(data, fd, delimeter);
	close(fd);
	exit (EXIT_SUCCESS);
}

char *replace_variables_in_heredoc(char *input, t_parse *data);

int read_line_heredoc(t_parse *data, int fd, char *delimeter)
{

	//printf("delimiter: %s\n", data->cmd->redir->delimiter);
	char *line;
			line = readline("heredoc> ");
		if (!line)
		{
			write(STDOUT_FILENO, "\n", 1);
			fprintf(stderr, "minishell: warning: here-document at line %d delimited by end-of-file (wanted '%s')\n", __LINE__, delimeter);
			return 130; // Return NULL to signal failure
		}
		// Remove trailing newline for comparison
		size_t len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (strcmp(line, delimeter) == 0)
		{
			free(line);
			//printf("Debug: Delimiter `%s` found, ending heredoc\n", data->cmd->redir->delimiter);
			return 0; // Exit heredoc loop
		}
		// Write line to file
		//printf("Debug: Writing line to `%s`: %s\n", file, line);
		line = replace_variables_in_heredoc(line, data);
		//printf("line after: %s\n", line);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1); // Add newline
		free(line);
			return 1; // Continue the heredoc loop
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
	}
}
