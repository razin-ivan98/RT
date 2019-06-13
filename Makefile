# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chorange <chorange@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/18 18:53:25 by chorange          #+#    #+#              #
#    Updated: 2019/06/13 21:26:31 by chorange         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = RTv1

FLAGS = -Wextra -Wall -Werror

OBJECTS = main.o init.o vect_math1.o vect_math2.o color.o read_scene1.o atof.o\
	get_next_line.o key_utils.o camera_dir_setup.o\
	transform.o	intersect_objs.o exit.o read_scene2.o LIBUI.o save.o

LIBS = -L./minilibx -lmlx -L./libft -lft -lm -framework OpenGL\
	-framework AppKit -framework OpenCL -framework OpenGL

INCLUDES = -I./libft/ -I./minilibx/ -I./frameworks/SDL2.framework/Headers

FRAMEWORKS = -F./frameworks -rpath ./frameworks -framework SDL2 -framework SDL2_ttf -framework SDL2_mixer

MLXPATH = ./minilibx
FTPATH = ./libft

all: $(NAME)

$(NAME): $(OBJECTS)
	make -C $(FTPATH)
	make -C $(MLXPATH)

	gcc $(FLAGS) $(OBJECTS) $(INCLUDES) $(LIBS) $(FRAMEWORKS) -o $(NAME) -g

$(OBJECTS): %.o: %.c
	gcc $(FLAGS) $(INCLUDES) -c  $< -o $@ -g

clean:
	make -C $(FTPATH) clean
	make -C $(MLXPATH) clean
	/bin/rm -f $(OBJECTS)

fclean: clean
	make -C $(FTPATH) fclean
	make -C $(MLXPATH) clean
	/bin/rm -f $(NAME)

re: fclean all
