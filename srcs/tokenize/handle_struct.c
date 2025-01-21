#include "../../includes/minishell.h"

// Initializes the fields of the t_data struct for starting a new shell instance.
void	struct_init(t_parse *data, t_shell *shell)
{
	shell->exit = 0;
	shell->last_exit = 0;
	data->cmd = NULL;
	data->exit = 0;
	data->head = NULL;
	data->tail = NULL;
	data->buf_index = 0;
	data->in_double_quote = 0;
	data->in_single_quote = 0;
}

// Resets specific fields in t_data struct to prepare for processing a new command.
void	reset_parse(t_parse *data)
{
	data->head = NULL;
	data->tail = NULL;
	data->in_double_quote = 0;
	data->in_single_quote = 0;
}
