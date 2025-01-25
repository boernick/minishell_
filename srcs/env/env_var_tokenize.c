/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_tokenize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:42:33 by prichugh          #+#    #+#             */
/*   Updated: 2025/01/23 12:42:33 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_pid_var(char *input, int *i, char *result, int *res_index)
{
	int		j;
	char	*pid_str;

	(*i)++;
	if (input[*i] == '\"' || input[*i] == ' ' || input[*i] == '\0')
	{
		result[(*res_index)++] = '$';
		return ;
	}
	if (input[*i] == '$')
	{
		pid_str = get_pid_as_string();
		if (pid_str)
		{
			j = 0;
			while (pid_str[j] != '\0')
				result[(*res_index)++] = pid_str[j++];
			free(pid_str);
		}
		(*i)++;
		return ;
	}
}

char	*handle_variable(t_expand_var *exp, t_parse *data, t_shell *shell)
{
	char		*var_val;

	handle_pid_var(exp->input, &(exp->i), exp->result, &(exp->res_index));
	while (ft_isalnum(exp->input[exp->i])
		|| exp->input[exp->i] == '_'
		|| (exp->var_index == 0 && exp->input[exp->i] == '?'))
		exp->var_name[exp->var_index++] = exp->input[(exp->i)++];
	exp->var_name[exp->var_index] = '\0';
	var_val = get_env_variable(exp->var_name, data, shell);
	ft_strlcpy(exp->result + exp->res_index, var_val, ft_strlen(var_val) + 1);
	exp->res_index += ft_strlen(var_val);
	free(var_val);
	return (exp->result);
}

char	*replace_variables_in_string(char *input, t_parse *data, t_shell *shell)
{
	t_expand_var	*exp;
	char			*ret;

	exp = malloc(sizeof(t_expand_var));
	init_expand_var(exp, input);
	data->in_double_quote = 0;
	data->in_single_quote = 0;
	while (input[exp->i] != '\0')
	{
		if ((input[exp->i] == '\'' && !data->in_double_quote)
			|| (input[exp->i] == '\"' && !data->in_single_quote))
			process_quote_env(input, data, exp);
		else if (input[exp->i] == '$' && !data->in_single_quote)
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

void	replace_single_token_env_var(t_token *token,
	t_parse *data, t_shell *shell)
{
	char	*new_value;

	new_value = replace_variables_in_string(token->value, data, shell);
	if (new_value != token->value)
	{
		free(token->value);
		token->value = new_value;
	}
	else
		free(new_value);
	trim_quotes(token->value, data);
}

void	replace_env_variables_in_tokens(t_token *tokens, t_parse *data,
		t_shell *shell)
{
	if (tokens)
		replace_single_token_env_var(tokens, data, shell);
	while (tokens)
	{
		if (tokens->type == TOKEN_FILE_ARG)
			trim_file_quotes(tokens->value);
		else if (tokens->type == TOKEN_ARG)
			replace_single_token_env_var(tokens, data, shell);
		tokens = tokens->next;
	}
}
