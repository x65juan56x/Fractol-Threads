/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_stuff.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:46 by jmondon           #+#    #+#             */
/*   Updated: 2025/04/30 16:31:36 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

uint32_t	color_iteration(int iter, int max_iter, int color_scheme,
			t_fractol_data *data)
{
	t_rgb		color;
	uint32_t	result;
	double		t;

	if (iter == max_iter)
		return (0x000000FF);
	t = (double)iter / max_iter;
	result = select_color_scheme(t, iter, max_iter, color_scheme);
	if (data && data->invert_mode)
	{
		color.r = (result >> 24) & 0xFF;
		color.g = (result >> 16) & 0xFF;
		color.b = (result >> 8) & 0xFF;
		invert_colors(&color);
		result = color_to_int(color);
	}
	return (result);
}

static void	mandelbrot_scheme(double t, t_rgb *color)
{
	color->r = (uint8_t)(255 * t);
	color->g = (uint8_t)(255 * (0.7 * t));
	color->b = (uint8_t)(255 * (0.4 + 0.6 * t));
}

static void	julia_scheme(double t, t_rgb *color)
{
	color->r = (uint8_t)(255 * (0.5 + 0.5 * sin(3.0 * t)));
	color->g = (uint8_t)(255 * (0.5 + 0.5 * sin(3.0 * t + 2.1)));
	color->b = (uint8_t)(255 * (0.5 + 0.5 * sin(3.0 * t + 4.2)));
}

static void	grayscale_scheme(double t, t_rgb *color)
{
	color->r = (uint8_t)(255 * t);
	color->g = (uint8_t)(255 * t);
	color->b = (uint8_t)(255 * t);
}

uint32_t	select_color_scheme(double t, int iter, int max_iter,
				int color_scheme)
{
	t_rgb	color;

	if (color_scheme == 0)
		mandelbrot_scheme(t, &color);
	else if (color_scheme == 1)
		julia_scheme(t, &color);
	else if (color_scheme == 2)
		poly_color(t, &color);
	else if (color_scheme == 3)
		return (cheshire_cat(iter, max_iter));
	else if (color_scheme == 4)
		return (vintage_stripes(iter, max_iter));
	else if (color_scheme >= 6 && color_scheme <= 9)
		return palette_set_color(color_scheme - 6, t);
	else
		grayscale_scheme(t, &color);
	return (color_to_int(color));
}
