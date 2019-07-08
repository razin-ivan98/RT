#define CW 1000
#define CH 1000
#define VW 1
#define VH 1
#define DEPTH 5
#define UPPER_BOUND 99999.9


typedef struct			s_quad
{
	double a;
	double b;
	double c;
	double d;
	double res;
}						t_quad;

typedef enum
{
	point,
	directional,
	ambient
}			light_type;

typedef enum
{
	sphere,
	cone,
	cylinder,
	plane,
	torus,
	paraboloid,
	triangle,
	poligonal,
	arrow
}			obj_type;

typedef struct s_vector
{
	double x;
	double y;
	double z;

}		t_vector;

typedef struct s_rgb
{
	int r;
	int g;
	int b;
}				t_rgb;

typedef struct s_obj
{
	int type;
	int id;
	t_vector center;
	t_vector	p2;
	t_vector	p3;
	t_vector dir;
	t_vector rot;
	double angle;
	double radius;
	t_rgb rgb;
	
	double specular;
	double reflective;
	double transparency;
	int tex;

	//struct s_obj *next;
}		t_obj;



typedef struct s_light
{
	int type;
	double intensity;
	t_vector center;
	t_vector dir;

//	struct s_light *next;
}				t_light;

typedef struct s_camera
{
	t_vector center;
	t_vector dir;
}		t_camera;

typedef struct s_scene
{
	int c_objs;
	int c_lights;
	double view_alpha;
	double view_beta;
	t_camera camera;
	t_obj		arrows[3];
	t_light lights[100];
	t_obj objs[100];
	int shadows_on;
		int			arrows_on;
}		t_scene;

typedef struct s_cl_obj
{
	int type;
	int id;
	double angle;
	double radius;
	t_rgb rgb;
	double specular;
	double reflective;
	double transparency;
	int tex;
	
	double3 center;
	double3 p2;
	double3 p3;
	double3 dir;
	double3 rot;

}		t_cl_obj;

typedef struct s_cl_camera
{
	double3 center;
	double3 dir;
}		t_cl_camera;

typedef struct s_cl_light
{
	int type;
	double intensity;

	double3 center;
	double3 dir;
}				t_cl_light;

typedef struct s_cl_scene
{
	int c_objs;
	int c_lights;
	t_cl_camera camera;
	t_cl_obj		arrows[3];
	t_cl_light lights[100];
	t_cl_obj objs[100];
	int shadows_on;
		int			arrows_on;
}		t_cl_scene;

t_rgb color_to_rgb(int color)
{
	t_rgb rgb;

	rgb.r = color >> 16;
	rgb.g = (color & 0x00ff00) >> 8;
	rgb.b = color & 0x0000ff;

	return (rgb);
}

int rgb_to_color(t_rgb rgb)
{
	int color;

	color = rgb.r * 0x010000 + rgb.g * 0x000100 + rgb.b;

	return (color);
}

void	put_point_to_image(__global char *image_data, int x, int y, int color)
{
	int	index = 0;

	if (x < 0 || y < 0 || x > CW - 1 || y > CH - 1)
		return ;
	else
	{
		index = CW * y * 4 + x * 4;
        image_data[index] = color & 0x0000ff;
		image_data[index + 2] = color >> 16;
		image_data[index + 1] = (color & 0x00ff00) >> 8;
	}
}

double3 vector_to_double3(t_vector a)
{
	return((double3)(a.x, a.y, a.z));
}

double3 rot(double3 P, double3 angles)
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

	double3 ret;

	ret.x = P.x * mat[0] + P.y * mat[1] + P.z * mat[2];
	ret.y = P.x * mat[4] + P.y * mat[5] + P.z * mat[6];
	ret.z = P.x * mat[8] + P.y * mat[9] + P.z * mat[10];




	return (ret);
}

double3 get_pixel_pisition(int x, int y)
{
	double3 pixel_pos;
	double x3;
	double y3;

	x3 = (double)VW/(double)CW*(double)x;
	y3 = (double)VH/(double)CH*(double)y;

	pixel_pos = (double3)(x3, y3, 1.0);
	return (pixel_pos);
}

double3	rotate_view(double3 point, double alpha, double beta)
{
	double3 tempo = point;

	point.y = tempo.y * cos(alpha) + tempo.z * sin(alpha);
	point.z = -tempo.y * sin(alpha) + tempo.z * cos(alpha);

	tempo = point;

	point.x = tempo.x * cos(beta) - tempo.z * sin(beta);
	point.z = tempo.x * sin(beta) + tempo.z * cos(beta);

	return (point);
}

/////////////////////
/*
double ray_intersect_triangle(double3 start, double3 dir, t_cl_obj *triangle)
{
	double3 normal, x, v[3], c[3], hitpoint;
	double d, t;

	v[0] = triangle->dir - triangle->center;
	v[1] = triangle->rot - triangle->dir;
	v[2] = triangle->center - triangle->rot;
	normal = normalize(cross(v[0], v[1]));
	x = start - triangle->center;
	if ((d = dot(dir,normal)) < 0.00001 && d > -0.00001)
		return (0);
	t = -dot(x, normal) / d;
	hitpoint = t * dir + start;
	c[0] = hitpoint - triangle->center;
	c[1] = hitpoint - triangle->dir;
	c[2] = hitpoint - triangle->rot;
	if(length(cross(v[0],c[0]))+length(cross(v[1],c[1]))+length(cross(v[2],c[2])) < length(cross(v[1], v[2])) + 0.001)
		return (t);
	return (0);
}*/



double ray_intersect_triangle(double3 start, double3 dir, t_cl_obj *triangle)
{
	double3		e1;
	double3		e2;
	double3		p;
	double		det;
	double		u;
	double3		q;
	double		v;
	double3		vertex_camera_direction;
	double		intersection;

	e1 = triangle->p2 - triangle->center;
	e2 = triangle->p3 - triangle->center;
	p = cross(dir, e2);
	det = dot(e1, p);
	if (det > -0.00001 && det < 0.000001)
		return (0.0);
	vertex_camera_direction = start - triangle->center;
	u = dot(vertex_camera_direction, p) * (1 / det);
	if (u < 0 || u > 1)
		return (0.0);
	q = cross(vertex_camera_direction, e1);
	v = dot(dir, q) * (1 / det);
	if (v < 0 || (u + v) > 1)
		return (0.0);
	intersection = dot(e2, q) * (1 / det);
	if (intersection > 0.00001)
		return (intersection);
	return (0.0);
}
/////////////////

double ray_intersect_paraboloid(double3 start, double3 dir, t_cl_obj *parab)
{
	t_quad		q;
	double3 	x, hitpoint;
	double dist ,u , z, len;
	double zeroThreshold = 0.0001;

	x = start - parab->center;

	u = dot(dir, parab->dir);
	z = dot(x, parab->dir);

	q.a = 2.f * (dot(dir, dir) - u * u);
	q.b = 2.f * (dot(dir, x) - u * (z + 2.f *  parab->radius));
	q.c = dot(x, x) - z * (z + 4.f *  parab->radius);
	if ((q.d = q.b * q.b - 2.f * q.a * q.c) >= 0.0)
	{
		q.d = sqrt(q.d);
		q.res = (-q.b - q.d) / q.a;
		if (q.res > zeroThreshold) {
			hitpoint = q.res * dir + x;
			len = dot(hitpoint, parab->dir);
			if (/*len < parab->angle && */len > -zeroThreshold)
				return (q.res);
		}
		q.res = (-q.b + q.d) / q.a;
		if (q.res > zeroThreshold) {
			hitpoint = q.res * dir + x;
			len = dot(hitpoint, parab->dir);
			if (len < parab->angle && len > -zeroThreshold)
				return (q.res);
		}
	}
	return (0);

}

///////////////
/*
int check_sphere(double3 start, double3 dir, double radius, double3 pos)
{
	t_quad		q;
	double3 	x;
	double zeroThreshold = 0.00001;

	x = start - pos;
	q.b = 2.0f * dot(dir, x);
	q.c = dot(x, x) - radius * radius;
	if ((q.d = (q.b * q.b - 4.0f * q.c)) < zeroThreshold)
		return (0);
	return (1);
}
void positive_discriminant(double Q, double2 koefs, double *solve, double b)
{
	double alpha, betha, ntmp;

	ntmp = -koefs[1] / 3.f;
		alpha = cbrt(-koefs[0] / 2.f + sqrt(Q));
		betha = -koefs[1] / (3.f * alpha);
		*solve = alpha + betha - b;
}

void negative_discriminant(double Q, double2 koefs, double *solve, double b)
{
	double cosphi, tmp, ntmp, phi;

	tmp = -3.f / koefs.y;
	ntmp = -koefs.y / 3.f;
	cosphi = -koefs.x * 0.5f * sqrt(tmp * tmp * tmp);
	phi = acos(cosphi);
	*solve = 2.f * sqrt(ntmp) * cos(phi / 3.f) - b;
}
void fourth_degree_equation_solver(double4 koefs, double2 *solve)
{
	double3 ferrari_koefs, cubic_resol_koefs;
	double2 cubic_two_params_koefs;
	double a2, a3, a4, p2, b2, Q, cubic_solve;
	t_quad q;

	a2 = koefs[3] * koefs[3];
	a3 = a2 * koefs[3];
	a4 = a2 * a2;
	//y4+py2+qy+r=0
	ferrari_koefs[2] = koefs[2] - 0.375f * a2;
	ferrari_koefs[1] = 0.125f * a3 - 0.5f * koefs[2] * koefs[3] + koefs[1];
	ferrari_koefs[0] = (-0.01171875f) * a4 + 0.0625f * koefs[2] * a2 - 0.25f * koefs[1] * koefs[3] + koefs[0];
	if (ferrari_koefs[1] * ferrari_koefs[1] < 1e-4)
	{
		q.b = ferrari_koefs[2];
		q.c = ferrari_koefs[0];
		q.d = q.b * q.b - 4.f * q.c + 1e-4;
		if (q.d >= 0.f)
		{
			q.d = sqrt(q.d);
			(*solve)[0] = (-q.b - q.d) / 2.f;
			(*solve)[1] = (-q.b + q.d) / 2.f;
			if ((*solve)[0] >= 0.f)
				(*solve)[0] = (q.res = -sqrt((*solve)[0]) - koefs[3] / 4.f) > 0.f ? q.res : sqrt((*solve)[0]) - koefs[3] / 4.f;
			if ((*solve)[1] >= 0.f)
				(*solve)[1] = (q.res = -sqrt((*solve)[1]) - koefs[3] / 4.f) > 0.f ? q.res : sqrt((*solve)[1]) - koefs[3] / 4.f;
		}
		return;
	}
	//y3+py2+qy+r=0
	cubic_resol_koefs[2] = (-0.5f) * ferrari_koefs[2];
	cubic_resol_koefs[1] = -ferrari_koefs[0];
	cubic_resol_koefs[0] = (-0.125f) * ferrari_koefs[1] * ferrari_koefs[1] + 0.5f * ferrari_koefs[0] * ferrari_koefs[2];
	p2 = cubic_resol_koefs[2] * cubic_resol_koefs[2];
	//y3+py+q=0
	cubic_two_params_koefs[1] = cubic_resol_koefs[1] - p2 / 3.f;
	cubic_two_params_koefs[0] = cubic_resol_koefs[0] + 2.f * p2  * cubic_resol_koefs[2] / 27.f - cubic_resol_koefs[2] * cubic_resol_koefs[1] / 3.f;
	//y = z+t, 3zt+p = 0
	b2 = cubic_two_params_koefs[0] * cubic_two_params_koefs[0];
	Q = cubic_two_params_koefs[1] * cubic_two_params_koefs[1] * cubic_two_params_koefs[1] / 27.f + cubic_two_params_koefs[0] * cubic_two_params_koefs[0] / 4.f;
	if (Q > 0)
		positive_discriminant(Q, cubic_two_params_koefs, &cubic_solve, cubic_resol_koefs[2] / 3.f);
	else
		negative_discriminant(Q, cubic_two_params_koefs, &cubic_solve, cubic_resol_koefs[2] / 3.f);
	if ((q.b = 2.f * cubic_solve - ferrari_koefs[2]) > 0.f ) {
		q.a = q.b;
		q.b = -sqrt(q.b);
		q.c = ferrari_koefs[1] / (-2.f * q.b) + cubic_solve;
		q.d = q.a - 4.f * q.c + 1e-4;
		if (q.d >= 0.f)
		{
			q.d = sqrt(q.d);
			(*solve)[0] =
					(q.res = (-q.b - q.d) / 2.f - koefs[3] / 4.f) > 0.f
					? q.res : (-q.b + q.d) / 2.f - koefs[3] / 4.f;
		}
		q.b = -q.b;
		q.c = ferrari_koefs[1] / (-2.f * q.b) + cubic_solve;
		q.d = q.a - 4.f * q.c + 1e-4;
		if (q.d >= 0.f)
		{
			q.d = sqrt(q.d);
			(*solve)[1] =
					(q.res = (-q.b - q.d) / 2.f - koefs[3] / 4.f) > 0.f
					? q.res : (-q.b + q.d) / 2.f - koefs[3] / 4.f;
		}
	}
}

double ray_intersect_torus(double3 start, double3 dir, t_cl_obj *torus)
{
	double3    x;
	double4 equation_koefs, dots,qq;
	double2 solve = (double2){-1.f, -1.f};
	double R2, r2, Rr, dist;

	Rr = torus->angle + torus->radius + 1;
	if (!check_sphere(start, dir, Rr, torus->center))
		return (0);
	x = start - torus->center;
	dist = length(x) - Rr;
	if (dist > 0.f)
		x += dir * dist;
	else
		dist = 0.f;
	R2 = torus->angle * torus->angle;
	r2 = torus->radius * torus->radius;
	//ax4+bx3+cx2+dx1+e
	//a == 1
	dots[0] = dot(x, torus->dir);
	dots[1] = dot(dir, torus->dir);
	dots[2] = dot(x, x);
	dots[3] = dot(x, dir);
	qq[0] = 1.0f - dots[1] * dots[1];
	qq[1] = 2.0f * (dots[3] - dots[0] * dots[1]);
	qq[2] = dots[2] - dots[0] * dots[0];
	qq[3] = dots[2] + R2 - r2;
	equation_koefs[3] = 4.0f * dots[3];
	equation_koefs[2] = 2.0f * qq[3] + equation_koefs[3] * equation_koefs[3] * 0.25f - 4.0f * R2 * qq[0];
	equation_koefs[1] = equation_koefs[3] * qq[3] - 4.0f * R2 * qq[1];
	equation_koefs[0] = qq[3] * qq[3] - 4.0f * R2 * qq[2];
	fourth_degree_equation_solver(equation_koefs, &solve);

	if(solve[0] > -0.00001)
	{
		if(solve[1] > 0)
			return (solve[0] < solve[1] ? solve[0] + dist : solve[1] + dist);
		else
			return (solve[0] + dist);
	}
	else if (solve[1] > -0.00001)
		return (solve[1] + dist);
	else
		return(0);
}*/
///////////////////
double ray_intersect_plane(double3 start, double3 dir, t_cl_obj *plane)
{
	double zeroThreshold = 0.0001;
	double dir_dot_c;
	double cen_dot_c;
	double t;

	cen_dot_c = dot((start - plane->center), plane->dir);
	dir_dot_c = dot(dir, plane->dir);
	if (dir_dot_c == 0 || (dir_dot_c < 0 && cen_dot_c < 0) || (dir_dot_c > 0 && cen_dot_c > 0))
	return(0);
	t = -cen_dot_c / dir_dot_c;
	return (t > zeroThreshold ? t : 0);

}

double ray_intersect_cylinder(double3 start, double3 dir, t_cl_obj *cyl)
{
	double zeroThreshold = 0.0001;

	start = start - cyl->center;

	double dot_start_cyl_dir = dot(start, cyl->dir);
	double dot_dir_cyl_dir = dot(dir, cyl->dir);

	double a = dot(dir, dir) - dot_dir_cyl_dir * dot_dir_cyl_dir;
	double b = 2 * (dot(dir, start) - dot_dir_cyl_dir * dot_start_cyl_dir);
	double c = dot(start, start) - dot_start_cyl_dir * dot_start_cyl_dir - cyl->radius * cyl->radius;
	double D = b*b - 4*a*c;

	if ( D < zeroThreshold )
		return (0.0);
	double qD = sqrt(D);
	double t1 = ( -b + qD)/(2*a); 
	double t2 = ( -b - qD)/(2*a);
	if (t1 <= zeroThreshold)
		return (0.0);
	double t = (t2 > zeroThreshold) ? t2 : t1; 

	return (t);
}

double ray_intersect_arrow(double3 start, double3 dir, t_cl_obj *cyl)
{
	double zeroThreshold = 0.0001;
	double3 tmp = start;
	start = start - cyl->center;

	double dot_start_cyl_dir = dot(start, cyl->dir);
	double dot_dir_cyl_dir = dot(dir, cyl->dir);

	double a = dot(dir, dir) - dot_dir_cyl_dir * dot_dir_cyl_dir;
	double b = 2 * (dot(dir, start) - dot_dir_cyl_dir * dot_start_cyl_dir);
	double c = dot(start, start) - dot_start_cyl_dir * dot_start_cyl_dir - 0.05 * 0.05;
	double D = b*b - 4*a*c;

	if ( D < zeroThreshold )
		return (0.0);
	double qD = sqrt(D);
	double t1 = ( -b + qD)/(2*a); 
	double t2 = ( -b - qD)/(2*a);
	if (t1 <= zeroThreshold)
		return (0.0);
	double t = (t2 > zeroThreshold) ? t2 : t1; 
	double3 hitpoint = t * dir + tmp;
	if (length(hitpoint - cyl->center) > 0.5)
		return 0.0;
		return (t);
}

double ray_intersect_cone(double3 start, double3 dir, t_cl_obj *cone)
{
	double zeroThreshold = 0.0001;

	start = start - cone->center;
	double k = tan(cone->angle);

	double dot_start_cone_dir = dot(start, cone->dir);
	double dot_dir_cone_dir = dot(dir, cone->dir);

	double a = dot(dir, dir) - (1 + k * k) * dot_dir_cone_dir * dot_dir_cone_dir;
	double b = 2 * (dot(dir, start) - (1 + k * k) * dot_dir_cone_dir * dot_start_cone_dir);
	double c = dot(start, start) - (1 + k * k) * dot_start_cone_dir * dot_start_cone_dir;
	double D = b*b - 4*a*c;

	if ( D < zeroThreshold )
		return (0.0);
	double qD = sqrt(D);
	double t1 = ( -b + qD)/(2*a);
	double t2 = ( -b - qD)/(2*a);

	if ((t1 <= t2 && t1 >= zeroThreshold) || (t1 >= zeroThreshold && t2 < zeroThreshold))
		return (t1);
	if ((t2 <= t1 && t2 >= zeroThreshold) || (t2 >= zeroThreshold && t1 < zeroThreshold))
		return (t2);
	return (0.0);
}


double ray_intersect_sphere(double3 start, double3 dir, t_cl_obj *obj)
{
	double zeroThreshold = 0.0001;

	start = start - obj->center;
	double a = dot(dir, dir);
	double b = dot(start, dir);
	double c = dot(start, start) - obj->radius * obj->radius;
	double D = b*b -a*c;

	if ( D < zeroThreshold )
		return (0.0);
	double qD = sqrt(D);
	double t1 = ( -b + qD)/(a);
	double t2 = ( -b - qD)/(a);
	if (t1 <= zeroThreshold) 
		return (0.0);
	double t = (t2 > zeroThreshold) ? t2 : t1; 
		return (t);
}

double ray_intersect_obj(double3 start, double3 dir, t_cl_obj *obj)
{
	if (obj->type == sphere)
		return (ray_intersect_sphere(start, dir, obj));
	else if (obj->type == cone)
		return (ray_intersect_cone(start, dir, obj));
	else if (obj->type == cylinder)
		return (ray_intersect_cylinder(start, dir, obj));
	else if (obj->type == plane)
		return (ray_intersect_plane(start, dir, obj));
	/*else if (obj->type == torus)
		return (ray_intersect_torus(start, dir, obj));*/
	else if (obj->type == paraboloid)
		return (ray_intersect_paraboloid(start, dir, obj));
	else if (obj->type == triangle)
		return (ray_intersect_triangle(start, dir, obj));
}

t_cl_obj *get_closest_object(double *closest_t, double3 start, double3 dir, t_cl_scene *cl_scene)
{
	double t = 0.0;
	int i = 0;
	t_cl_obj *closest_obj = 0;

	while (i < cl_scene->c_objs)
	{
		t = ray_intersect_obj(start, dir, &(cl_scene->objs[i]));
		if (t != 0.0 && t < (*closest_t))
		{
			(*closest_t) = t;
			closest_obj = &(cl_scene->objs[i]);
		}
		i++;
	}
	return (closest_obj);
}



double3 vector_project(double3 a, double3 b)
{
	double3 project;
	double dot_ab_div_dot_bb;

	dot_ab_div_dot_bb = dot(a, b) / dot(b, b);
	project = b * dot_ab_div_dot_bb;
	return (project);
}

double3 get_normal(double3 point, t_cl_obj obj)
{
	double3 normal;
	double3 project;
	double k;

	if (obj.type == sphere)
	{
		normal = point - obj.center;
		normal = normalize(normal);
	}
	else if (obj.type == cylinder)
	{
		normal = point - obj.center;
		project = vector_project(normal, obj.dir);
		normal = normalize(normal - project);
	}
	else if (obj.type == plane)
		normal = obj.dir;
	else if (obj.type == cone)
	{
		normal = point - obj.center;
		project = vector_project(normal, obj.dir);
		k = 1 + tan(obj.angle) * tan(obj.angle);
		project = project * k;
		normal = normalize(normal - project);
	}
	else if (obj.type == triangle)
	{
/*
		point -= obj.center;
		normal = point - dot(point, obj.dir) * obj.dir;
		normal = normalize(normal);
		normal = point - normal * obj.angle;
		normal =  (normalize(normal));*/


		normal = (normalize(cross(obj.p2 - obj.center, obj.p3 - obj.p2)));
	}

	else if (obj.type == paraboloid)
	{
		point = point - obj.center;
		k = dot(point, obj.dir);
		normal = point - obj.dir * (k + obj.radius);
		normal = normalize(normal);
	}
	
	return (normal);
}

double3 reflect_ray(double3 R, double3 N)
{
	return (N * (2 * dot(N, R)) - R);
}

double compute_lighting(double3 P, double3 N, double3 V, t_cl_obj obj, t_cl_scene *cl_scene, double *spec_intensity)
{
	double intensity = 0.0;

	double3 L;
	double t;
	double shadow_t;
	t_cl_obj *shadow_obj = 0;

	if (obj.type == arrow)
		return (1.0);

	for (int i = 0; i < cl_scene->c_lights; i++)
	{
		shadow_t = UPPER_BOUND;
		shadow_obj = 0;
		int j = 0;
		if (cl_scene->lights[i].type == ambient)
			intensity += cl_scene->lights[i].intensity;
		else
		{
			if (cl_scene->lights[i].type == point)
				L = cl_scene->lights[i].center - P;
			else
				L = cl_scene->lights[i].dir;
			if (cl_scene->shadows_on == 1)
			{
				while (j < cl_scene->c_objs)
				{
					t = ray_intersect_obj(P, L, &(cl_scene->objs[j]));
					if (t > 0.000001 && ((cl_scene->lights[i].type == point && length(L) > length(L * t)) || (cl_scene->lights[i].type != point)))
					{
						shadow_t = t;
						shadow_obj = &(cl_scene->objs[j]);
					}
					j++;
				}
				if (shadow_obj)
					continue ;
			}
			double n_dot_l = dot(N, L);
			if (n_dot_l > -0.0)
				intensity += cl_scene->lights[i].intensity * n_dot_l / (length(L));

			if (obj.specular > 0.0)
			{
				double3 R = reflect_ray(L, N);
				double r_dot_v = dot(R, V);
				if (r_dot_v > -0.0)
					*spec_intensity += cl_scene->lights[i].intensity * pow(r_dot_v / (length(R) * length(V)), obj.specular);
			}
		}
	}
	return (intensity);
}



double get_angle_bet_vecs(double3 a, double3 b)
{
	return(acos(dot(a, b)/length(a)/length(b)));
}

t_rgb get_rgb_from_texture_sphere(double3 P, t_cl_obj obj, __global char* data[5])
{
	t_rgb ret;


	P = P - obj.center;

	P = rot(P, obj.rot);

	double3 temp = P;

	temp.y = 0.0;


	double alpha = get_angle_bet_vecs( temp, (double3)(1.0f, 0.0f, 0.0f)) * (P.z >= 0.0 ? 1.0 : -1.0);
	double beta = get_angle_bet_vecs(temp, P) * (P.y >= 0.0 ? 1.0 : -1.0);

	alpha = (alpha)/2/M_PI;
	beta = (beta)/2/M_PI;

	int tmp;

	tmp = data[obj.tex][(int)(512.0 + 1024.0 * beta) * 3 * 1024 + (int)(512.0 + 1024.0 * alpha) * 3 + 2];

	if (tmp < 0)
		tmp += 255;
	else if (tmp >= 255)
		tmp -= 255;

	ret.r = tmp;

	tmp = data[obj.tex][(int)(512.0 + 1024.0 * beta) * 3 * 1024 + (int)(512.0 + 1024.0 * alpha) * 3 + 1];

	if (tmp < 0)
		tmp += 255;
	else if (tmp >= 255)
		tmp -= 255;

	ret.g = tmp;

	tmp = data[obj.tex][(int)(512.0 + 1024.0 * beta) * 3 * 1024 + (int)(512.0 + 1024.0 * alpha) * 3];

	if (tmp < 0)
		tmp += 255;
	else if (tmp >= 255)
		tmp -= 255;
	ret.b = tmp;





	return(ret);
}



double3	vec3_mul(double3 va, double3 vb)
{
	double3	v;

	v.x = va.x * vb.x;
	v.y = va.y * vb.y;
	v.z = va.z * vb.z;
	return (v);
}

t_rgb	get_rgb_from_texture_plane(double3 P, t_cl_obj obj, __global char* data[5])
{
	float	u;
	float	v;
	double3	u_axis;
	double3	v_axis;

	t_rgb ret;

	u_axis = (double3){obj.dir.y, obj.dir.z, -obj.dir.x};
	//if (obj->mat.texture.rotation != 0)
	//	vec3_rotate(&u_axis, vec3_fmul(obj->dir, obj->mat.texture.rotation));
	v_axis = cross(u_axis, obj.dir);
	u = dot(P, u_axis) * 1.0/*obj->mat.texture.scale*/;
	v = dot(P, v_axis) * 1.0/*obj->mat.texture.scale*/;
	u = u - floor(u);
	v = v - floor(v);
	/*if (obj->mat.texture.filtering)
		return (bilinear_filtering(obj, img, u, v));
	else
		return (rgb_to_vec3(img[(int)(v * (obj->mat.texture.h - 1))]
				[(int)(u * (obj->mat.texture.w - 1))]));*/

	int tmp;

	tmp = data[obj.tex][(int)(1024.0 * u) * 3 * 1024 + (int)(1024.0 * v) * 3 + 2];

	if (tmp < 0)
		tmp += 255;
	else if (tmp >= 255)
		tmp -= 255;

	ret.r = tmp;

	tmp = data[obj.tex][(int)(1024.0 * u) * 3 * 1024 + (int)(1024.0 * v) * 3 + 1];

	if (tmp < 0)
		tmp += 255;
	else if (tmp >= 255)
		tmp -= 255;

	ret.g = tmp;

	tmp = data[obj.tex][(int)(1024.0 * u) * 3 * 1024 + (int)(1024.0 * v) * 3];

	if (tmp < 0)
		tmp += 255;
	else if (tmp >= 255)
		tmp -= 255;
	ret.b = tmp;


	return (ret);
}

double3 rot_v(double3 P, double3 ax, double a)
{
	double3 ret;

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

double	sq(double a)
{
	return (a * a);
}

double	angle(double3 a, double3 b)
{
	return (acos(fabs((a.x * b.x + a.y * b.y + a.z * b.z)
			/ (sqrt(sq(a.x) + sq(a.y) + sq(a.z))
			* sqrt(sq(b.x) + sq(b.y) + sq(b.z))))));

}

double3 get_default_cylinder_P(double3 P, t_cl_obj obj)
{
	if (obj.dir.y == 1.0)
		return(P);
	if (obj.dir.x == 1.0)
		return(rot(P, (double3){0.0, 0.0, M_PI_2}));
	if (obj.dir.x == -1.0)
		return(rot(P, (double3){0.0, 0.0, -M_PI_2}));
	if (obj.dir.y == -1.0)
		return(rot(P, (double3){M_PI, 0.0, 0.0}));
	if (obj.dir.z == 1.0)
		return(rot(P, (double3){M_PI_2, 0.0, 0.0}));
	if (obj.dir.z == -1.0)
		return(rot(P, (double3){-M_PI_2, 0.0, 0.0}));
	
	double z = atan2(obj.dir.x, obj.dir.y);
	double y = atan2(obj.dir.x, obj.dir.z);

	double3 ret;
	
	
	ret = rot(P, (double3){0.0, 0.0, z});
	ret = rot(ret, (double3){0.0, y, 0.0});
		return(ret);
}		

double3		ft_vec3vop_r(double3 a, double3 b, char c)
{
	double3	to_vec3;

	//ft_bzero(&to_vec3, sizeof(t_vec3));
	if (c == '+')
	{
		to_vec3.x = a.x + b.x;
		to_vec3.y = a.y + b.y;
		to_vec3.z = a.z + b.z;
	}
	else if (c == '-')
	{
		to_vec3.x = a.x - b.x;
		to_vec3.y = a.y - b.y;
		to_vec3.z = a.z - b.z;
	}
	else if (c == '/')
	{
		to_vec3.x = a.x / b.x;
		to_vec3.y = a.y / b.y;
		to_vec3.z = a.z / b.z;
	}
	else if (c == '*')
	{
		to_vec3.x = a.x * b.x;
		to_vec3.y = a.y * b.y;
		to_vec3.z = a.z * b.z;
	}
	else if (c == 'c')
	{
		to_vec3.x = a.y * b.z - a.z * b.y;
		to_vec3.y = a.z * b.x - a.x * b.z;
		to_vec3.z = a.x * b.y - a.y * b.x;
	}
	return (to_vec3);
}

double3		ft_vec3sop_r(double3 from_vec3, double x, char c)
{
	double3 to_vec3	;

	//ft_bzero(&to_vec3, sizeof(t_vec3));
	if (c == '+')
	{
		to_vec3.x = from_vec3.x + x;
		to_vec3.y = from_vec3.y + x;
		to_vec3.z = from_vec3.z + x;
	}
	else if (c == '*')
	{
		to_vec3.x = from_vec3.x * x;
		to_vec3.y = from_vec3.y * x;
		to_vec3.z = from_vec3.z * x;
	}
	else if (c == '=')
	{
		to_vec3.x = x;
		to_vec3.y = x;
		to_vec3.z = x;
	}
	else if (c == '/')
	{
		to_vec3.x = from_vec3.x / x;
		to_vec3.y = from_vec3.y / x;
		to_vec3.z = from_vec3.z / x;
	}
	return (to_vec3);
}

double3	get_x_axe(double3 dir)
{
	double3 x_axe;

	x_axe = normalize(ft_vec3vop_r((double3){0.0, 1.0, 0.0}, dir, 'c'));
	if (length(x_axe) == 0.0)
		x_axe = normalize(ft_vec3vop_r((double3){1.0, 0.0, 0.0}, dir, 'c'));
	return (x_axe);
}


double3	get_local(t_cl_obj obj, double3 hit)
{
	double3 ret;
	double3 x_axe;
	double3 y_axe;


	hit = obj.center - hit;
	ret.y = dot(hit, obj.dir);
	x_axe = get_x_axe(obj.dir);
	hit = ft_vec3vop_r(hit, ft_vec3sop_r(obj.dir, ret.y, '*'), '-');
	y_axe = ft_vec3vop_r(x_axe, obj.dir, 'c');
	ret.x = dot(hit, x_axe);
	ret.z = dot(hit, y_axe);

	return ret;
}

t_rgb	get_rgb_from_texture_cylinder(double3 P, t_cl_obj obj, __global char* data[5])
{
	double3	d;
	float	u;
	float	v;
	int		i;
	int		j;

	//d = P - obj.center;
	d = get_local(obj, P);
	//double3 crosss = (obj.dir.y < 0 ? cross((double3){0.0, 1.0, 0.0}, obj.dir) : -cross((double3){0.0, 1.0, 0.0}, obj.dir));

	//d = rot_v(d, crosss, angle((double3){0.0, 100.0, 0.0}, obj.dir));
	//d = get_default_cylinder_P(d, obj);
	v = 0.5 + atan2(d.x, d.z) / M_PI * 0.5;
	u = d.y / (2.0);
	u = u - floor(u);

	t_rgb ret;

	int tmp;

	tmp = data[obj.tex][(int)(1024.0 * u) * 3 * 1024 + (int)(1024.0 * v) * 3 + 2];

	if (tmp < 0)
		tmp += 255;
	else if (tmp >= 255)
		tmp -= 255;

	ret.r = tmp;

	tmp = data[obj.tex][(int)(1024.0 * u) * 3 * 1024 + (int)(1024.0 * v) * 3 + 1];

	if (tmp < 0)
		tmp += 255;
	else if (tmp >= 255)
		tmp -= 255;

	ret.g = tmp;

	tmp = data[obj.tex][(int)(1024.0 * u) * 3 * 1024 + (int)(1024.0 * v) * 3];

	if (tmp < 0)
		tmp += 255;
	else if (tmp >= 255)
		tmp -= 255;
	ret.b = tmp;

	/*if (obj->mat.texture.filtering)
		return (bilinear_filtering(obj, img, u, v));
	else
	{
		i = ft_clamp(u * obj->mat.texture.w, 0, obj->mat.texture.w - 1);
		j = ft_clamp(v * obj->mat.texture.h, 0, obj->mat.texture.h - 1);
		return (rgb_to_vec3(img[j][i]));
	}*/
	return (ret);
}




t_rgb get_rgb_from_texture(double3 P, t_cl_obj obj, __global char* data[5])
{
	if (obj.tex < 0 || obj.tex > 4)
		return (obj.rgb);
	if (obj.type == sphere)
		return (get_rgb_from_texture_sphere(P, obj, data));
	else if (obj.type == cylinder || obj.type == cone || obj.type == paraboloid)
		return (get_rgb_from_texture_cylinder(P, obj, data));
	
	else if (obj.type == plane)
		return (get_rgb_from_texture_plane(P, obj, data));
	return (obj.rgb);
}

inline void recalc_rgb(t_rgb *ret, t_rgb *colorr, double intensity, double spec_intensity, double coeff, double areflective)
{
	if ((colorr->r *= intensity) >= 255.0)
		colorr->r = 255.0;
	if ((colorr->g *= intensity) >= 255.0)
		colorr->g = 255.0;
	if ((colorr->b *= intensity) >= 255.0)
		colorr->b = 255.0;

	if ((colorr->r += 255.0 * spec_intensity) >= 255.0)
		colorr->r = 255.0;
	if ((colorr->g += 255.0 * spec_intensity) >= 255.0)
		colorr->g = 255.0;
	if ((colorr->b += 255.0 * spec_intensity) >= 255.0)
		colorr->b = 255.0;

	ret->r += (colorr->r * coeff * areflective);
	ret->g += (colorr->g * coeff * areflective);
	ret->b += (colorr->b * coeff * areflective);

}

int get_index_by_id(t_cl_scene *scene, int id)
{
	int ret;
	int i = 0;

	while (i < scene->c_objs)
	{
		if (scene->objs[i].id == id)
			return(i);
		i++;
	}


	return (-1);
}

int is_intersect_arrow(t_cl_scene *cl_scene, double3 start, double3 dir)
{
	double t = 9999999.0;
	double tmp;
	int		index;
	int i = 0;
	while (i < 3)
	{
		tmp = ray_intersect_arrow(start, dir, &cl_scene->arrows[i]);
		if (tmp < t && tmp != 0.0)
		{
			t = tmp;
			index = i;
		}
		i++;
	}
	if (t == 9999999.0)
		return (-1);
	else
		return (rgb_to_color(cl_scene->arrows[index].rgb));
}

int cast_ray(t_cl_scene *cl_scene, double3 start, double3 dir, int depth, __global char* data[5])
{
	t_cl_obj closest_obj;
	t_cl_obj *ptr;
	double3 N;
	double closest_t;
	double intensity;
	double spec_intensity;
	double coeff = 1.0;
	int subt_index;
	t_rgb colorr;
	t_rgb ret = (t_rgb){0, 0, 0};
	if (cl_scene->arrows_on)
	{
		int arr = is_intersect_arrow(cl_scene, start, dir);
		if (arr != -1)
			return(arr);
	}
		while (depth >= 0)
		{
			ptr = 0;
			closest_t = 99999.0;
			spec_intensity = 0.0;
			ptr = get_closest_object(&closest_t, start, dir, cl_scene);
			if (ptr == 0)
				break;//////////////////////отражается ничто

			closest_obj = *ptr;


			start = start + dir * closest_t;

			

			colorr = get_rgb_from_texture(start, closest_obj, data);
			//return (rgb_to_color(colorr));
			N = get_normal(start, closest_obj);
			if (dot(N, dir)/length(N)/length(dir) > 0.0)
				N *= -1;

			intensity = compute_lighting(start, N, -dir, closest_obj, cl_scene, &spec_intensity);
			recalc_rgb(&ret, &colorr, intensity, spec_intensity, coeff, 1.0 - closest_obj.reflective/* - closest_obj.transparency*/);
			coeff *= closest_obj.reflective;
		/*	if (closest_obj.transparency > 0.0)
			{
				t_rgb ref = refr1(ptr, cl_scene, start, dir, depth, data);
				recalc_rgb(&ret, &ref, 1.0, 1.0, coeff, closest_obj.transparency);
			}*/
			depth--;
			if (closest_obj.reflective <= 0.0)
				break;
			dir = reflect_ray((-1.0) * dir, N);
		}
	return (rgb_to_color(ret));
}


__kernel void mishania(__global char *image_data, __global t_scene *scene, __global char *data0, __global char *data1, __global char *data2, __global char *data3, __global char *data4)
{
	int x = get_global_id(0) / CH;
	int y = get_global_id(0) % CH;
	double alpha = scene->view_alpha;
	double beta = scene->view_beta;
	double3 pixel_pos_3d;

	t_cl_scene cl_scene;
	int i = 0;
	int j = 0;
	t_vector ptr;

	ptr = scene->camera.center;
	cl_scene.camera.center = vector_to_double3(ptr);
	ptr = scene->camera.dir;
	cl_scene.camera.dir = vector_to_double3(ptr);

	cl_scene.c_lights = scene->c_lights;
	cl_scene.c_objs = scene->c_objs;

	cl_scene.shadows_on = scene->shadows_on;
	cl_scene.arrows_on = scene->arrows_on;
	while (i < scene->c_objs)
	{
		ptr = scene->objs[i].center;
		cl_scene.objs[i].center = vector_to_double3(ptr);
		ptr = scene->objs[i].dir;
		cl_scene.objs[i].dir = vector_to_double3(ptr);
		ptr = scene->objs[i].rot;
		cl_scene.objs[i].rot = vector_to_double3(ptr);

		ptr = scene->objs[i].p2;
		cl_scene.objs[i].p2 = vector_to_double3(ptr);
		ptr = scene->objs[i].p3;
		cl_scene.objs[i].p3 = vector_to_double3(ptr);



		cl_scene.objs[i].rgb = scene->objs[i].rgb;
		cl_scene.objs[i].radius = scene->objs[i].radius;
		cl_scene.objs[i].type = scene->objs[i].type;
		cl_scene.objs[i].reflective = scene->objs[i].reflective;
		cl_scene.objs[i].specular = scene->objs[i].specular;
		cl_scene.objs[i].angle = scene->objs[i].angle;
		cl_scene.objs[i].tex = scene->objs[i].tex;
		cl_scene.objs[i].transparency = scene->objs[i].transparency;
		cl_scene.objs[i].id = scene->objs[i].id;



		i++;
	}
	i = 0;
	while (i < 3)
	{
		ptr = scene->arrows[i].center;
		cl_scene.arrows[i].center = vector_to_double3(ptr);
		ptr = scene->arrows[i].dir;
		cl_scene.arrows[i].dir = vector_to_double3(ptr);




		cl_scene.arrows[i].rgb = scene->arrows[i].rgb;
		cl_scene.arrows[i].type = scene->arrows[i].type;

	//	cl_scene.arrows[i].id = scene->objs[i].id;



		i++;
	}
	/////
	/*ptr = scene->objs[i].center;
		cl_scene.objs[i].center = vector_to_double3(ptr);*/

/////

	while (j < scene->c_lights)
	{
		cl_scene.lights[j].type = scene->lights[j].type;
		ptr = scene->lights[j].center;
		cl_scene.lights[j].center = vector_to_double3(ptr);
		ptr = scene->lights[j].dir;
		cl_scene.lights[j].dir = vector_to_double3(ptr);
		cl_scene.lights[j].intensity = scene->lights[j].intensity;
		j++;
	}

	pixel_pos_3d = get_pixel_pisition(x - CW / 2, -y + CH / 2);
	pixel_pos_3d = rotate_view(pixel_pos_3d, alpha, beta);

	__global char *data[5];

	data[0] = data0;
	data[1] = data1;
	data[2] = data2;
	data[3] = data3;
	data[4] = data4;


	put_point_to_image(image_data, x, y, cast_ray(&cl_scene, cl_scene.camera.center, pixel_pos_3d, DEPTH, data));

}