/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 12:11:52 by chorange          #+#    #+#             */
/*   Updated: 2019/06/12 22:52:08 by chorange         ###   ########.fr       */
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
				rtv1->scene.objs[rtv1->scene.c_objs].reflective = 0.0;

				rtv1->scene.c_objs++;
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Delete Sphere"))
			{	
				if (!rtv1->scene.c_objs)
					return (0);
				int selected_id;

				selected_id = (&rtv1->scene.objs[0] - rtv1->selected)/sizeof(t_obj);

				rtv1->scene.objs[selected_id] = rtv1->scene.objs[rtv1->scene.c_objs - 1];
				rtv1->scene.c_objs--;
			}
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

		rtv1->selected->rot.y -= 0.05 * dx;
		rtv1->selected->rot.x += 0.05 * dy;
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
