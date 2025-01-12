/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:17:53 by nboer             #+#    #+#             */
/*   Updated: 2024/11/24 19:19:14 by nboer            ###   ########.fr       */
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
	char				*file;
	char				*delimiter;
	e_token_type		type;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_env
{
	char			*content;
	struct s_env	*next;
	int				export;
}	t_env;

typedef struct t_shell
{
	t_env	*env_lst;
	char	cwd[PATH_MAX];
	int		stdin;
	int		stdout;
	int		last_exit;
	int		exit;
	int		waitpid;
}	t_shell;

typedef struct s_cmd
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
	int		valid_input;
	int		n_pipes;
	int		n_cmds;
	t_cmd	*cmd; //list of cmds to pass to Nick's execution function
}			t_parse;

typedef struct	s_execution
{
	pid_t	pid;
	int		n_pipes;
	int		index_pipe;
	int		index_prev_pipe;
	int		**pipe_arr;
	int		n_cmds;
	int		index_cmd;
	int		infile;
	int		outfile;
	int		start_in;
	int		start_out;
	t_cmd	*cmd;
} t_execution;

//---------tokenize----------//
void	split_tokens(char *input, t_parse *data);
t_token	*new_token(e_token_type type, char *value);
void 	handle_buffer(t_parse *data, e_token_type token_type);
void	add_token_to_list(t_parse *data, t_token *new_token);
void	classify_token_types(t_parse *data);
char	*create_heredoc(char *delimiter);

//--------utils_token--------//
void	print_tokens(t_token *token_list);
char	*trim_whitespace(char *str);
void 	free_tokens(t_token *head);
//int		validate_input(t_token *tokens);
int		validate_input(t_token *tokens, t_parse *data);
char	*ft_itoa(int n);

//-----------Parse-----------//
void	parse_tokens(t_parse *data);
void	print_command_stack(t_cmd *cmd_stack); //for testing
void	free_command_stack(t_cmd *cmd_stack);

//-----------utils------------//
char	*ft_strdup(const char *src);
int		ft_isspace(char c);
int		ft_isalnum(char c);
char	*get_next_line(int fd);

//-----------signals----------//
void	handle_eof(void);
void	setup_signal_handlers(void);
void	handle_sigquit(int sig);
void	handle_sigint(int sig);

//------tokenize_and_parse-------//
void	tokenize(t_parse *parse, t_shell *shell);

//------handle_struct-------//
void	struct_init(t_parse *data, t_shell *shell);
void	reset_parse(t_parse *data);

//---------env_var---------//
void	replace_env_variables_in_tokens(t_token *tokens, t_parse *data);
char	*replace_variables_in_string(char *input, t_parse *data);

//---------init-----------//
int		t_env_init(t_shell *shell, char **envp);

//---------exec-----------//
char	*path_join(char *path_split, char *cmd_arg);
int		run_ex(t_cmd *cmd, char **path_env);
int		str_error(char *error);
int		handle_file(char *filename, int type);
void	exec_init(t_shell *shell, t_execution *pipex, t_cmd *cmd);
void	update_exec(t_execution *pipex);
void	create_pipes(t_execution *pipex);
pid_t	fork_child(void);
void	get_fd(t_execution *pipex, t_cmd *cmd);
void	clean_pipes(t_execution *pipex, t_cmd *cmd);
void	run_builtin(char **argv, t_shell *shell);
pid_t	run_single_cmd(t_shell *shell, t_execution *pipex, pid_t *pids);
void	run_single_builtin(t_execution *pipex, t_shell *shell);
int		run_path(t_cmd *cmd, char **path_env);
void	run_child_exec(t_execution *pipex, t_shell *shell);
void	waitpids(pid_t *pids, int n_pids, t_shell *shell, pid_t pid_last);
int		setup_redirections(t_cmd *cmd);
void	reset_fds(t_execution *pipex);
void	close_fd_in_out(t_cmd *cmd);
int		run_heredoc(t_execution *pipex);
int		fork_heredoc(t_cmd *cmd);
int		read_heredoc(t_cmd *cmd);
void	cleanup_heredoc(t_cmd *cmd_p);
int		read_line_heredoc(char *file, char* delimiter, int fd);

//---------builtins-----------//
int		builtin_env(char **argv, t_shell *shell);
int		builtin_pwd(char **argv, t_shell *shell);
int		builtin_echo(char **argv);
int		builtin_cd(char **argv, t_shell *shell);
int		builtin_exit(char **argv, t_shell *shell);
int		builtin_unset(char **argv, t_shell *shell);
int		builtin_export(char **argv, t_shell *shell);

//---------env-----------//
int		t_env_init(t_shell *shell, char **envp);
int		env_addback(t_shell *shell, char *envp);
int		env_del(t_shell *shell, char *env);
char	**envlst_to_array(t_shell *shell);
int		lst_len(t_env *lst);
char	*get_path_env(char **path_env);
char	*path_join(char *path_split, char *cmd_arg);
t_env	*get_env_lst(t_shell *shell, char *name);
void	free_t_env(t_shell *shell);

//---------error-----------//
int		str_error(char *error);
int		invalid_option(char *builtin, char *arg);
int		invalid_identifier(char *builtin, char *arg);
int		invalid_filedir(char *file);
int		invalid_filedir_builtin(char *builtin, char *file);
int		syntax_error(char *token);
int 	permission_denied(char *arg);

//---------utils-----------//
void	free_array(char **array);
void	free_envlst(t_env *lst);
void	free_int_array(t_execution *pipex, int i);
t_cmd	*find_cmdlst_index(t_cmd *cmd_lst, int	n);
int		cmdlst_length(t_cmd *cmd_lst);
int		check_num(char *str);
void	export_lst(t_env *env_lst);
void	export_lst_one(t_env *lst);
void	export_reset(t_env *lst);
int		export_check(char *str);
int		export_deldup(t_shell *shell, char *name);
int		export_is_valid(const char *str);
void	print_lst(t_env *lst);
char	*cd_update_path(t_shell *shell, char *str);
int		cd_check_error(int err_status, char *dir);
int 	cd_to_path(t_shell *shell, char *arg);
int		check_dir(char *path);
bool	exit_is_valid(char *pnum);
int		check_folder(char *filename);

//---------minishell-----------//
void	calibrate_exec(t_execution *pipex);
void	exec_mini(t_shell *shell, t_execution *pipex);

#endif
