/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:17:53 by nboer             #+#    #+#             */
/*   Updated: 2024/11/24 14:29:23 by nboer            ###   ########.fr       */
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
# include <linux/limits.h>
# include <errno.h>
# include <dirent.h>
# include <sys/types.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../includes/Libft/libft.h"
# include "../includes/ft_printf/ft_printf.h"
# define BUFFER_SIZE 1024

typedef enum
{
	TOKEN_WORD,				//all cmds and args before being split up
	TOKEN_ARG,
	TOKEN_CMD,
	TOKEN_FLAG_ARG,
	TOKEN_FILE_ARG,
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
	char				*file; //file name
	e_token_type		type; // append/write/read
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_env
{
	char			*content;
	struct s_env	*next;
}	t_env;

typedef struct t_shell
{
	t_env	*env_lst;
	char	*cwd[PATH_MAX];
	int		stdin;
	int		stdout;
}	t_shell;

typedef struct s_cmd // does prince
{
	char			*cmd;
	char			**argv;
	int				index;
	int				fdin;
	int				fdout;
	bool			is_builtin;
	struct s_cmd	*next;
	t_redirect		*redir;
}				t_cmd;

typedef struct	s_parse
{
	t_token	*head; //start of token linked list
	t_token	*tail; //end of token linked list
	char	buffer[BUFFER_SIZE]; //buffer to store string while tokenizing
	int		buf_index; //index for buffer while tokenizing
	int		in_single_quote; //var to keep track between in and out of single quote
	int		in_double_quote; //var to keep track between in and out of double quote
	int		last_exit_status; //needs to be implemented!
	int		exit;
	int		n_pipes;
	int		n_cmds;
	t_cmd	*cmd; //list of cmds to pass to Nick's execution function
}			t_parse;

typedef struct	s_execution // only need 1 of those, for example for n_pipes once. Its not needed 'per command'
{
	pid_t	pid;
	int		n_pipes; // to know when i reach the last pipe
	int		index_pipe; // to track the pipe where to write in
	int		index_prev_pipe; // to track the pipe where to read from
	int		**pipe_arr;
	int		n_cmds; // to know how often i need to fork // PRINCE: I NEED TO KNOW THE AMOUNT OF COMMANDS THERE ARE.
	int		index_cmd;
	int		infile; //first file to read from
	int		outfile; // file to output
	t_cmd	*cmd;
} t_execution;

typedef struct s_exec
{
	t_token		parse;
	t_token		parse_list;
	pid_t		ex_pid; // process ID number, if 0 -> process = child
	int			ex_fdin; // FD in for pipe
	int			ex_fdout; // FD out for pipe
	int			ex_tag_out; // numbertag that indicates whether the outfile is read/write/append
	int			ex_p_exit; //expand latest exit status of the most recently executed foreground pipe. (case $?)
	int			ex_n_cmd; //
}	t_exec;

//---------tokenize----------//
void	tokenize(char *input, t_parse *data);
t_token	*new_token(e_token_type type, char *value);
void 	handle_buffer(t_parse *data, e_token_type token_type);
void	add_token_to_list(t_parse *data, t_token *new_token);
void	classify_token_types(t_parse *data);

//--------utils_token--------//
void	print_tokens(t_token *token_list);
char	*trim_whitespace(char *str);
void 	free_tokens(t_token *head);
int		validate_input(t_token *tokens);
char	*ft_itoa(int n);

//-----------Parse-----------//
void	parse_tokens(t_parse *data);

//-----------utils------------//
char	*ft_strdup(const char *src);
int		ft_isspace(char c);
int		ft_isalnum(char c);

//-----------signals----------//
void	handle_sigint(int sig);

//------tokenize_and_parse-------//
void	tokenize_and_parse(t_parse *parse);

//------handle_struct-------//
void	struct_init(t_parse *shell);
void	reset_parse(t_parse *data);

//---------env_var---------//
void	replace_env_variables_in_tokens(t_token *tokens, t_parse *data);
char	*replace_variables_in_string(char *input, t_parse *data);

//---------init-----------//
//void	struct_init(t_shell *shell);
int		t_env_init(t_shell *shell, char **envp);

//---------exec-----------//
char	*path_join(char *path_split, char *cmd_arg);
void	run_ex(t_cmd *cmd, char **path_env);
int		str_error(char *error);
int		handle_file(char *filename, int type);
void	exec_init(t_execution *pipex, t_cmd *cmd);
void	update_exec(t_execution *pipex);
void	create_pipes(t_execution *pipex);
pid_t	fork_child(void);
void	get_fd(t_execution *pipex, t_cmd *cmd);
void	clean_pipes(t_execution *pipex, t_cmd *cmd);
int		is_builtin(char **argv);
int		run_builtin(int	n, char **argv, t_shell *shell);
void	waitpids(pid_t *pids, int n);
void	setup_redirections(t_cmd *cmd);

//---------builtins-----------//
void	builtin_env(t_shell *shell);
void	builtin_echo(char **argv, int n);
void	builtin_cd(char **argv, t_shell *shell);
void	builtin_exit(char **argv, t_shell *shell);
int		builtin_unset(char **argv, t_shell *shell);


//---------env-----------//
int		t_env_init(t_shell *shell, char **envp);
int		env_addback(t_shell *shell, char *envp);
int		env_del(t_shell *shell, char *env);
char	**envlst_to_array(t_shell *shell);
int		lst_len(t_env *lst);
char	*get_path_env(char **path_env);
char	*path_join(char *path_split, char *cmd_arg);
t_env	*get_env_lst(t_shell *shell, char *name)

//---------error-----------//
int		str_error(char *error);

//---------utils-----------//
void	free_array(char **array);
void	free_envlst(t_env *lst);
void	free_int_array(t_execution *pipex, int i);
t_cmd	*find_cmdlst_index(t_cmd *cmd_lst, int	n);
int		cmdlst_length(t_cmd *cmd_lst);

//---------minishell-----------//
void	calibrate_exec(t_execution *pipex);
void	exec_mini(t_shell *shell, t_execution *pipex, char **env);

#endif
