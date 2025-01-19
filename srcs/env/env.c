/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:20:22 by nick              #+#    #+#             */
/*   Updated: 2024/11/24 22:01:16 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//Takes an array of environment variables envp, creates a linked list,
//saves a copy of the env_array in SHELL->t_env and returns 0 when SUCCEED.
int	t_env_init(t_shell *shell, char **envp)
{
	t_env	*current;
	t_env	*new_node;
	int		i;

	if (!envp || !envp[0])
		return (-1);
	shell->env_lst = env_add_node(envp[0]);
	if (!shell->env_lst)
		return (-1);
	current = shell->env_lst;
	i = 1;
	while (envp[i])
	{
		new_node = env_add_node(envp[i]);
		if (!new_node)
		{
			free_envlst(shell->env_lst);
			return (-1);
		}
		current->next = new_node;
		current = new_node;
		i++;
	}
	return (0);
}

// Creates a single t_env node with the given environment variable string.
t_env	*env_add_node(char *env_str)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->content = strdup(env_str);
	if (!new_node->content)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

// Adds a new environment variable `value` to the `envp` list. 
// Return int to succes or failure. */
int	env_addback(t_shell *shell, char *envp)
{
	t_env	*current;
	t_env	*new;

	current = shell->env_lst;
	while (current->next)
		current = current->next;
	new = (t_env *) malloc(sizeof(t_env));
	if (!(new))
		return (-1);
	new->content = ft_strdup(envp);
	new->export = 0;
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
int	env_del(t_shell *shell, char *env)
{
	t_env	*lst;
	t_env	*prev;
	int		l;

	lst = shell->env_lst;
	prev = NULL;
	l = ft_strlen(env);
	while (lst)
	{
		if ((!ft_strncmp(lst->content, env, l) && lst->content[l] == '=') || 
			(!ft_strncmp(lst->content, env, strlen(lst->content))))
		{
			if (prev)
				prev->next = lst->next;
			else
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
