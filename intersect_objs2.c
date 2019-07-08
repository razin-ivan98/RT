#include "rtv1.h"

double ray_intersect_paraboloid(t_vector start, t_vector dir, t_obj *parab)
{
	t_s		q;
	t_vector 	x, hitpoint;
	double dist ,u , z, len;
	double zeroThreshold = 0.0001;

	x = vector_subt(start, parab->center);

	u = scal_mult(dir, parab->dir);
	z = scal_mult(x, parab->dir);

	q.a = 2.f * (scal_mult(dir, dir) - u * u);
	q.b = 2.f * (scal_mult(dir, x) - u * (z + 2.f *  parab->radius));
	q.c = scal_mult(x, x) - z * (z + 4.f *  parab->radius);
	if ((q.diskr = q.b * q.b - 2.f * q.a * q.c) >= 0.f)
	{
		q.sq_diskr = sqrt(q.diskr);
		q.t = (-q.b - q.sq_diskr) / q.a;
		if (q.t > 0.f) {
			hitpoint = vector_sum(vector_int_mult(dir, q.t), x);
			len = scal_mult(hitpoint, parab->dir);
			if (len < parab->angle && len > -zeroThreshold)
				return (q.t);
		}
		q.t = (-q.b + q.sq_diskr) / q.a;
		if (q.t > -zeroThreshold) {
			hitpoint = vector_sum(vector_int_mult(dir, q.t), x);
			len = scal_mult(hitpoint, parab->dir);
			if (len < parab->angle && len > -zeroThreshold)
				return (q.t);
		}
	}
	return (0);

}

double ray_intersect_arrow(t_vector start, t_vector dir, t_obj *cyl)
{
	double zeroThreshold = 0.0001;
	t_vector tmp = start;
	start = vector_subt(start, cyl->center);

	double dot_start_cyl_dir = scal_mult(start, cyl->dir);
	double dot_dir_cyl_dir = scal_mult(dir, cyl->dir);

	double a = scal_mult(dir, dir) - dot_dir_cyl_dir * dot_dir_cyl_dir;
	double b = 2 * (scal_mult(dir, start) - dot_dir_cyl_dir * dot_start_cyl_dir);
	double c = scal_mult(start, start) - dot_start_cyl_dir * dot_start_cyl_dir - 0.05 * 0.05;
	double D = b*b - 4*a*c;

	if ( D < zeroThreshold )
		return (0.0);
	double qD = sqrt(D);
	double t1 = ( -b + qD)/(2*a); 
	double t2 = ( -b - qD)/(2*a);
	if (t1 <= zeroThreshold)
		return (0.0);
	double t = (t2 > zeroThreshold) ? t2 : t1; 
	t_vector hitpoint = vector_sum(vector_int_mult(dir, t), tmp);
	if (vector_length(vector_subt(hitpoint, cyl->center)) > 0.5)
		return 0.0;
		return (t);
}

double ray_intersect_triangle(t_vector start, t_vector dir, t_obj *triangle)
{
	t_vector		e1;
	t_vector		e2;
	t_vector		p;
	double		det;
	double		u;
	t_vector		q;
	double		v;
	t_vector		vertex_camera_direction;
	double		intersection;

	e1 = vector_subt(triangle->p2, triangle->center);
	e2 = vector_subt(triangle->p3, triangle->center);
	p = vector_cross(dir, e2);
	det = scal_mult(e1, p);
	if (det > -0.00001 && det < 0.000001)
		return (0.0);
	vertex_camera_direction = vector_subt(start, triangle->center);
	u = scal_mult(vertex_camera_direction, p) * (1 / det);
	if (u < 0 || u > 1)
		return (0.0);
	q = vector_cross(vertex_camera_direction, e1);
	v = scal_mult(dir, q) * (1 / det);
	if (v < 0 || (u + v) > 1)
		return (0.0);
	intersection = scal_mult(e2, q) * (1 / det);
	if (intersection > 0.00001)
		return (intersection);
	return (0.0);
}