/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:26:12 by nboer             #+#    #+#             */
/*   Updated: 2025/01/19 18:49:13 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Transforms lst into array of char*
char	**envlst_to_array(t_shell *shell)
{
	t_env	*lst;
	char	**array;
	int		len;
	int		i;

	i = 0;
	lst = shell->env_lst;
	len = lst_len(lst);
	array = (char **) malloc(sizeof(char *) * (len + 1));
	if (!array)
		return (NULL);
	while (lst)
	{
		array[i] = ft_strdup(lst->content);
		if (!array[i])
		{
			free_array(array);
			return (NULL);
		}
		lst = lst->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

//traverses through a t_env list and returns its length
int	lst_len(t_env *lst)
{
	int	len;

	len = 0;
	while (lst)
	{
		len++;
		lst = lst->next;
	}
	return (len);
}

// search for an env variable name in the linked list and return a pointer.
t_env	*get_env_lst(t_shell *shell, char *name)
{
	t_env	*lst;
	int		len;

	len = ft_strlen(name);
	lst = shell->env_lst;
	if (!lst)
		return (NULL);
	while (lst)
	{
		if ((!(ft_strncmp(name, lst->content, len)) 
				&& (lst->content[len] == '=')))
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}
