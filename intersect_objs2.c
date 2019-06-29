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