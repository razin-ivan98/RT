/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_scene2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 12:58:18 by chorange          #+#    #+#             */
/*   Updated: 2019/07/12 18:12:46 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		set_obj_type(char *line, t_obj *obj)
{
	if (ft_strstr(line, "sphere"))
		obj->type = sphere;
	else if (ft_strstr(line, "cylinder"))
		obj->type = cylinder;
	else if (ft_strstr(line, "cone"))
		obj->type = cone;
	else if (ft_strstr(line, "plane"))
		obj->type = plane;
	else if (ft_strstr(line, "paraboloid"))
		obj->type = paraboloid;
	else if (ft_strstr(line, "triangle"))
		obj->type = triangle;
	else
		err_exit();
}

void		set_light_type(char *line, t_light *light)
{
	if (ft_strstr(line, "point"))
		light->type = point;
	else if (ft_strstr(line, "directional"))
		light->type = directional;
	else if (ft_strstr(line, "ambient"))
		light->type = ambient;
	else
		err_exit();
}

t_obj		*read_obj_parameters(char *line, t_obj **obj)
{
	if (ft_strchr(line, '}'))
		return (NULL);
	else if (ft_strstr(line, "type"))
		set_obj_type(line, *obj);
	else if (ft_strstr(line, "position"))
		(*obj)->center = read_vector(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "pos2"))
		(*obj)->p2 = read_vector(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "pos3"))
		(*obj)->p3 = read_vector(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "radius"))
		(*obj)->radius = ft_atof(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "color"))
		(*obj)->rgb = color_to_rgb(ft_atoi(ft_strchr(line, '=') + 1));
	else if (ft_strstr(line, "angle"))
		(*obj)->angle = ft_atof(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "direction"))
		(*obj)->dir = vector_normalize(read_vector(ft_strchr(line, '=') + 1));
	else if (ft_strstr(line, "rotate"))
		(*obj)->rot = (read_vector(ft_strchr(line, '=') + 1));
	else if (ft_strstr(line, "specular"))
		(*obj)->specular = ft_atof(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "reflective"))
		(*obj)->reflective = ft_atof(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "refractive"))
		(*obj)->refractive = ft_atof(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "smoothness"))
		(*obj)->smoothness = ft_atof(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "transparency"))
		(*obj)->transparency = ft_atof(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "texture-id"))
		(*obj)->tex = ft_atoi(ft_strchr(line, '=') + 1);
	return (*obj);
}

t_light		*read_light_parameters(char *line, t_light **light)
{
	if (ft_strchr(line, '}'))
		return (NULL);
	else if (ft_strstr(line, "type"))
		set_light_type(line, *light);
	else if (ft_strstr(line, "position"))
		(*light)->center = read_vector(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "direction"))
		(*light)->dir = vector_normalize(read_vector(ft_strchr(line, '=') + 1));
	else if (ft_strstr(line, "intensity"))
		(*light)->intensity = ft_atof(ft_strchr(line, '=') + 1);
	return (*light);
}

t_camera	*camera_init(char *line, t_camera *camera)
{
	if (ft_strchr(line, '}'))
		return (NULL);
	else if (ft_strstr(line, "position"))
		camera->center = read_vector(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "direction"))
		camera->dir = read_vector(ft_strchr(line, '=') + 1);
	return (camera);
}
