/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 12:11:52 by chorange          #+#    #+#             */
/*   Updated: 2019/07/16 20:50:15 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_list_item_addr list_utils(t_rtv1 *rtv1, int x, int y)
{
	t_list_item_addr ret;

	ret = LIBUI_IsListPressed(x - CH, y, rtv1->lists, rtv1->c_lists);
	return(ret);
}

int	mouse_pressed(int button, int x, int y, t_rtv1 *rtv1)
{
	int pressed_button;
//	int edit_pressed;
	char func[64];
	rtv1->prev_x = x;
	rtv1->prev_y = y;
	
	if (y < 0)
		return (0);
	if (button == SDL_BUTTON_LEFT)
	{
		if (x < 1000)
		{
			select_object(rtv1, x, y, &(rtv1->selected));

		}
		else
		{


			pressed_button = LIBUI_IsButtonPressed(x - CH, y, rtv1->buttons, rtv1->c_buttons);
			if (pressed_button == -1)
			{
				t_list_item_addr ret;
				ret = list_utils(rtv1, x, y);
				if (ret.item == -1)
				{
					rtv1->active_edit = LIBUI_IsEditPressed(x - CH, y, rtv1->edits, rtv1->c_edits);

					if (rtv1->active_edit == -1)
					{
						LIBUI_DisactiveAll(rtv1->edits, rtv1->c_edits, rtv1->lists, rtv1->c_lists);
						return (0);
					}
				}
				else
				{
					rtv1->active_edit = -1;
					ft_strcpy(func, rtv1->lists[ret.list].items[ret.item].function);
					///puts(func);
				}
				rtv1->lists[ret.list].items[ret.item].is_pressed = 1;
			}
			else
			{
				rtv1->active_edit = -1;
				//rtv1->buttons[pressed_button].is_pressed = 1;
				ft_strcpy(func, rtv1->buttons[pressed_button].function);
			}
			if (!ft_strcmp(func, "New Sphere"))
			{
				new_sphere(rtv1);
				refresh_selector_buttons(rtv1);
			}
			else if (!ft_strcmp(func, "New Cylinder"))
			{
				new_cylinder(rtv1);
				refresh_selector_buttons(rtv1);
			}
			else if (!ft_strcmp(func, "New Cone"))
			{
				new_cone(rtv1);
				refresh_selector_buttons(rtv1);
			}
			else if (!ft_strcmp(func, "New Plane"))
			{
				new_plane(rtv1);
				refresh_selector_buttons(rtv1);
			}
			else if (!ft_strcmp(func, "New PointL"))
			{
				new_point(rtv1);
				refresh_selector_buttons(rtv1);
			}
			else if (!ft_strcmp(func, "New PointD"))
			{
				new_directional(rtv1);
				refresh_selector_buttons(rtv1);
			}

			
			else if (!ft_strcmp(func, "Delete Object"))
			{
				int selected_id;
				if (rtv1->selected_light && rtv1->scene.c_lights)
				{
					selected_id = get_light_index_by_id(&rtv1->scene, rtv1->selected_light->id);
					rtv1->scene.lights[selected_id] = rtv1->scene.lights[rtv1->scene.c_lights - 1];
					rtv1->scene.c_lights--;
					rtv1->scene.arrows_on = 0;
					refresh_selector_buttons(rtv1);
					return(0);
				}


				if (!rtv1->scene.c_objs)
					return (0);
				

				selected_id = (rtv1->selected - &rtv1->scene.objs[0])/*sizeof(t_obj)*/;

				rtv1->scene.objs[selected_id] = rtv1->scene.objs[rtv1->scene.c_objs - 1];
				rtv1->scene.c_objs--;
				rtv1->scene.arrows_on = 0;
				refresh_selector_buttons(rtv1);
				/*if (!rtv1->scene.c_objs)
					rtv1->selected = NULL;
				else
					rtv1->selected = &rtv1->scene.objs[0];*/
			}

			else if (!ft_strcmp(func, "Save Scene"))
			{	
				save(rtv1, rtv1->scene_file_name);
			}



			else if (!ft_strcmp(func, "Save As"))
			{
				save_as(rtv1);
			}
			else if (!ft_strcmp(func, "Ambient"))
			{
				rtv1->scene.advanced = (rtv1->scene.advanced ? 0 : 1);
			}
			else if (!ft_strcmp(func, "Soft Shadows"))
			{
				rtv1->scene.soft = (rtv1->scene.soft ? 0 : 1);
			}
			else if (!ft_strcmp(func, "Export BMP"))
			{
				IMG_SavePNG(rtv1->surface, "screens/1.png");
			}


		/*	else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Expand"))
			{
				rtv1->list->expand = rtv1->list->expand ? 0 : 1;
				rtv1->list->ft_redraw(rtv1);
			}*/
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Edit"))
			{	
				if (rtv1->edit_window_active == 0)
					edit(rtv1);
			}
			else if (!ft_strcmp(rtv1->buttons[pressed_button].function, "Selector"))
			{	
				if (rtv1->selector_window_active == 0)
					scene_selector(rtv1);
			}
			else if (!ft_strcmp(func, "lol"))
			{
				if (rtv1->lists[0].is_dropped == 1)
					rtv1->lists[0].is_dropped = 0;
				else
					rtv1->lists[0].is_dropped = 1;
				//printf("\n%d\n\n", rtv1->lists[0].is_dropped);
			}
		}
		rtv1->left_mouse_pressed = 1;
	}
	/*else if (button == 4)
		rtv1->scene.camera.center.z -= 0.2;
	else if (button == 5)
		rtv1->scene.camera.center.z += 0.2;*/
	else if (button == SDL_BUTTON_RIGHT)
		rtv1->right_mouse_pressed = 1;
	else if (button == SDL_BUTTON_MIDDLE)
	{
		select_object(rtv1, x, y, &rtv1->selected);
		rtv1->mid_mouse_pressed = 1;
	}
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
	rtv1->arrow = -1;
	//if (x > 1000)
	{
		int i = 0;
		while (i < rtv1->c_buttons)
		{
			(rtv1->buttons)[i].is_pressed = 0;
			i++;
		}
		i = 0;
		while (i < rtv1->c_lists)
		{
			//rtv1->lists[i].is_dropped = 0;
			int j = 0;
			while (j < rtv1->lists[i].c_items)
			{
				(rtv1->lists)[i].items[j].is_pressed = 0;
				j++;
			}
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
		//if (!(rtv1->selected))
		//rtv1->selected = &(rtv1->scene.objs[0]);
		if (rtv1->selected && rtv1->arrow != -1 && !rtv1->selected_light)
		{
			if (rtv1->arrow == 0)
			{
				if (rtv1->selected->type == triangle)
					move_polygonal(0.001 * dx * rtv1->selected_t * (cos(rtv1->scene.view_beta) > 0 ? 1 : -1), 0.0, 0.0, rtv1);
				else
					rtv1->selected->center.x += 0.001 * dx * rtv1->selected_t * (cos(rtv1->scene.view_beta) > 0 ? 1 : -1);
				//rtv1->selected->center.y -= 0.001 * dy * rtv1->selected_t;
			}
			else if (rtv1->arrow == 1)
			{
				//rtv1->selected->center.x += 0.001 * dx * rtv1->selected_t;
				if (rtv1->selected->type == triangle)
					move_polygonal(0.0, -0.001 * dy * rtv1->selected_t ,0.0, rtv1);
				else
					rtv1->selected->center.y -= 0.001 * dy * rtv1->selected_t;
			}
			else if (rtv1->arrow == 2)
			{
				if (rtv1->selected->type == triangle)
					move_polygonal(0.0, 0.0, 0.001 * dx * rtv1->selected_t * (sin(rtv1->scene.view_beta) > 0 ? 1 : -1), rtv1);
				else
					rtv1->selected->center.z += 0.001 * dx * rtv1->selected_t * (sin(rtv1->scene.view_beta) > 0 ? 1 : -1);
				//rtv1->selected->center.y -= 0.001 * dy * rtv1->selected_t;
			}
			set_arrows_pos(rtv1, 0);
		}
		else if (rtv1->selected_light && rtv1->arrow != -1)
		{
			rtv1->selected_t = vector_length(vector_subt(rtv1->scene.camera.center, rtv1->selected_light->center));
			//printf("%d\n", rtv1->arrow);
			if (rtv1->arrow == 0)
			{
				rtv1->selected_light->center.x += 0.001 * dx * rtv1->selected_t * (cos(rtv1->scene.view_beta) > 0 ? 1 : -1);
				
			}
			else if (rtv1->arrow == 1)
			{
				rtv1->selected_light->center.y -= 0.001 * dy * rtv1->selected_t;
			}
			else if (rtv1->arrow == 2)
			{
				rtv1->selected_light->center.z += 0.001 * dx * rtv1->selected_t * (sin(rtv1->scene.view_beta) > 0 ? 1 : -1);
			}
			set_arrows_pos(rtv1, 1);
		}
		
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
			if (rtv1->selected && rtv1->arrow != -1)
			{
				
				if (rtv1->arrow == 0)
				{
					if (rtv1->selected->type == triangle)
						rot_polygonal(0.05 * dy, 0.0, 0.0, rtv1);
					else
						rtv1->selected->dir = vector_normalize(rot(rtv1->selected->dir, (t_vector){0.05 * dy, 0.0, 0.0}));
					//rtv1->selected->center.y -= 0.001 * dy * rtv1->selected_t;
				}
				else if (rtv1->arrow == 1)
				{
					//rtv1->selected->center.x += 0.001 * dx * rtv1->selected_t;
					if (rtv1->selected->type == triangle)
						rot_polygonal(0.0, 0.05 * dx, 0.0, rtv1);
					else
						rtv1->selected->dir = vector_normalize(rot(rtv1->selected->dir, (t_vector){0.0, 0.05 * dx, 0.0}));
				}
				else if (rtv1->arrow == 2)
				{
					if (rtv1->selected->type == triangle)
						rot_polygonal(0.0, 0.0, 0.05 * dy, rtv1);
					else
						rtv1->selected->dir = vector_normalize(rot(rtv1->selected->dir, (t_vector){0.0, 0.0, 0.05 * dy}));
					//rtv1->selected->center.y -= 0.001 * dy * rtv1->selected_t;
				}


			}

			
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
	else if (rtv1->active_edit != -1)
		LIBUI_InputLetter(key, rtv1->edits, rtv1->active_edit, rtv1->shift_pressed);
	else if (key == SDLK_RIGHT)
	{
		rtv1->scene.camera.center.x += 0.5 * cos(rtv1->scene.view_beta);
		rtv1->scene.camera.center.z += 0.5 * sin(rtv1->scene.view_beta);
	}
	else if (key == SDLK_LEFT)
	{
		rtv1->scene.camera.center.x -= 0.5 * cos(rtv1->scene.view_beta);
		rtv1->scene.camera.center.z -= 0.5 * sin(rtv1->scene.view_beta);
	}
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
	if (key == 1073742049)
		rtv1->shift_pressed = 1;
	//provider(rtv1);
	return (0);
}

int mouse_wheel(int y, t_rtv1 *rtv1)
{

	rtv1->scene.camera.center.x -= y * sin(rtv1->scene.view_beta);
	rtv1->scene.camera.center.z -= y * cos(rtv1->scene.view_beta);



	return (0);
}

int key_release(int key, t_rtv1 *rtv1)
{
	if (key == 1073742049)
		rtv1->shift_pressed = 0;
	return (0);
}