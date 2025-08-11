/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_stuff_keys.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:48:13 by jmondon           #+#    #+#             */
/*   Updated: 2025/08/10 14:10:00 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

static void	handle_nav_keys(keys_t key, t_fractol_data *data)
{
	if (key == MLX_KEY_ESCAPE)
		mlx_close_window(data->mlx);
	if (key == MLX_KEY_UP)
		data->offset_y -= 0.1 / data->zoom;
	if (key == MLX_KEY_DOWN)
		data->offset_y += 0.1 / data->zoom;
	if (key == MLX_KEY_LEFT)
		data->offset_x -= 0.1 / data->zoom;
	if (key == MLX_KEY_RIGHT)
		data->offset_x += 0.1 / data->zoom;
	if (key == MLX_KEY_C)
		data->color_scheme = (data->color_scheme + 1) % 10; /* ahora incluye paletas 6-9 */
	if (key == MLX_KEY_I)
		data->invert_mode = !data->invert_mode;
	if (key == MLX_KEY_R)
	{
		data->zoom = 1.0;
		data->offset_x = 0.0;
		data->offset_y = 0.0;
	}
	if (key == MLX_KEY_UP || key == MLX_KEY_DOWN || key == MLX_KEY_LEFT ||
		key == MLX_KEY_RIGHT || key == MLX_KEY_C || key == MLX_KEY_I ||
		key == MLX_KEY_R)
		render_fractal_with_params(data);
}

static void	handle_thread_keys(keys_t key, t_fractol_data *data)
{
	if (key == MLX_KEY_T || key == MLX_KEY_Y)
	{
		int new_count = data->thread_count;
		if (key == MLX_KEY_T && new_count > 1)
			new_count--;
		else if (key == MLX_KEY_Y && new_count < MAX_THREADS)
			new_count++;
		if (new_count != data->thread_count)
		{
			thread_pool_resize(data, new_count);
			render_fractal_with_params(data);
		}
	}
}

static void	handle_nova_keys(keys_t key, t_fractol_data *data)
{
	if (data->fractal_type != FRACTAL_NOVA)
		return ;
	if (key == MLX_KEY_S)
	{
		data->nova_p -= 0.1;
		if (data->nova_p < 2.1)
			data->nova_p = 2.1;
	}
	else if (key == MLX_KEY_W)
	{
		data->nova_p += 0.1;
		if (data->nova_p > 10.0)
			data->nova_p = 10.0;
	}
	if (key == MLX_KEY_S || key == MLX_KEY_W)
		render_fractal_with_params(data);
}

static void	handle_fractal_keys(keys_t key, t_fractol_data *data)
{
	if (key == MLX_KEY_1 || key == MLX_KEY_KP_1)
		change_fractal(data, "mandelbrot");
	if (key == MLX_KEY_2 || key == MLX_KEY_KP_2)
		change_fractal(data, "julia");
	if (key == MLX_KEY_3 || key == MLX_KEY_KP_3)
		change_fractal(data, "nova");
	if (key == MLX_KEY_KP_ADD || key == MLX_KEY_EQUAL)
	{
		data->max_iter += (int)(data->max_iter * 0.1) + 1;
		render_fractal_with_params(data);
	}
	if (key == MLX_KEY_KP_SUBTRACT || key == MLX_KEY_MINUS)
	{
		data->max_iter -= (int)(data->max_iter * 0.1) + 1;
		if (data->max_iter < data->base_iter)
			data->max_iter = data->base_iter;
		render_fractal_with_params(data);
	}
}

static void	handle_help_key(keys_t key, t_fractol_data *data)
{
	if (key == MLX_KEY_H)
	{
		data->show_help = !data->show_help;
		toggle_help(data);
	}
}

void	key_handler(mlx_key_data_t keydata, void *param)
{
	t_fractol_data *data;

	data = (t_fractol_data *)param;
	if (keydata.action == MLX_PRESS)
	{
		handle_nav_keys(keydata.key, data);
		handle_fractal_keys(keydata.key, data);
		handle_nova_keys(keydata.key, data);
		handle_thread_keys(keydata.key, data);
		handle_help_key(keydata.key, data);
	}
}
