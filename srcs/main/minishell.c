/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:53:08 by nboer             #+#    #+#             */
/*   Updated: 2024/11/12 23:52:07 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	minishell(char **argv, int argc, t_data *shell, t_execution *pipex, char **env)
{
	(void) shell;

	pid_t		*pids;
	int			i;

	exec_init(pipex, argc, argv); // tellen hoeveel cmd args er zijn en het opslaan in struct
	// if there are pipes
	create_pipes(pipex);
	pids = malloc(pipex->n_cmds * sizeof(pid_t));
	i = 0;
	while (pipex->index_cmd < pipex->n_cmds)
	{
		pids[i] = fork_child();
		if (pids[i++] == 0) //case child
		{
			if (is_builtin(pipex, argv) != -1)
				run_builtin(is_builtin(pipex, argv), argv, shell);
			else
			{
				get_fd(pipex);
				clean_pipes(pipex);
				run_ex(argv[pipex->index_cmd + 2], env);
				exit(EXIT_SUCCESS); // this should be removed?
			}
		}
		else
			update_exec(pipex);
	}
	clean_pipes(pipex);
	waitpids(pids, pipex->n_cmds); // wait for child process, but WHY these inputs in function?
}

int	main(int argc, char *argv[], char **envp)
{
	//t_execution	pipex;
	t_data		shell;

	(void) argv; //til now I didn't have any reason to keep it
	struct_init(&shell); //function to initiate the struct and set some parameters
	t_env_init(&shell, envp);
	if (argc != 1)
	{
		printf("\"./minishell\" must be the only argument\n");
		return (0);
	}
	start_program(&shell);
	//minishell(argv, argc, &shell, &pipex, envp); //run minishell //prince:uncommnented for merging
	// while (shell.exit == 0) //while no exit signal
	// {
	// 	//handle signals
	// 	if (1) //TO-DO if input is correct
	// 		minishell(argv, argc, &shell, &pipex, envp); //run minishell
	// }
	//free t_env
	return (0);
}
