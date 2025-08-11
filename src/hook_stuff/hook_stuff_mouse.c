/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_stuff_mouse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:30:00 by jmondon           #+#    #+#             */
/*   Updated: 2025/08/10 10:30:00 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

static void	get_world_units_per_pixel(t_fractol_data *data, double *ux, double *uy)
{
	const double scale = 1.0 / data->zoom;
	if (data->w >= data->h)
	{
		const double aspect = (double)data->w / data->h;
		*ux = (2.0 * scale * aspect) / (double)(data->w - 1);
		*uy = (2.0 * scale) / (double)(data->h - 1);
	}
	else
	{
		const double inv_aspect = (double)data->h / data->w;
		*ux = (2.0 * scale) / (double)(data->w - 1);
		*uy = (2.0 * scale * inv_aspect) / (double)(data->h - 1);
	}
}

static void	calculate_mouse_coords(t_fractol_data *data, t_complex_num *mouse)
{
	double ux;
	double uy;

	get_world_units_per_pixel(data, &ux, &uy);
	mouse->r = data->offset_x + (data->mouse_x - data->w / 2.0) * ux;
	mouse->i = data->offset_y + (data->mouse_y - data->h / 2.0) * uy;
}

static void	apply_zoom(t_fractol_data *data, double ydelta, double zoom_speed)
{
	if (ydelta > 0)
		data->zoom *= zoom_speed;
	else
		data->zoom /= zoom_speed;
	if (data->zoom < 1e-12)
		data->zoom = 1e-12;
	if (data->zoom > 1e12)
		data->zoom = 1e12;
}

static void	update_offset(t_fractol_data *data, t_complex_num *mouse)
{
	double ux;
	double uy;

	get_world_units_per_pixel(data, &ux, &uy);
	data->offset_x = mouse->r - (data->mouse_x - data->w / 2.0) * ux;
	data->offset_y = mouse->i - (data->mouse_y - data->h / 2.0) * uy;
}

void	scroll_handler(double xdelta, double ydelta, void *param)
{
	t_fractol_data	*data;
	t_complex_num	mouse;
	double			zoom_speed;

	(void)xdelta;
	data = (t_fractol_data *)param;
	if (ydelta == 0)
		return ;
	zoom_speed = 1.1;
	calculate_mouse_coords(data, &mouse);
	apply_zoom(data, ydelta, zoom_speed);
	update_offset(data, &mouse);
	render_fractal_with_params(data);
}

void	cursor_handler(double xpos, double ypos, void *param)
{
	t_fractol_data	*data;

	data = (t_fractol_data *)param;
	data->mouse_x = (int)xpos;
	data->mouse_y = (int)ypos;
}
