/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 15:11:02 by nboer             #+#    #+#             */
/*   Updated: 2025/01/19 15:11:14 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// check if exit code is valid
bool	exit_is_valid(char *pnum)
{
	while (*pnum == ' ' || (*pnum >= 9 && *pnum <= 13))
		pnum++;
	if (*pnum == '-' || *pnum == '+')
		pnum++;
	while (ft_isdigit(*pnum))
		pnum++;
	if (*pnum != '\0')
		return (false);
	return (true);
}

//checks if the given string only consists of numbers
int	check_num(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(ft_isdigit(str[i])))
			return (0);
		i++;
	}
	return (1);
}

// print one env variable for export builtin
void	export_lst_one(t_env *lst)
{
	int	i;
	int	equal;

	i = 0;
	equal = 0;
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	while (lst->content[i])
	{
		ft_putchar_fd(lst->content[i], STDOUT_FILENO);
		if (lst->content[i] == '=' && !equal++)
			ft_putchar_fd('"', 1);
		i++;
	}
	if (equal)
		ft_putchar_fd('"', STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	lst->export = 1;
}
