/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_nova.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:04:10 by jmondon           #+#    #+#             */
/*   Updated: 2025/04/30 16:31:55 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	calculate_nova(t_complex_num nova_c, int max_iter, double p)
{
	t_nova	n_v;

	n_v.iter = -1;
	n_v.p = p;
	c_set(&n_v.vars.z, 1.0, 0.0);
	c_set(&n_v.vars.c, nova_c.r, nova_c.i);
	while (++n_v.iter < max_iter)
	{
		n_v.z_prev.r = n_v.vars.z.r;
		n_v.z_prev.i = n_v.vars.z.i;
		c_pow(&n_v.vars.z, n_v.p, &n_v.zp);
		c_pow(&n_v.vars.z, n_v.p - 1.0, &n_v.zpm1);
		c_set(&n_v.zp_minus1, n_v.zp.r - 1.0, n_v.zp.i);
		c_set(&n_v.p_zpm1, n_v.p * n_v.zpm1.r, n_v.p * n_v.zpm1.i);
		c_div(&n_v.zp_minus1, &n_v.p_zpm1, &n_v.div_result);
		n_v.vars.z.r = n_v.vars.z.r - n_v.div_result.r + n_v.vars.c.r;
		n_v.vars.z.i = n_v.vars.z.i - n_v.div_result.i + n_v.vars.c.i;
		n_v.diff.r = n_v.vars.z.r - n_v.z_prev.r;
		n_v.diff.i = n_v.vars.z.i - n_v.z_prev.i;
		if (n_v.diff.r * n_v.diff.r + n_v.diff.i * n_v.diff.i < 0.0001)
			break ;
	}
	return (n_v.iter);
}

// void	render_nova(mlx_image_t *img, t_fractol_data *data)
// {
// 	t_fractal		n;
// 	unsigned int	x;
// 	unsigned int	y;

// 	n.max_iter = 64;
// 	init_pov(&n.pov, WIDTH, HEIGHT, 1.25);
// 	y = -1;
// 	while (++y < img->height)
// 	{
// 		x = -1;
// 		while (++x < img->width)
// 		{
// 			n.num.r = pix_to_math(x, img->width - 1, n.pov.min_x, n.pov.max_x);
// 			n.num.i = pix_to_math(y, img->height - 1, n.pov.min_y, n.pov.max_y);
// 			n.iter = calculate_nova(n.num, n.max_iter, data->nova_p);
// 			n.color = color_iteration(n.iter, n.max_iter, 2, data);
// 			mlx_put_pixel(img, x, y, n.color);
// 		}
// 	}
// }
