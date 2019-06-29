/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 11:10:50 by chorange          #+#    #+#             */
/*   Updated: 2019/06/29 16:33:17 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		scene_init(t_rtv1 *rtv1, char *name)
{

	ft_strcpy(rtv1->scene_file_name, name);
	
	rtv1->scene.shadows_on = 1;
	read_scene(&(rtv1->scene), rtv1->scene_file_name);
}

static void	compile_from_file(char *file_name, t_rtv1 *rtv1)
{
	size_t	source_size;
	char	*source_str;
	int		fd;

	fd = open(file_name, O_RDONLY);
	if (fd <= 0)
	{
		err_exit();
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_str[read(fd, source_str, MAX_SOURCE_SIZE)] = 0;
	source_size = ft_strlen(source_str);
	close(fd);
	rtv1->program = clCreateProgramWithSource(rtv1->context, 1,
		(const char **)&source_str, (const size_t *)&source_size, &rtv1->ret);
	rtv1->ret = clBuildProgram(rtv1->program, 1, &rtv1->device_id,
		NULL, NULL, NULL);
		printf("%d", rtv1->ret);

	size_t l_size;
	char *logg = NULL;
		clGetProgramBuildInfo(rtv1->program, rtv1->device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &l_size);
		logg = malloc(l_size);
		clGetProgramBuildInfo(rtv1->program, rtv1->device_id, CL_PROGRAM_BUILD_LOG, l_size, logg, NULL);
		puts(logg);
	free(logg);
//	free(source_str);
}

static void	kernel_init(t_rtv1 *rtv1)
{
	rtv1->ret = clGetPlatformIDs(1, &rtv1->platform_id,
		&rtv1->ret_num_platforms);
	rtv1->ret = clGetDeviceIDs(rtv1->platform_id, CL_DEVICE_TYPE_GPU,
		1, &rtv1->device_id, &rtv1->ret_num_devices);
	rtv1->context = clCreateContext(NULL, 1, &rtv1->device_id, NULL,
		NULL, &rtv1->ret);
	rtv1->command_queue = clCreateCommandQueue(rtv1->context, rtv1->device_id,
		0, &rtv1->ret);
	compile_from_file("kernels/kernel.cl", rtv1);
	rtv1->kernel = clCreateKernel(rtv1->program, "mishania", &rtv1->ret);
	rtv1->memobj = clCreateBuffer(rtv1->context, CL_MEM_READ_WRITE,
		CW * CH * 4, NULL, &rtv1->ret);
	rtv1->ret = clEnqueueWriteBuffer(rtv1->command_queue, rtv1->memobj,
		CL_TRUE, 0, CW * CH * 4, (char *)rtv1->surface->pixels, 0, NULL, NULL);////////////////////////
	rtv1->ret = clSetKernelArg(rtv1->kernel, 0, sizeof(cl_mem),
		(void *)&rtv1->memobj);
	rtv1->utils_memobj = clCreateBuffer(rtv1->context,
		CL_MEM_READ_WRITE, sizeof(t_scene), NULL, &rtv1->ret);

	rtv1->global_work_size = CW * CH;

	SDL_Surface *textures[5];
	textures[0] = SDL_LoadBMP("tex1.bmp");
	textures[1] = SDL_LoadBMP("tex2.bmp");
	textures[2] = SDL_LoadBMP("tex3.bmp");
	textures[3] = SDL_LoadBMP("tex4.bmp");
	textures[4] = SDL_LoadBMP("tex5.bmp");

	char *texes;
	//texes = (char *)malloc(1024 * 1024 * 3 * 5 + 1);
	/*
	ft_strncpy(texes, textures[0]->pixels, 1024 * 1024 * 3);
	ft_strlcat(texes, textures[1]->pixels, 1024 * 1024 * 3);
	ft_strlcat(texes, textures[2]->pixels, 1024 * 1024 * 3);
	ft_strlcat(texes, textures[3]->pixels, 1024 * 1024 * 3);
	ft_strlcat(texes, textures[4]->pixels, 1024 * 1024 * 3);*/
	

	cl_mem tex2;
	cl_mem tex3;
	cl_mem tex4;
	cl_mem tex5;


	rtv1->tex = clCreateBuffer(rtv1->context, CL_MEM_READ_WRITE,
		1024 * 1024 * 3, NULL, &rtv1->ret);
	rtv1->ret = clEnqueueWriteBuffer(rtv1->command_queue,
			rtv1->tex, CL_TRUE, 0,
			1024 * 1024 * 3, (unsigned char *)textures[0]->pixels, 0, NULL, NULL);
	rtv1->ret = clSetKernelArg(rtv1->kernel, 2,
			sizeof(cl_mem), (void *)&rtv1->tex);

	rtv1->tex = clCreateBuffer(rtv1->context, CL_MEM_READ_WRITE,
		1024 * 1024 * 3, NULL, &rtv1->ret);
	rtv1->ret = clEnqueueWriteBuffer(rtv1->command_queue,
			rtv1->tex, CL_TRUE, 0,
			1024 * 1024 * 3, (unsigned char *)textures[1]->pixels, 0, NULL, NULL);
	rtv1->ret = clSetKernelArg(rtv1->kernel, 3,
			sizeof(cl_mem), (void *)&rtv1->tex);

	rtv1->tex = clCreateBuffer(rtv1->context, CL_MEM_READ_WRITE,
		1024 * 1024 * 3, NULL, &rtv1->ret);
	rtv1->ret = clEnqueueWriteBuffer(rtv1->command_queue,
			rtv1->tex, CL_TRUE, 0,
			1024 * 1024 * 3, (unsigned char *)textures[2]->pixels, 0, NULL, NULL);
	rtv1->ret = clSetKernelArg(rtv1->kernel, 4,
			sizeof(cl_mem), (void *)&rtv1->tex);

	rtv1->tex = clCreateBuffer(rtv1->context, CL_MEM_READ_WRITE,
		1024 * 1024 * 3, NULL, &rtv1->ret);
	rtv1->ret = clEnqueueWriteBuffer(rtv1->command_queue,
			rtv1->tex, CL_TRUE, 0,
			1024 * 1024 * 3, (unsigned char *)textures[3]->pixels, 0, NULL, NULL);
	rtv1->ret = clSetKernelArg(rtv1->kernel, 5,
			sizeof(cl_mem), (void *)&rtv1->tex);

	rtv1->tex = clCreateBuffer(rtv1->context, CL_MEM_READ_WRITE,
		1024 * 1024 * 3, NULL, &rtv1->ret);
	rtv1->ret = clEnqueueWriteBuffer(rtv1->command_queue,
			rtv1->tex, CL_TRUE, 0,
			1024 * 1024 * 3, (unsigned char *)textures[4]->pixels, 0, NULL, NULL);
	rtv1->ret = clSetKernelArg(rtv1->kernel, 6,
			sizeof(cl_mem), (void *)&rtv1->tex);
}

void		graphics_init(t_rtv1 *rtv1)
{
	rtv1->guide_on = 0;
	rtv1->shift_pressed = 0;
	rtv1->dropped_list = -1;
	rtv1->active_edit = -1;
	SDL_Init(SDL_INIT_VIDEO);
    rtv1->surface = SDL_CreateRGBSurface(0, CW, CH, 32, 0, 0, 0, 0);
	rtv1->ui = SDL_CreateRGBSurface(0, 400, CH, 32, 0, 0, 0, 0);
	rtv1->tmp = SDL_CreateRGBSurface(0, 400, CH, 32, 0, 0, 0, 0);

	SDL_FillRect(rtv1->tmp, &((SDL_Rect){0, 0, 400, CH}), 0x00FFFF00);

    rtv1->window = SDL_CreateWindow("Game", 400, 200, CW + 400, CH, SDL_WINDOW_SHOWN);
	//SDL_Window *window = SDL_CreateWindow("Game", 400, 200, CW + 400, CH, SDL_WINDOW_SHOWN);
	
    rtv1->renderer = SDL_CreateRenderer(rtv1->window, -1, SDL_RENDERER_ACCELERATED);

    rtv1->rect.x = 0;
    rtv1->rect.y = 0;
    rtv1->rect.w = CW;
    rtv1->rect.h = CH;

	rtv1->c_buttons = 0;
	rtv1->c_edits = 0;
	rtv1->c_lists = 0;
	LIBUI_NewButton((t_but_constr){20, 20, "New Sphere", "New Sphere", 0x0000ff55}, rtv1->buttons, &(rtv1->c_buttons));
	LIBUI_NewButton((t_but_constr){20, 60, "New Cylinder", "New Cylinder", 0x0000ff55}, rtv1->buttons, &(rtv1->c_buttons));
	LIBUI_NewButton((t_but_constr){20, 100, "New Cone", "New Cone", 0x0000ff55}, rtv1->buttons, &(rtv1->c_buttons));
	LIBUI_NewButton((t_but_constr){20, 140, "New Plane", "New Plane", 0x0000ff55}, rtv1->buttons, &(rtv1->c_buttons));
	LIBUI_NewButton((t_but_constr){20, 180, "New Triangle", "New Triangle", 0x0000ff55}, rtv1->buttons, &(rtv1->c_buttons));
	LIBUI_NewButton((t_but_constr){20, 220, "New Paraboloid", "New Paraboloid", 0x0000ff55}, rtv1->buttons, &(rtv1->c_buttons));

	

	LIBUI_NewButton((t_but_constr){20, 280, "Delete Object", "Delete Object", 0x0000ff55}, rtv1->buttons, &(rtv1->c_buttons));
	
	LIBUI_NewButton((t_but_constr){20, 340, "Save Scene", "Save Scene", 0x0000ff55}, rtv1->buttons, &(rtv1->c_buttons));



	LIBUI_NewButton((t_but_constr){20, 400+20, "Radius+", "Radius+", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);
    LIBUI_NewButton((t_but_constr){180, 400+20, "Radius-", "Radius-", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);

    LIBUI_NewButton((t_but_constr){20, 400+70, "Angle+", "Angle+", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);
    LIBUI_NewButton((t_but_constr){180, 400+70, "Angle-", "Angle-", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);


    LIBUI_NewButton((t_but_constr){20, 400+120, "Specular+", "Specular+", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);
    LIBUI_NewButton((t_but_constr){180, 400+120, "Specular-", "Specular-", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);

    LIBUI_NewButton((t_but_constr){20, 400+170, "Reflective+", "Reflective+", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);
    LIBUI_NewButton((t_but_constr){180, 400+170, "Reflective-", "Reflective-", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);

	LIBUI_NewButton((t_but_constr){20, 400+300, "Edit", "Edit", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);


	LIBUI_NewButton((t_but_constr){200, 400+300, "Texture1", "Texture1", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);
	LIBUI_NewButton((t_but_constr){200, 400+350, "Texture2", "Texture2", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);
	LIBUI_NewButton((t_but_constr){200, 400+400, "Texture3", "Texture3", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);
	LIBUI_NewButton((t_but_constr){200, 400+450, "Texture4", "Texture4", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);
	LIBUI_NewButton((t_but_constr){200, 400+500, "Texture5", "Texture5", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);
	LIBUI_NewButton((t_but_constr){200, 400+550, "Rand Color", "Rand Color", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);

	//LIBUI_drop_list(rtv1, 200, 20);
	t_list_constr tmp;

	tmp.x = 200;
	tmp.y = 20;
	tmp.text = "Texture";
	tmp.function = "lol";
	tmp.color  = 0x000000ff;
	tmp.c_items = 5;
	ft_strcpy(tmp.items_text[0], "Texture1");
	ft_strcpy(tmp.items_text[1], "Texture2");
	ft_strcpy(tmp.items_text[2], "Texture3");
	ft_strcpy(tmp.items_text[3], "Texture4");
	ft_strcpy(tmp.items_text[4], "Texture5");

	ft_strcpy(tmp.items_function[0], "Texture1");
	ft_strcpy(tmp.items_function[1], "Texture2");
	ft_strcpy(tmp.items_function[2], "Texture3");
	ft_strcpy(tmp.items_function[3], "Texture4");
	ft_strcpy(tmp.items_function[4], "Texture5");

	LIBUI_NewList(tmp, rtv1->lists, &rtv1->c_lists);

	LIBUI_NewEdit((t_edit_constr){200, 240, "Name", "Name", 0x00550000}, rtv1->edits, &rtv1->c_edits);
	LIBUI_NewButton((t_but_constr){200, 300, "Save As", "Save As", 0x0000ff55}, rtv1->buttons, &rtv1->c_buttons);

	kernel_init(rtv1);
}
