/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_julia.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 06:47:33 by jmondon           #+#    #+#             */
/*   Updated: 2025/04/30 16:31:45 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	calculate_julia(t_complex_num julia_z, t_complex_num julia_c, int max_iter)
{
	t_complex_vars	vars;
	int				iter;
	double			temp;

	iter = -1;
	c_set(&vars.z, julia_z.r, julia_z.i);
	c_set(&vars.c, julia_c.r, julia_c.i);
	while (is_in_radius(&vars.z, 4.0) && ++iter < max_iter)
	{
		temp = vars.z.r;
		vars.z.r = vars.z.r * vars.z.r - vars.z.i * vars.z.i + vars.c.r;
		vars.z.i = 2 * temp * vars.z.i + vars.c.i;
	}
	return (iter);
}

void	render_julia(mlx_image_t *img, t_fractol_data *data)
{
	t_fractal		j;
	unsigned int	x;
	unsigned int	y;

	j.max_iter = 150;
	init_pov(&j.pov, WIDTH, HEIGHT, 3.0);
	y = -1;
	while (++y < img->height)
	{
		x = -1;
		while (++x < img->width)
		{
			j.num.r = pix_to_math(x, img->width - 1, j.pov.min_x, j.pov.max_x);
			j.num.i = pix_to_math(y, img->height - 1, j.pov.min_y, j.pov.max_y);
			j.iter = calculate_julia(j.num, data->julia_c, j.max_iter);
			j.color = color_iteration(j.iter, j.max_iter, 1, data);
			mlx_put_pixel(img, x, y, j.color);
		}
	}
}
