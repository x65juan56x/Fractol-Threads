/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_nums_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:47:05 by jmondon           #+#    #+#             */
/*   Updated: 2025/01/31 22:01:38 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_atoi(const char *str, t_all *vars)
{
	unsigned int	i;
	long			value;

	i = 0;
	value = 0;
	while (str[i] >= 48 && str[i] <= 57)
	{
		value = (value * 10) + str[i] - 48;
		i++;
		vars->pos++;
	}
	return ((int)value);
}

int	ft_alng(unsigned long long n, int base)
{
	int	l;

	l = 0;
	if (n <= 0)
	{
		l = 1;
		n = -n;
	}
	while (n > 0)
	{
		n /= base;
		l++;
	}
	return (l);
}

int	ft_alngptr(unsigned long long n, int base)
{
	int	l;

	l = 0;
	while (n > 0)
	{
		n /= base;
		l++;
	}
	return (l);
}

char	*ft_diutoa(const char *s, t_all *vars, long i)
{
	int		l;
	char	*a;

	if (s[vars->pos + 1] == 'u')
		i = (unsigned int)i;
	l = ft_alng(i, 10);
	a = (char *)malloc((l + 1) * sizeof(char));
	if (!a)
		return (NULL);
	a[l] = '\0';
	if (i < 0)
	{
		a[0] = '-';
		i *= -1;
	}
	else if (i == 0)
		a[0] = '0';
	while (l > 0 && a[l - 1] != '-')
	{
		a[l - 1] = (i % 10) + '0';
		i = i / 10;
		l--;
	}
	return (a);
}

void	ft_xtoa(const char *s, t_all *vars, unsigned long n, unsigned int ifz)
{
	int		mod;
	char	*base;

	base = "0123456789abcdef";
	if (s[vars->pos + 1] == 'X')
		base = "0123456789ABCDEF";
	if (n > 0 || !ifz)
	{
		mod = n % 16;
		ifz = 1;
		ft_xtoa(s, vars, n / 16, ifz);
		ft_putchar(base[mod], vars);
		if (n == 0)
			return ;
	}
}
