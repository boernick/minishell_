/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 18:34:37 by nboer             #+#    #+#             */
/*   Updated: 2025/01/19 18:38:56 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//loop through array and free all values
void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

// returns cmd node pointer at a given index
t_cmd	*find_cmdlst_index(t_cmd *cmd_lst, int n)
{
	t_cmd	*lst;

	lst = cmd_lst;
	while (n >= 0)
	{
		lst = lst->next;
		n--;
	}
	return (lst);
}

// traverses through cmd list and returns its lenght
int	cmdlst_length(t_cmd *cmd_lst)
{
	t_cmd	*lst;
	int		i;

	i = 0;
	lst = cmd_lst;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

// prints the content of every t_env with newline
void	print_lst(t_env *lst)
{
	while (lst)
	{
		ft_putstr_fd(lst->content, 2);
		ft_putstr_fd("\n", 2);
		lst = lst->next;
	}
}
