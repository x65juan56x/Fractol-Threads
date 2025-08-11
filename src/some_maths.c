/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   some_maths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:22:59 by jmondon           #+#    #+#             */
/*   Updated: 2025/04/30 16:37:06 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	init_pov(t_pov *pov, int width, int height, double scale)
{
	double	aspect_ratio;

	aspect_ratio = (double)width / height;
	if (width >= height)
	{
		pov->min_x = -1.0 * scale * aspect_ratio;
		pov->max_x = 1.0 * scale * aspect_ratio;
		pov->min_y = -1.0 * scale;
		pov->max_y = 1.0 * scale;
	}
	else
	{
		aspect_ratio = (double)height / width;
		pov->min_x = -1.0 * scale;
		pov->max_x = 1.0 * scale;
		pov->min_y = -1.0 * scale * aspect_ratio;
		pov->max_y = 1.0 * scale * aspect_ratio;
	}
}

double	pix_to_math(int pix, int mx_pix, double mn_rng, double mx_rng)
{
	return (mn_rng + ((double)pix / mx_pix) * (mx_rng - mn_rng));
}

void	init_custom_view(t_pov *view, t_fractol_data *data)
{
	double	scale;
	double	aspect_ratio;
	double	inv_aspect;

	scale = 1.0 / data->zoom;
	aspect_ratio = (double)data->w / data->h;
	if (data->w >= data->h)
	{
		view->min_x = -scale * aspect_ratio + data->offset_x;
		view->max_x = scale * aspect_ratio + data->offset_x;
		view->min_y = -scale + data->offset_y;
		view->max_y = scale + data->offset_y;
	}
	else
	{
		inv_aspect = (double)data->h / data->w;
		view->min_x = -scale + data->offset_x;
		view->max_x = scale + data->offset_x;
		view->min_y = -scale * inv_aspect + data->offset_y;
		view->max_y = scale * inv_aspect + data->offset_y;
	}
}
