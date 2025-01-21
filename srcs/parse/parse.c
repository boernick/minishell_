/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:30:01 by prichugh          #+#    #+#             */
/*   Updated: 2024/11/19 16:20:11 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//frees redir stack
void	free_redir_stack(t_redirect *redir)
{
	t_redirect	*temp_redir;

	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC && redir->file != NULL)
			unlink(redir->file);
		if (redir->file)
			free(redir->file);
		temp_redir = redir;
		redir = redir->next;
		free(temp_redir);
	}
}

//frees commands stack
void	free_command_stack(t_cmd *cmd_stack)
{
	t_cmd	*temp;
	int		i;

	while (cmd_stack)
	{
		temp = cmd_stack;
		if (cmd_stack->cmd)
			free(cmd_stack->cmd);
		if (cmd_stack->argv)
		{
			i = 0;
			while (cmd_stack->argv[i])
			{
				free(cmd_stack->argv[i]);
				i++;
			}
			free(cmd_stack->argv);
		}
		if (cmd_stack->redir)
			free_redir_stack(cmd_stack->redir);
		cmd_stack = cmd_stack->next;
		free(temp);
	}
}


//TESTING: print command struct
void print_command_stack(t_cmd *cmd_stack)
{
	int cmd_index = 0;

	printf("\nCommand Stack:\n");
	while (cmd_stack)
	{
		printf("Command %d:\n", cmd_index++);
		printf("  Command: %s\n", cmd_stack->cmd ? cmd_stack->cmd : "NULL");

		if (cmd_stack->argv)
		{
			printf("  Arguments:\n");
			for (int i = 0; cmd_stack->argv[i]; i++)
				printf("    argv[%d]: %s\n", i, cmd_stack->argv[i]);
		}
		else
		{
			printf("  Arguments: None\n");
		}

		printf("  Builtin: %s\n", cmd_stack->is_builtin ? "Yes" : "No");

		if (cmd_stack->redir)
		{
			printf("  Redirections:\n");
			t_redirect *redir = cmd_stack->redir;
			while (redir)
			{
				const char *redir_type;
				switch (redir->type)
				{
					case TOKEN_REDIR_IN:
						redir_type = "<";
						break;
					case TOKEN_REDIR_OUT:
						redir_type = ">";
						break;
					case TOKEN_REDIR_APPEND:
						redir_type = ">>";
						break;
					case TOKEN_HEREDOC:
						redir_type = "<<";
						break;
					default:
						redir_type = "Unknown";
				}

				printf("    Type: %s, File: %s\n", redir_type, redir->file ? redir->file : "NULL");
				redir = redir->next;
			}
		}
		else
		{
			printf("  Redirections: None\n");
		}
		printf("\n");
		cmd_stack = cmd_stack->next;
	}
} // above function is for testing

void	add_cmd_to_list(t_parse *data, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!new_cmd)
		return ;
	if (data->cmd == NULL)
		data->cmd = new_cmd;
	else
	{
		current = data->cmd;
		while (current->next)
			current = current->next;
		current->next = new_cmd;
	}
}

bool	is_builtin_(char *cmd)
{
	if (!cmd)
		return (false);
	return (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "cd", 3) == 0 ||
		ft_strncmp(cmd, "pwd", 4) == 0
		|| ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0
		|| ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0);
}

void	exit_perror(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
//ORIGINAL
// void add_argument_to_cmd(t_cmd *cmd, char *arg)
// {
// 	int i = 0;

// 	while (cmd->argv && cmd->argv[i]) // Find the current size of argv
// 		i++;

// 	char **new_argv = (char **)malloc(sizeof(char *) * (i + 2)); // Allocate space for new argv
// 	for (int j = 0; j < i; j++)
// 		new_argv[j] = cmd->argv[j]; // Copy existing arguments
// 	new_argv[i] = ft_strdup(arg);
// 	new_argv[i + 1] = NULL;
// 	free(cmd->argv);
// 	cmd->argv = new_argv;
// }

void	add_argument_to_cmd(t_cmd *cmd, char *arg)
{
	int		i;
	int		j;
	char	**new_argv;

	i = 0;
	while (cmd->argv && cmd->argv[i])
		i++;
	new_argv = (char **)malloc(sizeof(char *) * (i + 2));
	j = 0;
	while (j < i)
	{
		new_argv[j] = cmd->argv[j];
		j++;
	}
	new_argv[i] = ft_strdup(arg);
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

//for testing
void print_parsed_data(t_parse *data)
{
t_cmd *cmd = data->cmd;
int cmd_index = 0;

printf("Parsed Command List:\n");

while (cmd)
{
	printf("Command %d:\n", cmd_index++);
	printf("  Command: %s\n", cmd->cmd ? cmd->cmd : "NULL");

	if (cmd->argv)
	{
		printf("  Arguments:\n");
		for (int i = 0; cmd->argv[i]; i++)
			printf("    argv[%d]: %s\n", i, cmd->argv[i]);
	}
	else
	{
		printf("  Arguments: None\n");
	}

	printf("  Builtin: %s\n", cmd->is_builtin ? "Yes" : "No");

	if (cmd->redir)
	{
		printf("  Redirections:\n");
		t_redirect *redir = cmd->redir;
		while (redir)
		{
			const char *redir_type;
			if (redir->type == TOKEN_REDIR_IN)
				redir_type = "<";
			else if (redir->type == TOKEN_REDIR_OUT)
				redir_type = ">";
			else if (redir->type == TOKEN_REDIR_APPEND)
				redir_type = ">>";
			else if (redir->type == TOKEN_HEREDOC)
				redir_type = "<<";
			else
				redir_type = "Unknown";
			printf("    Type: %s, File: %s\n", redir_type, redir->file);
			redir = redir->next;
		}
	}
	else //assuming current_token->next is non-NULL when accessing current_token->next->value.
		//This is risky and leads to segmentation faults if current_token->next is NULL.
	{
		printf("  Redirections: None\n");
	}

	printf("\n");
	cmd = cmd->next;
}
printf("Number of commands: %d\n", data->n_cmds);
printf("Number of pipes: %d\n", data->n_pipes);
}

void	add_redirection_to_cmd(t_cmd *cmd, t_redirect *new_redir)
{
	t_redirect	*current;

	if (!cmd->redir)
		cmd->redir = new_redir;
	else
	{
		current = cmd->redir;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
}

//for testing
char *get_token_type_name(e_token_type type) {
    switch (type) {
        case TOKEN_CMD:         return "TOKEN_CMD";
        case TOKEN_ARG:         return "TOKEN_ARG";
        case TOKEN_PIPE:        return "TOKEN_PIPE";
        case TOKEN_REDIR_IN:    return "TOKEN_REDIR_IN";
        case TOKEN_REDIR_OUT:   return "TOKEN_REDIR_OUT";
        case TOKEN_REDIR_APPEND:return "TOKEN_REDIR_APPEND";
        case TOKEN_HEREDOC:     return "TOKEN_HEREDOC";
        case TOKEN_WORD:        return "TOKEN_WORD";
		case TOKEN_SKIP:        return "TOKEN_SKIP";
        default:                return "UNKNOWN_TYPE";
    }
}


//ORIGINAL
// void	parse_tokens(t_parse *data, t_shell *shell)
// {
// 	t_token		*cur_tkn;
// 	t_cmd		*current_cmd;
// 	t_redirect	*redir;

// 	cur_tkn = data->head;
// 	current_cmd = NULL;
// 	redir = NULL;
// 	data->n_cmds = 0;
// 	data->n_pipes = 0;
// 	while (cur_tkn)
// 	{
// 		if (cur_tkn->type == TOKEN_SKIP)
// 		{
// 			if (cur_tkn->next)
// 				cur_tkn = cur_tkn->next;
// 			else
// 			{
// 				data->valid_input = 0;
// 				break ;
// 			}
// 		}
// 		if (cur_tkn->type == TOKEN_CMD)
// 		{
// 			if (!current_cmd)
// 			{
// 				t_cmd *new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
// 				if (!new_cmd)
// 					exit_perror("Failed to allocate memory for command");
// 				new_cmd->cmd = ft_strdup(cur_tkn->value);
// 				new_cmd->argv = (char **)malloc(sizeof(char *) * 2);
// 				new_cmd->argv[0] = ft_strdup(cur_tkn->value);
// 				new_cmd->argv[1] = NULL;
// 				new_cmd->index = data->n_cmds;
// 				new_cmd->is_builtin = is_builtin_(new_cmd->cmd);
// 				new_cmd->redir = NULL;
// 				new_cmd->next = NULL;
// 				add_cmd_to_list(data, new_cmd);
// 				current_cmd = new_cmd;
// 				data->n_cmds++;
// 			}
// 			else if (current_cmd->cmd == NULL)
// 			{
// 				current_cmd->cmd = ft_strdup(cur_tkn->value);
// 				current_cmd->argv = (char **)malloc(sizeof(char *) * 2);
// 				current_cmd->argv[0] = ft_strdup(cur_tkn->value);
// 				current_cmd->argv[1] = NULL;
// 				current_cmd->is_builtin = is_builtin_(current_cmd->cmd);
// 			}
// 		}
// 		else if (cur_tkn->type == TOKEN_ARG || cur_tkn->type == TOKEN_FLAG_ARG)
// 		{
// 			if (current_cmd)
// 				add_argument_to_cmd(current_cmd, cur_tkn->value);
// 		}
// 		else if (cur_tkn->type == TOKEN_REDIR_IN
// 			|| cur_tkn->type == TOKEN_REDIR_OUT
// 			|| cur_tkn->type == TOKEN_REDIR_APPEND)
// 				{
// 			if (!current_cmd)
// 			{
// 				t_cmd *new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
// 				if (!new_cmd)
// 					exit_perror("Failed to allocate memory for placeholder command");
// 				new_cmd->cmd = NULL;
// 				new_cmd->argv = NULL;
// 				new_cmd->index = data->n_cmds;
// 				new_cmd->is_builtin = false;
// 				new_cmd->redir = NULL;
// 				new_cmd->next = NULL;
// 				add_cmd_to_list(data, new_cmd);
// 				current_cmd = new_cmd;
// 				data->n_cmds++;
// 			}
// 			if (cur_tkn->next == NULL || cur_tkn->next->value == NULL)
// 			{
// 				fprintf(stderr, "Error: Missing file for redirection.\n");
// 				break ;
// 			}
// 			redir = (t_redirect *)malloc(sizeof(t_redirect));
// 			if (!redir)
// 				exit_perror("Failed to allocate memory for redirection");

// 			redir->file = ft_strdup(cur_tkn->next->value);
// 			redir->type = cur_tkn->type;
// 			redir->next = NULL;
// 			add_redirection_to_cmd(current_cmd, redir);
// 			cur_tkn = cur_tkn->next;
// 		}
// 		else if (cur_tkn->type == TOKEN_HEREDOC)
// 		{
// 			if (!current_cmd) {
// 				t_cmd *new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
// 				if (!new_cmd)
// 					exit_perror("Failed to allocate memory for placeholder command");
// 				new_cmd->cmd = NULL;
// 				new_cmd->argv = NULL;
// 				new_cmd->index = data->n_cmds;
// 				new_cmd->is_builtin = false;
// 				new_cmd->redir = NULL;
// 				new_cmd->next = NULL;
// 				add_cmd_to_list(data, new_cmd);
// 				current_cmd = new_cmd;
// 				data->n_cmds++;
// 			}
// 			if (!cur_tkn->next)
// 			{
// 				exit_perror("Error: Missing delimiter for heredoc.\n");
// 				break ;
// 			}
// 			redir = (t_redirect *)malloc(sizeof(t_redirect));
// 			if (!redir)
// 				exit_perror("Failed to allocate memory for heredoc redirection");
// 			redir->file = create_heredoc();
// 			redir->type = TOKEN_HEREDOC;
// 			redir->next = NULL;
// 			add_redirection_to_cmd(current_cmd, redir);
// 			char *delimeter = ft_strdup(cur_tkn->next->value);
// 			cur_tkn = cur_tkn->next;
// 			data->exit = run_heredoc(data, redir, delimeter, shell);
// 			free(delimeter);
// 			if (data->exit == 130)
// 			{
// 				data->valid_input = 0;
// 				shell->last_exit = 130;
// 				if (access(redir->file, F_OK) == 0 && data->exit == 130)
// 					unlink(redir->file);
// 				break ;
// 			}
// 		}
// 		else if (cur_tkn->type == TOKEN_PIPE)
// 		{
// 			data->n_pipes++;
// 			current_cmd = NULL;
// 		}
// 		cur_tkn = cur_tkn->next;
// 	}
// }




////////////////////////////////////
int malloc_error(size_t n)
{
	ft_putstr_fd("minishell: malloc: cannot allocate ", STDERR_FILENO);
	ft_putnbr_fd(n, STDERR_FILENO);
	ft_putendl_fd(" bytes (or more)", STDERR_FILENO);
	return (1);
}

int	process_token_skip(t_token **cur_tkn, t_parse *data)
{
	if ((*cur_tkn)->next)
	{
		*cur_tkn = (*cur_tkn)->next;
	}
	else
	{
		if (data->n_cmds > 0)
			return (1);
		data->valid_input = 0;
		return (0);
	}
	return (1);
}

void	process_token_cmd(t_token *cur_tkn, t_parse *data, t_cmd **current_cmd)
{
	t_cmd	*new_cmd;

	if (!*current_cmd)
	{
		new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
		if (!new_cmd)
			exit_perror("Failed to allocate memory for command");
		new_cmd->cmd = ft_strdup(cur_tkn->value);
		new_cmd->argv = (char **)malloc(sizeof(char *) * 2);
		new_cmd->argv[0] = ft_strdup(cur_tkn->value);
		new_cmd->argv[1] = NULL;
		new_cmd->index = data->n_cmds;
		new_cmd->is_builtin = is_builtin_(new_cmd->cmd);
		new_cmd->redir = NULL;
		new_cmd->next = NULL;
		add_cmd_to_list(data, new_cmd);
		*current_cmd = new_cmd;
		data->n_cmds++;
	}
	else if ((*current_cmd)->cmd == NULL)
	{
		(*current_cmd)->cmd = ft_strdup(cur_tkn->value);
		(*current_cmd)->argv = (char **)malloc(sizeof(char *) * 2);
		(*current_cmd)->argv[0] = ft_strdup(cur_tkn->value);
		(*current_cmd)->argv[1] = NULL;
		(*current_cmd)->is_builtin = is_builtin_((*current_cmd)->cmd);
	}
}
void	add_cmd_to_token(t_token *cur_tkn, t_parse *data,
			t_cmd **current_cmd)
{
	t_cmd	*new_cmd;

	if (!*current_cmd)
	{
		new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
		if (!new_cmd)
		{
			malloc_error(sizeof(t_cmd));
			exit (EXIT_FAILURE);
		}
		new_cmd->cmd = ft_strdup(cur_tkn->value);
		new_cmd->argv = (char **)malloc(sizeof(char *) * 2);
		new_cmd->argv[0] = ft_strdup(cur_tkn->value);
		new_cmd->argv[1] = NULL;
		new_cmd->index = data->n_cmds;
		new_cmd->is_builtin = is_builtin_(new_cmd->cmd);
		new_cmd->redir = NULL;
		new_cmd->next = NULL;
		add_cmd_to_list(data, new_cmd);
		*current_cmd = new_cmd;
		data->n_cmds++;
	}
	else if ((*current_cmd)->cmd == NULL)
	{
		(*current_cmd)->cmd = ft_strdup(cur_tkn->value);
		(*current_cmd)->argv = (char **)malloc(sizeof(char *) * 2);
		(*current_cmd)->argv[0] = ft_strdup(cur_tkn->value);
		(*current_cmd)->argv[1] = NULL;
		(*current_cmd)->is_builtin = is_builtin_((*current_cmd)->cmd);
	}
}

void process_token_redirection(t_token *cur_tkn, t_parse *data,
		t_cmd **current_cmd)
{
	t_cmd		*new_cmd;
	t_redirect	*redir;

	if (!*current_cmd)
	{
		new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
		if (!new_cmd)
		{
			malloc_error(sizeof(t_cmd));
			exit (EXIT_FAILURE);
		}
		new_cmd->cmd = NULL;
		new_cmd->argv = NULL;
		new_cmd->index = data->n_cmds;
		new_cmd->is_builtin = false;
		new_cmd->redir = NULL;
		new_cmd->next = NULL;
		add_cmd_to_list(data, new_cmd);
		*current_cmd = new_cmd;
		data->n_cmds++;
	}
	redir = (t_redirect *)malloc(sizeof(t_redirect));
	if (!redir)
	{
		malloc_error(sizeof(t_redirect));
		exit (EXIT_FAILURE);
	}
	redir->file = ft_strdup(cur_tkn->next->value);
	redir->type = cur_tkn->type;
	redir->next = NULL;
	add_redirection_to_cmd(*current_cmd, redir);
}


int	process_token_heredoc(t_token *cur_tkn, t_parse *data,
			t_cmd **current_cmd, t_shell *shell)
{
	t_cmd		*new_cmd;
	t_redirect	*redir;
	char		*delimeter;

	if (!*current_cmd)
	{
		new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
		if (!new_cmd)
		{
			malloc_error(sizeof(t_cmd));
			return (0);
		}
		new_cmd->cmd = NULL;
		new_cmd->argv = NULL;
		new_cmd->index = data->n_cmds;
		new_cmd->is_builtin = false;
		new_cmd->redir = NULL;
		new_cmd->next = NULL;
		add_cmd_to_list(data, new_cmd);
		*current_cmd = new_cmd;
		data->n_cmds++;
	}
	redir = (t_redirect *)malloc(sizeof(t_redirect));
	if (!redir)
	{
		malloc_error(sizeof(t_cmd));
		return (0);
	}
	redir->file = create_heredoc();
	redir->type = TOKEN_HEREDOC;
	redir->next = NULL;
	add_redirection_to_cmd(*current_cmd, redir);
	delimeter = ft_strdup(cur_tkn->next->value);
	cur_tkn = cur_tkn->next;
	data->exit = run_heredoc(data, redir, delimeter, shell);
	free(delimeter);
	if (data->exit == 130)
	{
		data->valid_input = 0;
		shell->last_exit = 130;
		if (access(redir->file, F_OK) == 0 && data->exit == 130)
			unlink(redir->file);
		return (0);
	}
	return (1);
}
void	init_parse_data(t_parse *data, t_token **cur_tkn, t_cmd **current_cmd)
{
	*cur_tkn = data->head;
	*current_cmd = NULL;
	data->n_cmds = 0;
	data->n_pipes = 0;
}

void process_token_pipe(t_parse *data, t_cmd **current_cmd)
{
	data->n_pipes++;
	*current_cmd = NULL;
}

void	parse_tokens(t_parse *data, t_shell *shell)
{
	t_token		*cur_t;
	t_cmd		*current_cmd;

	init_parse_data(data, &cur_t, &current_cmd);
	while (cur_t)
	{
		if (cur_t->type == TOKEN_SKIP && !process_token_skip(&cur_t, data))
			break ;
		if (cur_t->type == TOKEN_CMD)
			add_cmd_to_token(cur_t, data, &current_cmd);
		else if ((cur_t->type == TOKEN_ARG || cur_t->type == TOKEN_FLAG_ARG)
			&& current_cmd)
			add_argument_to_cmd(current_cmd, cur_t->value);
		else if (cur_t->type == TOKEN_REDIR_IN || cur_t->type == TOKEN_REDIR_OUT
			|| cur_t->type == TOKEN_REDIR_APPEND)
				process_token_redirection(cur_t, data, &current_cmd);
		else if (cur_t->type == TOKEN_HEREDOC
			&& (!process_token_heredoc(cur_t, data, &current_cmd, shell)))
				break ;
		else if (cur_t->type == TOKEN_PIPE)
			process_token_pipe(data, &current_cmd);
		cur_t = cur_t->next;
	}
}
