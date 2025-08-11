/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:47:43 by jmondon           #+#    #+#             */
/*   Updated: 2025/01/31 15:26:18 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdarg.h>

void	filt_form(const char *s, t_all *vars)
{
	if (s[vars->pos + 1] == 'c' || s[vars->pos + 1] == '%')
		print_c_per(s, vars, vars->args);
	else if (s[vars->pos + 1] == 's')
		ft_print_s(vars, vars->args);
	else if (s[vars->pos + 1] == 'd' || s[vars->pos + 1] == 'i'
		|| s[vars->pos + 1] == 'u')
		ft_print_diu(s, vars, vars->args);
	else if (s[vars->pos + 1] == 'x' || s[vars->pos + 1] == 'X')
		ft_print_hex(s, vars, vars->args);
	else if (s[vars->pos + 1] == 'p')
		ft_print_p(s, vars, vars->args);
}

void	filt_flags(const char *s, t_all *vars)
{
	if (s[vars->pos + 1] == ' ')
		vars->space = 1;
	else if (s[vars->pos + 1] == '#')
		vars->sharp = 1;
	else if (s[vars->pos + 1] == '+')
		vars->plus = 1;
	else if (s[vars->pos + 1] == '-')
		vars->dash = 1;
	else if (s[vars->pos + 1] == '0')
		vars->zero = 1;
	else if (s[vars->pos + 1] == '.')
	{
		vars->dot = 1;
		if (s[vars->pos + 2] >= 48 && s[vars->pos + 2] <= 57)
		{
			vars->prec = ft_atoi(s + vars->pos + 2, vars);
			vars->pos--;
		}
	}
}

void	pre_filt(const char *s, t_all *vars)
{
	while (ft_strchr(vars->all, s[vars->pos + 1]))
	{
		if (ft_strchr(vars->flags, s[vars->pos + 1]))
			filt_flags(s, vars);
		else if (!vars->dot)
		{
			vars->mfw = ft_atoi(s + vars->pos + 1, vars);
			vars->pos--;
		}
		vars->pos++;
	}
	filt_form(s, vars);
}

void	ft_initialize_flags(t_all *vars)
{
	vars->dash = 0;
	vars->zero = 0;
	vars->sharp = 0;
	vars->space = 0;
	vars->plus = 0;
	vars->mfw = 0;
	vars->dot = 0;
	vars->prec = 0;
}

int	ft_printf(const char *s, ...)
{
	t_all	vars;

	vars.forms = "cspdiuxX%";
	vars.all = "0123456789+-# .";
	vars.flags = "0#+- .";
	vars.pos = 0;
	vars.len = 0;
	va_start(vars.args, s);
	while (s[vars.pos])
	{
		if (s[vars.pos] == '%')
		{
			ft_initialize_flags(&vars);
			pre_filt(s, &vars);
		}
		else
			ft_putchar(s[vars.pos], &vars);
		vars.pos++;
	}
	va_end(vars.args);
	return (vars.len);
}
