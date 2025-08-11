/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:22:14 by jmondon           #+#    #+#             */
/*   Updated: 2025/08/10 14:10:00 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void    ft_error(void)
{
    ft_printf("%s", mlx_strerror(mlx_errno));
    exit(EXIT_FAILURE);
}

static int  set_base_iter(const char *type)
{
    if (!ft_strcmp(type, "mandelbrot"))
        return (100);
    if (!ft_strcmp(type, "julia"))
        return (150);
    if (!ft_strcmp(type, "nova"))
        return (64);
    return (-1);
}

static void assign_enum(t_fractol_data *data)
{
    if (!ft_strcmp(data->type, "mandelbrot"))
        data->fractal_type = FRACTAL_MANDELBROT;
    else if (!ft_strcmp(data->type, "julia"))
        data->fractal_type = FRACTAL_JULIA;
    else if (!ft_strcmp(data->type, "nova"))
        data->fractal_type = FRACTAL_NOVA;
}

static void init_data(t_fractol_data *data, char **av)
{
    data->w = WIDTH;
    data->h = HEIGHT;
    data->zoom = 1.0;
    data->offset_x = 0.0;
    data->offset_y = 0.0;
    data->color_scheme = 0;
    data->invert_mode = false;
    data->show_help = false;
    data->type = ft_strdup(av[1]);
    data->nova_p = 3.0;
    c_set(&data->julia_c, -0.75, 0.11);
    data->base_iter = set_base_iter(data->type);
    if (data->base_iter < 0)
    {
        ft_printf("Fractal desconocido: %s\n", data->type);
        ft_printf("Valores válidos: mandelbrot | julia | nova\n");
        exit(EXIT_FAILURE);
    }
    data->max_iter = data->base_iter;
    data->help_img = NULL;
    assign_enum(data);
    data->mlx = mlx_init(WIDTH, HEIGHT, "Fractol", true);
    if (!data->mlx)
        ft_error();
    data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
    if (!data->img || (mlx_image_to_window(data->mlx, data->img, 0, 0) < 0))
        ft_error();
    thread_pool_init(data, DEFAULT_THREADS);
}

static void if_julia(t_fractol_data *data, int ac, char **av)
{
    if (!ft_strcmp(data->type, "julia"))
    {
        if (ac >= 3)
            data->julia_c.r = ft_atof(av[2]);
        if (ac >= 4)
            data->julia_c.i = ft_atof(av[3]);
    }
}

static void print_help(void)
{
    ft_printf("Usage: ./fractol [FRACTAL_TYPE] [OPTIONAL_PARAMETERS]\n\n");
    ft_printf("Available fractal types:\n");
    ft_printf("  mandelbrot     : Mandelbrot Set\n");
    ft_printf("  julia [re] [im]: Julia Set (default: -0.75 0.11)\n");
    ft_printf("  nova           : Nova Fractal\n");
    ft_printf("Examples:\n");
    ft_printf("  ./fractol mandelbrot\n");
    ft_printf("  ./fractol julia -0.75 0.0\n");
    ft_printf("  ./fractol julia 0.0 1.0\n");
    ft_printf("  ./fractol julia 0.285 0.01\n");
    ft_printf("  ./fractol nova\n");
    ft_printf("Press H in runtime for controls.\n");
    exit(EXIT_SUCCESS);
}

int main(int ac, char **av)
{
    t_fractol_data    data;

    if (ac < 2)
        print_help();
    init_data(&data, av);
    if_julia(&data, ac, av);
    /* First render */
    render_fractal_with_params(&data);
    /* Hooks */
    mlx_key_hook(data.mlx, &key_handler, &data);
    mlx_scroll_hook(data.mlx, &scroll_handler, &data);
    mlx_cursor_hook(data.mlx, &cursor_handler, &data);
    mlx_resize_hook(data.mlx, &resize_handler, &data);
    mlx_loop(data.mlx);
    data.shutdown = true;
    thread_pool_destroy(&data);
    free(data.type);
    mlx_terminate(data.mlx);
    return (EXIT_SUCCESS);
}
