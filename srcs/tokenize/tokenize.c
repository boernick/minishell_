/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_and_parse.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prichugh <prichugh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:30:11 by prichugh          #+#    #+#             */
/*   Updated: 2024/11/13 18:30:11 by prichugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_eof(t_shell *shell, t_parse *parse)
{
	printf("exit\n");
	free_tokens(parse->head);
	free_command_stack(parse->cmd);
	free_envlst(shell->env_lst);
	clear_history();
	exit(0);
}

void	reset_parse(t_parse *data)
{
	data->head = NULL;
	data->tail = NULL;
	data->in_double_quote = 0;
	data->in_single_quote = 0;
}

void	process_tokens(char *input, t_parse *data, t_shell *shell)
{
	if (input && *input)
		add_history(input);
	reset_parse(data);
	split_tokens(input, data);
	data->exit = shell->last_exit;
	if (validate_input(data->head, data, shell) && data->valid_input)
	{
		data->valid_input = 1;
		classify_token_types(data, shell);
		replace_env_variables_in_tokens(data->head, data, shell);
	}
	else
	{
		shell->last_exit = data->exit;
		data->valid_input = 0;
	}
	free(input);
}

void	tokenize(t_parse *data, t_shell *shell,
		t_sigaction *sa_int, t_sigaction *sa_quit)
{
	char	*input;

	if (data->cmd)
	{
		free_command_stack(data->cmd);
		data->cmd = NULL;
	}
	input = readline("MINISHELL>>> ");
	if (!input)
		handle_eof(shell, data);
	if (ft_strlen(input) == 0)
	{
		free(input);
		data->valid_input = 0;
		return ;
	}
	inside_process_signals(sa_int, sa_quit);
	process_tokens(input, data, shell);
}
