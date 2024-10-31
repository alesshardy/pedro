/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrellie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 16:10:34 by pgrellie          #+#    #+#             */
/*   Updated: 2023/11/23 16:38:16 by pgrellie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pedro_lib.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*dest;
	size_t			x;

	dest = s;
	x = 0;
	while (x < n)
	{
		*dest = 0;
		dest++;
		x++;
	}
}
