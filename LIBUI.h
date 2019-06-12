#ifndef LIBUI_H
# define LIBUI_H
# include "frameworks/SDL2.framework/Headers/SDL.h"
//# include <SDL_ttf.h>
//# include <SDL_mixer.h>
#include "frameworks/SDL2_image.framework/Headers/SDL_image.h"
#include "frameworks/SDL2_ttf.framework/Headers/SDL_ttf.h"

typedef struct s_but_constr
{
    int x;
    int y;
    char *text;
    char *function;
    int color;
}               t_but_constr;

typedef struct s_LIBUI_Button
{
    int x;
    int y;
    SDL_Surface *surface;
    SDL_Surface *pressed;
    char function[128];
    int is_pressed;
}               t_LIBUI_Button;


    int LIBUI_IsButtonPressed(int x, int y, t_LIBUI_Button *buttons, int c_buttons);
    void LIBUI_NewButton(t_but_constr button, t_LIBUI_Button *buttons, int *c_buttons);
#endif