#include "rtv1.h"

int get_free_light_id(t_rtv1 *rtv1)
{
    int id;
    int i;
    int f_exit;

    f_exit = 0;
    i = 0;
    id = rand()%200;
    while (rtv1->scene.c_lights)
    {
        i = 0;
        while (i < rtv1->scene.c_lights)
        {
            if (rtv1->scene.lights[i].id == id)
                break;
            
            i++;
            if (i == rtv1->scene.c_lights)
                f_exit = 1;
        }
        if (f_exit)
            break;
        id = rand()%200;
    }
    return (id);
}

void new_point(t_rtv1 *rtv1)
{
    if (rtv1->scene.c_lights == 100)
        return;
    rtv1->scene.lights[rtv1->scene.c_lights].type = point;
    rtv1->scene.lights[rtv1->scene.c_lights].center = (t_vector){0.0, 1.0, 0.0};
    rtv1->scene.lights[rtv1->scene.c_lights].intensity = 0.2;
    rtv1->scene.lights[rtv1->scene.c_lights].id = get_free_light_id(rtv1);
    rtv1->scene.c_lights++;
}

void new_ambient(t_rtv1 *rtv1)
{
    if (rtv1->scene.c_lights == 100)
        return;
    rtv1->scene.lights[rtv1->scene.c_lights].type = ambient;
    rtv1->scene.lights[rtv1->scene.c_lights].center = (t_vector){0.0, 0.0, 0.0};
    rtv1->scene.lights[rtv1->scene.c_lights].intensity = 0.2;
    rtv1->scene.lights[rtv1->scene.c_lights].id = get_free_light_id(rtv1);
    rtv1->scene.c_lights++;
}

void new_directional(t_rtv1 *rtv1)
{
    if (rtv1->scene.c_lights == 100)
        return;
    rtv1->scene.lights[rtv1->scene.c_lights].type = directional;
    rtv1->scene.lights[rtv1->scene.c_lights].center = (t_vector){0.0, 0.0, 0.0};
    rtv1->scene.lights[rtv1->scene.c_lights].dir = (t_vector){0.0, -1.0, 0.0};
    rtv1->scene.lights[rtv1->scene.c_lights].intensity = 0.2;
    rtv1->scene.lights[rtv1->scene.c_lights].id = get_free_light_id(rtv1);
    rtv1->scene.c_lights++;
}