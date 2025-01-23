#include "../../includes/minishell.h"

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
