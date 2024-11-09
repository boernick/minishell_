/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:20:22 by nick              #+#    #+#             */
/*   Updated: 2024/11/09 15:15:21 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Takes an array of environment variables envp, creates a linked list,
//saves a copy of the env_array in SHELL->t_env and returns 0 when SUCCEED.
int	t_env_init(t_data *shell, char **envp)
{
	t_env	*current;
	int		i;
	
	current = (t_env *)malloc(sizeof(t_env));
	if (!(current))
		return (-1);
	current->content = ft_strdup(envp[0]);
	if (!(current->content))
	{
		free_envlst(current);
		return (-1);
	}
	current->next = NULL;
	shell->env_lst = current;
	i = 1;
	while (envp && envp[i])
	{
		current->next = (t_env *)malloc(sizeof(t_env));
		if (!(current->next))
		{
			free_envlst(current);
			return (-1);
		}
		current = current->next;
		current->content = ft_strdup(envp[i]);
		if (!(current->content))
		{
			free_envlst(current);
			return (-1);
		}
		current->next = NULL;
		i++;
	}
	return (0);
}
// Adds a new environment variable `value` to the `envp` list. 
// Return int to succes or failure. */
int	env_addback(t_data *shell, char *envp)
{
	t_env	*current;
	t_env	*new;
	
	current = shell->env_lst;
	while(current->next)
		current = current->next;
	new = (t_env *) malloc(sizeof(t_env));
	if (!(new))
		return (-1);
	new->content = ft_strdup(envp);
	if (!(new->content))
	{
		free(new);
		return (-1);
	}
	new->next = NULL;
	current->next = new;
	return (0);
}

// Deletes an environment variable from the list and
// relinks the remaining nodes.
int	env_del(t_data *shell, char *env)
{
	t_env	*lst;
	t_env	*prev;
	int		l;

	lst = shell->env_lst;
	prev = NULL;
	l = ft_strlen(env);
	while (lst) // for every node
	{
		if (!ft_strncmp(lst->content, env, l) && lst->content[l] == '=')
		{
			if (prev)
				prev->next = lst->next;
			else // case head
				shell->env_lst = lst->next;	
			free(lst->content);
			free(lst);
			return (0);
		}
		prev = lst;
		lst = lst->next;
	}
	return (-1);
}

// Transforms lst into array of char*
char	**envlst_to_array(t_data *shell)
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
		if (!(array[i] = ft_strdup(lst->content)))
		{
			//free_array_2
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
