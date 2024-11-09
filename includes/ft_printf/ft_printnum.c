/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnum.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 14:49:11 by nboer             #+#    #+#             */
/*   Updated: 2024/06/05 18:48:10 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printnum(int n)
{
	long	num;
	int		len;

	len = 0;
	num = n;
	if (num < 0)
	{
		len++;
		ft_putchar('-');
		num *= -1;
	}
	if (num >= 10)
	{
		len += ft_printnum(num / 10);
		len += ft_printnum(num % 10);
	}
	if (num <= 9)
	{
		len++;
		ft_putchar(num + '0');
	}
	return (len);
}

int	ft_print_unsigned(unsigned int n)
{
	int		len;

	len = 0;
	if (n >= 10)
	{
		len += ft_printnum(n / 10);
		len += ft_printnum(n % 10);
	}
	if (n <= 9)
	{
		len++;
		ft_putchar(n + '0');
	}
	return (len);
}

int	ft_print_hex(int nbr, char c)
{
	char			i;
	unsigned int	number;
	char			*base;
	int				len;

	len = 1;
	if (c == 'X')
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	number = nbr;
	if (number >= 16)
		len += ft_print_hex((number / 16), c);
	i = base[number % 16];
	write(1, &i, 1);
	return (len);
}
