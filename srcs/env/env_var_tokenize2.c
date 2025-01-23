/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_tokenize2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:41:58 by prichugh          #+#    #+#             */
/*   Updated: 2025/01/23 12:41:58 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_exit_status_variable(char *var_name, t_parse *data)
{
	char	*exit_status_str;
	char	*remaining;
	char	*result;

	exit_status_str = ft_itoa(data->exit);
	remaining = var_name + 1;
	result = ft_strjoin(exit_status_str, remaining);
	free(exit_status_str);
	if (!result)
		return (NULL);
	return (result);
}

char	*get_env_variable(char *var_name, t_parse *data, t_shell *shell)
{
	char	*env_value;
	t_env	*env_node;
	char	*equals_sign;

	if (ft_strncmp(var_name, "?", 1) == 0)
		return (handle_exit_status_variable(var_name, data));
	env_value = getenv(var_name);
	if (env_value)
		return (ft_strdup(env_value));
	if (shell->env_lst && shell)
		env_node = get_env_lst(shell, var_name);
	if (env_node)
	{
		equals_sign = ft_strchr(env_node->content, '=');
		if (equals_sign)
			return (ft_strdup(equals_sign + 1));
	}
	return (ft_strdup(""));
}

char	*get_pid_as_string(void)
{
	pid_t	pid;
	char	buffer[16];
	int		index;
	char	*pid_str;
	int		reverse_index;

	pid = getpid();
	index = 0;
	while (pid > 0)
	{
		buffer[index] = (pid % 10) + '0';
		index++;
		pid /= 10;
	}
	pid_str = malloc(index + 1);
	if (!pid_str)
		return (NULL);
	reverse_index = 0;
	while (reverse_index < index)
	{
		pid_str[reverse_index] = buffer[index - 1 - reverse_index];
		reverse_index++;
	}
	pid_str[index] = '\0';
	return (pid_str);
}

void	process_quote_env(char *input, t_parse *data, t_expand_var *exp)
{
	if (input[exp->i] == '\'' && !data->in_double_quote)
	{
		exp->result[exp->res_index++] = input[exp->i++];
		data->in_single_quote = !data->in_single_quote;
	}
	else if (input[exp->i] == '\"' && !data->in_single_quote)
	{
		exp->result[exp->res_index++] = input[exp->i++];
		data->in_double_quote = !data->in_double_quote;
	}
}

void	handle_quotes(char *input, int *i, char *quote_char)
{
	if ((input[*i] == '\'' || input[*i] == '\"') && *quote_char == 0)
	{
		*quote_char = input[*i];
		(*i)++;
	}
	else if (input[*i] == *quote_char)
	{
		*quote_char = 0;
		(*i)++;
	}
}
