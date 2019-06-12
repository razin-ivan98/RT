/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 10:54:31 by chorange          #+#    #+#             */
/*   Updated: 2019/04/12 11:15:42 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_rgb	color_to_rgb(int color)
{
	t_rgb	rgb;

	rgb.r = color >> 16;
	rgb.g = (color & 0x00ff00) >> 8;
	rgb.b = color & 0x0000ff;
	return (rgb);
}

int		rgb_to_color(t_rgb rgb)
{
	int	color;

	color = rgb.r * 0x010000 + rgb.g * 0x000100 + rgb.b;
	return (color);
}
