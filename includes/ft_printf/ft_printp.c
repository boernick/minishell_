/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 19:50:32 by nboer             #+#    #+#             */
/*   Updated: 2024/06/05 18:19:14 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_checkp(unsigned long ptr)
{
	int	count;

	count = 0;
	if (ptr == 0)
		count += ft_printstr("(nil)");
	else
	{
		count += ft_printstr("0x");
		count += ft_printp(ptr);
	}
	return (count);
}

int	ft_printp(unsigned long ptr)
{
	char			i;
	unsigned long	number;
	char			*base;
	int				len;

	len = 0;
	base = "0123456789abcdef";
	number = (unsigned long) ptr;
	if (number >= 16)
	{
		len += ft_printp(number / 16);
	}
	i = base[number % 16];
	write(1, &i, 1);
	len++;
	return (len);
}
