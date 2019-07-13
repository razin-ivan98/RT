#include "rtv1.h"

void edit_mouse_pressed(int button, int x, int y, t_rtv1 *rtv1)
{
    int pressed_button = LIBUI_IsButtonPressed(x, y, rtv1->edit_buttons, rtv1->c_edit_buttons);
    char func[64];
    if (pressed_button == -1)
        return;
    ft_strcpy(func, rtv1->edit_buttons[pressed_button].function);

    if (!ft_strcmp(func, "Transparency+"))
	{	
		rtv1->selected->transparency += 0.1;
		if (rtv1->selected->transparency > 1.0)
			rtv1->selected->transparency = 1.0;
	}
    else if (!ft_strcmp(func, "Transparency-"))
    {	
        rtv1->selected->transparency -= 0.1;
        if (rtv1->selected->transparency < 0.0)
            rtv1->selected->transparency = 0.0;
    }
    else if (!ft_strcmp(func, "Refractive+"))
    {
        rtv1->selected->refractive += 0.1;
    }
    else if (!ft_strcmp(func, "Refractive-"))
    {	
        rtv1->selected->refractive -= 0.1;
        if (rtv1->selected->refractive < 1.0)
            rtv1->selected->refractive = 1.0;
    }
    else if (!ft_strcmp(func, "Radius+"))
    {	
        rtv1->selected->radius += 0.1;
    }
    else if (!ft_strcmp(func, "Radius-") && rtv1->selected->radius > 0.1)
    {	
     rtv1->selected->radius -= 0.1;
    }

    else if (!ft_strcmp(func, "Angle+"))
    {	
        rtv1->selected->angle += 0.1;
    }
        else if (!ft_strcmp(func, "Angle-"))
    {	
        rtv1->selected->angle -= 0.1;
    }

    else if (!ft_strcmp(func, "Specular+"))
    {	
        rtv1->selected->specular += 20.0;
    }
    else if (!ft_strcmp(func, "Specular-") && rtv1->selected->specular > 0.0)
    {	
        rtv1->selected->specular -= 20.0;
    }

    else if (!ft_strcmp(func, "Reflective+") && rtv1->selected->reflective < 1.0)
    {	
        rtv1->selected->reflective += 0.1;
        if (rtv1->selected->reflective > 1.0)
            rtv1->selected->reflective = 1.0;
    }
    else if (!ft_strcmp(func, "Reflective-"))
    {	
        rtv1->selected->reflective -= 0.1;
        if (rtv1->selected->reflective < 0.0)
            rtv1->selected->reflective = 0.0;
    }
    else if (!ft_strcmp(func, "Smoothness+") && rtv1->selected->reflective < 1.0)
    {	
        rtv1->selected->smoothness += 0.1;
        if (rtv1->selected->smoothness > 1.0)
            rtv1->selected->smoothness = 1.0;
    }
    else if (!ft_strcmp(func, "Smoothness-"))
    {	
        rtv1->selected->smoothness -= 0.1;
        if (rtv1->selected->smoothness < 0.0)
            rtv1->selected->smoothness = 0.0;
    }
    			
    else if (!ft_strcmp(func, "Texture1"))
    {	
        rtv1->selected->tex = 0;
    }
    else if (!ft_strcmp(func, "Texture2"))
    {
        rtv1->selected->tex = 1;
    }
    else if (!ft_strcmp(func, "Texture3"))
    {	
        rtv1->selected->tex = 2;
    }
    else if (!ft_strcmp(func, "Texture4"))
    {	
        rtv1->selected->tex = 3;
    }
    else if (!ft_strcmp(func, "Texture5"))
    {	
        rtv1->selected->tex = 4;
    }
        else if (!ft_strcmp(func, "Rand Color"))
    {	
        rtv1->selected->tex = -1;
        rtv1->selected->rgb = (t_rgb){rand()%255, rand()%255, rand()%255};
    }
}

void	edit_mouse_release(int button, int x, int y, t_rtv1 *rtv1)
{
	(void)x;
	(void)y;
	(void)button;

    int i = 0;
	while (i < rtv1->c_selector_buttons)
	{
		(rtv1->selector_buttons)[i].is_pressed = 0;
		i++;
	}
    i = 0;
	while (i < rtv1->c_edit_buttons)
	{
		(rtv1->edit_buttons)[i].is_pressed = 0;
		i++;
	}
    i = 0;
    while (i < rtv1->c_buttons)
	{
		(rtv1->buttons)[i].is_pressed = 0;
		i++;
	}
    i = 0;
	while (i < rtv1->c_lists)
	{
			//rtv1->lists[i].is_dropped = 0;
		int j = 0;
		while (j < rtv1->lists[i].c_items)
		{
			(rtv1->lists)[i].items[j].is_pressed = 0;
			j++;
		}
		i++;
	}

}