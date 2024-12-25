#include "../../includes/minishell.h"

char *get_env_variable(char *var_name, t_parse *data)
{
	char *env_value;
	char *exit_status_str;

    if (strcmp(var_name, "?") == 0) // Replace with your custom strcmp
    {
        exit_status_str = ft_itoa(data->exit);
        return (exit_status_str); // Return the exit status string
    }
    env_value = getenv(var_name);
    if (!env_value)
        return ft_strdup(""); // Return an empty string if variable doesn't exist
    return ft_strdup(env_value); // Duplicate the value for safe usage
}

//itterate throug and copy input string. If a $ sign is encountertered
//it will be replaced by its appropiate enviornment variable while the
//rest of the new string remains the same.
char *replace_variables_in_string(char *input, t_parse *data)
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
            i++; // Skip the `$`
			if (input[i] == '\0' || input[i] == ' ') // Handle single `$` or `$ $`
			{
				result[res_index++] = '$';
				continue;
			}
            char var_name[256] = {0};
            int var_index = 0;

            while (ft_isalnum(input[i]) || input[i] == '_' || (var_index == 0 && input[i] == '?'))
                var_name[var_index++] = input[i++];
            var_name[var_index] = '\0';

            char *var_value = get_env_variable(var_name, data);
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
    char *src = str;
    char *dest = str;

    while (*src)
    {
        if (*src != '\'' && *src != '\"')
            *dest++ = *src;
        src++;
    }
    *dest = '\0'; // Null-terminate the trimmed string
}

//itterate through the list of tokens looking for WORD tokens. Call replace_variables_in_string
//on those tokens.
void replace_env_variables_in_tokens(t_token *tokens, t_parse *data)
{
    char *new_value;

    while (tokens)
    {
        if (tokens->type == TOKEN_ARG)
        {
            new_value = replace_variables_in_string(tokens->value, data);
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
