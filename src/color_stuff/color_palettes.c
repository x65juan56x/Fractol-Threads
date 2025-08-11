/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_palettes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:23:17 by jmondon           #+#    #+#             */
/*   Updated: 2025/04/30 16:31:24 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"
#include <stdio.h>
#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

static void	init_magma(t_cpoly *v)
{
	v->c0[0] = -0.002136485053939582;
	v->c0[1] = -0.000749655052795221;
	v->c0[2] = -0.005386127855323933;
	v->c1[0] = 0.2516605407371642;
	v->c1[1] = 0.6775232436837668;
	v->c1[2] = 2.494026599312351;
	v->c2[0] = 8.353717279216625;
	v->c2[1] = -3.577719514958484;
	v->c2[2] = 0.3144679030132573;
	v->c3[0] = -27.66873308576866;
	v->c3[1] = 14.26473078096533;
	v->c3[2] = -13.64921318813922;
	v->c4[0] = 52.17613981234068;
	v->c4[1] = -27.94360607168351;
	v->c4[2] = 12.94416944238394;
	v->c5[0] = -50.76852536473588;
	v->c5[1] = 29.04658282127291;
	v->c5[2] = 4.23415299384598;
	v->c6[0] = 18.65570506591883;
	v->c6[1] = -11.48977351997711;
	v->c6[2] = -5.601961508734096;
}

void	poly_color(double t, t_rgb *color)
{
	t_cpoly	v;
	int		i;

	init_magma(&v);
	t *= 2.0;
	if (t >= 1.0)
		t = 2.0 - t;
	i = -1;
	while (++i < 3)
	{
		v.rgb[i] = v.c0[i] + t * (v.c1[i] + t * (v.c2[i] + t * (v.c3[i]
						+ t * (v.c4[i] + t * (v.c5[i] + t * v.c6[i])))));
		v.rgb[i] = fmin(fmax(v.rgb[i] * 255.0, 0.0), 255.0);
	}
	color->r = (uint8_t)v.rgb[0];
	color->g = (uint8_t)v.rgb[1];
	color->b = (uint8_t)v.rgb[2];
}

uint32_t	cheshire_cat(int iter, int max_iter)
{
	t_rgb	color;
	double	t;
	double	wave;
	double	freq;

	freq = 10.0;
	t = (double)iter / max_iter;
	wave = 0.5 + 0.5 * sin(t * freq * M_PI);
	color.r = (uint8_t)(140 + wave * (255 - 140));
	color.g = (uint8_t)(0 + wave * 105);
	color.b = (uint8_t)(211 + wave * (180 - 211));
	return (color_to_int(color));
}

uint32_t	vintage_stripes(int iter, int max_iter)
{
	t_rgb	color;
	double	t;
	int		band;

	t = (double)iter / max_iter;
	band = (int)(t * 8.0) % 2;
	if (band == 0)
	{
		color.r = 225;
		color.g = 198;
		color.b = 153;
	}
	else
	{
		color.r = 10;
		color.g = 10;
		color.b = 10;
	}
	return (color_to_int(color));
}

static uint32_t make_rgba(uint8_t r,uint8_t g,uint8_t b){ return ((uint32_t)r<<24)|((uint32_t)g<<16)|((uint32_t)b<<8)|0xFF; }
static uint32_t hex_to_rgba(const char *hex)
{
    unsigned int v=0; sscanf(hex, "%x", &v); return make_rgba((v>>16)&0xFF,(v>>8)&0xFF,v&0xFF); }

static uint32_t palette_pick(const uint32_t *arr, int count, double t)
{ if (t < 0) t = 0; if (t>1) t=1; int idx = (int)(t * (count-1)+0.5); if (idx<0) idx=0; if (idx>=count) idx=count-1; return arr[idx]; }

uint32_t palette_set_color(int palette_index, double t)
{
    static bool inited = false;
    static uint32_t p1[5], p2[5], p3[10], p4[5];
    if (!inited)
    {
        p1[0]=hex_to_rgba("fffcf2"); p1[1]=hex_to_rgba("ccc5b9"); p1[2]=hex_to_rgba("403d39"); p1[3]=hex_to_rgba("252422"); p1[4]=hex_to_rgba("eb5e28");
        p2[0]=hex_to_rgba("f4f1de"); p2[1]=hex_to_rgba("e07a5f"); p2[2]=hex_to_rgba("3d405b"); p2[3]=hex_to_rgba("81b29a"); p2[4]=hex_to_rgba("f2cc8f");
        p3[0]=hex_to_rgba("006466"); p3[1]=hex_to_rgba("065a60"); p3[2]=hex_to_rgba("0b525b"); p3[3]=hex_to_rgba("144552"); p3[4]=hex_to_rgba("1b3a4b"); p3[5]=hex_to_rgba("212f45"); p3[6]=hex_to_rgba("272640"); p3[7]=hex_to_rgba("312244"); p3[8]=hex_to_rgba("3e1f47"); p3[9]=hex_to_rgba("4d194d");
        p4[0]=hex_to_rgba("3d5a80"); p4[1]=hex_to_rgba("98c1d9"); p4[2]=hex_to_rgba("e0fbfc"); p4[3]=hex_to_rgba("ee6c4d"); p4[4]=hex_to_rgba("293241");
        inited = true;
    }
    if (palette_index == 0) return palette_pick(p1,5,t);
    if (palette_index == 1) return palette_pick(p2,5,t);
    if (palette_index == 2) return palette_pick(p3,10,t);
    if (palette_index == 3) return palette_pick(p4,5,t);
    return 0x000000FF;
}
