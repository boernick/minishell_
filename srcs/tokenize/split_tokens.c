#include "../../includes/minishell.h"

//split_tokens_utils
t_token *new_token(t_token_type type, char *value)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
	{
		malloc_error(sizeof(t_token));
		exit(EXIT_FAILURE);
	}
	token->type = type;
	token->value = ft_strdup(value);
	token->next = NULL;
	return (token);
}


//split_tokens_utils
void add_token_to_list(t_parse *data, t_token *new_token)
{
	if (data->head == NULL)
		data->head = new_token;
	else
		data->tail->next = new_token;
	data->tail = new_token;
}

//split_tokens_utils
void	handle_buffer(t_parse *data, t_token_type token_type)
{
	if (data->buf_index > 0)
	{
		data->buffer[data->buf_index] = '\0';
		add_token_to_list(data, new_token(token_type, data->buffer));
		data->buf_index = 0;
	}
}

//error_ext
int	ambiguous_redirect_error(char *token)
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

//validate_input_utils
int	validate_pipe_start(t_token *tokens, t_parse *data)
{
	if (tokens->value[0] == '|')
	{
		syntax_error("|", data);
		return (0);
	}
	return (1);
}
//validate_input
int	validate_operator_sequence2(t_token *next, t_parse *data, t_shell *shell)
{
	char	*expanded_var;

	if ((next && (next->type == TOKEN_REDIR_IN || next->type == TOKEN_REDIR_OUT ||
	next->type == TOKEN_REDIR_APPEND)))
	{
		syntax_error(next->value, data);
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
//validate_input
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
//validate_input
int validate_operator_sequence(t_token *next, t_parse *data)
{
	if (!next || next->type == TOKEN_PIPE)
	{
		if (next)
			syntax_error(next->value, data);
		else
			syntax_error("newline", data);
		return (0);
	}
	return (1);
}
//validate_input_utils
int	validate_heredoc(t_token *next, t_parse *data)
{
	if (!next || next->type != TOKEN_WORD)
	{
		if (next)
			syntax_error(next->value, data);
		else
			syntax_error("newline", data);
		return (0);
	}
	return (1);
}
//validate_input
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
		syntax_error("newline", data);
		return (0);
	}
	return (1);
}
//split_tokens
void process_quote(char *input, t_parse *data, int *i)
{
	if (input[*i] == '\'' && !data->in_double_quote)
	{
		data->buffer[data->buf_index++] = input[*i];
		data->in_single_quote = !data->in_single_quote;
		(*i)++;
	}
	if (input[*i] == '\"' && !data->in_single_quote)
	{
		data->buffer[data->buf_index++] = input[*i];
		data->in_double_quote = !data->in_double_quote;
		(*i)++;
	}

}
//split_tokens
void	process_pipe(char *input, t_parse *data, int *i)
{
	(void)input;
	handle_buffer(data, TOKEN_WORD);
	add_token_to_list(data, new_token(TOKEN_PIPE, "|"));
	(*i)++;
}
//split_tokens
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
//split_tokens
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
//split_tokens
void	finalize_splitting(t_parse *data)
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
//split_tokens
void	split_tokens(char *input, t_parse *data)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		if (data->buf_index >= BUFFER_SIZE - 1)
			exit_perror("Buffer overflow detected\n");
		if ((input[i] == '\'' && !data->in_double_quote) ||
			(input[i] == '\"' && !data->in_single_quote))
			process_quote(input, data, &i);
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
	finalize_splitting(data);
}
