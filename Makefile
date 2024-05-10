# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/11 20:26:23 by TheTerror         #+#    #+#              #
#    Updated: 2024/04/26 10:40:03 by jm               ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME			=	ircserv

LIBFT			=	$(LIBFT_PATH)libftpp.a
LIBFT_PATH		=	./libftpp/

CXX				=	c++

CFLAGS			=	-Wall -Wextra -Werror -std=c++98 -MMD -g3

FILES			=	main UserGettersSetters UserRegistration ServerGettersSetters \
					UserCommands ChannelGettersSetters ChannelMember

INCLUDE			=	Include
CLASSES			=	Server User Channel

OBJS_DIR		=	objs/

# DEPS_DIR		=	deps/

HEADER			=	$(CLASSES:=.hpp) $(INCLUDE:=.h)

SRC				=	$(FILES:=.cpp) $(CLASSES:=.cpp)

OBJ				=	$(addprefix $(OBJS_DIR), $(SRC:.cpp=.o))

# DEPENDENCIES	=	$(addprefix $(DEPS_DIR), $(OBJ:.o=.d))
DEPENDENCIES	=	(OBJ:.o=.d)

RM				=	rm -fr

-include $(DEPENDENCIES)

$(OBJS_DIR)%.o	: %.cpp $(HEADER)
				$(CXX) $(CFLAGS) -c $< -o $@

all				:	make_libft $(NAME)

$(NAME)			: $(OBJS_DIR) $(OBJ) Makefile
			$(CXX) -o $(NAME) $(OBJ) $(LIBFT)

$(OBJS_DIR)		:
			mkdir -p $(OBJS_DIR) #$(DEPS_DIR)

make_libft		:
			make -C $(LIBFT_PATH)

clean			:
			make fclean -C $(LIBFT_PATH)
			$(RM) $(OBJS_DIR) #$(DEPS_DIR)

fclean			:	clean
			$(RM) $(NAME)

re				:	fclean all

.PHONY			:	all clean fclean re
