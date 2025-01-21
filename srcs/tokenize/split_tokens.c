#include "../../includes/minishell.h"

//creats a new token and allocates memory for it. Sets the value and type of the new
//token to the given input.
t_token *new_token(e_token_type type, char *value)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
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
	if (data->head == NULL)
		data->head = new_token;
	else
		data->tail->next = new_token;
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


int ambiguous_redirect_error(char *token)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
	return (1);
}

int validate_heredoc(t_token *next, t_parse *data);
int validate_operator_sequence(t_token *next, t_parse *data);
int validate_tokens(t_token *tokens, t_parse *data, t_shell *shell);
int validate_pipe_start(t_token *tokens, t_parse *data);
int validate_empty_input(t_token *tokens);


int	validate_empty_input(t_token *tokens)
{
	return (tokens == NULL);
}

int	validate_pipe_start(t_token *tokens, t_parse *data)
{
	if (tokens->value[0] == '|') {
		syntax_error("|");
		data->valid_input = 0;
		data->exit = 2;
		return (0);
	}
	return (1);
}

int	validate_operator_sequence2(t_token *next, t_parse *data, t_shell *shell)
{
	char	*expanded_var;

	if ((next && (next->type == TOKEN_REDIR_IN || next->type == TOKEN_REDIR_OUT ||
	next->type == TOKEN_REDIR_APPEND)))
	{
		syntax_error(next->value);
		data->valid_input = 0;
		data->exit = 2;
		return (0);
	}
	if (next->value[0] == '$')
	{
		expanded_var = replace_variables_in_heredoc(next->value, data, shell);
		if (!expanded_var || expanded_var[0] == '\0')
		{
			ambiguous_redirect_error(next->value);
			free(expanded_var);
			data->exit = 1;
			return 0;
		}
		free(expanded_var);
	}
	return (1);
}

int	validate_tokens(t_token *current, t_parse *data, t_shell *shell)
{
	if (current->type == TOKEN_PIPE || current->type == TOKEN_REDIR_IN
		|| current->type == TOKEN_REDIR_OUT
		|| current->type == TOKEN_REDIR_APPEND)
	{
		if (!validate_operator_sequence(current->next, data))
			return (0);
	}
	if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
		|| current->type == TOKEN_REDIR_APPEND)
	{
		if (!validate_operator_sequence2(current->next, data, shell))
			return (0);
	}
	if (current->type == TOKEN_HEREDOC
		&& !validate_heredoc(current->next, data))
		return (0);
	return (1);
}

int validate_operator_sequence(t_token *next, t_parse *data)
{
	if (!next || next->type == TOKEN_PIPE )
	{
		syntax_error(next ? next->value : "newline");
		data->valid_input = 0;
		data->exit = 2;
		return (0);
	}
	return (1);
}

int	validate_heredoc(t_token *next, t_parse *data)
{
	if (!next || next->type != TOKEN_WORD)
	{
		syntax_error(next ? next->value : "newline");
		data->valid_input = 0;
		data->exit = 2;
		return (0);
	}
	return (1);
}

int	validate_input(t_token *tokens, t_parse *data, t_shell *shell)
{
	t_token *current = tokens;
	t_token *last = tokens;

	current = tokens;
	last = tokens;
	if (!current)
		return (1);
	if (!validate_pipe_start(tokens, data))
		return (0);
	while (current)
	{
		if (!validate_tokens(current, data, shell))
			return (0);
		current = current->next;
	}
	while (last && last->next)
		last = last->next;
	if (last->type == TOKEN_PIPE || last->type == TOKEN_REDIR_IN
		|| last->type == TOKEN_REDIR_OUT || last->type == TOKEN_REDIR_APPEND)
	{
		syntax_error("newline");
		data->valid_input = 0;
		data->exit = 2;
		return (0);
	}
	return (1);
}

void	process_quote(char *input, t_parse *data, char quote_type, int *i)
{
	data->buffer[data->buf_index++] = input[*i];
	if (quote_type == '\'')
		data->in_single_quote = !data->in_single_quote;
	else
		data->in_double_quote = !data->in_double_quote;
	(*i)++;
}

void	process_pipe(char *input, t_parse *data, int *i)
{
	(void)input;
	handle_buffer(data, TOKEN_WORD);
	add_token_to_list(data, new_token(TOKEN_PIPE, "|"));
	(*i)++;
}

void	process_redirection(char *input, t_parse *data, int *i)
{
	handle_buffer(data, TOKEN_WORD);
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token_to_list(data, new_token(TOKEN_REDIR_APPEND, ">>"));
		*i += 2;
	}
	else if (input[*i] == '>')
	{
		add_token_to_list(data, new_token(TOKEN_REDIR_OUT, ">"));
		(*i)++;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token_to_list(data, new_token(TOKEN_HEREDOC, "<<"));
		*i += 2;
	}
	else
	{
		add_token_to_list(data, new_token(TOKEN_REDIR_IN, "<"));
		(*i)++;
	}
}

void	process_whitespace(t_parse *data, int *i)
{
	handle_buffer(data, TOKEN_WORD);
	(*i)++;
}

void	append_character(char *input, t_parse *data, int *i)
{
	data->buffer[data->buf_index++] = input[*i];
	(*i)++;
}

void	finalize_parsing(t_parse *data)
{
	handle_buffer(data, TOKEN_WORD);
	if (data->in_single_quote || data->in_double_quote)
	{
		data->valid_input = 0;
		fprintf(stderr, "minishell: syntax error: unclosed quote detected\n"); //cannot use fprintf
		free_tokens(data->head);
		data->head = data->tail = NULL;
	}
	else
	{
		//free_tokens(data->head);
		data->valid_input = 1;
	}
}

void	split_tokens(char *input, t_parse *data)
{
	int i;

	i = 0;
	while (input[i] != '\0')
	{
		if (data->buf_index >= BUFFER_SIZE - 1)
		{
			printf("Buffer overflow detected\n"); // Use error msg
			exit(EXIT_FAILURE);
		}
		if ((input[i] == '\'' && !data->in_double_quote) ||
			(input[i] == '\"' && !data->in_single_quote))
			process_quote(input, data, input[i], &i);
		else if ((input[i] == ' ' || input[i] == '\t' || input[i] == '\n')
			&& !data->in_single_quote && !data->in_double_quote)
			process_whitespace(data, &i);
		else if (input[i] == '|'
			&& !(data->in_single_quote || data->in_double_quote))
			process_pipe(input, data, &i);
		else if ((input[i] == '>' || input[i] == '<')
			&& !(data->in_single_quote || data->in_double_quote))
			process_redirection(input, data, &i);
		else
			data->buffer[data->buf_index++] = input[i++];
	}
	finalize_parsing(data);
}
