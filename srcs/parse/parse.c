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

	//frees commands stack which includes redir stacks
	void free_command_stack(t_cmd *cmd_stack)
	{
		t_cmd *temp;
		while (cmd_stack)
		{
			temp = cmd_stack;

			// Free command string
			if (cmd_stack->cmd)
				free(cmd_stack->cmd);

			// Free arguments
			if (cmd_stack->argv)
			{
				for (int i = 0; cmd_stack->argv[i]; i++)
					free(cmd_stack->argv[i]);
				free(cmd_stack->argv);
			}

			// Free redirections
			t_redirect *redir = cmd_stack->redir;
			while (redir)
			{
				t_redirect *temp_redir = redir;
				if (redir->file)
					free(redir->file);
				redir = redir->next;
				free(temp_redir);
			}

			cmd_stack = cmd_stack->next;
			free(temp); // Free the command node
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
	}

void add_cmd_to_list(t_parse *data, t_cmd *new_cmd)
{
    if (!new_cmd)
        return;

    if (data->cmd == NULL) // If the list is empty, initialize it
    {
        data->cmd = new_cmd;
    }
    else // Append to the end of the list
    {
        t_cmd *current = data->cmd;
        while (current->next) // Traverse to the end
            current = current->next;
        current->next = new_cmd;
    }
}

bool is_builtin_(char *cmd)
{
    if (!cmd)
        return false;
    return (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "cd", 3) == 0 ||
            ft_strncmp(cmd, "pwd", 4) == 0 || ft_strncmp(cmd, "export", 7) == 0 ||
            ft_strncmp(cmd, "unset", 6) == 0 || ft_strncmp(cmd, "env", 4) == 0 ||
            ft_strncmp(cmd, "exit", 5) == 0);
}

void exit_perror(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void add_argument_to_cmd(t_cmd *cmd, char *arg)
{
    int i = 0;

    while (cmd->argv && cmd->argv[i]) // Find the current size of argv
        i++;

    char **new_argv = (char **)malloc(sizeof(char *) * (i + 2)); // Allocate space for new argv
    for (int j = 0; j < i; j++)
        new_argv[j] = cmd->argv[j]; // Copy existing arguments

    new_argv[i] = ft_strdup(arg); // Add new argument
    new_argv[i + 1] = NULL;

    free(cmd->argv); // Free old argv
    cmd->argv = new_argv;
}

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
        else
        {
            printf("  Redirections: None\n");
        }

        printf("\n");
        cmd = cmd->next;
    }
    printf("Number of commands: %d\n", data->n_cmds);
    printf("Number of pipes: %d\n", data->n_pipes);
}

void parse_tokens(t_parse *data)
{
    t_token *current_token = data->head;
    t_cmd *current_cmd = NULL;
    t_redirect *redir;

    data->n_cmds = 0;
    data->n_pipes = 0;

    while (current_token)
    {
        if (current_token->type == TOKEN_CMD)
        {
            data->n_cmds++; // Increment command count

            t_cmd *new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
            if (!new_cmd)
                exit_perror("Failed to allocate memory for command");

            // Initialize new command
            new_cmd->cmd = ft_strdup(current_token->value);
            new_cmd->argv = (char **)malloc(sizeof(char *) * 2);
            new_cmd->argv[0] = ft_strdup(current_token->value);
            new_cmd->argv[1] = NULL;
            new_cmd->index = data->n_cmds - 1;
            new_cmd->is_builtin = is_builtin_(new_cmd->cmd);
            new_cmd->redir = NULL;
            new_cmd->next = NULL;

            add_cmd_to_list(data, new_cmd);
            current_cmd = new_cmd;
        }
        else if (current_token->type == TOKEN_ARG || current_token->type == TOKEN_FLAG_ARG)
        {
            if (current_cmd)
                add_argument_to_cmd(current_cmd, current_token->value);
        }
        else if (current_token->type == TOKEN_REDIR_IN || current_token->type == TOKEN_REDIR_OUT ||
                 current_token->type == TOKEN_REDIR_APPEND || current_token->type == TOKEN_HEREDOC)
        {
            if (current_cmd)
            {
                redir = (t_redirect *)malloc(sizeof(t_redirect));
                if (!redir)
                    exit_perror("Failed to allocate memory for redirection");

                redir->file = ft_strdup(current_token->next->value);
                redir->type = current_token->type;
                redir->next = current_cmd->redir;
                current_cmd->redir = redir;

                current_token = current_token->next; // Skip the file token
            }
        }
        else if (current_token->type == TOKEN_PIPE)
        {
            data->n_pipes++; // Increment pipe count
            current_cmd = NULL;
        }

        current_token = current_token->next;
    }

    //print_parsed_data(data); only for testing
}
