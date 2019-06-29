/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 19:25:19 by cocummin          #+#    #+#             */
/*   Updated: 2019/06/29 13:27:37 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
/*
void	texture_load(void *mlx_ptr, char **data, char *file_name)
{
	int endian;
	int bytes;
	int width;
	int height;
	int len;

	bytes = 8;
	endian = 0;
	len = 1024;
	void* image = mlx_xpm_file_to_image(mlx_ptr,
	file_name, &width, &height);
	*data = mlx_get_data_addr(image,
	&bytes, &len, &endian);
}
*/
void		provider(t_rtv1 *rtv1)
{



	rtv1->ret = clEnqueueWriteBuffer(rtv1->command_queue,
			rtv1->utils_memobj, CL_TRUE, 0,
			sizeof(t_scene), &(rtv1->scene), 0, NULL, NULL);
	rtv1->ret = clSetKernelArg(rtv1->kernel, 1,
			sizeof(cl_mem), (void *)&rtv1->utils_memobj);
	rtv1->ret = clEnqueueNDRangeKernel(rtv1->command_queue, rtv1->kernel, 1,
			NULL, &rtv1->global_work_size, NULL, 0, NULL, NULL);
	rtv1->ret = clEnqueueReadBuffer(rtv1->command_queue, rtv1->memobj,
			CL_TRUE, 0, CW * CH * 4, (void *)rtv1->surface->pixels, 0, NULL, NULL);

	rtv1->screen = SDL_CreateTextureFromSurface(rtv1->renderer, rtv1->surface);

	SDL_BlitSurface(rtv1->tmp, NULL, rtv1->ui, NULL);
	int i = 0;
    while (i < rtv1->c_buttons)
    {
        SDL_BlitSurface((rtv1->buttons[i].is_pressed ? rtv1->buttons[i].pressed : rtv1->buttons[i].surface), &((SDL_Rect){-rtv1->buttons[i].x, -rtv1->buttons[i].y, rtv1->buttons[i].x + 100, rtv1->buttons[i].y + 30}), rtv1->ui, NULL);
        i++;
    }
	i = 0;
	while (i < rtv1->c_edits)
    {
        SDL_BlitSurface((rtv1->active_edit == i ? rtv1->edits[i].with_text_active : rtv1->edits[i].with_text), &((SDL_Rect){-rtv1->edits[i].x, -rtv1->edits[i].y, rtv1->edits[i].x + 100, rtv1->edits[i].y + 30}), rtv1->ui, NULL);
        i++;
    }
	i = 0;
	while (i < rtv1->c_lists)
	{
		SDL_BlitSurface((rtv1->lists[i].items[0].is_pressed ? rtv1->lists[i].items[0].pressed : rtv1->lists[i].items[0].surface), &((SDL_Rect){-rtv1->lists[i].items[0].x, -rtv1->lists[i].items[0].y, rtv1->lists[i].items[0].x + 100, rtv1->lists[i].items[0].y + 30}), rtv1->ui, NULL);
		
		if (rtv1->lists[i].is_dropped)
		{
			//puts("lol");
			int j = 1;
			//printf("%d\n\n", rtv1->lists[i].c_items);
			while (j < rtv1->lists[i].c_items)
			{
				//printf("%d\n", j);
								//puts(rtv1->lists[i].items[j].function);
				SDL_BlitSurface((rtv1->lists[i].items[j].is_pressed ? rtv1->lists[i].items[j].pressed : rtv1->lists[i].items[j].surface), &((SDL_Rect){-rtv1->lists[i].items[j].x, -rtv1->lists[i].items[j].y, rtv1->lists[i].items[j].x + 100, rtv1->lists[i].items[j].y + 30}), rtv1->ui, NULL);
				j++;
			}
		}
		i++;
	}

	rtv1->ui_tex = SDL_CreateTextureFromSurface(rtv1->renderer, rtv1->ui);
	//SDL_FreeSurface(rtv1->surface);
	SDL_RenderClear(rtv1->renderer); //Очистка рендера
    SDL_RenderCopy(rtv1->renderer, rtv1->screen, NULL, &rtv1->rect); //Копируем в рендер фон
	SDL_RenderCopy(rtv1->renderer, rtv1->ui_tex, NULL, &((SDL_Rect){CH, 0, 400, CH})); //Копируем в рендер фон
	SDL_DestroyTexture(rtv1->screen);
	SDL_DestroyTexture(rtv1->ui_tex);
    SDL_RenderPresent(rtv1->renderer); //Погнали!!

	if (!(rtv1->selected))
		rtv1->selected = &(rtv1->scene.objs[0]);



	/*int x,y;
        SDL_GetRelativeMouseState(&x,&y);*/

	SDL_Event ev;
    while(SDL_PollEvent(&ev))
        switch(ev.type)
        {
            case SDL_QUIT: 
                SDL_DestroyWindow(rtv1->window);
                SDL_Quit();
                exit(1);
			case SDL_KEYDOWN:
				key_pressed(ev.key.keysym.sym, rtv1);
				break;
			case SDL_KEYUP:
				key_release(ev.key.keysym.sym, rtv1);
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouse_pressed(ev.button.button, ev.button.x, ev.button.y, rtv1);
				break;
			case SDL_MOUSEBUTTONUP:
				mouse_release(ev.button.button, ev.button.x, ev.button.y, rtv1);
				break;
			case SDL_MOUSEMOTION:
				mouse_move(ev.motion.x, ev.motion.y, rtv1);
			default: break;
        }
	

    SDL_Delay(10);
}

int			main(int ac, char **av)
{
	static t_rtv1 rtv1;

	if (ac != 2)
	{
		ft_putendl("Argument is not valid.");
		err_exit();
	}
	scene_init(&rtv1, av[1]);
	graphics_init(&rtv1);
	set_start_angles(&(rtv1.scene));
	while(1)
		provider(&rtv1);
	/*mlx_hook(rtv1.win_ptr, 2, 0, key_pressed, &rtv1);
	mlx_hook(rtv1.win_ptr, 4, 0, mouse_pressed, &rtv1);
	mlx_hook(rtv1.win_ptr, 5, 0, mouse_release, &rtv1);
	mlx_hook(rtv1.win_ptr, 6, 0, mouse_move, &rtv1);
	mlx_hook(rtv1.win_ptr, 17, 0, clean_exit, &rtv1);
	mlx_loop(rtv1.mlx_ptr);*/
	return (0);
}
