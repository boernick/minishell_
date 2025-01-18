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




int validate_input(t_token *tokens, t_parse *data)
{
    t_token *current = tokens;

    if (!current)
        return (1); // Empty input is valid (no tokens)

    // Check if the input starts with a pipe
    if (current->value[0] == '|')
    {
        syntax_error("|");
        data->valid_input = 0;
        data->exit = 2; // Syntax error exit status
        return (0);
    }

    while (current)
    {
        t_token *next = current->next;

        // Check for invalid operator sequences
        if (current->type == TOKEN_PIPE || current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT || current->type == TOKEN_REDIR_APPEND)
        {
            // Handle cases like `>` at the end or `| |`
            if (!next || next->type == TOKEN_PIPE ) //|| next->type == TOKEN_REDIR_IN || next->type == TOKEN_REDIR_OUT || next->type == TOKEN_REDIR_APPEND
            {
                syntax_error(next ? next->value : "newline");
                data->valid_input = 0;
                data->exit = 2;
                return (0);
            }
        }
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT || current->type == TOKEN_REDIR_APPEND)
		{
				if ((next && (next->type == TOKEN_REDIR_IN || next->type == TOKEN_REDIR_OUT ||
				next->type == TOKEN_REDIR_APPEND)))
				{
					syntax_error(next->value);
					data->valid_input = 0;
					data->exit = 2;
					return (0);
				}
		}
        // Handle heredoc (`<<`) without a delimiter
        if (current->type == TOKEN_HEREDOC && (!next || next->type != TOKEN_WORD))
        {
            syntax_error(next ? next->value : "newline");
            data->valid_input = 0;
            data->exit = 2;
            return (0);
        }

        current = next;
    }

    // Check if the last token is an operator
    t_token *last = tokens;
    while (last && last->next)
        last = last->next;

    if (last->type == TOKEN_PIPE || last->type == TOKEN_REDIR_IN || last->type == TOKEN_REDIR_OUT || last->type == TOKEN_REDIR_APPEND)
    {
        syntax_error("newline");
        data->valid_input = 0;
        data->exit = 2;
        return (0);
    }

    return (1); // Input is valid
}

//This function takes the input from the command line prompt and splits it up
//into tokens. Tokens can consist of words (later split up into args and commands)
//pipes, and redirect. It also pays attention to quote state to ensure words within
//quotes are grouped in one token.
void split_tokens(char *input, t_parse *data)
{
    int i = 0;

    while (input[i] != '\0')
    {
        if (data->buf_index >= BUFFER_SIZE - 1)
        {
            printf("Buffer overflow detected\n");
            data->valid_input = 0;
            exit(EXIT_FAILURE);
        }

        if (input[i] == '\'' && !data->in_double_quote)
        {
            data->buffer[data->buf_index++] = input[i];
            data->in_single_quote = !data->in_single_quote;
            i++;
            continue;
        }

        if (input[i] == '\"' && !data->in_single_quote)
        {
            data->buffer[data->buf_index++] = input[i];
            data->in_double_quote = !data->in_double_quote;
            i++;
            continue;
        }

		if (strncmp(&input[i], "$EMPTY", 6) == 0)
        {
            // Skip over "$EMPTY" and move to the next character
            i += 6;
            continue;
        }

        if (isspace(input[i]) && !data->in_single_quote && !data->in_double_quote)
        {
         	handle_buffer(data, TOKEN_WORD);
            i++;
            continue;
        }

        if (input[i] == '|' && !data->in_single_quote && !data->in_double_quote)
        {
            handle_buffer(data, TOKEN_WORD);
            add_token_to_list(data, new_token(TOKEN_PIPE, "|"));
            i++;
            continue;
        }

        if (input[i] == '>' && !data->in_single_quote && !data->in_double_quote)
        {
            handle_buffer(data, TOKEN_WORD);
            if (input[i + 1] == '>')
            {
                add_token_to_list(data, new_token(TOKEN_REDIR_APPEND, ">>"));
                i += 2;
            }
            else
            {
                add_token_to_list(data, new_token(TOKEN_REDIR_OUT, ">"));
                i++;
            }
            continue;
        }

        if (input[i] == '<' && !data->in_single_quote && !data->in_double_quote)
        {
            handle_buffer(data, TOKEN_WORD);
            if (input[i + 1] == '<')
            {
                add_token_to_list(data, new_token(TOKEN_HEREDOC, "<<"));
                i += 2;
            }
            else
            {
                add_token_to_list(data, new_token(TOKEN_REDIR_IN, "<"));
                i++;
            }
            continue;
        }

        data->buffer[data->buf_index++] = input[i++];
    }

    handle_buffer(data, TOKEN_WORD);

    if (data->in_single_quote || data->in_double_quote)
    {
        data->valid_input = 0;
        fprintf(stderr, "minishell: syntax error: unclosed quote detected\n");
        free_tokens(data->head);
        data->head = data->tail = NULL;
        return;
    }
	//print_tokens(data->head);
    data->valid_input = 1;
}
