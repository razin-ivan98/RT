/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 19:25:19 by cocummin          #+#    #+#             */
/*   Updated: 2019/07/31 14:18:22 by chorange         ###   ########.fr       */
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

/*	int h = 0;
	//srand(time(NULL));
	while (h < 500)
	{
		rtv1->scene.rands[h] = (double)rand() / (double)rand();
		//printf("%f\n", rtv1->scene.rands[h]);
		h++;
	}*/
		int h = 0;
	
	//srand(time(NULL));
	int fd;

	fd = open("/dev/urandom", O_RDONLY);
	char str[501];
	int rand_suka;
	read(fd, str, 501);
	close (fd);
	//double *ptr = (double *)(&str[0]);
	while (h < 500)
	{
		rand_suka = rand()%2;

		rtv1->scene.rands[h] = (double)str[h];
		if (rtv1->scene.rands[h] < 0.0000001 || rtv1->scene.rands[h] > -0.0000001)
			rtv1->scene.rands[h] = (double)rand()/12.58;
		else
			rtv1->scene.rands[h] += rtv1->scene.rands[h+1] / 59.36;
		rtv1->scene.rands[h] *= rand_suka == 1 ? -1.0 : 1.0;
		while (fabs(rtv1->scene.rands[h]) < 1000.0)
		{
			rtv1->scene.rands[h] *= 10.0;
		}
		while (fabs(rtv1->scene.rands[h]) > 10000.0)
		{
			rtv1->scene.rands[h] /= 10.0;
		}
		//printf("%f\n", rtv1->scene.rands[h]);
		h++;
		//sign *= -1.0; 
	}
	rtv1->ret = clEnqueueWriteBuffer(rtv1->command_queue, rtv1->memobj,
		CL_TRUE, 0, CW * CH * 4, (char *)rtv1->surface->pixels, 0, NULL, NULL);////////////////////////
	rtv1->ret = clSetKernelArg(rtv1->kernel, 0, sizeof(cl_mem),
		(void *)&rtv1->memobj);
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

	i = 0;
	if (rtv1->edit_window_active)
	{
        while (i < rtv1->c_edit_buttons)
        {
            SDL_BlitSurface((rtv1->edit_buttons[i].is_pressed ? rtv1->edit_buttons[i].pressed : rtv1->edit_buttons[i].surface), &((SDL_Rect){-rtv1->edit_buttons[i].x, -rtv1->edit_buttons[i].y, rtv1->edit_buttons[i].x + 100, rtv1->edit_buttons[i].y + 30}), rtv1->edit_surface, NULL);
            i++;
        }
        rtv1->edit_texture = SDL_CreateTextureFromSurface(rtv1->edit_renderer, rtv1->edit_surface);
        SDL_RenderClear(rtv1->edit_renderer); //Очистка рендера
        SDL_RenderCopy(rtv1->edit_renderer, rtv1->edit_texture, NULL, NULL); //Копируем в рендер фон
        SDL_DestroyTexture(rtv1->edit_texture);
        SDL_RenderPresent(rtv1->edit_renderer); //Погнали!!
	}

	i = 0;
	if (rtv1->selector_window_active)
	{
        while (i < rtv1->c_selector_buttons)
        {
            SDL_BlitSurface((rtv1->selector_buttons[i].is_pressed ? rtv1->selector_buttons[i].pressed : rtv1->selector_buttons[i].surface), &((SDL_Rect){-rtv1->selector_buttons[i].x, -rtv1->selector_buttons[i].y, rtv1->selector_buttons[i].x + 100, rtv1->selector_buttons[i].y + 30}), rtv1->selector_surface, NULL);
            i++;
        }
        rtv1->selector_texture = SDL_CreateTextureFromSurface(rtv1->selector_renderer, rtv1->selector_surface);
        SDL_RenderClear(rtv1->selector_renderer); //Очистка рендера
        SDL_RenderCopy(rtv1->selector_renderer, rtv1->selector_texture, NULL, NULL); //Копируем в рендер фон
        SDL_DestroyTexture(rtv1->selector_texture);
        SDL_RenderPresent(rtv1->selector_renderer); //Погнали!!
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
	{
		//printf("\n%d", ev.window.windowID);
        switch(ev.type)
        {
			
       /*     case SDL_QUIT:
				
				if (ev.window.windowID == 1)
				{
					//if (rtv1->edit_window_active)
				//		SDL_DestroyWindow(rtv1->edit_window);
					SDL_DestroyWindow(rtv1->window);
					SDL_Quit();
					exit(1);
				}
				else
				{
					//SDL_DestroyWindow(rtv1->edit_window);
					SDL_DestroyWindow(rtv1->window);
					SDL_Quit();
					exit(1);
				}
				break;*/
			case SDL_WINDOWEVENT:
				if (ev.window.event == 14 && ev.window.windowID == 1)
				{
					if (rtv1->edit_window_active)
						SDL_DestroyWindow(rtv1->edit_window);
					if (rtv1->selector_window_active)
						SDL_DestroyWindow(rtv1->edit_window);
					SDL_DestroyWindow(rtv1->window);
					SDL_Quit();
					exit(1);
				}
				else if (ev.window.event == 14 && ev.window.windowID == rtv1->edit_window_active)
				{
					destroy_edit(rtv1);
					//SDL_DestroyWindow(rtv1->window);
					//SDL_Quit();
					//exit(1);
				}
				else if (ev.window.event == 14 && ev.window.windowID == rtv1->selector_window_active)
				{
					destroy_scene_selector(rtv1);
					//SDL_DestroyWindow(rtv1->window);
					//SDL_Quit();
					//exit(1);
				}
				//printf("\t%d", ev.window.event);
				break;
			case SDL_KEYDOWN:
				//printf("%d\n", ev.window.windowID);
				key_pressed(ev.key.keysym.sym, rtv1);
				break;
			case SDL_KEYUP:
				key_release(ev.key.keysym.sym, rtv1);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ev.window.windowID == 1)
					mouse_pressed(ev.button.button, ev.button.x, ev.button.y, rtv1);
				else if (ev.window.windowID == rtv1->edit_window_active)
					edit_mouse_pressed(ev.button.button, ev.button.x, ev.button.y, rtv1);
				else if (ev.window.windowID == rtv1->selector_window_active)
					selector_mouse_pressed(ev.button.button, ev.button.x, ev.button.y, rtv1);
				break;
			case SDL_MOUSEBUTTONUP:
				if (ev.window.windowID == 1)
					mouse_release(ev.button.button, ev.button.x, ev.button.y, rtv1);
				else if (ev.window.windowID == rtv1->edit_window_active)
					edit_mouse_release(ev.button.button, ev.button.x, ev.button.y, rtv1);
				else if (ev.window.windowID == rtv1->selector_window_active)
					selector_mouse_release(ev.button.button, ev.button.x, ev.button.y, rtv1);
				break;
			case SDL_MOUSEMOTION:
				if (ev.window.windowID == 1)
					mouse_move(ev.motion.x, ev.motion.y, rtv1);
				break;
			case SDL_MOUSEWHEEL:
				if (ev.window.windowID == 1)
					mouse_wheel(ev.wheel.y, rtv1);
				break;
			default: break;
        }
	}

    SDL_Delay(10);
}

int			main(int ac, char **av)
{
	static t_rtv1 rtv1;

	if (ac < 2 || ac > 4)
	{
		ft_putendl("Argument is not valid.");
		err_exit();
	}
	if ((ac > 2 && (ft_strcmp(av[2], "obj")) && (ft_strcmp(av[2], "admin"))) || ac == 2)
	{
		if (check_crypto_key(av[1]))
		{
			ft_putendl("иди нахуй, файл поврежден");
			exit(-2);
		}
	}
	rtv1.from_obj = 0;
	if (ac > 2 && (!ft_strcmp(av[2], "obj") || !ft_strcmp(av[2], "obj")))
	{
		rtv1.from_obj = 1;
	}
	scene_init(&rtv1, av[1]);
	//printf("\n%d\n", rtv1.scene.c_lights);
	graphics_init(&rtv1);
	set_start_angles(&(rtv1.scene));
	while(1)
		provider(&rtv1);

	return (0);
}
