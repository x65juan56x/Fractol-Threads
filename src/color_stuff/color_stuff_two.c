/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_stuff_two.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:43:56 by jmondon           #+#    #+#             */
/*   Updated: 2025/04/30 16:31:29 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

uint32_t	color_to_int(t_rgb color)
{
	return ((color.r << 24) | (color.g << 16) | (color.b << 8) | 0xFF);
}

void	invert_colors(t_rgb *color)
{
	color->r = 255 - color->r;
	color->g = 255 - color->g;
	color->b = 255 - color->b;
}
