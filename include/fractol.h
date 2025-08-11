/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:22:22 by jmondon           #+#    #+#             */
/*   Updated: 2025/08/10 14:10:00 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FRACTOL_H
# define FRACTOL_H

# include "../MLX42/include/MLX42/MLX42.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <math.h>
# include <stdint.h>
# include <pthread.h>
# include "../libraries/ft_printf/ft_printf.h"

# define WIDTH 1080
# define HEIGHT 720
# define DEFAULT_THREADS 12
# define MAX_THREADS 32

/* Fractal type enum */
typedef enum e_fractal_type
{
    FRACTAL_MANDELBROT = 0,
    FRACTAL_JULIA = 1,
    FRACTAL_NOVA = 2
} t_fractal_type;

/* Complex number */
typedef struct s_complex_num
{
    double r; double i;
} t_complex_num;

/* POV */
typedef struct s_pov
{
    double min_x;
    double max_x;
    double min_y;
    double max_y;
} t_pov;

/* Helper structs */
typedef struct s_complex_vars
{
    t_complex_num z;
    t_complex_num c;
} t_complex_vars;

/* Nova container */
typedef struct s_nova
{
    int iter;
    double p;
    t_complex_vars vars;
    t_complex_num zp;
    t_complex_num zpm1;
    t_complex_num zp_minus1;
    t_complex_num p_zpm1;
    t_complex_num div_result;
    t_complex_num z_prev;
    t_complex_num diff;
} t_nova;

/* Color polynomial */
typedef struct s_cpoly
{
    double c0[3];
    double c1[3];
    double c2[3];
    double c3[3];
    double c4[3];
    double c5[3];
    double c6[3];
    double rgb[3];
} t_cpoly;

typedef struct s_rgb
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} t_rgb;

typedef struct s_fractal
{
    t_pov pov;
    t_complex_num num;
    int max_iter;
    int iter;
    uint32_t color;
} t_fractal;

/* Thread job */
typedef struct s_job
{
    int start_y;
    int end_y; /* exclusive */
} t_job;

/* Main state */
typedef struct s_fractol_data
{
    mlx_t *mlx;
    mlx_image_t *img;
    int w;
    int h;
    double zoom;
    double offset_x;
    double offset_y;

    double min_x;
    double max_x;
    double min_y;
    double max_y;
    double dx;
    double dy;

    char *type;
    t_fractal_type fractal_type;
    int base_iter;
    int max_iter;
    t_complex_num julia_c;
    double nova_p;

    int color_scheme;
    bool invert_mode;
    int mouse_x;
    int mouse_y;
    bool show_help;

    mlx_image_t *help_img;
    mlx_image_t *help_text_imgs[12];

    /* Threading */
    int thread_count;
    pthread_t *threads;
    t_job *jobs;
    int frame_id;
    int remaining;
    bool shutdown;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
} t_fractol_data;

/* Errors */
void ft_error(void);

/* View / mapping */
void update_view(t_fractol_data *data);
void init_custom_view(t_pov *view, t_fractol_data *data);
void init_pov(t_pov *pov, int width, int height, double scale);

double pix_to_math(int pix, int mx_pix, double mn_rng, double mx_rng);

/* Rendering */
void render_fractal_with_params(t_fractol_data *data);
void change_fractal(t_fractol_data *data, const char *new_type);
void draw_range(t_fractol_data *d, int y0, int y1);

/* Thread pool */
void thread_pool_init(t_fractol_data *d, int threads);
void thread_pool_destroy(t_fractol_data *d);
void thread_pool_resize(t_fractol_data *d, int new_count);

/* Fractals */
int calculate_mandelbrot(t_complex_num mand_c, int max_iter);
int calculate_julia(t_complex_num julia_z, t_complex_num julia_c, int max_iter);
int calculate_nova(t_complex_num nova_c, int max_iter, double p);
void render_nova(mlx_image_t *img, t_fractol_data *data);

/* Math helpers */
void c_set(t_complex_num *num, double real, double imag);
void c_mul(t_complex_num *a, t_complex_num *b, t_complex_num *result);
void c_div(t_complex_num *a, t_complex_num *b, t_complex_num *result);
void c_pow(t_complex_num *z, double n, t_complex_num *result);
int  is_in_radius(t_complex_num *num, double radius_squared);

/* Color */
uint32_t color_iteration(int iter, int max_iter, int color_scheme, t_fractol_data *data);
uint32_t select_color_scheme(double t, int iter, int max_iter, int color_scheme);
void     poly_color(double t, t_rgb *color);
uint32_t cheshire_cat(int iter, int max_iter);
uint32_t vintage_stripes(int iter, int max_iter);
uint32_t color_to_int(t_rgb color);
void     invert_colors(t_rgb *color);
uint32_t palette_set_color(int palette_index, double t);

/* Help */
void create_help_panel(t_fractol_data *data);
void toggle_help(t_fractol_data *data);

/* Hooks */
void key_handler(mlx_key_data_t keydata, void *param);
void scroll_handler(double xdelta, double ydelta, void *param);
void cursor_handler(double xpos, double ypos, void *param);
void resize_handler(int32_t width, int32_t height, void *param);

/* Libft prototypes */
int   ft_strcmp(const char *s1, const char *s2);
char *ft_strdup(const char *s);
double ft_atof(const char *str);

#endif /* FRACTOL_H */