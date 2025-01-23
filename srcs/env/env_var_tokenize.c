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
		return (env_value);
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
		return NULL;
	reverse_index = 0;
	while (reverse_index < index)
	{
		pid_str[reverse_index] = buffer[index - 1 - reverse_index];
		reverse_index++;
	}
	pid_str[index] = '\0';
	return (pid_str);
}

int find_first_quote(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return i;
	}
	return -1;
}

int	find_matching_quote(char *str, char quote_char, int start)
{
	int	i;

	i = start + 1;
	while (str[i])
	{
		if (str[i] == quote_char)
			return i;
		i++;
	}
	return (-1);
}

void trim_and_shift(char *str, int start, int end, int len)
{
	int	i;

	i = start;
	while (i < len)
	{
		str[i] = str[i + 1];
		i++;
	}
	len--;
	i = end - 1;
	while (i < len)
	{
		str[i] = str[i + 1];
		i++;
	}
	str[len - 1] = '\0';
}

void	trim_quotes(char *str)
{
	int		len = 0;
	int		start;
	int		end;
	char	quote_char;

	len = 0;
	start = -1;
	end = -1;
	while (str[len])
		len++;
	start = find_first_quote(str);
	if (start == -1)
		return ;
	quote_char = str[start];
	end = find_matching_quote(str, quote_char, start);
	if (end == -1 || end <= start)
		return ;
	trim_and_shift(str, start, end, len);
}

void	trim_file_quotes(char *str)
{
	int		i;
	int		j;
	char	temp[1024];

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			i++;
			continue ;
		}
		temp[j++] = str[i++];
	}
	temp[j] = '\0';
	i = 0;
	while (temp[i])
	{
		str[i] = temp[i];
		i++;
	}
	str[i] = '\0';
}

//ORIGINAL
// //itterate through the list of tokens looking for WORD tokens. Call replace_variables_in_string
// //on those tokens.
// void replace_env_variables_in_tokens(t_token *tokens, t_parse *data, t_shell *shell)
// {
//     char *new_value;

// 	if (tokens)
//     {
//         new_value = replace_variables_in_string(tokens->value, data, shell);
//         if (new_value != tokens->value) // Only free if they are different
//         {
//             free(tokens->value);
//             tokens->value = new_value;
//         }
//         else
//             free(new_value); // Free the new_value since it wasn't needed

//         // Trim quotes from the first token value
//         trim_quotes(tokens->value);
//     }
//     while (tokens)
//     {
// 		if (tokens->type == TOKEN_FILE_ARG)
// 			trim_file_quotes(tokens->value);
//         else if (tokens->type == TOKEN_ARG)
//         {
//             new_value = replace_variables_in_string(tokens->value, data, shell);
//             if (new_value != tokens->value) // Only free if they are different
//             {
//                 free(tokens->value);
//                 tokens->value = new_value;
//             }
//             else
//                 free(new_value); // Free the new_value since it wasn't needed

//             // Trim quotes from the token value
//             trim_quotes(tokens->value);
//         }
//         tokens = tokens->next;
//     }
// }
// #include "../../includes/minishell.h"

// char *get_env_variable(char *var_name, t_parse *data)
// {
//     char *env_value;
//     char *exit_status_str;

//     if (strcmp(var_name, "?") == 0) // Replace with your custom strcmp
//     {
//         exit_status_str = ft_itoa(data->exit);
//         return (exit_status_str); // Return the exit status string
//     }
//     env_value = getenv(var_name);
//     if (!env_value)
//         return ft_strdup(""); // Return an empty string if variable doesn't exist
//     return ft_strdup(env_value); // Duplicate the value for safe usage
// }

// char *replace_variables_in_string(char *input, t_parse *data)
// {
//     char result[1024] = {0};  // Buffer to store the final result
//     int res_index = 0;        // Index for writing to result buffer
//     int i = 0;                // Index for reading input
//     char quote_char = 0;      // Tracks the active quote type (0 if none)

//     while (input[i] != '\0')
//     {
//         if ((input[i] == '\'' || input[i] == '\"') && quote_char == 0)
//         {
//             // Start of a quoted section
//             quote_char = input[i];
//             result[res_index++] = input[i++]; // Keep the quote in the result
//         }
//         else if (input[i] == quote_char)
//         {
//             // End of the quoted section
//             result[res_index++] = input[i++];
//             quote_char = 0;
//         }
//         else if (input[i] == '$' && quote_char != '\'') // Expand only outside single quotes
//         {
//             i++; // Skip the `$`
//             if (input[i] == '\0' || input[i] == ' ') // Handle single `$` or `$ $`
//             {
//                 result[res_index++] = '$';
//                 continue;
//             }

//             char var_name[256] = {0};
//             int var_index = 0;

//             while (ft_isalnum(input[i]) || input[i] == '_' || (var_index == 0 && input[i] == '?'))
//                 var_name[var_index++] = input[i++];
//             var_name[var_index] = '\0';

//             if (var_name[0] == '\0') // Handle case of `$` with no valid variable name
//             {
//                 result[res_index++] = '$';
//             }
//             else
//             {
//                 char *var_value = get_env_variable(var_name, data);
//                 if (!var_value)
//                     var_value = "";
//                 strcpy(result + res_index, var_value); // Use custom strcpy if required
//                 res_index += ft_strlen(var_value);
//                 free(var_value);
//             }
//         }
//         else
//         {
//             result[res_index++] = input[i++];
//         }
//     }
//     result[res_index] = '\0';
//     return ft_strdup(result);
// }

// void trim_quotes(char *str)
// {
//     char *src = str;
//     char *dest = str;

//     while (*src)
//     {
//         if (*src != '\'' && *src != '\"')
//             *dest++ = *src;
//         src++;
//     }
//     *dest = '\0'; // Null-terminate the trimmed string
// }

// void replace_env_variables_in_tokens(t_token *tokens, t_parse *data)
// {
//     char *new_value;

//     while (tokens)
//     {
//         if (tokens->type == TOKEN_ARG)
//         {
//             new_value = replace_variables_in_string(tokens->value, data);
//             if (new_value != tokens->value) // Only free if they are different
//             {
//                 free(tokens->value);
//                 tokens->value = new_value;
//             }
//             else
//                 free(new_value); // Free the new_value since it wasn't needed

//             // Trim quotes from the token value
//             trim_quotes(tokens->value);
//         }
//         tokens = tokens->next;
//     }
// }


// void expand_env_variables_heredoc(char *line)
// {
// 	char buffer[4096]; // Temporary buffer to build the expanded string
// 	size_t buf_idx = 0;
// 	size_t len = strlen(line);

// 	for (size_t i = 0; i < len; i++) {
// 		if (line[i] == '$' && i + 1 < len) {
// 			// Handle $$ (PID)
// 			if (line[i + 1] == '$') {
// 				buf_idx += snprintf(buffer + buf_idx, sizeof(buffer) - buf_idx, "%d", getpid());
// 				i++;
// 			}
// 			// Handle environment variables
// 			else if ((line[i + 1] >= 'a' && line[i + 1] <= 'z') ||
// 						(line[i + 1] >= 'A' && line[i + 1] <= 'Z') ||
// 						line[i + 1] == '_') {
// 				const char *start = line + i + 1;
// 				const char *end = start;

// 				// Find the end of the variable name
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
// 				if (var_value) {
// 					buf_idx += snprintf(buffer + buf_idx, sizeof(buffer) - buf_idx, "%s", var_value);
// 				}

// 				i += var_len; // Skip past the variable name
// 				i--;          // Offset increment in the for loop
// 			} else {
// 				buffer[buf_idx++] = line[i]; // Copy literal `$`
// 			}
// 		} else {
// 			buffer[buf_idx++] = line[i]; // Copy literal character
// 		}

// 		if (buf_idx >= sizeof(buffer) - 1) {
// 			fprintf(stderr, "Error: Expanded line is too long.\n");
// 			return;
// 		}
// 	}
// 	buffer[buf_idx] = '\0';
// 	strcpy(line, buffer); // Copy the expanded result back to the original string
// 	}

///////////////////////////////////////////////////////
void process_quote_env(char *input, t_parse *data, t_expand_var *exp)
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

void handle_quotes(char *input, int *i, char *quote_char)
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
//void	process_quote(char *input, t_parse *data, char quote_type, int *i);
void init_expand_var(t_expand_var *expand, char *input)
{
	if (!expand)
	{
		malloc_error(sizeof(t_expand_var));
		exit (EXIT_FAILURE);
	}
	expand->i = 0;
	expand->res_index = 0;
	expand->var_index = 0;
	ft_memset(expand->result, 0, sizeof(expand->result));
	ft_memset(expand->var_name, 0, sizeof(expand->var_name));
	expand->input = input;
}


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
void reset_expand_var(t_expand_var *expand)
{
	expand->var_index = 0;
	ft_memset(expand->var_name, 0, sizeof(expand->var_name));
}

char *handle_variable(t_expand_var *exp, t_parse *data, t_shell *shell)
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
	return (exp->result);
}

char *replace_variables_in_string(char *input, t_parse *data, t_shell *shell)
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


// void	handle_variable_replacement(t_parse *data, t_shell *shell, char *input, int *res_index, char *result, int *i)
// {
// 	char	*var_value;
// 	int		var_index;

// 	char var_name[256] = {0};
// 	var_index = 0;
// 	handle_pid_replacement(input, i, result, res_index);
// 	while (ft_isalnum(input[*i]) || input[*i] == '_'
// 		|| (var_index == 0 && input[*i] == '?') )
// 		var_name[var_index++] = input[*i++];
// 	var_name[var_index] = '\0';
// 	var_value = get_env_variable(var_name, data, shell);
// 	ft_strlcpy(result + *res_index, var_value, ft_strlen(var_value) + 1);
// 	*res_index += ft_strlen(var_value);
// 	free(var_value);
// }

// char *replace_variables_in_string(char *input, t_parse *data, t_shell *shell)
// {
// 	char	result[1024];
// 	int		res_index;
// 	int		i;

// 	res_index = 0;
// 	i = 0;
// 	data->in_double_quote = 0;
// 	data->in_single_quote = 0;
// 	ft_memset(result, 0, sizeof(result));
// 	while (input[i] != '\0')
// 	{
// 		if ((input[i] == '\'' && !data->in_double_quote) ||
// 			(input[i] == '\"' && !data->in_single_quote))
// 			process_quote(input, data, &i);
// 		else if (input[i] == '$' && !data->in_single_quote)
// 		{

// 			handle_variable_replacement(data, shell, input, &res_index, result, &i);
// 			// char var_name[256] = {0};
// 			// var_index = 0;
// 			// while (ft_isalnum(input[i]) || input[i] == '_'
// 			// 	|| (var_index == 0 && input[i] == '?') )
// 			// 	var_name[var_index++] = input[i++];
// 			// var_name[var_index] = '\0';
// 			// var_value = get_env_variable(var_name, data, shell);
// 			// ft_strlcpy(result + res_index, var_value, ft_strlen(var_value) + 1);
// 			// res_index += ft_strlen(var_value);
// 			// free(var_value);
// 		}
// 		else
// 			result[res_index++] = input[i++];
// 	}
// 	result[res_index] = '\0';
// 	return	(ft_strdup(result));
// }

//// ////
void replace_single_token_env_var(t_token *token, t_parse *data, t_shell *shell)
{
	char *new_value;

	new_value = replace_variables_in_string(token->value, data, shell);
	if (new_value != token->value)
	{
		free(token->value);
		token->value = new_value;
	}
	else
		free(new_value);
	trim_quotes(token->value);
}

void replace_env_variables_in_tokens(t_token *tokens, t_parse *data,
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
