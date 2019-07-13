#include "rtv1.h"

void selector_mouse_pressed(int button, int x, int y, t_rtv1 *rtv1)
{
    int pressed_button = LIBUI_IsButtonPressed(x, y, rtv1->selector_buttons, rtv1->c_selector_buttons);
    char func[64];
    int  id;
    if (pressed_button == -1)
        return;
    ft_strcpy(func, rtv1->selector_buttons[pressed_button].function);
    id = ft_atoi(func);
	//printf("%d\n", id);
	if (ft_strstr(func, "object"))
	{
		id = get_index_by_id(&rtv1->scene, id);
		rtv1->selected = &rtv1->scene.objs[id];
		rtv1->selected_light = NULL;
		set_arrows_pos(rtv1, 0);
	
	}
	else if (ft_strstr(func, "light"))
	{
		id = get_light_index_by_id(&rtv1->scene, id);
		rtv1->selected_light = &rtv1->scene.lights[id];
	//	rtv1->selected = NULL;
		set_arrows_pos(rtv1, 1);
	}
	
	rtv1->scene.arrows_on = 1;
}

void	selector_mouse_release(int button, int x, int y, t_rtv1 *rtv1)
{
	(void)x;
	(void)y;
	(void)button;

	int i = 0;
	while (i < rtv1->c_edit_buttons)
	{
		(rtv1->edit_buttons)[i].is_pressed = 0;
		i++;
	}
    i = 0;
	while (i < rtv1->c_selector_buttons)
	{
		(rtv1->selector_buttons)[i].is_pressed = 0;
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