/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_stuff_resize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:35:39 by jmondon           #+#    #+#             */
/*   Updated: 2025/04/30 16:32:47 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

static void	recreate_main_image(t_fractol_data *data)
{
	if (data->img)
		mlx_delete_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, data->w, data->h);
	if (!data->img || (mlx_image_to_window(data->mlx, data->img, 0, 0) < 0))
		ft_error();
	render_fractal_with_params(data);
}

static void	enable_help_elements(t_fractol_data *data)
{
	int	i;

	data->help_img->enabled = true;
	i = -1;
	while (++i < 12)
		if (data->help_text_imgs[i])
			data->help_text_imgs[i]->enabled = true;
}

static void	recreate_help_panel(t_fractol_data *data)
{
	int	i;

	i = -1;
	if (data->help_img)
	{
		while (++i < 12)
			if (data->help_text_imgs[i])
				mlx_delete_image(data->mlx, data->help_text_imgs[i]);
		mlx_delete_image(data->mlx, data->help_img);
		data->help_img = NULL;
	}
	create_help_panel(data);
	enable_help_elements(data);
}

void	resize_handler(int32_t width, int32_t height, void *param)
{
	t_fractol_data	*data;

	data = (t_fractol_data *)param;
	data->w = width;
	data->h = height;
	recreate_main_image(data);
	if (data->show_help)
		recreate_help_panel(data);
}
