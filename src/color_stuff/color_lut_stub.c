#include "../include/fractol.h"
/* Stub implementations to satisfy linker after reverting LUT-based color optimization */
void	mark_color_lut_dirty(t_fractol_data *data)
{
	(void)data;
}
void	ensure_color_lut(t_fractol_data *data)
{
	(void)data;
}
