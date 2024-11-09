/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:55:35 by nboer             #+#    #+#             */
/*   Updated: 2024/06/12 14:55:35 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	check_param(va_list args, char var)
{
	if (var == 'c')
		ft_putchar(va_arg(args, int));
	else if (var == 's')
		return (ft_printstr(va_arg (args, char *)));
	else if (var == 'p')
		return (ft_checkp(va_arg(args, unsigned long)));
	else if (var == 'd' || var == 'i')
		return (ft_printnum(va_arg(args, int)));
	else if (var == 'u')
		return (ft_print_unsigned(va_arg(args, unsigned int)));
	else if (var == 'x' || var == 'X')
		return (ft_print_hex(va_arg(args, int), var));
	else if (var == '%')
		ft_putchar('%');
	return (1);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	int		len;
	va_list	args;

	va_start(args, str);
	i = 0;
	len = 0;
	if (str == 0)
		return (-1);
	while (str[i] != '\0')
	{
		if (str[i] == '%')
		{
			len += check_param(args, str[i + 1]);
			i++;
		}
		else
		{
			ft_putchar(str[i]);
			len++;
		}
		i++;
	}
	va_end(args);
	return (len);
}
