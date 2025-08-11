/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:58:47 by jmondon           #+#    #+#             */
/*   Updated: 2025/01/31 22:03:47 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ifsharp(const char *s, t_all *vars)
{
	if (s[vars->pos + 1] == 'X')
		return ("0X");
	return ("0x");
}

void	ft_print_hex(const char *s, t_all *vars, va_list args)
{
	unsigned int	n;
	int				len;

	n = va_arg(args, unsigned int);
	len = ft_alng(n, 16);
	if (!n && !vars->prec && vars->dot)
		len = 0;
	if (vars->prec < 0 || vars->prec < len || !vars->dot)
		vars->prec = len;
	if (vars->sharp && n)
		vars->mfw -= 2;
	if (!vars->dash && vars->mfw > vars->prec
		&& (!vars->dot || vars->prec < 0) && vars->zero)
		ft_putnchar('0', (vars->mfw - vars->prec), vars);
	else if (!vars->dash && vars->mfw > vars->prec)
		ft_putnchar(' ', (vars->mfw - vars->prec), vars);
	ft_putnstr(ifsharp(s, vars), 2 * (vars->sharp && n), vars);
	ft_putnchar('0', (vars->prec - len), vars);
	if (len)
		ft_xtoa(s, vars, n, n);
	if (vars->dash && vars->mfw > vars->prec)
		ft_putnchar(' ', vars->mfw - vars->prec, vars);
	vars->pos++;
}

void	ft_right(t_all *vars, size_t n, int len)
{
	if (!vars->dash && vars->mfw > vars->prec && !vars->dot && vars->zero)
	{
		if (vars->plus)
			ft_putnchar(sign(*vars), 1, vars);
		else if (vars->space)
			ft_putnchar(' ', 1, vars);
		write(1, "0x", 2 * vars->zero);
	}
	if (!vars->dash && vars->mfw > vars->prec && !vars->dot && vars->zero)
		ft_putnchar('0', (vars->mfw - vars->prec), vars);
	else if (!vars->dash && vars->mfw > vars->prec)
		ft_putnchar(' ', (vars->mfw - (vars->prec + 2)), vars);
	if (!(!vars->dash && vars->mfw > vars->prec && !vars->dot && vars->zero))
	{
		if (vars->plus)
			ft_putnchar(sign(*vars), 1, vars);
		else if (vars->space)
			ft_putnchar(' ', 1, vars);
		write(1, "0x", 2 * vars->zero);
	}
	write(1, "0x", 2 * !vars->zero);
	ft_putnchar('0', (vars->prec - (len - 2)) * (n != 0), vars);
	ft_putnchar('0', (vars->prec) * (vars->dot && !n), vars);
}

void	ft_left(t_all *vars, int len)
{
	if (vars->dash && vars->mfw > vars->prec && vars->prec - len > 0)
		ft_putnchar(' ', vars->mfw - (vars->prec + 2), vars);
	else if (vars->dash && vars->mfw > vars->prec)
		ft_putnchar(' ', vars->mfw - len, vars);
}

void	ft_print_p(const char *s, t_all *vars, va_list args)
{
	long long	n;
	int			len;

	n = va_arg(args, long long);
	if (!n)
		ft_print_nil(vars);
	else
	{
		len = ft_alngptr(n, 16) + 2;
		len *= !(!n && !vars->prec && vars->dot);
		if (vars->prec < len || !vars->dot)
			vars->prec = len - 2;
		vars->mfw = vars->mfw - (vars->plus || vars->space);
		ft_right(vars, n, len);
		if (len)
			ft_xtoa(s, vars, n, n);
		ft_left(vars, len);
		vars->len += 2;
	}
	vars->pos++;
}
