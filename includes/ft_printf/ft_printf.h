/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 19:01:40 by nboer             #+#    #+#             */
/*   Updated: 2024/06/12 18:54:55 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>

int				ft_printf(const char *str, ...);
int				ft_printstr(char *s);
int				ft_printnum(int n);
void			ft_putchar(char c);
int				check_param(va_list args, char var);
int				ft_print_hex(int nbr, char c);
int				ft_print_unsigned(unsigned int n);
int				ft_printp(unsigned long ptr);
int				ft_checkp(unsigned long ptr);
void			ft_putstr(char *str);

#endif
