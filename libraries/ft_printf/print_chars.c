/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_chars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:46:53 by jmondon           #+#    #+#             */
/*   Updated: 2025/01/31 17:27:13 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	print_c_per(const char *s, t_all *vars, va_list args)
{
	char	c;

	if (s[vars->pos + 1] == 'c')
		c = va_arg(args, int);
	else
		c = '%';
	if (c == '\0')
	{
		if (!vars->dash && vars->mfw > 1 && s[vars->pos + 1] == 'c')
			ft_putnchar(' ', vars->mfw - 1, vars);
		write(1, "\0", 1);
		vars->len++;
		if (vars->dash && vars->mfw - 1 > 0 && s[vars->pos + 1] == 'c')
			ft_putnchar(' ', vars->mfw - 1, vars);
	}
	else
	{
		if (!vars->dash && vars->mfw > 1 && s[vars->pos + 1] == 'c')
			ft_putnchar(' ', vars->mfw - 1, vars);
		ft_putchar(c, vars);
		if (vars->dash && vars->mfw - 1 > 0 && s[vars->pos + 1] == 'c')
			ft_putnchar(' ', vars->mfw - 1, vars);
	}
	vars->pos++;
}

void	ft_print_s(t_all *vars, va_list args)
{
	char	*str;

	str = va_arg(args, char *);
	if (str == NULL && vars->dot && vars->prec < 6)
		str = "";
	else if (!str)
		str = "(null)\0";
	if (!vars->dot || vars->prec > (int)ft_strlen(str) || vars->prec < 0)
		vars->prec = ft_strlen(str);
	if (!vars->dash && vars->mfw - vars->prec > 0)
		ft_putnchar(' ', vars->mfw - vars->prec, vars);
	ft_putnstr(str, vars->prec, vars);
	if (vars->dash && vars->mfw - vars->prec > 0)
		ft_putnchar(' ', vars->mfw - vars->prec, vars);
	vars->pos++;
}

void	ft_print_nil(t_all *vars)
{
	char	*str;

	str = "(nil)";
	if (!vars->dot || vars->prec > (int)ft_strlen(str) || vars->prec < 0)
		vars->prec = ft_strlen(str);
	if (!vars->dash && vars->mfw - vars->prec > 0)
		ft_putnchar(' ', vars->mfw - vars->prec, vars);
	ft_putnstr(str, vars->prec, vars);
	if (vars->dash && vars->mfw - vars->prec > 0)
		ft_putnchar(' ', vars->mfw - vars->prec, vars);
}
