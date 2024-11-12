/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:17:53 by nboer             #+#    #+#             */
/*   Updated: 2024/11/12 23:29:50 by nick             ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include "../includes/Libft/libft.h"
# define BUFFER_SIZE 1024

typedef enum
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,			//	<
	TOKEN_REDIR_OUT,		// >
	TOKEN_REDIR_APPEND,		// >>
	TOKEN_HEREDOC			// <<
}	e_token_type;

typedef struct	s_token
{
	e_token_type	type; //type of token
	char			*value; //pointer to string containing token value
	struct s_token	*next; //pointer to next token in linked list
}					t_token;

typedef struct s_redirect
{
	int		file; //file name
	int	to_file; // to a file or to print
	int		redir_type; // append/write/read
	char	*argument; // text to write into
}	t_redirect;

typedef struct s_env
{
	char			*content;
	struct s_env	*next;
}	t_env;

//prichugh: "I cominbined my s_data struct and yours, can seperate mine into a parsing struct"
// if one single struct will be too messy or data is often irrelevant in parsing/exec we could separate between token struct and execution struct
typedef struct	s_data
{
	t_token	*head; //start of token linked list
	t_token	*tail; //end of token linked list
	char	buffer[BUFFER_SIZE]; //buffer to store string while tokenizing
	int		buf_index; //index for buffer while tokenizing
	int		in_single_quote; //var to keep track between in and out of single quote
	int		in_double_quote; //var to keep track between in and out of double quote
	//int		i;
	int		last_exit_status; //needs to be implemented!
	t_env		*env_lst; // linkedlst featuring the current envp
	char*		cmd;
	int			exit; // to quit minishell
}			t_data;

typedef struct	s_execution
{
	pid_t	pid;
	int		n_pipes; // to know when i reach the last pipe 			/PRINCE
	int		index_pipe; // to track the pipe where to write in
	int		index_prev_pipe; // to track the pipe where to read from
	int		**pipe_arr;
	int		n_cmds; // to know how often i need to fork // PRINCE: I NEED TO KNOW THE AMOUNT OF COMMANDS THERE ARE.
	int		index_cmd;
	int		infile; //first file to read from
	int		outfile; // file to output
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
//---------tokenize.c----------//
void	tokenize(char *input, t_data *data);
t_token	*new_token(e_token_type type, char *value);
void 	handle_buffer(t_data *data, e_token_type token_type);
void	add_token_to_list(t_data *data, t_token *new_token);

//--------utils_token.c--------//
void	print_tokens(t_token *token_list);
char	*trim_whitespace(char *str);
void 	free_tokens(t_token *head);
int		validate_input(t_token *tokens);
char	*ft_itoa(int n);

//-----------utils.c------------//
char	*ft_strdup(const char *src);
int		ft_isspace(char c);
int		ft_isalnum(char c);

/* SIGNALNS */
//-----------signals.c----------//
void	handle_sigint(int sig);

 /*Start_program*/
//------start_program.c-------//
void	start_program(t_data *data);

//------handle_struct.c-------//
void	reset_data(t_data *data);
void	struct_init(t_data *shell);

//---------env_var.c---------//
void	replace_env_variables_in_tokens(t_token *tokens, t_data *data);
char	*replace_variables_in_string(char *input, t_data *data);

//---------inti.c-----------//
//void	struct_init(t_data *shell);
int		t_env_init(t_data *shell, char **envp);


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
int		run_builtin(int	n, char **argv, t_data *shell);
void	waitpids(pid_t *pids, int n);

/* BUILTINS */
void	builtin_env(t_data *shell);
void	builtin_echo(char **argv, int n);

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
void	free_int_array(t_execution *pipex, int i);

/*MINISHELL*/
void	minishell(char **argv, int argc, t_data *shell, t_execution *pipex, char **env);



#endif
