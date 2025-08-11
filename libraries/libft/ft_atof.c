/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 06:14:29 by jmondon           #+#    #+#             */
/*   Updated: 2025/04/30 16:33:39 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static double	ft_process_decimals(const char *str, int i, double result)
{
	double	decimal;
	double	power;

	decimal = 0.0;
	power = 0.1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		decimal += (str[i] - '0') * power;
		power *= 0.1;
		i++;
	}
	return (result + decimal);
}

double	ft_atof(const char *str)
{
	int		i;
	int		sign;
	double	result;

	i = 0;
	sign = 1;
	result = 0.0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10.0 + (str[i] - '0');
		i++;
	}
	if (str[i] == '.')
		result = ft_process_decimals(str, i + 1, result);
	return (result * sign);
}
