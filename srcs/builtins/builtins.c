/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 23:06:21 by nick              #+#    #+#             */
/*   Updated: 2024/11/11 23:38:47 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>


void	builtin_env(t_data *shell)
{
	t_env	*lst;
	
	lst = shell->env_lst;
	while(lst)
	{
		printf("%s\n", lst->content);
		lst = lst->next;
	}
	exit(EXIT_SUCCESS);
}

// write text to the terminal or to a file
void	builtin_echo(char **argv, int n)
{
	// case in which only argument is echo is not yet fixed
	int i;
	
	if (strncmp(argv[1], "echo", 4) == 0)
	{
		i = 2;
		if (!strncmp(argv[i], "-n", 2))
			i++;
		while (argv[i])
		{
			printf("%s", argv[i]);
			if (argv[i + 1])
				printf(" ");
			i++;
		}
	}
	if (n == 1)
		printf("\n");
	exit(EXIT_SUCCESS); // child process needs to be stopped somehow
}

void	builtin_cd(t_data *shell)
{
	(void) shell;
}

void	builtin_pwd(t_data *shell)
{
	(void) shell;
}

void	builtin_export(char **argv, t_data *shell)
{
	char	*str;
	// check if export is the first command

	// case if there is an = sign in argv[2]
	if (argv[2][0] == '=')
	{
		argv[2]++;
		ft_putstr_fd(ft_strjoin(argv[2], " is not a valid string"), 2);
	}
	str = ft_strnstr(argv[2], "=", 1);
	if (str)
		env_addback(shell, str);
	else
		str_error("builtin export: something else\n");
}

int	builtin_unset(char **argv, t_data *shell)
{
	if (!(ft_strncmp(argv[1], "unset", 5)))
		str_error("first argument not unset");
	if (env_del(shell, argv[2]) == -1)
		str_error("No existing env variable match found");
	return (0);
}

void	builtin_exit(t_data *shell)
{
	(void) shell;
}
