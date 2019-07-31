#include "rtv1.h"

void destroy_buttons(t_rtv1 *rtv1)
{
    int i;

    i = 0;
    while (i < rtv1->c_selector_buttons)
    {
        SDL_FreeSurface(rtv1->selector_buttons[i].pressed);
        SDL_FreeSurface(rtv1->selector_buttons[i].surface);
        i++;
    }
    rtv1->c_selector_buttons = 0;
}

void refresh_selector_buttons(t_rtv1 *rtv1)
{
    int i;
    char str[64];
    char *tmp;

    

    i = 0;
    SDL_FillRect(rtv1->selector_surface, &((SDL_Rect){0, 0, 300, 1000}), 0x00FFFF00);
    destroy_buttons(rtv1);
    while (i < rtv1->scene.c_objs)
    {
        int id = 9999;
        tmp = ft_itoa(rtv1->scene.objs[i].id);
       // free(tmp);
        ft_strcpy(str, tmp);
        ft_strcat(str, " object");
        if (rtv1->scene.objs[i].type == sphere)
            ft_strcat(str, " sph");
        else if (rtv1->scene.objs[i].type == cone)
            ft_strcat(str, " cone");
        else if (rtv1->scene.objs[i].type == cylinder)
            ft_strcat(str, " cyl");
        else if (rtv1->scene.objs[i].type == plane)
            ft_strcat(str, " plane");
        else if (rtv1->scene.objs[i].type == triangle)
            ft_strcat(str, " poly");
        
        if (id == 9999 && rtv1->scene.objs[i].type != triangle)     
            LIBUI_NewButton((t_but_constr){20, 20 + 35*i, str, str, 0x0000ff55}, rtv1->selector_buttons, &(rtv1->c_selector_buttons));
        if (rtv1->scene.objs[i].type == triangle)
            id = rtv1->scene.objs[i].id;
        free(tmp);
        i++;
    }
    i = 0;
    while (i < rtv1->scene.c_lights)
    {
        tmp = ft_itoa(rtv1->scene.lights[i].id);
       // free(tmp);
       // ft_strcpy(str, tmp);
        ft_strcpy(str, tmp);
        ft_strcat(str, " light");
        if (rtv1->scene.lights[i].type == point)
            ft_strcat(str, " point");
        else if (rtv1->scene.lights[i].type == directional)
            ft_strcat(str, " dir");
        else if (rtv1->scene.lights[i].type == ambient)
            ft_strcat(str, " amb");
        LIBUI_NewButton((t_but_constr){150, 20 + 35*i, str, str, 0x0000ff55}, rtv1->selector_buttons, &(rtv1->c_selector_buttons));
        free(tmp);
        i++;
    }
}

void scene_selector(t_rtv1 *rtv1)
{
    rtv1->selector_surface = SDL_CreateRGBSurface(0, 300, 1000, 32, 0, 0, 0, 0);
    rtv1->selector_window = SDL_CreateWindow("Selector", 100, 200, 300, 1000, SDL_WINDOW_SHOWN);
    rtv1->selector_renderer = SDL_CreateRenderer(rtv1->selector_window, -1, SDL_RENDERER_ACCELERATED);
    
	rtv1->selector_window_active = rtv1->next_win_ID;
    rtv1->next_win_ID++;

    

    refresh_selector_buttons(rtv1);
}

void destroy_scene_selector(t_rtv1 *rtv1)
{
    SDL_DestroyWindow(rtv1->selector_window);
	rtv1->selector_window_active = 0;
    SDL_FreeSurface(rtv1->selector_surface);
}