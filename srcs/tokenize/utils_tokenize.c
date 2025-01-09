/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:30:25 by prichugh          #+#    #+#             */
/*   Updated: 2024/11/13 18:30:25 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Trims leading and trailing whitespace from a given string.
// Returns a pointer to the trimmed portion of the original string.
char *trim_whitespace(char *str)
{
	int	start;
	int	end;

	start = 0;
	end = ft_strlen(str) - 1;
	while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n')
		start++;
	while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n'))
		end--;
	str[end + 1] = '\0';
	return (&str[start]);
}

// FOR TESTING ONLY: prints each token in the linked list along with its type.
void print_tokens(t_token *token_list)
{
    t_token *current = token_list;

    while (current != NULL)
	{
        printf("Token: %s, Type: ", current->value);

        switch (current->type) {
            case TOKEN_CMD:				printf("COMMAND");         break;
            case TOKEN_FLAG_ARG:		printf("FLAG ARGUMENT");   break;
            case TOKEN_ARG:				printf("GENERAL ARGUMENT"); break;
            case TOKEN_FILE_ARG:		printf("FILE ARGUMENT");   break;
            case TOKEN_PIPE:			printf("PIPE");            break;
            case TOKEN_HEREDOC:			printf("HEREDOC");         break;
            case TOKEN_REDIR_APPEND:	printf("REDIR APPEND");    break;
            case TOKEN_REDIR_IN:		printf("REDIR IN");        break;
            case TOKEN_REDIR_OUT:		printf("REDIR OUT");       break;
            default:					printf("UNKNOWN");         break;
        }
        printf("\n");

        current = current->next;
    }
}

// void	free_envlst(t_env *lst)
// {
// 	(void)lst;
// }
// Frees all nodes in a linked list of tokens
void free_tokens(t_token *head)
{
	t_token *temp;

	while (head)
	{
		temp = head->next;
		if (head->value)
			free(head->value);
		free(head);
		head = temp;
	}
}
//Make preliminary checks on input. Ensure operators (|, <<, >) are not next to
//eachother. That input doesn't start or end with an operator, that operator are
//not next to eachother and check for missing args after operators. Current Error
//msgs are incoorect for testing purposes but need to be updated to match bash!!
//

// int	validate_input(t_token *tokens, t_parse *data)
// {
// 	t_token *current = tokens;
// 	int token_count = 0;

// 	// Check for empty input
// 	if (!current)
// 		return (1); // Empty input is valid (no tokens)
// 	// Check if the input starts with a |
// 	if (current->value[0] == '|')
// 	{
// 		printf("minishell: syntax error near unexpected token '%s'\n", current->value);
// 		return (0);
// 	}

// 	while (current)
// 	{
// 		token_count++;

// 		// Check for invalid sequences of operators
// 		if ((current->value[0] == '<' || current->value[0] == '>' || current->value[0] == '|'))
// 		{
// 			if (current->next && (current->next->value[0] == '<' || current->next->value[0] == '>' || current->next->value[0] == '|'))
// 			{
// 				if (!(current->value[0] == '>' && current->next->value[0] == '>') &&
// 					!(current->value[0] == '<' && current->next->value[0] == '<'))
// 				{
// 					printf("minishell: syntax error near unexpected token '%s'\n", current->next->value);
// 					return (0);
// 				}
// 			}
// 		}
// 		// Check for missing arguments after redirection operators
// 		if (current->value[0] == '<' || current->value[0] == '>')
// 		{
// 			if (!current->next || current->next->value[0] == '|' || current->next->value[0] == '<' || current->next->value[0] == '>')
// 			{
// 				printf("minishell: syntax error near unexpected token '%s'\n", current->next ? current->next->value : "newline");
// 				return (0);
// 			}
// 		}
// 		// Check for heredoc (`<<`) without a delimiter
// 		if (current->type == TOKEN_HEREDOC)
// 		{
// 			if (!current->next || current->next->type != TOKEN_WORD)
// 			{
// 				printf("minishell: syntax error near unexpected token '%s'\n", current->next ? current->next->value : "newline");
// 				return (0);
// 			}
// 		}
// 		current = current->next;
// 	}
// 	// Check if the input ends with an operator
// 	if (tokens)
// 	{
// 		t_token *last = tokens;
// 		while (last && last->next)
// 			last = last->next;
// 		if (last->value[0] == '|' || last->value[0] == '<' || last->value[0] == '>')
// 		{
// 			printf("minishell: syntax error near unexpected token 'newline'\n");
// 			return (0);
// 		}
// 	}
// 	return (1); // Input passed validation
// }

int	validate_input(t_token *tokens, t_parse *data)
{
	t_token *current = tokens;
	int token_count = 0;

	if (!current)
		return (1); // Empty input is valid (no tokens)

	// Check if the input starts with a |
	if (current->value[0] == '|')
	{
		syntax_error(current->value);
		//printf("minishell: syntax error near unexpected token '%s'\n", current->value);
		data->valid_input = 0;
		data->exit = 2; // Set exit status for syntax error
		return (0);
	}

	while (current)
	{
		token_count++;
		if ((current->value[0] == '<' || current->value[0] == '>' || current->value[0] == '|'))
		{
			if (current->next && (current->next->value[0] == '<' || current->next->value[0] == '>' || current->next->value[0] == '|'))
			{
				if (!(current->value[0] == '>' && current->next->value[0] == '>') &&
					!(current->value[0] == '<' && current->next->value[0] == '<'))
				{
					data->valid_input = 0;
					syntax_error(current->value);
					//printf("minishell: syntax error near unexpected token '%s'\n", current->next->value);
					data->exit = 2;
					return (0);
				}
			}
		}
		if (current->value[0] == '<' || current->value[0] == '>')
		{
			if (!current->next || current->next->value[0] == '|' || current->next->value[0] == '<' || current->next->value[0] == '>')
			{
				if (current->next)
				{
					data->valid_input = 0;
					syntax_error(current->value);
					//printf("minishell: syntax error near unexpected token '%s'\n", current->next->value);
				}
				else
				{
					data->valid_input = 0;
					syntax_error(current->value);
					//printf("minishell: syntax error near unexpected token 'newline'\n");
				}
				data->exit = 2;
				return (0);
			}
		}
		if (current->type == TOKEN_HEREDOC)
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				if (current->next)
				{
					data->valid_input = 0;
					syntax_error(current->value);
					//printf("minishell: syntax error near unexpected token '%s'\n", current->next->value);
				}
				else
				{
					data->valid_input = 0;
					syntax_error(current->value);
					//printf("minishell: syntax error near unexpected token 'newline'\n");
				}
				data->exit = 2;
				return (0);
			}
		}
		current = current->next;
	}

	// Check if the input ends with an operator
	if (tokens)
	{
		t_token *last = tokens;
		while (last && last->next)
			last = last->next;
		if (last->value[0] == '|' || last->value[0] == '<' || last->value[0] == '>')
		{
			data->valid_input = 0;
			printf("minishell: syntax error near unexpected token 'newline'\n");
			data->exit = 2;
			return (0);
		}
	}
	return (1);
}
