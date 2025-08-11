/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_text_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:47:05 by jmondon           #+#    #+#             */
/*   Updated: 2025/01/28 18:17:14 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_strchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (1);
		i++;
	}
	return (0);
}

size_t	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putchar(char c, t_all *vars)
{
	write(1, &c, 1);
	vars->len++;
}

void	ft_putnchar(char c, int mfw, t_all *vars)
{
	while (mfw > 0)
	{
		write(1, &c, 1);
		mfw--;
		vars->len++;
	}
}

void	ft_putnstr(char *s, int prec, t_all *vars)
{
	int	i;

	i = 0;
	while (s[i] != 0 && prec > 0)
	{
		write(1, &s[i], 1);
		vars->len++;
		prec--;
		i++;
	}
}
