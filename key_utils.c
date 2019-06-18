/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 12:11:52 by chorange          #+#    #+#             */
/*   Updated: 2019/06/17 19:20:22 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int	mouse_pressed(int button, int x, int y, t_rtv1 *rtv1)
{
	int pressed_button;
	rtv1->prev_x = x;
	rtv1->prev_y = y;
	rtv1->selected_t = 9999999.9;
	if (y < 0)
		return (0);
	if (button == SDL_BUTTON_LEFT)
	{
		if (x < 1000)
			select_object(rtv1, x, y);
		else
		{
			pressed_button = LIBUI_IsButtonPressed(x - CH, y, rtv1->buttons, rtv1->c_buttons);
			if (pressed_button == -1)
				return (0);
			rtv1->buttons[pressed_button].is_pressed = 1;
			if (!ft_strcmp(rtv1->buttons[pressed_button].function, "New Sphere"))
			{
				rtv1->scene.objs[rtv1->scene.c_objs].type = sphere;
				rtv1->scene.objs[rtv1->scene.c_objs].center = (t_vector){0.0, 0.0, 7.0};
				rtv1->scene.objs[rtv1->scene.c_objs].rot = (t_vector){0.0, 0.0, 0.0};
				rtv1->scene.objs[rtv1->scene.c_objs].radius = 1.0;
				rtv1->scene.objs[rtv1->scene.c_objs].specular = 20.0;
				rtv1->scene.objs[rtv1->scene.c_objs].rgb = (t_rgb){rand()%255, rand()%255, rand()%255};
				rtv1->scene.objs[rtv1->scene.c_objs].reflective = 0.0;
				rtv1->scene.objs[rtv1->scene.c_objs].tex = -1;

				rtv1->scene.c_objs++;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "New Cylinder"))
			{
				rtv1->scene.objs[rtv1->scene.c_objs].type = cylinder;
				rtv1->scene.objs[rtv1->scene.c_objs].center = (t_vector){0.0, 0.0, 7.0};
				rtv1->scene.objs[rtv1->scene.c_objs].dir = (t_vector){0.0, 1.0, 0.0};
				rtv1->scene.objs[rtv1->scene.c_objs].rot = (t_vector){0.0, 0.0, 0.0};
				rtv1->scene.objs[rtv1->scene.c_objs].radius = 1.0;
				rtv1->scene.objs[rtv1->scene.c_objs].rgb = (t_rgb){rand()%255, rand()%255, rand()%255};
				rtv1->scene.objs[rtv1->scene.c_objs].specular = 20.0;
				rtv1->scene.objs[rtv1->scene.c_objs].reflective = 0.0;
				rtv1->scene.objs[rtv1->scene.c_objs].tex = -1;

				rtv1->scene.c_objs++;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "New Cone"))
			{
				rtv1->scene.objs[rtv1->scene.c_objs].type = cone;
				rtv1->scene.objs[rtv1->scene.c_objs].center = (t_vector){0.0, 0.0, 7.0};
				rtv1->scene.objs[rtv1->scene.c_objs].dir = (t_vector){0.0, 1.0, 0.0};
				rtv1->scene.objs[rtv1->scene.c_objs].rot = (t_vector){0.0, 0.0, 0.0};
				rtv1->scene.objs[rtv1->scene.c_objs].rgb = (t_rgb){rand()%255, rand()%255, rand()%255};
				rtv1->scene.objs[rtv1->scene.c_objs].angle = 0.2;
				rtv1->scene.objs[rtv1->scene.c_objs].specular = 20.0;
				rtv1->scene.objs[rtv1->scene.c_objs].reflective = 0.0;
				rtv1->scene.objs[rtv1->scene.c_objs].tex = -1;

				rtv1->scene.c_objs++;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "New Plane"))
			{
				rtv1->scene.objs[rtv1->scene.c_objs].type = plane;
				rtv1->scene.objs[rtv1->scene.c_objs].center = (t_vector){0.0, 0.0, 7.0};
				rtv1->scene.objs[rtv1->scene.c_objs].dir = (t_vector){0.0, 1.0, 0.0};
				rtv1->scene.objs[rtv1->scene.c_objs].rot = (t_vector){0.0, 0.0, 0.0};
				rtv1->scene.objs[rtv1->scene.c_objs].rgb = (t_rgb){rand()%255, rand()%255, rand()%255};
				rtv1->scene.objs[rtv1->scene.c_objs].specular = 20.0;
				rtv1->scene.objs[rtv1->scene.c_objs].reflective = 0.0;
				rtv1->scene.objs[rtv1->scene.c_objs].tex = -1;
				rtv1->scene.c_objs++;
			}
			
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Delete Object"))
			{	
				if (!rtv1->scene.c_objs)
					return (0);
				int selected_id;

				selected_id = (rtv1->selected - &rtv1->scene.objs[0])/*sizeof(t_obj)*/;

				rtv1->scene.objs[selected_id] = rtv1->scene.objs[rtv1->scene.c_objs - 1];
				rtv1->scene.c_objs--;
				/*if (!rtv1->scene.c_objs)
					rtv1->selected = NULL;
				else
					rtv1->selected = &rtv1->scene.objs[0];*/
			}

			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Save Scene"))
			{	
				save(rtv1);
			}

			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Radius+"))
			{	
				rtv1->selected->radius += 0.1;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Radius-") && rtv1->selected->radius > 0.1)
			{	
				rtv1->selected->radius -= 0.1;
			}

			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Angle+"))
			{	
				rtv1->selected->angle += 0.1;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Angle-"))
			{	
				rtv1->selected->angle -= 0.1;
			}

			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Specular+"))
			{	
				rtv1->selected->specular += 20.0;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Specular-") && rtv1->selected->specular > 0.0)
			{	
				rtv1->selected->specular -= 20.0;
			}

			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Reflective+") && rtv1->selected->reflective < 1.0)
			{	
				rtv1->selected->reflective += 0.1;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Reflective-") && rtv1->selected->reflective > 0.0)
			{	
				rtv1->selected->reflective -= 0.1;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Texture1"))
			{	
				rtv1->selected->tex = 0;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Texture2"))
			{	
				rtv1->selected->tex = 1;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Texture3"))
			{	
				rtv1->selected->tex = 2;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Texture4"))
			{	
				rtv1->selected->tex = 3;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Texture5"))
			{	
				rtv1->selected->tex = 4;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Rand Color"))
			{	
				rtv1->selected->tex = -1;
				rtv1->selected->rgb = (t_rgb){rand()%255, rand()%255, rand()%255};
			}
		/*	else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Edit"))
			{	
				pthread_t thread;
				pthread_create(&thread, NULL, edit, rtv1);
			}*/
		} 
	}
	/*else if (button == 4)
		rtv1->scene.camera.center.z -= 0.2;
	else if (button == 5)
		rtv1->scene.camera.center.z += 0.2;*/
	else if (button == SDL_BUTTON_RIGHT)
		rtv1->right_mouse_pressed = 1;
	else if (button == SDL_BUTTON_MIDDLE)
		rtv1->mid_mouse_pressed = 1;
	//provider(rtv1);
	return (0);
}

int	mouse_release(int button, int x, int y, t_rtv1 *rtv1)
{
	(void)x;
	(void)y;
	(void)button;
	rtv1->left_mouse_pressed = 0;
	rtv1->right_mouse_pressed = 0;
	rtv1->mid_mouse_pressed = 0;
	if (x > 1000)
	{
		int i = 0;
		while (i < rtv1->c_buttons)
		{
			(rtv1->buttons)[i].is_pressed = 0;
			i++;
		}
	}
	//provider(rtv1);
	return (0);
}

t_vector rot_v(t_vector P, t_vector ax, double a)
{
	t_vector ret;

	double m[9];

	double cos_a = cos(a);
	double sin_a = sin(a);
	double one_min_cos_a = 1 - cos_a;
	double xy = ax.x * ax.y;
	double xz = ax.x * ax.z;
	double yz = ax.y * ax.z;

	m[0] = cos_a + one_min_cos_a * ax.x * ax.x;
	m[1] = one_min_cos_a * xy - sin_a * ax.z;//
	m[2] = one_min_cos_a * xz + sin_a * ax.y;//
	m[3] = one_min_cos_a * xy + sin_a * ax.z;
	m[4] = cos_a + one_min_cos_a * ax.y * ax.y;
	m[5] = one_min_cos_a * yz - sin_a * ax.x;
	m[6] = one_min_cos_a * xz - sin_a * ax.y;
	m[7] = one_min_cos_a * yz + sin_a * ax.x;
	m[8] = cos_a + one_min_cos_a * ax.z * ax.z;

	ret.x = P.x * m[0] + P.y * m[1] + P.z * m[2];
	ret.y = P.x * m[3] + P.y * m[4] + P.z * m[5];
	ret.z = P.x * m[6] + P.y * m[7] + P.z * m[8];

	return ret;
}

int	mouse_move(int x, int y, t_rtv1 *rtv1)
{
	int dx;
	int dy;

	if (rtv1->right_mouse_pressed)
	{
		dx = x - rtv1->prev_x;
		dy = y - rtv1->prev_y;
		rtv1->scene.view_alpha += atan((double)dy * 0.001);
		rtv1->scene.view_beta += atan((double)dx * 0.001);
	}
	else if (rtv1->left_mouse_pressed)
	{
		dx = x - rtv1->prev_x;
		dy = y - rtv1->prev_y;
		if (!(rtv1->selected))
			rtv1->selected = &(rtv1->scene.objs[0]);
		rtv1->selected->center.x += 0.001 * dx * rtv1->selected_t;
		rtv1->selected->center.y -= 0.001 * dy * rtv1->selected_t;
	}
	else if (rtv1->mid_mouse_pressed)
	{
		dx = x - rtv1->prev_x;
		dy = y - rtv1->prev_y;
		if (!(rtv1->selected))
			rtv1->selected = &(rtv1->scene.objs[0]);
		if (rtv1->selected->type == sphere)
		{
			rtv1->selected->rot.y -= 0.05 * dx;
			rtv1->selected->rot.x += 0.05 * dy;
		}
		else
		{
			rtv1->selected->dir = vector_normalize(rot(rtv1->selected->dir, (t_vector){-0.05 * dy, 0.05 * dx, 0.0}));
			//rtv1->selected->dir = vector_normalize(rot_v(rtv1->selected->dir, (t_vector){1.0, 0.0,0.0}, -0.05 * dy));
			//rtv1->selected->dir = vector_normalize(rot_v(rtv1->selected->dir, (t_vector){0.0,1.0,0.0}, 0.05 * dx));
			//rtv1->selected->rot.y -= 0.05 * dx;
			//rtv1->selected->rot.x += 0.05 * dy;
		}
	}
	rtv1->prev_x = x;
	rtv1->prev_y = y;
	//provider(rtv1);
	return (0);
}

int	key_pressed(int key, t_rtv1 *rtv1)
{
	if (!(rtv1->selected))
		rtv1->selected = &(rtv1->scene.objs[0]);

	if (key == SDLK_ESCAPE)
		exit(1);
	else if (key == SDLK_RIGHT)
		rtv1->scene.camera.center.x += 0.5;
	else if (key == SDLK_LEFT)
		rtv1->scene.camera.center.x -= 0.5;
	else if (key == SDLK_UP)
		rtv1->scene.camera.center.y += 0.5;
	else if (key == SDLK_DOWN)
		rtv1->scene.camera.center.y -= 0.5;
	else if (key == SDLK_KP_PLUS)
		rtv1->selected->center.z += 0.2;
	else if (key == SDLK_KP_MINUS)
		rtv1->selected->center.z -= 0.2;
	else if (key == SDLK_s)
	{
		if (rtv1->scene.shadows_on == 1)
			rtv1->scene.shadows_on = 0;
		else
			rtv1->scene.shadows_on = 1;
	}
	//provider(rtv1);
	return (0);
}
