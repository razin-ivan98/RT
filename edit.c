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


void edit (t_rtv1 *rtv1)
{
    rtv1->edit_surface = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
    rtv1->edit_window = SDL_CreateWindow("Properties", 1800, 200, 640, 480, SDL_WINDOW_SHOWN);
    rtv1->edit_renderer = SDL_CreateRenderer(rtv1->edit_window, -1, SDL_RENDERER_ACCELERATED);
    rtv1->c_edit_buttons = 0;
    rtv1->edit_window_active = rtv1->next_win_ID;
	rtv1->next_win_ID++;
	

    SDL_FillRect(rtv1->edit_surface, &((SDL_Rect){0, 0, 640, 480}), 0x00FFFF00);

    LIBUI_NewButton((t_but_constr){20, 20, "Radius+", "Radius+", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
    LIBUI_NewButton((t_but_constr){180, 20, "Radius-", "Radius-", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);

    LIBUI_NewButton((t_but_constr){20, 70, "Angle+", "Angle+", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
    LIBUI_NewButton((t_but_constr){180, 70, "Angle-", "Angle-", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);

    LIBUI_NewButton((t_but_constr){20, 120, "Radius+", "Radius+", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
    LIBUI_NewButton((t_but_constr){180, 120, "Radius+", "Radius+", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);

    LIBUI_NewButton((t_but_constr){20, 170, "Specular+", "Specular+", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
    LIBUI_NewButton((t_but_constr){180, 170, "Specular-", "Specular-", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);

    LIBUI_NewButton((t_but_constr){20, 220, "Reflective+", "Reflective+", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
    LIBUI_NewButton((t_but_constr){180, 220, "Reflective-", "Reflective-", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);

    LIBUI_NewButton((t_but_constr){20, 270, "Transparency+", "Transparency+", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
    LIBUI_NewButton((t_but_constr){180, 270, "Transparency-", "Transparency-", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);

    LIBUI_NewButton((t_but_constr){20, 320, "Refractive+", "Refractive+", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
    LIBUI_NewButton((t_but_constr){180, 320, "Refractive-", "Refractive-", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);

    LIBUI_NewButton((t_but_constr){20, 370, "Smoothness+", "Smoothness+", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
    LIBUI_NewButton((t_but_constr){180, 370, "Smoothness-", "Smoothness-", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);

	LIBUI_NewButton((t_but_constr){340, 20, "Texture1", "Texture1", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
	LIBUI_NewButton((t_but_constr){340, 70, "Texture2", "Texture2", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
	LIBUI_NewButton((t_but_constr){340, 120, "Texture3", "Texture3", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
	LIBUI_NewButton((t_but_constr){340, 170, "Texture4", "Texture4", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
	LIBUI_NewButton((t_but_constr){340, 220, "Texture5", "Texture5", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
	LIBUI_NewButton((t_but_constr){340, 270, "Rand Color", "Rand Color", 0x0000ff55}, rtv1->edit_buttons, &rtv1->c_edit_buttons);
 //   while(1)
   // {
    
}

void destroy_edit(t_rtv1 *rtv1)
{
	int i;

	i = 0;
	while (i < rtv1->c_edit_buttons)
	{
		SDL_FreeSurface(rtv1->edit_buttons[i].pressed);
		SDL_FreeSurface(rtv1->edit_buttons[i].surface);
		i++;
	}
	rtv1->c_edit_buttons = 0;
	SDL_FreeSurface(rtv1->edit_surface);
	SDL_DestroyWindow(rtv1->edit_window);
	rtv1->edit_window_active = 0;
	
}