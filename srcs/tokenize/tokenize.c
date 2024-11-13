#include "../../includes/minishell.h"

//creats a new token and allocates memory for it. Sets the value and type of the new
//token to the given input.
t_token *new_token(e_token_type type, char *value)
{
	t_token *token = (t_token *)malloc(sizeof(t_token));
	if (!token)
	{
		perror("Failed to allocate token");
		exit(EXIT_FAILURE);
	}
	token->type = type;
	token->value = ft_strdup(value);
	token->next = NULL;
	return (token);
}

//Takes pointer to data and a pointer to a new token so it can add the new token
//at the end (tail) of the list of tokens.
void add_token_to_list(t_parse *data, t_token *new_token)
{
    if (data->head == NULL) {
        data->head = new_token;
    } else {
        data->tail->next = new_token;
    }
    data->tail = new_token;
}

//takes the buffer containing a segment of input from the user which has been identified
//as a token. Functions New_token and add_token_to_list are called for next steps
void handle_buffer(t_parse *data, e_token_type token_type)
{
	if (data->buf_index > 0)
	{
		data->buffer[data->buf_index] = '\0';
		add_token_to_list(data, new_token(token_type, data->buffer));
		data->buf_index = 0;
	}
}

//This function takes the input from the command line prompt and splits it up
//into tokens. Tokens can consist of words (later split up into args and commands)
//pipes, and redirect. It also pays attention to quote state to ensure words within
//quotes are grouped in one token.
void	tokenize(char *input, t_parse *data)
{
	int i;

	i = 0;
	while (input[i] != '\0') {
		if (data->buf_index >= BUFFER_SIZE - 1)
		{
			printf("Buffer overflow detected\n");
			exit(EXIT_FAILURE);
		}

		// Handle single quotes
	if (input[i] == '\'' && !data->in_double_quote) {
			data->buffer[data->buf_index++] = input[i];  // Add quote to buffer
			data->in_single_quote = !data->in_single_quote;
			i++;
			continue;
        }

		// Handle double quotes
        if (input[i] == '\"' && !data->in_single_quote) {
			data->buffer[data->buf_index++] = input[i];  // Add quote to buffer
			data->in_double_quote = !data->in_double_quote;
			i++;
			continue;
		}

		// Ignore backslashes and semicolons
		if (input[i] == '\\' || input[i] == ';') {
			i++;
			continue;
		}

		// Handle whitespace outside of quotes
		if (isspace(input[i]) && !data->in_single_quote && !data->in_double_quote) {
			handle_buffer(data, TOKEN_WORD);
			i++;
			continue;
		}

		// Handle pipe
		if (input[i] == '|') {
			handle_buffer(data, TOKEN_WORD);
			add_token_to_list(data, new_token(TOKEN_PIPE, "|"));
			i++;
			continue;
		}

		// Handle redirection: output
		if (input[i] == '>') {
			handle_buffer(data, TOKEN_WORD);
			if (input[i + 1] == '>') {
				add_token_to_list(data, new_token(TOKEN_REDIR_APPEND, ">>"));
				i += 2;
			} else
			{
				add_token_to_list(data, new_token(TOKEN_REDIR_OUT, ">"));
				i++;
			}
			continue;
		}

		// Handle redirection: input
		if (input[i] == '<') {
			handle_buffer(data, TOKEN_WORD);
			if (input[i + 1] == '<') {
				add_token_to_list(data, new_token(TOKEN_HEREDOC, "<<"));
				i += 2;
			} else
			{
				add_token_to_list(data, new_token(TOKEN_REDIR_IN, "<"));
				i++;
			}
			continue;
		}

		// Add regular characters to buffer
		data->buffer[data->buf_index++] = input[i++];
	}
	// Finalize last token if available
	handle_buffer(data, TOKEN_WORD);

	if (data->in_single_quote || data->in_double_quote) {
		fprintf(stderr, "Error: Unclosed quote detected\n");
		if (data->head) {  // Only free tokens if they exist
			free_tokens(data->head);
		}
		data->head = data->tail = NULL; // Reset head and tail
		return;
    }
}
