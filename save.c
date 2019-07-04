#include "rtv1.h"

void save(t_rtv1 *rtv1, char *filename)
{
    int fd;

    fd = open(filename, O_TRUNC | O_RDWR | O_CREAT ,  S_IRWXU | S_IRWXG | S_IRWXO);

    int i;
    i = 0;
    ft_putstr_fd("camera_init\n{\n\tposition = (", fd);
    ft_putstr_fd(ft_ftoa(rtv1->scene.camera.center.x), fd);
    ft_putstr_fd(", ", fd);
    ft_putstr_fd(ft_ftoa(rtv1->scene.camera.center.y), fd);
    ft_putstr_fd(", ", fd);
    ft_putstr_fd(ft_ftoa(rtv1->scene.camera.center.z), fd);
    ft_putstr_fd(")\n\tdirection = (", fd);
    ft_putstr_fd(ft_ftoa(rtv1->scene.camera.dir.x), fd);
    ft_putstr_fd(", ", fd);
    ft_putstr_fd(ft_ftoa(rtv1->scene.camera.dir.y), fd);
    ft_putstr_fd(", ", fd);
    ft_putstr_fd(ft_ftoa(rtv1->scene.camera.dir.z), fd);
    ft_putstr_fd(")\n}\n\n", fd);

    while (i < rtv1->scene.c_objs)
    {
        ft_putstr_fd("new object\n{\n\ttype = ", fd);
        if (rtv1->scene.objs[i].type == sphere)
            ft_putstr_fd("sphere\n", fd);
        else if (rtv1->scene.objs[i].type == cylinder)
            ft_putstr_fd("cylinder\n", fd);
        else if (rtv1->scene.objs[i].type == cone)
            ft_putstr_fd("cone\n", fd);
        else if (rtv1->scene.objs[i].type == plane)
            ft_putstr_fd("plane\n", fd);
        else if (rtv1->scene.objs[i].type == paraboloid)
            ft_putstr_fd("paraboloid\n", fd);
        else if (rtv1->scene.objs[i].type == triangle)
            ft_putstr_fd("triangle\n", fd);
        ft_putstr_fd("\tposition = (", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].center.x), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].center.y), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].center.z), fd);

        ft_putstr_fd(")\n\tpos2 = (", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].p2.x), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].p2.y), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].p2.z), fd);

        ft_putstr_fd(")\n\tpos3 = (", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].p3.x), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].p3.y), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].p3.z), fd);

        ft_putstr_fd(")\n\tdirection = (", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].dir.x), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].dir.y), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].dir.z), fd);
        ft_putstr_fd(")\n\trotate = (", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].rot.x), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].rot.y), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].rot.z), fd);
        ft_putstr_fd(")\n\tradius = ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].radius), fd);
        ft_putstr_fd("\n\tcolor = ", fd);
        ft_putstr_fd(ft_itoa(rgb_to_color(rtv1->scene.objs[i].rgb)), fd);
        ft_putstr_fd("\n\tspecular = ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].specular), fd);
        ft_putstr_fd("\n\treflective = ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].reflective), fd);
        ft_putstr_fd("\n\tangle = ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.objs[i].angle), fd);
        ft_putstr_fd("\n\ttexture-id = ", fd);
        ft_putstr_fd(ft_itoa(rtv1->scene.objs[i].tex), fd);
        ft_putstr_fd("\n}\n\n", fd);
        i++;
    }
    i = 0;
    while (i < rtv1->scene.c_lights)
    {
        ft_putstr_fd("new light\n{\n\ttype = ", fd);
        if (rtv1->scene.lights[i].type == point)
            ft_putstr_fd("point\n", fd);
        else if (rtv1->scene.lights[i].type == ambient)
            ft_putstr_fd("ambient\n", fd);
        else if (rtv1->scene.lights[i].type == directional)
            ft_putstr_fd("directional\n", fd);

        ft_putstr_fd("\tintensity = ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.lights[i].intensity), fd);
        ft_putstr_fd("\n\tposition = (", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.lights[i].center.x), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.lights[i].center.y), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.lights[i].center.z), fd);
        ft_putstr_fd(")\n\tdirection = (", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.lights[i].dir.x), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.lights[i].dir.y), fd);
        ft_putstr_fd(", ", fd);
        ft_putstr_fd(ft_ftoa(rtv1->scene.lights[i].dir.z), fd);
        ft_putstr_fd(")\n}\n", fd);
        i++;
    }

    ft_putstr_fd("\n\nend", fd);

    close(fd);

}

void save_as(t_rtv1 *rtv1)
{
    char full_name[256];
    char base_name[64];

    ft_strcpy(base_name, rtv1->edits[LIBUI_GetEditWithFunction(rtv1->edits, rtv1->c_edits, "Name")].text);
    if (ft_strlen(base_name))
    {
        ft_strcpy(full_name, "scenes/");
        ft_strcat(full_name, base_name);
        save(rtv1, full_name);
        ft_strcpy(rtv1->scene_file_name, full_name);
    }
}