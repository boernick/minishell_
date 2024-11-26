/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 18:02:10 by nboer             #+#    #+#             */
/*   Updated: 2024/06/12 13:19:39 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *lit, size_t len)
{
	size_t		i;
	size_t		j;
	size_t		l_len;
	char		*big_mod;

	i = 0;
	big_mod = (char *)big;
	l_len = ft_strlen(lit);
	if (big == lit || l_len == 0)
		return (big_mod);
	while (big[i] != '\0' && i + l_len <= len)
	{
		j = 0;
		while (i + j < len && big_mod[i + j] != '\0' && lit[j] != '\0'
			&& big_mod[i + j] == lit[j])
			j++;
		if (j == l_len)
			return (big_mod + i);
		i++;
	}
	return (NULL);
}
/*
int	main(void)
{
    char *s1 = "see FF your retur FF return FF now FF";
    char *s2 = "return";
    size_t len = 32;

    char *result = ft_strnstr(s1, s2, len);

    if (result != NULL)
        printf("Substring found at index: %ld\n", result - s1);
    else
        printf("Substring not found\n");

    return 0;
}
*/
