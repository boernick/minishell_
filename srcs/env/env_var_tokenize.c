#include "../../includes/minishell.h"

char *get_env_variable(char *var_name, t_parse *data)
{
	char *exit_status_str;
	char *env_value;

	if (strcmp(var_name, "?") == 0) // ADD MY OWN STRCMP
	{
		exit_status_str = malloc(12);//set to correct exit status
		exit_status_str = ft_itoa(data->last_exit_status);
		return exit_status_str;
	}
	env_value = getenv(var_name);
	if (!env_value)
		return (ft_strdup(""));
	return (ft_strdup(env_value));
}

//itterate throug and copy input string. If a $ sign is encountertered
//it will be replaced by its appropiate enviornment variable while the
//rest of the new string remains the same.
char	*replace_variables_in_string(char *input, t_parse *data)
{
	char result[1024] = {0};  // Buffer to store the final result
	int res_index;
	int i;
	int var_index;

	res_index = 0;
	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '$')
		{
			i++;
			char var_name[256] = {0};
			var_index = 0;
			while (ft_isalnum(input[i]) || input[i] == '_')
				var_name[var_index++] = input[i++];
			var_name[var_index] = '\0';
			char *var_value = get_env_variable(var_name, data);
			if (!var_value)
				var_value = "";
			strcpy(result + res_index, var_value); //ADD MY OWN STRCPY!!
			res_index += ft_strlen(var_value);
			free(var_value);
		}
			else
				result[res_index++] = input[i++];
	}
	result[res_index] = '\0';
	return ft_strdup(result);
}

//itterate through the list of tokens looking for WORD tokens. Call replace_variables_in_string
//on those tokens.
void replace_env_variables_in_tokens(t_token *tokens, t_parse *data)
{
	char *new_value;

	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			new_value = replace_variables_in_string(tokens->value, data);
			if (new_value != tokens->value) // Only free if they are different
			{
				free(tokens->value);
				tokens->value = new_value;
			}
			else
				free(new_value); // Free the new_value since it wasn't needed
		}
		tokens = tokens->next;
	}
}
