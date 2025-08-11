/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   some_complex_maths.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:22:27 by jmondon           #+#    #+#             */
/*   Updated: 2025/04/30 16:36:59 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	c_set(t_complex_num *num, double real, double imag)
{
	num->r = real;
	num->i = imag;
}

void	c_mul(t_complex_num *a, t_complex_num *b, t_complex_num *result)
{
	result->r = a->r * b->r - a->i * b->i;
	result->i = a->r * b->i + a->i * b->r;
}

void	c_div(t_complex_num *a, t_complex_num *b, t_complex_num *result)
{
	double	denom;

	denom = b->r * b->r + b->i * b->i;
	result->r = (a->r * b->r + a->i * b->i) / denom;
	result->i = (a->i * b->r - a->r * b->i) / denom;
}

void	c_pow(t_complex_num *z, double n, t_complex_num *result)
{
	double	r;
	double	a;

	r = sqrt(z->r * z->r + z->i * z->i);
	a = atan2(z->i, z->r);
	result->r = pow(r, n) * cos(a * n);
	result->i = pow(r, n) * sin(a * n);
}

int	is_in_radius(t_complex_num *num, double radius_squared)
{
	return (num->r * num->r + num->i * num->i <= radius_squared);
}
