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

// int run_heredoc(t_parse *data, t_cmd *cmd, char *delimeter, t_shell *shell)
// {
// 	if (!cmd)
// 		return (EXIT_FAILURE);
// 	if (cmd->redir && cmd->redir->type == TOKEN_HEREDOC)
// 		return (fork_heredoc(data, cmd, delimeter, shell));
// 	cmd = cmd->next;
// 	return (EXIT_SUCCESS);
// }

int run_heredoc(t_parse *data, t_redirect *redir, char *delimeter, t_shell *shell)
{
	if (!redir)
		return (EXIT_FAILURE);
	if (redir && redir->type == TOKEN_HEREDOC)
		return (fork_heredoc(data, redir, delimeter, shell));
	return (EXIT_SUCCESS);
}

int	fork_heredoc(t_parse *data, t_redirect *redir, char *delimeter, t_shell *shell)
{
	pid_t pid;
	int status;
	int ret;

	pid = fork();
	if (pid < 0)
		str_error("failed to create heredoc");
	else if (pid == 0)
		return (read_heredoc(data, redir, delimeter, shell));
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

int	read_heredoc(t_parse *data, t_redirect *redir, char *delimeter, t_shell *shell)
{
	int	fd;
	int	read_heredoc;

	switch_signal_handler(SIGINT, SIG_DFL);
	switch_signal_handler(SIGQUIT, SIG_IGN);
	read_heredoc = 1;
	fd = open(redir->file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (EXIT_FAILURE);
	while (read_heredoc == 1)
		read_heredoc = read_line_heredoc(data, fd, delimeter, shell);
	close(fd);
	exit (EXIT_SUCCESS);
}

char *replace_variables_in_heredoc(char *input, t_parse *data, t_shell *shell);

int heredoc_eof_warning(int line_number, char *delimeter, t_parse *data)
{
	data->valid_input = 0;
	ft_putstr_fd("minishell: warning: here-document at line ", STDERR_FILENO);
	ft_putnbr_fd(line_number, STDERR_FILENO);
	ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(delimeter, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
	return (0);
}

int	read_line_heredoc(t_parse *data, int fd, char *delimeter, t_shell *shell)
{
	char	*line;
	char	*expanded_line;
	size_t	len;

	line = readline("heredoc> ");
	if (!line)
		return  (heredoc_eof_warning(__LINE__, delimeter, data));
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	if (strcmp(line, delimeter) == 0)
	{
		free(line);
		return 0;
	}
	expanded_line = replace_variables_in_heredoc(line, data, shell);
	free(line);
	if (expanded_line)
	{
		write(fd, expanded_line, ft_strlen(expanded_line));
		write(fd, "\n", 1);
		free(expanded_line);
	}
	return (1);
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
