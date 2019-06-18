#include "rtv1.h"

static int	mouse_pressed1(int button, int x, int y, t_rtv1 *rtv1, t_LIBUI_Button *buttons, int c_buttons)
{
	int pressed_button;
	if (y < 0)
		return (0);
	if (button == SDL_BUTTON_LEFT)
	{

			pressed_button = LIBUI_IsButtonPressed(x, y, buttons, c_buttons);
			if (pressed_button == -1)
				return (0);
			buttons[pressed_button].is_pressed = 1;

			if (!ft_strcmp(buttons[pressed_button].function, "Radius+"))
			{	
				rtv1->selected->radius += 0.1;
			}
			else if (!ft_strcmp(buttons[pressed_button].function, "Radius-"))
			{	
				rtv1->selected->radius -= 0.1;
			}
	}
	return (0);
}

static int	mouse_release1(t_LIBUI_Button *buttons ,int c_buttons)
{


		int i = 0;
		while (i < c_buttons)
		{
			(buttons)[i].is_pressed = 0;
			i++;
		}

	return (0);
}


void *edit (t_rtv1 *rtv1)
{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
    SDL_Window *window = SDL_CreateWindow("Properties", 200, 200, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    t_LIBUI_Button buttons[20];
    int c_buttons = 0;

    SDL_FillRect(surface, &((SDL_Rect){0, 0, 640, 480}), 0x00FFFF00);

    LIBUI_NewButton((t_but_constr){20, 20, "Radius+", "Radius+", 0x0000ff55}, buttons, &c_buttons);
    LIBUI_NewButton((t_but_constr){180, 20, "Radius-", "Radius-", 0x0000ff55}, buttons, &c_buttons);

    LIBUI_NewButton((t_but_constr){20, 70, "Angle+", "Angle+", 0x0000ff55}, buttons, &c_buttons);
    LIBUI_NewButton((t_but_constr){180, 70, "Angle-", "Angle-", 0x0000ff55}, buttons, &c_buttons);

    LIBUI_NewButton((t_but_constr){20, 120, "Radius+", "Radius+", 0x0000ff55}, buttons, &c_buttons);
    LIBUI_NewButton((t_but_constr){180, 120, "Radius+", "Radius+", 0x0000ff55}, buttons, &c_buttons);

    LIBUI_NewButton((t_but_constr){20, 170, "Specular+", "Specular+", 0x0000ff55}, buttons, &c_buttons);
    LIBUI_NewButton((t_but_constr){180, 170, "Specular-", "Specular-", 0x0000ff55}, buttons, &c_buttons);

    LIBUI_NewButton((t_but_constr){20, 220, "Reflective+", "Reflective+", 0x0000ff55}, buttons, &c_buttons);
    LIBUI_NewButton((t_but_constr){180, 220, "Reflective-", "Reflective-", 0x0000ff55}, buttons, &c_buttons);

    

    while(1)
    {
        int i = 0;
        while (i < c_buttons)
        {
            SDL_BlitSurface((buttons[i].is_pressed ? buttons[i].pressed : buttons[i].surface), &((SDL_Rect){-buttons[i].x, -buttons[i].y, buttons[i].x + 100, buttons[i].y + 30}), surface, NULL);
            i++;
        }
        SDL_Texture *screen = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderClear(renderer); //Очистка рендера
        SDL_RenderCopy(renderer, screen, NULL, NULL); //Копируем в рендер фон
        SDL_DestroyTexture(screen);
        SDL_RenderPresent(renderer); //Погнали!!

        SDL_Event ev;
        while(SDL_PollEvent(&ev))
            switch(ev.type)
            {
                case SDL_QUIT: 
                    SDL_DestroyWindow(window);
                    return(NULL);
                case SDL_MOUSEBUTTONDOWN:
                    mouse_pressed1(ev.button.button, ev.button.x, ev.button.y, rtv1, buttons, c_buttons);
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouse_release1(buttons, c_buttons);
                    break;
                default: break;
            }
	

        SDL_Delay(10);
    }
}