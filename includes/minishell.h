/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:17:53 by nboer             #+#    #+#             */
/*   Updated: 2024/11/09 20:09:00 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <fcntl.h>
# include <limits.h>
# include <errno.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "../includes/Libft/libft.h"

typedef struct s_token
{
	char			*content;
	// 
	//
}	t_token;

typedef struct s_env
{
	char			*content;
	struct s_env	*next;
}	t_env;

// if one single struct will be too messy or data is often irrelevant in parsing/exec we could separate between token struct and execution struct
typedef struct s_data
{
	t_env		*env_lst; // linkedlst featuring the current envp
	char*		cmd;
	int			exit; // to quit minishell
}	t_data;

typedef struct	s_execution
{
	pid_t	pid;
	int		n_pipes; // to know when i reach the last pipe 			/PRINCE
	int		index_pipe; // to track the pipe where to write in
	int		index_prev_pipe; // to track the pipe where to read from
	int		**pipe_arr;
	int		n_cmds; // to know how often i need to fork // PRINCE: I NEED TO KNOW THE AMOUNT OF COMMANDS THERE ARE.
	int		index_cmd;
	int		infile; //first file to read from // PRINCE
	int		outfile; // file to output		// PRINCE 
} t_execution;

typedef struct s_exec
{
	pid_t		ex_pid; // process ID number, if 0 -> process = child
	int			ex_fdin; // FD in for pipe
	int			ex_fdout; // FD out for pipe
	int			ex_tag_out; // numbertag that indicates whether the outfile is read/write/append
	int			ex_p_exit; //expand latest exit status of the most recently executed foreground pipe. (case $?)
	char*		arg; // or cmd?
}	t_exec;

/* TOKENIZATION */

/* PARSE */

/* EXECUTE */
char	*path_join(char *path_split, char *cmd_arg);
void	run_ex(char *arg, char **path_env);
int		str_error(char *error);
int		handle_file(char *filename, int type);
void	exec_init(t_execution *pipex, int argc, char **argv);
void	update_exec(t_execution *pipex);
void	create_pipes(t_execution *pipex);
pid_t	fork_child(void);
void	get_fd(t_execution *pipex);
void	clean_pipes(t_execution *pipex);
int		is_builtin(t_execution *pipex, char **argv);
int		run_builtin(int	n, char **argv);
void	waitpids(pid_t *pids, int n);

/* BUILTINS */
void	builtin_env(t_data *shell);
void	builtin_echo(char **argv, int n);
void	builtin_cd

/* ENV */
int		t_env_init(t_data *shell, char **envp);
int		env_addback(t_data *shell, char *envp);
int		env_del(t_data *shell, char *env);
char	**envlst_to_array(t_data *shell);
int		lst_len(t_env *lst);

char	*get_path_env(char **path_env);
char	*path_join(char *path_split, char *cmd_arg);

/* ERROR */
int		str_error(char *error);

/* UTILS ?*/
void	free_array(char **array);
void	free_envlst(t_env *lst);
void	struct_init(t_data *shell);
void	free_int_array(t_execution *pipex, int i);

/*MINISHELL*/
void	minishell(char **argv, int argc, t_data *shell, t_execution *pipex, char **env);


#endif
