# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chorange <chorange@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/18 18:53:25 by chorange          #+#    #+#              #
#    Updated: 2019/06/29 15:58:51 by chorange         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = RTv1

FLAGS = #-Wextra -Wall -Werror

OBJECTS = main.o init.o vect_math1.o vect_math2.o color.o read_scene1.o atof.o\
	get_next_line.o key_utils.o camera_dir_setup.o intersect_objs2.o\
	transform.o	intersect_objs.o exit.o read_scene2.o  save.o 

LIBS = -L./libft -lft -L./LIBUI/ -lui -lm -framework OpenGL\
	-framework AppKit -framework OpenCL -framework OpenGL

INCLUDES = -I./libft/ -I./frameworks/SDL2.framework/Headers

FRAMEWORKS = -F./frameworks -rpath ./frameworks -framework SDL2 -framework SDL2_ttf -framework SDL2_mixer

FTPATH = ./libft
UIPATH = ./LIBUI

all: $(NAME)

$(NAME): $(OBJECTS)
	make -C $(FTPATH)
	make -C $(UIPATH)

	gcc $(FLAGS) $(OBJECTS) $(INCLUDES) $(LIBS) $(FRAMEWORKS) -o $(NAME) -g

$(OBJECTS): %.o: %.c
	gcc $(FLAGS) $(INCLUDES) -c  $< -o $@ -g

clean:
	make -C $(FTPATH) clean
	make -C $(UIPATH) clean
	/bin/rm -f $(OBJECTS)

fclean: clean
	make -C $(FTPATH) fclean
	make -C $(UIPATH) fclean
	/bin/rm -f $(NAME)

re: fclean all
