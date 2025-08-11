/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nums.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:50:29 by jmondon           #+#    #+#             */
/*   Updated: 2025/01/30 15:03:15 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	sign(t_all vars)
{
	if (vars.plus)
		return ('+');
	return ('-');
}

void	ft_print_diu(const char *s, t_all *vars, va_list args)
{
	char	*nbr;
	long	n;
	int		len;
	int		neg;

	n = (long)va_arg(args, int);
	neg = (n < 0 && s && s[vars->pos + 1] != 'u');
	if (neg)
		vars->plus = 0;
	if (n < 0 && s[vars->pos + 1] != 'u')
		n *= -1;
	nbr = ft_diutoa(s, vars, n);
	len = ft_strlen(nbr);
	if (*nbr == '0' && !vars->prec && vars->dot)
		len = 0;
	if (vars->prec < len || !vars->dot)
		vars->prec = len;
	ft_print_nbr(vars, nbr, len, neg);
	vars->len += len;
	free(nbr);
	vars->pos++;
}

void	ft_print_nbr(t_all *vars, char *nbr, int len, int neg)
{
	int	aux_a;
	int	aux_b;

	aux_a = vars->zero && (!vars->dot || vars->prec < 0);
	aux_b = !vars->zero || (vars->dot && !(vars->prec < 0));
	vars->mfw -= (vars->space && !neg && !vars->plus && vars->mfw);
	if ((neg) || vars->plus)
		ft_putnchar(sign(*vars), aux_a, vars);
	else if (vars->space)
		ft_putnchar(' ', vars->zero && !vars->dot, vars);
	if (!vars->dash && vars->mfw > vars->prec
		&& (!vars->dot || vars->prec < 0) && vars->zero)
		ft_putnchar('0', vars->mfw - vars->prec - neg - vars->plus, vars);
	else if (!vars->dash && vars->mfw > vars->prec)
		ft_putnchar(' ', vars->mfw - vars->prec - neg - vars->plus, vars);
	if (neg || vars->plus)
		ft_putnchar(sign(*vars), aux_b, vars);
	else if (vars->space)
		ft_putnchar(' ', !vars->zero || vars->dot, vars);
	ft_putnchar('0', vars->prec - len, vars);
	write(1, nbr, len);
	if (vars->dash && vars->mfw > vars->prec)
		ft_putnchar(' ', vars->mfw - vars->prec - neg - vars->plus, vars);
}
