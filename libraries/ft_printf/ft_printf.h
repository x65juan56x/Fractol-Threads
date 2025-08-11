/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:47:39 by jmondon           #+#    #+#             */
/*   Updated: 2025/01/31 21:59:17 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct s_all
{
	va_list	args;
	char	*forms;
	char	*all;
	char	*flags;
	int		dash;
	int		zero;
	int		sharp;
	int		space;
	int		plus;
	int		mfw;
	int		dot;
	int		prec;
	int		pos;
	int		len;
}		t_all;

/*-FT_PRINTF------------------------------------------------------------------*/

int		ft_printf(const char *str, ...);

void	ft_initialize_flags(t_all *vars);

void	pre_filt(const char *s, t_all *vars);

void	filt_flags(const char *s, t_all *vars);

void	filt_form(const char *s, t_all *vars);

/*-FT_PRINTF_TEXT_UTILS-------------------------------------------------------*/

int		ft_strchr(const char *s, int c);

size_t	ft_strlen(char *str);

void	ft_putchar(char c, t_all *vars);

void	ft_putnchar(char c, int mfw, t_all *vars);

void	ft_putnstr(char *s, int prec, t_all *vars);

/*-FT_PRINTF_NUMS_UTILS-------------------------------------------------------*/

int		ft_atoi(const char *str, t_all *vars);

int		ft_alng(unsigned long long n, int base);

int		ft_alngptr(unsigned long long n, int base);

char	*ft_diutoa(const char *s, t_all *vars, long i);

void	ft_xtoa(const char *s, t_all *vars, unsigned long n, unsigned int ifz);

/*-PRINT_CHARS----------------------------------------------------------------*/

void	print_c_per(const char *s, t_all *vars, va_list args);

void	ft_print_s(t_all *vars, va_list args);

void	ft_print_nil(t_all *vars);

/*-PRINT_NUMS-----------------------------------------------------------------*/

char	sign(t_all vars);

void	ft_print_diu(const char *s, t_all *vars, va_list args);

void	ft_print_nbr(t_all *vars, char *nbr, int len, int neg);

/*-PRINT_HEX------------------------------------------------------------------*/

char	*ifsharp(const char *s, t_all *vars);

void	ft_print_hex(const char *s, t_all *vars, va_list args);

void	ft_right(t_all *vars, size_t n, int len);

void	ft_left(t_all *vars, int len);

void	ft_print_p(const char *s, t_all *vars, va_list args);

#endif