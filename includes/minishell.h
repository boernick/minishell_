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
	t_cmd	*cmd;
}	t_execution;

typedef struct sigaction	t_sigaction;
typedef t_sigaction			t_sigact;

void	check_temp_files(t_cmd *cmd); //for testing
void	print_temp_files(t_redirect *redir);// for testing
void	process_running_sigint_handler(int signum);
void	process_running_sigquit_handler(int signum);
void	init_signal_handlers(t_sigaction *sa_int, t_sigaction *sa_quit);

//---------env_var_tokenize----------//
char	*get_env_variable(char *var_name, t_parse *data, t_shell *shell);
char	*get_pid_as_string(void);

//---------tokenize----------//
void	split_tokens(char *input, t_parse *data);
t_token	*new_token(t_token_type type, char *value);
void	handle_buffer(t_parse *data, t_token_type token_type);
void	add_token_to_list(t_parse *data, t_token *new_token);
void	classify_token_types(t_parse *data, t_shell *shell);
char	*create_heredoc(void);

//--------utils_token--------//
void	print_tokens(t_token *token_list);
char	*trim_whitespace(char *str); // not used
void	free_tokens(t_token *head);
int		validate_input(t_token *tokens, t_parse *data, t_shell *shell);
char	*ft_itoa(int n);

//-----------Parse-----------//
void	parse_tokens(t_parse *data, t_shell *shell);
void	print_command_stack(t_cmd *cmd_stack); //for testing
void	free_command_stack(t_cmd *cmd_stack);

//-----------utils------------//
char	*ft_strdup(const char *src);
int		ft_isalnum(char c);
char	*get_next_line(int fd);

//-----------signals----------//
void	handle_eof(t_shell *shell, t_parse *parse);
void	setup_signal_handlers(int signum);
void	handle_sigquit(int sig);
void	handle_sigint(int sig);
void	inside_process_signals(t_sigact *sa_int, t_sigact *sa_quit);
void	outside_process_signals(t_sigact *sa_int, t_sigact *sa_quit);

//------tokenize_and_parse-------//
void	tokenize(t_parse *parse, t_shell *shell,
			t_sigaction *sa_int, t_sigaction *sa_quit);

//------handle_struct-------//
void	struct_init(t_parse *data, t_shell *shell);
void	reset_parse(t_parse *data);

//---------env_var---------//
void	replace_env_variables_in_tokens(t_token *tokens,
			t_parse *data, t_shell *shell);
char	*replace_variables_in_string(char *input,
			t_parse *data, t_shell *shell);
char	*replace_variables_in_heredoc(char *input, t_parse *data,
			t_shell *shell);

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
int		syntax_error(char *token);
int		permission_denied(char *arg);
int		cmd_not_found(char *arg);
int		is_a_directory(char *arg);
int		invalid_argument_count(int ret);

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
void	calibrate_exec(t_execution *pipex);
void	exec_mini(t_shell *shell, t_execution *pipex);
void	cleanup(t_parse *parse, t_shell *shell);
void	run_mini(t_parse *parse, t_shell *shell, t_execution *pipex);


#endif
