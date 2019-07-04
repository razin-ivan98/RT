#include "rtv1.h"

int get_free_id(t_rtv1 *rtv1)
{
    int id;
    int i;
    int f_exit;

    f_exit = 0;
    i = 0;
    id = rand();
    while (1)
    {
        i = 0;
        while (i < rtv1->scene.c_objs)
        {
            if (rtv1->scene.objs[i].id == id)
                break;
            
            i++;
            if (i == rtv1->scene.c_objs)
                f_exit = 1;
        }
        if (f_exit)
            break;
        id = rand();
    }
    return (id);
}


void new_sphere(t_rtv1 *rtv1)
{
    rtv1->scene.objs[rtv1->scene.c_objs].type = sphere;
	rtv1->scene.objs[rtv1->scene.c_objs].center = (t_vector){0.0, 0.0, 7.0};
    rtv1->scene.objs[rtv1->scene.c_objs].rot = (t_vector){0.0, 0.0, 0.0};
    rtv1->scene.objs[rtv1->scene.c_objs].radius = 1.0;
    rtv1->scene.objs[rtv1->scene.c_objs].specular = 20.0;
    rtv1->scene.objs[rtv1->scene.c_objs].rgb = (t_rgb){rand()%255, rand()%255, rand()%255};
    rtv1->scene.objs[rtv1->scene.c_objs].reflective = 0.0;
    rtv1->scene.objs[rtv1->scene.c_objs].tex = -1;
    rtv1->scene.objs[rtv1->scene.c_objs].id = get_free_id(rtv1);
    rtv1->scene.c_objs++;

}

void new_cone(t_rtv1 *rtv1)
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
    rtv1->scene.objs[rtv1->scene.c_objs].id = get_free_id(rtv1);

    rtv1->scene.c_objs++;
}

void new_cylinder(t_rtv1 *rtv1)
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
    rtv1->scene.objs[rtv1->scene.c_objs].id = get_free_id(rtv1);
    rtv1->scene.c_objs++;
}

void new_plane(t_rtv1 *rtv1)
{
    rtv1->scene.objs[rtv1->scene.c_objs].type = plane;
    rtv1->scene.objs[rtv1->scene.c_objs].center = (t_vector){0.0, 0.0, 7.0};
    rtv1->scene.objs[rtv1->scene.c_objs].dir = (t_vector){0.0, 1.0, 0.0};
    rtv1->scene.objs[rtv1->scene.c_objs].rot = (t_vector){0.0, 0.0, 0.0};
    rtv1->scene.objs[rtv1->scene.c_objs].rgb = (t_rgb){rand()%255, rand()%255, rand()%255};
    rtv1->scene.objs[rtv1->scene.c_objs].specular = 20.0;
    rtv1->scene.objs[rtv1->scene.c_objs].reflective = 0.0;
    rtv1->scene.objs[rtv1->scene.c_objs].tex = -1;
        rtv1->scene.objs[rtv1->scene.c_objs].id = get_free_id(rtv1);
    rtv1->scene.c_objs++;
}

void new_paraboloid(t_rtv1 *rtv1)
{
    rtv1->scene.objs[rtv1->scene.c_objs].type = paraboloid;
    rtv1->scene.objs[rtv1->scene.c_objs].center = (t_vector){0.0, 0.0, 7.0};
    rtv1->scene.objs[rtv1->scene.c_objs].dir = (t_vector){0.0, 1.0, 0.0};
    rtv1->scene.objs[rtv1->scene.c_objs].rot = (t_vector){0.0, 0.0, 0.0};
    rtv1->scene.objs[rtv1->scene.c_objs].rgb = (t_rgb){rand()%255, rand()%255, rand()%255};
    rtv1->scene.objs[rtv1->scene.c_objs].specular = 20.0;
    rtv1->scene.objs[rtv1->scene.c_objs].reflective = 0.0;
    rtv1->scene.objs[rtv1->scene.c_objs].tex = -1;
    rtv1->scene.objs[rtv1->scene.c_objs].angle = 1.0;
    rtv1->scene.objs[rtv1->scene.c_objs].radius = 1.0;
        rtv1->scene.objs[rtv1->scene.c_objs].id = get_free_id(rtv1);
    rtv1->scene.c_objs++;
}  

void new_triangle(t_rtv1 *rtv1)
{
    rtv1->scene.objs[rtv1->scene.c_objs].type = triangle;
    rtv1->scene.objs[rtv1->scene.c_objs].center = (t_vector){-1.0, 0.0, 6.0};
    rtv1->scene.objs[rtv1->scene.c_objs].p2 = (t_vector){0.0, 1.0, 6.0};
    rtv1->scene.objs[rtv1->scene.c_objs].p3 = (t_vector){1.0, 0.0, 6.0};
    rtv1->scene.objs[rtv1->scene.c_objs].rgb = (t_rgb){rand()%255, rand()%255, rand()%255};
    rtv1->scene.objs[rtv1->scene.c_objs].specular = 20.0;
    rtv1->scene.objs[rtv1->scene.c_objs].reflective = 0.0;
    rtv1->scene.objs[rtv1->scene.c_objs].tex = -1;
    rtv1->scene.objs[rtv1->scene.c_objs].angle = 2.0;
    rtv1->scene.objs[rtv1->scene.c_objs].radius = 0.5;
        rtv1->scene.objs[rtv1->scene.c_objs].id = get_free_id(rtv1);
    rtv1->scene.c_objs++;
}