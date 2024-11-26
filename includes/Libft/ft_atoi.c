/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:02:29 by nboer             #+#    #+#             */
/*   Updated: 2024/07/21 13:54:33 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	is_neg;
	int	num;

	i = 0;
	is_neg = 1;
	num = 0;
	while ((nptr[i] == 32) || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-')
	{
		is_neg *= -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while ((nptr[i] >= '0') && (nptr[i] <= '9'))
	{
		num = 10 * num + nptr[i] - '0';
		i++;
	}
	return (is_neg * num);
}
/*
int	main(void)
{
	int result = atoi("+45445");
	int myresult = ft_atoi("+45445");
	printf("%d\n", result);
	printf("%d\n", myresult);

	return (0);
}*/
