/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 12:23:52 by chorange          #+#    #+#             */
/*   Updated: 2019/07/05 18:17:36 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vector		rotate_view(t_vector point, double alpha, double beta)
{
	t_vector tempo;

	tempo = point;
	point.y = tempo.y * cos(alpha) + tempo.z * sin(alpha);
	point.z = -tempo.y * sin(alpha) + tempo.z * cos(alpha);
	tempo = point;
	point.x = tempo.x * cos(beta) - tempo.z * sin(beta);
	point.z = tempo.x * sin(beta) + tempo.z * cos(beta);
	return (point);
}

static t_vector	get_pixel_pisition(int x, int y)
{
	t_vector	pixel_pos;
	double		x3;
	double		y3;

	x3 = (double)VW / (double)CW * (double)x;
	y3 = (double)VW / (double)CW * (double)y;
	pixel_pos = vector_init(x3, y3, 1.0);
	return (pixel_pos);
}

static t_obj	*get_closest_object(double *closest_t, t_vector start,
t_vector dir, t_scene *scene)
{
	double	t;
	int		i;
	t_obj	*closest_obj;

	t = 0.0;
	i = 0;
	closest_obj = NULL;
	
	while (i < scene->c_objs)
	{
		t = ray_intersect_obj(start, dir, &(scene->objs[i]));
		if (t != 0.0 && t < (*closest_t))
		{
			(*closest_t) = t;
			closest_obj = &(scene->objs[i]);
		}
		i++;
	}
	return (closest_obj);
}

void			set_arrows_pos(t_rtv1 *rtv1)
{
	rtv1->scene.arrows[0].center = vector_sum(rtv1->selected->center, (t_vector){0.5, 0.0, 0.0});
	rtv1->scene.arrows[1].center = vector_sum(rtv1->selected->center, (t_vector){0.0, 0.5, 0.0});
	rtv1->scene.arrows[2].center = vector_sum(rtv1->selected->center, (t_vector){0.0, 0.0, 0.5});
}

int get_intersect_arrow(t_rtv1 *rtv1, t_vector start,  t_vector dir)
{
	int i = 0;
	int ret = -1;
	double t = 999999.0;
	double tmp;

	while (i < 3)
	{
		tmp = ray_intersect_arrow(start, dir, &rtv1->scene.arrows[i]);
		if (tmp < t && tmp != 0.0)
		{
			t = tmp;
			ret = i;
		}
		i++;
	}

	return (ret);
}

void			select_object(t_rtv1 *rtv1, int x, int y, t_obj **out)
{
	t_vector		pixel_pos_3d;
	static t_obj	*ptr;
	int arrow;

	ptr = NULL;
	//rtv1->left_mouse_pressed = 1;
	pixel_pos_3d = get_pixel_pisition(x - CW / 2, -y + CH / 2);
	pixel_pos_3d = rotate_view(pixel_pos_3d, rtv1->scene.view_alpha,
			rtv1->scene.view_beta);

	if (rtv1->scene.arrows_on)
	{
		arrow = get_intersect_arrow(rtv1, rtv1->scene.camera.center, pixel_pos_3d);
		if (arrow != -1)
		{
			rtv1->arrow = arrow;
			return;
		}
	}
	rtv1->selected_t = 9999999.9;
	ptr = get_closest_object(&(rtv1->selected_t), rtv1->scene.camera.center,
			pixel_pos_3d, &(rtv1->scene));
	if (ptr)
	{
		*out = ptr;
		rtv1->scene.arrows_on = 1;
		set_arrows_pos(rtv1);
	}
	else
		rtv1->left_mouse_pressed = 0;
}
