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

// For testing: prints each token in the linked list along with its type.
void print_tokens(t_token *token_list)
{
    t_token *current = token_list;

    while (current != NULL) {
        printf("Token: %s, Type: ", current->value);

        //Check the type and print accordingly
        if (current->type == TOKEN_WORD) {
            printf("COMMAND");
        } else if (current->type == TOKEN_WORD) {
            printf("ARGUMENT");
        } else if (current->type == TOKEN_PIPE) {
            printf("PIPE");
        } else if (current->type == TOKEN_HEREDOC) {
            printf("HEREDOC");
		} else if (current->type == TOKEN_REDIR_APPEND) {
            printf("REDIR APPEND");
		} else if (current->type == TOKEN_REDIR_IN) {
            printf("REDIR IN");
		} else if (current->type == TOKEN_REDIR_OUT) {
            printf("REDIR OUT");
        } else {
            printf("UNKNOWN");
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
	t_token	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}
//Make preliminary checks on input. Ensure operators (|, <<, >) are not next to
//eachother. That input doesn't start or end with an operator, that operator are
//not next to eachother and check for missing args after operators. Current Error
//msgs are incoorect for testing purposes but need to be updated to match bash!!
int	validate_input(t_token *tokens)
{
	t_token *current = tokens;
	int token_count;

	token_count = 0;
    // Check for empty input (null or no tokens)
    if (!current) {
        return 1; // Empty prompt for no tokens
	}
	if (current->value[0] == '|' || current->value[0] == '<' || current->value[0] == '>')
	{
		printf("Syntax error: Operator at the start of input\n");
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
				printf("syntax error Invalid sequence '%s %s'\n", current->value, current->next->value);
					return (0);
				}
			}
		}
		if ((current->value[0] == '<' || current->value[0] == '>'))
		{
			if (!current->next || !current->next->value[0]
				|| current->next->value[0] == '|'
				|| current->next->value[0] == '<'
				|| current->next->value[0] == '>')
			{
				printf("syntax error Missing argument after '%s'\n", current->value);
				return (0);
			}
		}
		current = current->next;
	}
    if (tokens)
	{
        t_token *last = tokens;
        while (last->next) {
            last = last->next;
        }
        if (last->value[0] == '|' || last->value[0] == '<' || last->value[0] == '>')
		{
            printf("Syntax error: Operator at the end of input\n");
            return 0;
        }
	}

	return (1); // Input passed validation
}
