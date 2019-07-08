/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect_math2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 12:00:58 by chorange          #+#    #+#             */
/*   Updated: 2019/07/08 21:28:56 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double		vector_length(t_vector a)
{
	double result;

	result = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	return (result);
}

t_vector	vector_init(double x, double y, double z)
{
	t_vector ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}

t_vector	vector_project(t_vector a, t_vector b)
{
	t_vector	project;
	double		dot;

	dot = scal_mult(a, b) / scal_mult(b, b);
	project = vector_int_mult(b, dot);
	return (project);
}

t_vector	vector_normalize(t_vector a)
{
	t_vector c;

	c = vector_int_div(a, sqrt(scal_mult(a, a)));
	return (c);
}

t_vector	vector_cross(t_vector a, t_vector b)
{
	t_vector	r;

	r.x = a.y * b.z - a.z * b.y;
	r.y = a.z * b.x - a.x * b.z;
	r.z = a.x * b.y - a.y * b.x;
	return (r);
}

t_vector rot(t_vector P, t_vector angles)
{
	double A = cos(angles.x);
    double B = sin(angles.x);
    double C = cos(angles.y);
    double D = sin(angles.y);
    double E = cos(angles.z);
    double F = sin(angles.z);

    double AD = A * D;
    double BD = B * D;

	double mat[16];

    mat[0] =   C * E;
    mat[1] =  -C * F;
    mat[2] =  -D;
    mat[4] = -BD * E + A * F;
    mat[5] =  BD * F + A * E;
    mat[6] =  -B * C;
    mat[8] =  AD * E + B * F;
    mat[9] = -AD * F + B * E;
    mat[10]=   A * C;

    mat[3] =  mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0.0;
    mat[15]=  1.0;

	t_vector ret;

	ret.x = P.x * mat[0] + P.y * mat[1] + P.z * mat[2];
	ret.y = P.x * mat[4] + P.y * mat[5] + P.z * mat[6];
	ret.z = P.x * mat[8] + P.y * mat[9] + P.z * mat[10];




	return (ret);
}