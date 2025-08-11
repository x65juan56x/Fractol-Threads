/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_please.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:16:50 by jmondon           #+#    #+#             */
/*   Updated: 2025/08/10 14:10:00 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

static void create_help_background(t_fractol_data *data)
{
    unsigned int    x;
    unsigned int    y;

    data->help_img = mlx_new_image(data->mlx, data->w, data->h);
    if (!data->help_img)
        return ;
    y = -1;
    while (++y < data->help_img->height)
    {
        x = -1;
        while (++x < data->help_img->width)
            mlx_put_pixel(data->help_img, x, y, 0x0000007F);
    }
    mlx_image_to_window(data->mlx, data->help_img, 0, 0);
}

static void create_help_texts(t_fractol_data *data)
{
    int         i;
    const char  *help_texts[12];

    help_texts[0] = "--- CONTROLS ---";
    help_texts[1] = "ESC: Exit program";
    help_texts[2] = "1-3: Change fractal type";
    help_texts[3] = "+/-: Adjust iterations";
    help_texts[4] = "Arrow keys: Move through the fractal";
    help_texts[5] = "Mouse wheel: Zoom in/out";
    help_texts[6] = "R: Restore zoom and position";
    help_texts[7] = "C: Change color scheme";
    help_texts[8] = "I: Invert colors";
    help_texts[9] = "S/W: Adjust Nova's exponent";
    help_texts[10] = "T/Y: Dec/Inc threads";
    help_texts[11] = "H: Show/hide this help";
    i = -1;
    while (++i < 12)
        data->help_text_imgs[i] = mlx_put_string(data->mlx,
                help_texts[i],
                data->w / 2 - 140,
                100 + i * 32);
}

static void hide_help_elements(t_fractol_data *data)
{
    int i;

    if (data->help_img)
        data->help_img->enabled = false;
    i = -1;
    while (++i < 12)
        if (data->help_text_imgs[i])
            data->help_text_imgs[i]->enabled = false;
}

void    create_help_panel(t_fractol_data *data)
{
    create_help_background(data);
    create_help_texts(data);
    hide_help_elements(data);
}

void    toggle_help(t_fractol_data *data)
{
    int i;

    if (!data->help_img)
        create_help_panel(data);
    data->help_img->enabled = data->show_help;
    i = -1;
    while (++i < 12)
        if (data->help_text_imgs[i])
            data->help_text_imgs[i]->enabled = data->show_help;
}
