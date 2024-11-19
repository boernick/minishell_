/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:53:08 by nboer             #+#    #+#             */
/*   Updated: 2024/11/19 15:11:24 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec_mini(t_shell *shell, t_execution *pipex, char **env)
{
	(void) shell;

	pid_t		*pids;
	t_cmd		cmd_lst;
	int			i;
	
	ft_putstr_fd("calibrate exec..\n", 2);
	calibrate_exec(&cmd_lst);
	ft_putstr_fd("initiating exec..\n", 2);
	exec_init(pipex, &cmd_lst); // tellen hoeveel cmd args er zijn en het opslaan in struct
	if (pipex->n_pipes > 0)
		create_pipes(pipex);
	pids = malloc(pipex->n_cmds * sizeof(pid_t));
	if (!pids)
		str_error("malloc failed");
	i = 0;
	while (pipex->index_cmd < pipex->n_cmds)
	{
		pids[i] = fork_child();
		if (pids[i++] == 0) //case child
		{
			if (cmd_lst.is_builtin == 1)
				run_builtin(is_builtin(cmd_lst.argv), cmd_lst.argv, shell);
			else
			{
				get_fd(pipex);
				clean_pipes(pipex);
				run_ex(cmd_lst.cmd, env);
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
	t_execution	pipex;
	t_shell		shell;
	t_parse		parse;

	(void) argv; //til now I didn't have any reason to keep it
	struct_init(&parse); //function to initiate the struct and set some parameters
	t_env_init(&shell, envp);
	if (argc != 1)
	{
		printf("\"./minishell\" must be the only argument\n");
		return (0);
	}
	// tokenize_and_parse(&parse);
	exec_mini(&shell, &pipex, envp); 
	// while (shell.exit == 0) //while no exit signal
	// {
	// 	//handle signals
	// 	if (1) //TO-DO if input is correct
	// 		minishell(argv, argc, &shell, &pipex, envp); //run minishell
	// }
	//free t_env
	return (0);
}
