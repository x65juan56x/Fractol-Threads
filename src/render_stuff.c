/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_stuff.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:15:04 by jmondon           #+#    #+#             */
/*   Updated: 2025/08/10 14:12:00 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

/* forward so thread_pool can call */
void draw_range(t_fractol_data *d, int y0, int y1);

/* Compute view bounds + increments once per full render */
void    update_view(t_fractol_data *d)
{
    if (d->w <= 0 || d->h <= 0)
        return ;
    double scale = 1.0 / d->zoom;
    double aspect = (double)d->w / d->h;
    if (d->w >= d->h)
    {
        d->min_x = -scale * aspect + d->offset_x;
        d->max_x =  scale * aspect + d->offset_x;
        d->min_y = -scale + d->offset_y;
        d->max_y =  scale + d->offset_y;
    }
    else
    {
        double inv_aspect = (double)d->h / d->w;
        d->min_x = -scale + d->offset_x;
        d->max_x =  scale + d->offset_x;
        d->min_y = -scale * inv_aspect + d->offset_y;
        d->max_y =  scale * inv_aspect + d->offset_y;
    }
    if (d->w > 1)
        d->dx = (d->max_x - d->min_x) / (double)(d->w - 1);
    else
        d->dx = 0.0;
    if (d->h > 1)
        d->dy = (d->max_y - d->min_y) / (double)(d->h - 1);
    else
        d->dy = 0.0;
}

static inline int  fast_mandelbrot(double cr, double ci, int max_iter)
{
    double x = cr;
    double y = ci;
    double q = (x - 0.25) * (x - 0.25) + y * y;
    if (q * (q + (x - 0.25)) <= 0.25 * y * y)
        return (max_iter);
    if ((x + 1.0) * (x + 1.0) + y * y <= 0.0625)
        return (max_iter);
    double zr = 0.0, zi = 0.0, zr2 = 0.0, zi2 = 0.0;
    int i = 0;
    while (zr2 + zi2 <= 4.0 && i < max_iter)
    {
        zi = 2.0 * zr * zi + ci;
        zr = zr2 - zi2 + cr;
        zr2 = zr * zr;
        zi2 = zi * zi;
        ++i;
    }
    return (i);
}

void draw_range(t_fractol_data *d, int y0, int y1)
{
    int y = y0;
    double ci = d->min_y + d->dy * y0;
    while (y < y1)
    {
        double cr = d->min_x;
        int x = 0;
        if (d->fractal_type == FRACTAL_MANDELBROT)
        {
            while (x < d->w)
            {
                int iter = fast_mandelbrot(cr, ci, d->max_iter);
                uint32_t color = color_iteration(iter, d->max_iter, d->color_scheme, d);
                mlx_put_pixel(d->img, x, y, color);
                cr += d->dx; ++x;
            }
        }
        else if (d->fractal_type == FRACTAL_JULIA)
        {
            while (x < d->w)
            {
                t_complex_num z; z.r = cr; z.i = ci;
                int iter = calculate_julia(z, d->julia_c, d->max_iter);
                uint32_t color = color_iteration(iter, d->max_iter, d->color_scheme, d);
                mlx_put_pixel(d->img, x, y, color);
                cr += d->dx; ++x;
            }
        }
        else if (d->fractal_type == FRACTAL_NOVA)
        {
            while (x < d->w)
            {
                t_complex_num c; c.r = cr; c.i = ci;
                int iter = calculate_nova(c, d->max_iter, d->nova_p);
                uint32_t color = color_iteration(iter, d->max_iter, d->color_scheme, d);
                mlx_put_pixel(d->img, x, y, color);
                cr += d->dx; ++x;
            }
        }
        ci += d->dy; ++y;
    }
}

void    render_fractal_with_params(t_fractol_data *d)
{
    if (!d || !d->img)
        return ;
    update_view(d);
    if (!d->threads || d->thread_count <= 1)
    {
        draw_range(d, 0, d->h);
        return ;
    }
    /* Prepare jobs */
    int rows_per = d->h / d->thread_count;
    int extra = d->h % d->thread_count;
    int y = 0;
    for (int i = 0; i < d->thread_count; ++i)
    {
        d->jobs[i].start_y = y;
        int take = rows_per + (i < extra ? 1 : 0);
        y += take;
        d->jobs[i].end_y = y;
    }
    pthread_mutex_lock(&d->mtx);
    d->remaining = d->thread_count;
    d->frame_id++;
    pthread_cond_broadcast(&d->cond);
    while (d->remaining > 0)
        pthread_cond_wait(&d->cond, &d->mtx);
    pthread_mutex_unlock(&d->mtx);
}

void    change_fractal(t_fractol_data *d, const char *new_type)
{
    if (!d || !new_type)
        return ;
    if (!ft_strcmp(new_type, "mandelbrot"))
        d->fractal_type = FRACTAL_MANDELBROT;
    else if (!ft_strcmp(new_type, "julia"))
        d->fractal_type = FRACTAL_JULIA;
    else if (!ft_strcmp(new_type, "nova"))
        d->fractal_type = FRACTAL_NOVA;
    if (d->fractal_type == FRACTAL_MANDELBROT)
        d->base_iter = 100;
    else if (d->fractal_type == FRACTAL_JULIA)
        d->base_iter = 150;
    else
        d->base_iter = 64;
    d->max_iter = d->base_iter;
    render_fractal_with_params(d);
}
