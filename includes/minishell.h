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

typedef enum t_type
{
	TOKEN_WORD,
	TOKEN_ARG,
	TOKEN_CMD,
	TOKEN_FLAG_ARG,
	TOKEN_FILE_ARG,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SKIP
}	t_token_type;

typedef struct s_expand_var
{
	char	result[1024];
	int		res_index;
	int		i;
	int		var_index;
	char	var_name[256];
	char	*input;
}	t_expand_var;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_redirect
{
	char				*file;
	t_token_type		type;
	struct s_redirect	*next;
	int					processed;
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

typedef struct s_parse
{
	t_token	*head;
	t_token	*tail;
	char	buffer[BUFFER_SIZE];
	int		buf_index;
	int		in_single_quote;
	int		in_double_quote;
	int		last_exit_status;
	int		exit;
	int		valid_input;
	int		n_pipes;
	int		n_cmds;
	t_cmd	*cmd;
}			t_parse;

typedef struct s_execution
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
	t_token	*head;
	t_cmd	*cmd;
}	t_execution;

typedef struct sigaction	t_sigaction;
typedef t_sigaction			t_sigact;

//---------env_var_tokenize----------//
char	*get_env_variable(char *var_name, t_parse *data, t_shell *shell);
char	*get_pid_as_string(void);
void	init_expand_var(t_expand_var *expand, char *input);
void	handle_pid_var(char *input, int *i, char *result, int *res_index);
char	*handle_variable(t_expand_var *exp, t_parse *data, t_shell *shell);
void	process_quote_env(char *input, t_parse *data, t_expand_var *exp);

//---------tokenize----------//
void	split_tokens(char *input, t_parse *data);
t_token	*new_token(t_token_type type, char *value);
void	handle_buffer(t_parse *data, t_token_type token_type);
void	add_token_to_list(t_parse *data, t_token *new_token);
void	classify_token_types(t_parse *data, t_shell *shell);
char	*create_heredoc(void);
void	finalize_splitting(t_parse *data);
int		validate_heredoc(t_token *next, t_parse *data);

//--------utils_token--------//
char	*trim_whitespace(char *str); // not used
void	free_tokens(t_token *head);
int		validate_input(t_token *tokens, t_parse *data, t_shell *shell);
void	process_quote(char *input, t_parse *data, int *i);
void	trim_quotes(char *str, t_parse *date);
void	trim_file_quotes(char *str);

//-----------Parse-----------//
void	parse_tokens(t_parse *data, t_shell *shell);
void	free_command_stack(t_cmd *cmd_stack);
int		init_cmd_redir(t_parse *data, t_cmd **current_cmd);
void	add_redirection_to_cmd(t_cmd *cmd, t_redirect *new_redir);
void	add_cmd_to_token(t_token *cur_tkn, t_parse *data,
			t_cmd **current_cmd);
void	add_argument_to_cmd(t_cmd *cmd, char *arg);
void	init_cmd_cmd(t_cmd **current_cmd, t_parse *data, char *cmd_value);
bool	is_builtin_(char *cmd);
void	add_cmd_to_list(t_parse *data, t_cmd *new_cmd);

//-----------signals----------//
void	handle_eof(t_shell *shell, t_parse *parse);
void	setup_signal_handlers(int signum);
void	handle_sigquit(int sig);
void	inside_process_signals(t_sigact *sa_int, t_sigact *sa_quit);
void	outside_process_signals(t_sigact *sa_int, t_sigact *sa_quit);
void	process_running_sigint_handler(int signum);
void	init_signal_handlers(t_sigaction *sa_int, t_sigaction *sa_quit);

//------tokenize_and_parse-------//
void	tokenize(t_parse *parse, t_shell *shell,
			t_sigaction *sa_int, t_sigaction *sa_quit);
int		check_empty_variable(t_token *current_token,
			t_parse *data, t_shell *shell);
void	classify_command_token(t_token *current_token, int *cmd_flag);
void	classify_file_arg(t_token **current_token);
void	classify_word_arg(t_token *current_token);

//------handle_struct-------//
void	struct_init(t_parse *data, t_shell *shell);
void	reset_parse(t_parse *data);
void	init_parse_data(t_parse *data, t_token **cur_tkn, t_cmd **current_cmd);

//---------env_var---------//
void	replace_env_variables_in_tokens(t_token *tokens,
			t_parse *data, t_shell *shell);
char	*replace_variables_in_string(char *input,
			t_parse *data, t_shell *shell);
char	*replace_variables_in_heredoc(char *input, t_parse *data,
			t_shell *shell);
void	reset_expand_var(t_expand_var *expand);

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
void	clean_pipes(t_execution *pipex);
void	run_builtin(char **argv, t_shell *shell);
pid_t	run_single_cmd(t_shell *shell, t_execution *pipex, pid_t *pids);
void	run_single_builtin(t_execution *pipex, t_shell *shell);
int		run_path(t_cmd *cmd, char **path_env);
void	run_child_exec(t_execution *pipex, t_shell *shell);
void	waitpids(pid_t *pids, int n_pids, t_shell *shell, pid_t pid_last);
int		setup_redirections(t_cmd *cmd);
void	reset_fds(t_execution *pipex);
void	close_fd_in_out(t_cmd *cmd);
void	close_start_fds(t_execution *pipex);

//---------heredoc-----------//
int		run_heredoc(t_parse *data, t_redirect *redir,
			char *delimeter, t_shell *shell);
int		fork_heredoc(t_parse *data, t_redirect *redir,
			char *delimeter, t_shell *shell);
int		read_heredoc(t_parse *data, t_redirect *redir,
			char *delimeter, t_shell *shell);
void	cleanup_heredoc(t_cmd *cmd_p);
int		read_line_heredoc(t_parse *data, int fd,
			char *delimeter, t_shell *shell);
int		heredoc_exit(t_redirect *redir, t_parse *data,
			int exit_code, t_shell *shell);
char	*replace_variables_in_heredoc(char *input, t_parse *data,
			t_shell *shell);

//---------builtins-----------//
int		builtin_env(char **argv, t_shell *shell);
int		builtin_pwd(char **argv, t_shell *shell);
int		builtin_echo(char **argv);
int		builtin_cd(char **argv, t_shell *shell);
int		builtin_exit(char **argv, t_shell *shell);
int		builtin_unset(char **argv, t_shell *shell);
int		builtin_export(char **argv, t_shell *shell);

//---------env-----------//
t_env	*env_add_node(char *env_str);
int		t_env_init(t_shell *shell, char **envp);
int		env_addback(t_shell *shell, char *envp);
int		env_del(t_shell *shell, char *env);
char	**envlst_to_array(t_shell *shell);
int		lst_len(t_env *lst);
char	*get_path_env(char **path_env);
char	*path_join(char *path_split, char *cmd_arg);
t_env	*get_env_lst(t_shell *shell, char *name);

//---------error-----------//
int		str_error(char *error);
int		invalid_option(char *builtin, char *arg);
int		invalid_identifier(char *builtin, char *arg);
int		invalid_filedir(char *file);
int		invalid_filedir_builtin(char *builtin, char *file);
int		syntax_error(char *token, t_parse *data);
int		permission_denied(char *arg);
int		cmd_not_found(char *arg);
int		is_a_directory(char *arg);
int		invalid_argument_count(int ret);
int		malloc_error(size_t n);
int		ambiguous_redirect_error(char *token);
void	exit_perror(const char *msg);
int		heredoc_eof_warning(int line_number, char *delimeter, t_parse *data);

//---------utils-----------//
void	free_array(char **array);
void	free_envlst(t_env *lst);
void	free_int_array(t_execution *pipex, int i);
t_cmd	*find_cmdlst_index(t_cmd *cmd_lst, int n);
int		cmdlst_length(t_cmd *cmd_lst);
int		check_num(char *str);
void	export_lst(t_env *env_lst);
void	export_lst_one(t_env *lst);
void	export_reset(t_env *lst);
int		export_check(char *str);
int		export_deldup(t_shell *shell, char *name);
int		export_is_valid_str(const char *str);
void	export_env_var(char *env, t_shell *shell);
void	print_lst(t_env *lst);
char	*cd_update_path(t_shell *shell, char *str);
int		cd_check_error(int err_status, char *dir);
int		cd_to_path(t_shell *shell, char *arg);
int		check_dir(char *path);
bool	exit_is_valid(char *pnum);
int		check_folder(char *filename);

//---------minishell-----------//
void	exec_mini(t_shell *shell, t_execution *pipex);
void	cleanup(t_parse *parse, t_shell *shell, t_execution *pipex);
void	cleanup_child(t_shell *shell, t_execution *pipex);
void	run_mini(t_parse *parse, t_shell *shell, t_execution *pipex);

#endif
