/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 23:06:21 by nick              #+#    #+#             */
/*   Updated: 2024/11/24 14:41:17 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	builtin_env(t_shell *shell)
{
	t_env	*lst;

	lst = shell->env_lst;
	while(lst)
	{
		ft_printf("%s\n", lst->content);
		lst = lst->next;
	}
	exit(EXIT_SUCCESS);
}

// write text to the terminal or to a file
void	builtin_echo(char **argv, int n)
{
	n = 0;

	(void) n;
	int i;
	if (strncmp(argv[0], "echo", 4) == 0)
	{
		i = 2;
		if (!strncmp(argv[i], "-n", 2))
			i++;
		while (argv[i])
		{
			ft_printf("%s", argv[i]);
			if (argv[i + 1])
				ft_printf(" ");
			i++;
		}
	}
	exit(EXIT_SUCCESS);
	// question for prince: how did you save the command line arguments that are filled in?
	// aswer: using a struct with a linked list with each node containing a string (*value)
}

void	builtin_cd(char **argv, t_shell *shell)
{
	t_env	*env;
	t_env	*pwd;
	env = shell->env_lst;
	
	if (argv[1] && argv[2])
		ft_putstr_fd("Error: too many arguments", STDERR_FILENO);
	getcwd(shell->cwd, PATH_MAX);

	// retreive environment variables as linked list
	// Use getcwd() to get the current working directory after a successful chdir() call, then update PWD accordingly.
	// look for PWD = .... and rewrite this string to the given input
		//case relative path ../../path
		//case absolute path nick/42berlin/home
		//case .. moves up directory level
		//case - switches to the previous PWD = So overwrite with env variable OLDPWD which should constantly be updated.

	if (argv[2] == NULL)
		pwd = get_env_lst(env, "PWD")
		
		
		// if (cd cannot change the specified directory -> means doesnt exist or no permissions)
		// error handling and the working directory remains unchanged

}

void	builtin_pwd(t_shell *shell)
{
	// retreive environment variables as linked list
	// check the PWD path en print out its current directory
	// i can use getcwd() here aswell.

	(void) *shell;
}

void	builtin_export(char **argv, t_shell *shell)
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

int	builtin_unset(char **argv, t_shell *shell)
{
	if (env_del(shell, argv[2]) == -1)
		str_error("No existing env variable match found");
	else 
		ft_putstr_fd("ENV variable deleted", 1);
	return (0);
}

void	builtin_exit(char **argv, t_shell *shell) // need to make sure that prince is counting like the normal argv with argv[0] does aswell.
{
	if (argv[2] && argv[2][0] == '-')
		str_error("Error: no options integrated for builtin");
	if (!ft_isalpha(argv[2] 
}
