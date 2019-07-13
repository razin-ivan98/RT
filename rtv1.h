/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 10:45:16 by chorange          #+#    #+#             */
/*   Updated: 2019/07/13 20:39:01 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H
# include "math.h"
# include <fcntl.h>
# include "get_next_line/get_next_line.h"
# include <OpenCL/opencl.h>
# include "libft/libft.h"
# include "frameworks/SDL2.framework/Headers/SDL.h"
# include "frameworks/SDL2_image.framework/Headers/SDL_image.h"
# include "LIBUI/LIBUI.h"
# include <pthread.h>

# define MAX_SOURCE_SIZE 40000
# define CW 1000
# define CH 1000
# define VW 1
# define VH 1
# define ERR "Please, set number of scene in range from 1 to 8 as a argument"
# define ZERO 0.0001

typedef enum	e_light_type
{
	point,
	directional,
	ambient
}				t_light_type;

typedef enum	e_obj_type
{
	sphere,
	cone,
	cylinder,
	plane,
	torus,
	paraboloid,
	triangle,
	arrow
}				t_obj_type;

typedef struct	s_vector
{
	double x;
	double y;
	double z;

}				t_vector;


typedef struct	s_rgb
{
	int r;
	int g;
	int b;
}				t_rgb;

typedef struct	s_s
{
	double a;
	double b;
	double c;
	double diskr;
	double sq_diskr;
	double t1;
	double t2;
	double t;
	double k;
}				t_s;

typedef struct	s_obj
{
	int			type;
	int			id;
	t_vector	center;
	t_vector	p2;
	t_vector	p3;
	t_vector	dir;
	t_vector	rot;
	double		angle;
	double		radius;
	t_rgb		rgb;
	double		specular;
	double		reflective;
	double		transparency;
	double		refractive;
	double		smoothness;
	int			tex;

}				t_obj;

typedef struct	s_light
{
	int			type;
	double		intensity;
	t_vector	center;
	t_vector	dir;
	int			id;
}				t_light;

typedef struct	s_camera
{
	t_vector center;
	t_vector dir;
}				t_camera;

typedef struct	s_scene
{
	int			c_objs;
	int			c_lights;
	double		view_alpha;
	double		view_beta;
	t_camera	camera;
	t_obj		arrows[3];
	t_light		lights[100];
	t_obj		objs[100];
	int			shadows_on;
	int			arrows_on;
	int			advanced;
	int			soft;
	double			rands[500];
}				t_scene;

typedef struct	s_rtv1
{
	SDL_Surface			*tmp;
	SDL_Surface			*ui;
	SDL_Surface			*surface;
	SDL_Window			*window;
	SDL_Renderer		*renderer;
	SDL_Rect			rect;
	SDL_Texture			*screen;
	SDL_Texture			*ui_tex;
	SDL_Event			event;

	SDL_Surface			*edit_surface;
	SDL_Window			*edit_window;
	SDL_Renderer		*edit_renderer;
	SDL_Texture			*edit_texture;

	t_LIBUI_Button		edit_buttons[30];
	int					c_edit_buttons;

	SDL_Surface			*selector_surface;
	SDL_Window			*selector_window;
	SDL_Renderer		*selector_renderer;
	SDL_Texture			*selector_texture;

	t_LIBUI_Button		selector_buttons[200];
	int					c_selector_buttons;

	int					selector_window_active;
	int					edit_window_active;

	int					next_win_ID;

	t_LIBUI_Button		buttons[40];
	t_LIBUI_List		lists[5];
	t_LIBUI_Edit		edits[5];
	int					c_edits;
	int					c_buttons;
	int					c_lists;
	int					active_edit;
	int					dropped_list;


	int					shift_pressed;

	char				scene_file_name[128];

	t_obj				*selected;
	t_light				*selected_light;

	double				selected_t;
	int					left_mouse_pressed;
	int					right_mouse_pressed;
	int					mid_mouse_pressed;
	int					prev_x;
	int					prev_y;
	int					guide_on;
	int					arrow;

	int					from_obj;

	t_scene				scene;

	cl_int				ret;
	cl_platform_id		platform_id;
	cl_uint				ret_num_platforms;

	cl_device_id		device_id;
	cl_uint				ret_num_devices;

	cl_context			context;
	cl_command_queue	command_queue;

	cl_program			program;
	cl_kernel			kernel;

	char				file_name[40];
	char				kernel_name[40];

	cl_mem				memobj;

	cl_mem				utils_memobj;

	cl_mem				tex;

	size_t				global_work_size;
}				t_rtv1;

void			read_scene(t_scene *scene, char *file_name);
void			read_obj(t_rtv1 *rtv1, char *file_name);
double			ft_atof(char *str);
char			*ft_ftoa(double n);
t_vector		read_vector(char *str);
t_camera		*camera_init(char *line, t_camera *camera);
t_light			*read_light_parameters(char *line, t_light **light);
t_obj			*read_obj_parameters(char *line, t_obj **obj);
void			set_light_type(char *line, t_light *light);
void			set_obj_type(char *line, t_obj *obj);
void			graphics_init(t_rtv1 *rtv1);
void			set_start_angles(t_scene *scene);

t_vector		vector_normalize(t_vector a);
t_vector		vector_project(t_vector a, t_vector b);
t_vector		vector_subt(t_vector a, t_vector b);
t_vector		vector_sum(t_vector a, t_vector b);
t_vector		vector_int_mult(t_vector a, double b);
t_vector		vector_int_div(t_vector a, double b);
double			scal_mult(t_vector a, t_vector b);
double			vector_length(t_vector a);
void			scene_init(t_rtv1 *rtv1, char *number);
t_vector		vector_init(double x, double y, double z);
t_vector	vector_cross(t_vector a, t_vector b);

t_rgb			color_to_rgb(int color);
int				rgb_to_color(t_rgb rgb);

int				mouse_pressed(int button, int x, int y, t_rtv1 *rtv1);
int				key_pressed(int key, t_rtv1 *rtv1);
int				key_release(int key, t_rtv1 *rtv1);
int				mouse_release(int button, int x, int y, t_rtv1 *rtv1);
int				mouse_move(int x, int y, t_rtv1 *rtv1);
int mouse_wheel(int y, t_rtv1 *rtv1);

void edit_mouse_pressed(int button, int x, int y, t_rtv1 *rtv1);
void	edit_mouse_release(int button, int x, int y, t_rtv1 *rtv1);

void selector_mouse_pressed(int button, int x, int y, t_rtv1 *rtv1);
void	selector_mouse_release(int button, int x, int y, t_rtv1 *rtv1);

void			provider(t_rtv1 *rtv1);
double			ray_intersect_obj(t_vector start, t_vector dir, t_obj *obj);

void			err_exit(void);
int				clean_exit(t_rtv1 *rtv1);

t_vector		rotate_view(t_vector point, double alpha, double beta);
void			select_object(t_rtv1 *rtv1, int x, int y, t_obj **out);

void			set_arrows_pos(t_rtv1 *rtv1, int mode);





double ray_intersect_arrow(t_vector start, t_vector dir, t_obj *cyl);


void save(t_rtv1 *rtv1, char *filename);
void save_as(t_rtv1 *rtv1);


void new_triangle(t_rtv1 *rtv1);
void new_paraboloid(t_rtv1 *rtv1);
void new_plane(t_rtv1 *rtv1);
void new_cylinder(t_rtv1 *rtv1);
void new_cone(t_rtv1 *rtv1);
void new_sphere(t_rtv1 *rtv1);

void new_point(t_rtv1 *rtv1);
void new_ambient(t_rtv1 *rtv1);
void new_directional(t_rtv1 *rtv1);


void move_polygonal(double x, double y, double z, t_rtv1 *rtv1);
void rot_polygonal(double x, double y, double z, t_rtv1 *rtv1);


char *get_crypto_key(char *msg);


int				check_crypto_key(char *file_name);


void edit (t_rtv1 *rtv1);
void scene_selector(t_rtv1 *rtv1);
void refresh_selector_buttons(t_rtv1 *rtv1);
t_vector rot(t_vector P, t_vector angles);




int get_index_by_id(t_scene *scene, int id);
int get_light_index_by_id(t_scene *scene, int id);

//void *edit (t_rtv1 *rtv1);

#endif
