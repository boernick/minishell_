#include "../../includes/minishell.h"

char *get_env_variable(char *var_name, t_parse *data, t_shell *shell)
{
	char	*env_value;
	char	*exit_status_str;
	t_env	*env_node;
	char	*equals_sign;

    if (ft_strncmp(var_name, "?", 1) == 0)
    {
        exit_status_str = ft_itoa(data->exit);

        // Concatenate the exit status with the rest of `var_name`, if any
        char *remaining = var_name + 1; // Skip `?`
        char *result = ft_strjoin(exit_status_str, remaining);

        free(exit_status_str); // Free the temporary exit status string
        return result;
    }
	env_value = getenv(var_name);
	if (env_value)
		return ft_strdup(env_value);
	if (shell->env_lst && shell)
		env_node = get_env_lst(shell, var_name);
	if (env_node)
	{
		equals_sign = ft_strchr(env_node->content, '=');
		if (equals_sign)
			return ft_strdup(equals_sign + 1); // Return the value part only
	}
	return ft_strdup(""); // Duplicate the value for safe usage
}

char *get_pid_as_string(void)
{
    pid_t pid = getpid();
    char buffer[16]; // Temporary buffer to store PID digits
    int index = 0;

    // Convert PID to string (without for or do-while loops)
    while (pid > 0)
    {
        buffer[index] = (pid % 10) + '0'; // Get the last digit
        index++;
        pid /= 10;
    }

    // Allocate memory for the result and reverse the string
    char *pid_str = malloc(index + 1);
    if (!pid_str)
        return NULL;

    int reverse_index = 0;
    while (reverse_index < index)
    {
        pid_str[reverse_index] = buffer[index - 1 - reverse_index];
        reverse_index++;
    }
    pid_str[index] = '\0'; // Null-terminate the string
    return pid_str;
}

//itterate throug and copy input string. If a $ sign is encountertered
//it will be replaced by its appropiate enviornment variable while the
//rest of the new string remains the same.
char *replace_variables_in_string(char *input, t_parse *data, t_shell *shell)
{
    char result[1024] = {0};  // Buffer to store the final result
    int res_index = 0;        // Index for writing to result buffer
    int i = 0;                // Index for reading input
    char quote_char = 0;      // Tracks the active quote type (0 if none)

    while (input[i] != '\0')
    {
        if ((input[i] == '\'' || input[i] == '\"') && quote_char == 0)
        {
            // Start of a quoted section
            quote_char = input[i];
            i++;
        }
        else if (input[i] == quote_char)
        {
            // End of the quoted section
            quote_char = 0;
            i++;
        }
        else if (input[i] == '$' && quote_char != '\'') // Expand only outside single quotes
        {
			i++; // Skip the first `$`
			if (input[i] == '\"' || input[i] == ' ' || !input[i]) // Handle single `$` or `$ $`
			{
				result[res_index++] = '$';
				continue;
			}
			if (input[i] == '$') // Handle `$$` (expand to PID)
			{
				// Expand `$$` into PID
				char *pid_str = get_pid_as_string();
				if (pid_str)
				{
					int j = 0;
					// Copy the PID into the result buffer
					while (pid_str[j] != '\0')
					{
						result[res_index] = pid_str[j];
						res_index++;
						j++;
					}
					free(pid_str);
				}
				i++; // Skip the second `$` after handling `$$`
			}
            char var_name[256] = {0};
            int var_index = 0;

            while (ft_isalnum(input[i]) || input[i] == '_' || (var_index == 0 && input[i] == '?') )
				var_name[var_index++] = input[i++];
            var_name[var_index] = '\0';

            char *var_value = get_env_variable(var_name, data, shell);
            if (!var_value)
                var_value = "";

            strcpy(result + res_index, var_value); // Use custom strcpy if required
            res_index += ft_strlen(var_value);
            free(var_value);
        }
        else
        {
            result[res_index++] = input[i++];
        }
    }
    result[res_index] = '\0';
    return ft_strdup(result);
}

void trim_quotes(char *str)
{
    int len = 0;
    while (str[len]) // Find the length of the string
        len++;

    if (len > 1 && ((str[0] == '\'' || str[0] == '\"') && (str[len - 1] == '\'' || str[len - 1] == '\"')))
    {
        // Shift the string one character to the left and null-terminate
        for (int i = 0; i < len - 1; i++)
            str[i] = str[i + 1];
        str[len - 2] = '\0'; // Null-terminate the string
    }
}

void trim_file_quotes(char *str)
{
    int i = 0, j = 0;
    char temp[1024]; // Temporary buffer for the final string (adjust size if needed)

    while (str[i])
    {
        if (str[i] == '\'' || str[i] == '\"') // Skip quotes
        {
            i++;
            continue;
        }
        temp[j++] = str[i++]; // Copy non-quote characters to temp
    }
    temp[j] = '\0'; // Null-terminate the result

    // Copy the processed string back to the original
    i = 0;
    while (temp[i])
    {
        str[i] = temp[i];
        i++;
    }
    str[i] = '\0'; // Null-terminate the original string
}

//itterate through the list of tokens looking for WORD tokens. Call replace_variables_in_string
//on those tokens.
void replace_env_variables_in_tokens(t_token *tokens, t_parse *data, t_shell *shell)
{
    char *new_value;

	if (tokens)
    {
        new_value = replace_variables_in_string(tokens->value, data, shell);
        if (new_value != tokens->value) // Only free if they are different
        {
            free(tokens->value);
            tokens->value = new_value;
        }
        else
            free(new_value); // Free the new_value since it wasn't needed

        // Trim quotes from the first token value
        trim_quotes(tokens->value);
    }
    while (tokens)
    {
		if (tokens->type == TOKEN_FILE_ARG)
			trim_file_quotes(tokens->value);
        else if (tokens->type == TOKEN_ARG)
        {
            new_value = replace_variables_in_string(tokens->value, data, shell);
            if (new_value != tokens->value) // Only free if they are different
            {
                free(tokens->value);
                tokens->value = new_value;
            }
            else
                free(new_value); // Free the new_value since it wasn't needed

            // Trim quotes from the token value
            trim_quotes(tokens->value);
        }
        tokens = tokens->next;
    }
}
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


void expand_env_variables_heredoc(char *line) {
    char buffer[4096]; // Temporary buffer to build the expanded string
    size_t buf_idx = 0;
    size_t len = strlen(line);

    for (size_t i = 0; i < len; i++) {
        if (line[i] == '$' && i + 1 < len) {
            // Handle $$ (PID)
            if (line[i + 1] == '$') {
                buf_idx += snprintf(buffer + buf_idx, sizeof(buffer) - buf_idx, "%d", getpid());
                i++;
            }
            // Handle environment variables
            else if ((line[i + 1] >= 'a' && line[i + 1] <= 'z') ||
                     (line[i + 1] >= 'A' && line[i + 1] <= 'Z') ||
                     line[i + 1] == '_') {
                const char *start = line + i + 1;
                const char *end = start;

                // Find the end of the variable name
                while ((*end >= 'a' && *end <= 'z') ||
                       (*end >= 'A' && *end <= 'Z') ||
                       (*end >= '0' && *end <= '9') ||
                       *end == '_') {
                    end++;
                }

                char var_name[256];
                size_t var_len = end - start;
                strncpy(var_name, start, var_len);
                var_name[var_len] = '\0';

                char *var_value = getenv(var_name);
                if (var_value) {
                    buf_idx += snprintf(buffer + buf_idx, sizeof(buffer) - buf_idx, "%s", var_value);
                }

                i += var_len; // Skip past the variable name
                i--;          // Offset increment in the for loop
            } else {
                buffer[buf_idx++] = line[i]; // Copy literal `$`
            }
        } else {
            buffer[buf_idx++] = line[i]; // Copy literal character
        }

        if (buf_idx >= sizeof(buffer) - 1) {
            fprintf(stderr, "Error: Expanded line is too long.\n");
            return;
        }
    }
    buffer[buf_idx] = '\0';
    strcpy(line, buffer); // Copy the expanded result back to the original string
}

