/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 15:08:46 by nboer             #+#    #+#             */
/*   Updated: 2025/01/19 18:41:53 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	export_is_valid_str(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// remove duplicates in the env list to be exported
int	export_deldup(t_shell *shell, char *name)
{
	t_env	*lst;
	int		len;
	int		ret;

	ret = EXIT_SUCCESS;
	lst = shell->env_lst;
	len = 0;
	while (name[len] && name[len] != '=')
		len++;
	while (lst)
	{
		if (lst->content && !(ft_strncmp(lst->content, name, len)))
		{
			ret = env_del(shell, name);
			return (ret);
		}
		lst = lst->next;
	}
	return (ret);
}

// check if export builtin identifiers are valid
int	export_check(char *str)
{
	int		ret;

	ret = EXIT_SUCCESS;
	if (str[0] == '=' || ft_isdigit(str[0]))
		ret = invalid_identifier("export", str);
	return (ret);
}

// print env variable list in alphabetic order
void	export_lst(t_env *env_lst)
{
	t_env	*lst;
	t_env	*cheapest;
	int		n_printed;
	int		len;

	lst = env_lst;
	len = lst_len(env_lst);
	n_printed = 0;
	while (n_printed < len)
	{
		cheapest = NULL;
		while (lst)
		{
			if ((!cheapest && !lst->export) 
				|| (!lst->export && ft_strncmp(lst->content,
						cheapest->content, strlen(lst->content)) < 0))
				cheapest = lst;
			lst = lst->next;
		}
		export_lst_one(cheapest);
		lst = env_lst;
		n_printed++;
	}
	export_reset(env_lst);
}

// reset env linked list for the next export command
void	export_reset(t_env *lst)
{
	while (lst)
	{
		lst->export = 0;
		lst = lst->next;
	}
}
