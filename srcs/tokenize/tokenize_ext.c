#include "../../includes/minishell.h"

//WORKING
//Go trough the list of tokens and correcltly split up WORD_TOKENS
//to be arguments or commands
// void	classify_token_types(t_data *data)
// {
// 	char *new_value;

// 	new_value = data->head;

// 	while (data->head->next)
// 	{
// 		if (data->head->type == TOKEN_WORD)
// 		{
// 			new_value = replace_variables_in_string(tokens->value, data);
// 			if (new_value != tokens->value) // Only free if they are different
// 			{
// 				free(tokens->value);
// 				tokens->value = new_value;
// 			}
// 			else
// 				free(new_value); // Free the new_value since it wasn't needed
// 		}
// 		tokens = tokens->next;
// 	}
// }
