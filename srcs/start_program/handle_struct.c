#include "../../includes/minishell.h"

// Initializes the fields of the t_data struct for starting a new shell instance.
void	struct_init(t_data *shell)
{
	//shell->arg = "cat";
	shell->exit = 0;
	shell->head = NULL;
	shell->tail = NULL;
	shell->buf_index = 0;
	shell->in_double_quote = 0;
	shell->in_single_quote = 0;
}

// Resets specific fields in t_data struct to prepare for processing a new command.
void	reset_data(t_data *data)
{
	data->head = NULL;
	data->tail = NULL;
	data->in_double_quote = 0;
	data->in_single_quote = 0;
}
