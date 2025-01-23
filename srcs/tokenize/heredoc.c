/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 14:23:55 by prichugh          #+#    #+#             */
/*   Updated: 2024/12/03 14:23:55 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t g_interrupted = false;

// static void	switch_signal_handler(int signal, __sighandler_t handler)
// {
// 	struct sigaction	sa;


// 	sa.sa_handler = handler;
// 	sigemptyset(&sa.sa_mask);
// 	sigaddset(&sa.sa_mask, signal);
// 	sa.sa_flags = 0;
// 	sigaction(signal, &sa, NULL);
// 	if (signal == SIGINT)
// 		printf("[DEBUG] Signal handler for SIGINT switched\n");
// 	if (signal == SIGQUIT)
// 		printf("[DEBUG] Signal handler for SIGQUIT switched\n");
// }

// Custom SIGINT handler for heredoc
void heredoc_sigint_handler(int signum)
{
	(void)signum;
	g_interrupted = true;
	write(STDOUT_FILENO, "\n", 1);
}

// Set custom signal handlers for heredoc
void	setup_heredoc_signals(struct sigaction *prev_sigint, struct sigaction *prev_sigquit)
{
	struct sigaction	sa;

	sigaction(SIGINT, NULL, prev_sigint);
	sigaction(SIGQUIT, NULL, prev_sigquit);
	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

// int	get_tempfile_name(char *tempfile)
// {
// 	int	i;

// 	ft_memcpy(tempfile, ".tmphered000", 13);
// 	i = 1;
// 	while (i < 1000)
//
// 		tempfile[11] = '0' + (i % 10);
// 		tempfile[10] = '0' + ((i / 10) % 10);
// 		tempfile[9] = '0' + (i / 100);
// 		if (access(tempfile, F_OK) != 0)
// 			return (0);
// 		i++;
// 	}
// 	return (-1);
// }

char *generate_temp_filename(void)
{
	static int	counter = 0;
	char		*filename;

	filename = malloc(64);
	if (!filename)
		return (NULL);
	sprintf(filename, "/tmp/heredoc_temp_%d.txt", counter++);
	return (filename);
}

// void	expand_env_variables_heredoc(char *line)
// {
// 	char buffer[4096];
// 	size_t buf_idx = 0;
// 	size_t len = strlen(line);

// 	for (size_t i = 0; i < len; i++) {
// 		if (line[i] == '$' && i + 1 < len) {
// 			if (line[i + 1] == '$')
// 			{
// 				buf_idx += snprintf(buffer + buf_idx, sizeof(buffer) - buf_idx, "%d", getpid());
// 				i++;
// 			}
// 			else if ((line[i + 1] >= 'a' && line[i + 1] <= 'z') ||
// 						(line[i + 1] >= 'A' && line[i + 1] <= 'Z') ||
// 						line[i + 1] == '_')
// 						{
// 				const char *start = line + i + 1;
// 				const char *end = start;
// 				while ((*end >= 'a' && *end <= 'z') ||
// 						(*end >= 'A' && *end <= 'Z') ||
// 						(*end >= '0' && *end <= '9') ||
// 						*end == '_') {
// 					end++;
// 				}
// 				char var_name[256];
// 				size_t var_len = end - start;
// 				strncpy(var_name, start, var_len);
// 				var_name[var_len] = '\0';
// 				char *var_value = getenv(var_name);
// 				if (var_value)
// 					buf_idx += snprintf(buffer + buf_idx, sizeof(buffer) - buf_idx, "%s", var_value);
// 				i += var_len;
// 				i--;
// 			}
// 			else
// 				buffer[buf_idx++] = line[i];
// 		}
// 		else
// 			buffer[buf_idx++] = line[i];
// 		if (buf_idx >= sizeof(buffer) - 1)
// 		{
// 			fprintf(stderr, "Error: Expanded line is too long.\n");
// 			return ;
// 		}
// 	}
// 	buffer[buf_idx] = '\0';
// 	strcpy(line, buffer);
// }

char	*create_heredoc(void)
{
	char	*temp_file;
	int		fd;

	temp_file = generate_temp_filename();
	if (!temp_file)
	{
		ft_putstr_fd("Failed to allocate memory for temp file name",
			STDERR_FILENO);
		return (NULL);
	}
	fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("Failed to create heredoc file", STDERR_FILENO);
		free(temp_file);
		return (NULL);
	}
	return (temp_file);
}

















char *expand_pid(void)
{
	char *pid_str;

	pid_str = get_pid_as_string();
	if (pid_str)
		return (pid_str);
	else
		return ("");
}

void handle_dollar(char *input, int *i, char *result, int *res_index)
{
	char	*pid_str;

	if (input[*i + 1] == '$')
	{
		pid_str = expand_pid();
		strcpy(result + *res_index, pid_str);
		*res_index += ft_strlen(pid_str);
		free(pid_str);
		(*i)++;
	}
	else
	{
		result[*res_index] = '$';
		(*res_index)++;
	}
}
void	expand_var(char *input, int *i, char *result, int *res_index, t_parse *data, t_shell *shell)
{
	char	var_name[256];
	int		var_index;
	char	*var_value;

	var_index = 0;
	ft_memset(var_name, 0, sizeof(var_name));
	while (ft_isalnum(input[*i]) || input[*i] == '_'
		|| (*i == 0 && input[*i] == '?'))
		var_name[var_index++] = input[(*i)++];
	var_name[var_index] = '\0';
	var_value = get_env_variable(var_name, data, shell);
	if (!var_value)
		var_value = "";
	ft_strlcpy(result + *res_index, var_value, ft_strlen(var_value) + 1);
	*res_index += ft_strlen(var_value);
}


void	init_rep_var_heredoc(int *i, int *res_index, char *results)
{
	*i = 0;
	*res_index = 0;
	ft_memset(results, 0, sizeof(results));
}
// char	*replace_variables_in_heredoc(char *input, t_parse *data,
// 			t_shell *shell)
// {
// 	char	result[1024];
// 	int		res_index;
// 	int		i;

// 	init_rep_var_heredoc(&i, &res_index, result);
// 	while (input[i] != '\0')
// 	{
// 		if (input[i] == '$')
// 		{
// 			i++;
// 			if (input[i] == ' ' || !input[i])
// 			{
// 				result[res_index++] = '$';
// 				//continue ;
// 			}
// 			if (input[i] == '$')
// 				handle_dollar(input, &i, result, &res_index);
// 			else
// 				expand_var(input, &i, result, &res_index, data, shell);
// 		}
// 		else
// 			result[res_index++] = input[i++];
// 	}
// 	result[res_index] = '\0';
// 	return (ft_strdup(result));
// }

char	*replace_variables_in_heredoc(char *input, t_parse *data, t_shell *shell)
{

	t_expand_var	*exp;
	char			*ret;

	exp = malloc(sizeof(t_expand_var));
	init_expand_var(exp, input);
	while (input[exp->i] != '\0')
	{
		if (input[exp->i] == '$' && !data->in_single_quote)
			handle_variable(exp, data, shell);
		else
		{
			exp->result[exp->res_index++] = input[exp->i++];
			reset_expand_var(exp);
		}
	}
	exp->result[exp->res_index] = '\0';
	ret = ft_strdup(exp->result);
	free(exp);
	return (ret);
}








// char	*replace_variables_in_heredoc(char *input, t_parse *data, t_shell *shell)
// {
// 	char result[1024] = {0};  // Buffer to store the final result
// 	int res_index = 0;        // Index for writing to result buffer
// 	int i = 0;                // Index for reading input
// 	while (input[i] != '\0')
// 	{
// 		if (input[i] == '$') // Expand only outside single quotes
// 		{
// 			i++; // Skip the `$`
// 			if (input[i] == ' ' || !input[i]) // Handle single `$` or `$ $`
// 			{
// 				result[res_index++] = '$';
// 				continue;
// 			}
// 			if (input[i] == '$') // Handle `$$` (expand to PID)
// 			{
// 				// Expand `$$` into PID
// 				char *pid_str = get_pid_as_string();
// 				if (pid_str)
// 				{
// 					int j = 0;
// 					// Copy the PID into the result buffer
// 					while (pid_str[j] != '\0')
// 					{
// 						result[res_index] = pid_str[j];
// 						res_index++;
// 						j++;
// 					}
// 					free(pid_str);
// 				}
// 				i++; // Skip the second `$` after handling `$$`
// 			}
// 			char var_name[256] = {0};
// 			int var_index = 0;
// 			while (ft_isalnum(input[i]) || input[i] == '_' || (var_index == 0 && input[i] == '?'))
// 				var_name[var_index++] = input[i++];
// 			var_name[var_index] = '\0';
// 			char *var_value = get_env_variable(var_name, data, shell);
// 			if (!var_value)
// 				var_value = "";
// 			strcpy(result + res_index, var_value); // Use custom strcpy if required
// 			res_index += ft_strlen(var_value);
// 			free(var_value);
// 		}
// 		else
// 			result[res_index++] = input[i++];
// 	}
// 	result[res_index] = '\0';
// 	return ft_strdup(result);
// }
