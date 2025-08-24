/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_mandelbrot.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:01:50 by jmondon           #+#    #+#             */
/*   Updated: 2025/04/30 16:31:51 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	calculate_mandelbrot(t_complex_num mandel_c, int max_iter)
{
	t_complex_vars	vars;
	int				iter;
	double			temp;

	iter = -1;
	c_set(&vars.z, 0, 0);
	c_set(&vars.c, mandel_c.r, mandel_c.i);
	while (is_in_radius(&vars.z, 4.0) && ++iter < max_iter)
	{
		temp = vars.z.r;
		vars.z.r = vars.z.r * vars.z.r - vars.z.i * vars.z.i + vars.c.r;
		vars.z.i = 2 * temp * vars.z.i + vars.c.i;
	}
	return (iter);
}

// void	render_mandelbrot(mlx_image_t *img, t_fractol_data *data)
// {
// 	t_fractal		m;
// 	unsigned int	x;
// 	unsigned int	y;

// 	m.max_iter = data->max_iter;
// 	init_pov(&m.pov, WIDTH, HEIGHT, 2.0);
// 	y = -1;
// 	while (++y < img->height)
// 	{
// 		x = -1;
// 		while (++x < img->width)
// 		{
// 			m.num.r = pix_to_math(x, img->width - 1, m.pov.min_x, m.pov.max_x);
// 			m.num.i = pix_to_math(y, img->height - 1, m.pov.min_y, m.pov.max_y);
// 			m.iter = calculate_mandelbrot(m.num, m.max_iter);
// 			m.color = color_iteration(m.iter, m.max_iter, 0, data);
// 			mlx_put_pixel(img, x, y, m.color);
// 		}
// 	}
// }
