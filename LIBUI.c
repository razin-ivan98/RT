#include "LIBUI.h"

t_LIBUI_Button LIBUI_CreateButton(t_but_constr input)
{
    t_LIBUI_Button button;

    button.is_pressed = 0;
    button.x = input.x;
    button.y = input.y;
    strcpy(button.function, input.function);
    button.surface = SDL_CreateRGBSurface(0, 100, 30, 32,0,0,0,0);
    SDL_FillRect(button.surface, &((SDL_Rect){0, 0, 100, 30}), input.color);
    button.pressed = SDL_CreateRGBSurface(0, 100, 30, 32, 0,0,0,0);
    SDL_FillRect(button.pressed, &((SDL_Rect){0, 0, 100, 30}), input.color - 0x11111100);


    TTF_Init();
    SDL_Surface *text;
    TTF_Font *font = TTF_OpenFont("lato/Lato-Bold.ttf", 14);
    text = TTF_RenderText_Blended(font, input.text, (SDL_Color){0, 0, 0, 0});
    TTF_Quit();

    SDL_BlitSurface(text, &((SDL_Rect){ -8 , -8 ,100, 30 }), button.surface, NULL);
    SDL_BlitSurface(text, &((SDL_Rect){ -8 , -8 ,100, 30 }), button.pressed, NULL);



    return (button);
}

void LIBUI_NewButton(t_but_constr button, t_LIBUI_Button *buttons, int *c_buttons)
{

    buttons[*c_buttons] = LIBUI_CreateButton(button);
    (*c_buttons)++;
}

int LIBUI_IsButtonPressed(int x, int y, t_LIBUI_Button *buttons, int c_buttons)
{
    int i = 0;
    while (i < c_buttons)
    {
        if ((buttons[i]).x < x && (buttons[i]).y < y && (buttons[i]).y + 30 > y&& (buttons[i]).x + 100 > x)
            return(i);
        i++;
    }
    return -1;
}
