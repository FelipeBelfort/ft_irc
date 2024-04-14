# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: TheTerror <jfaye@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/23 16:59:24 by TheTerror         #+#    #+#              #
#    Updated: 2024/02/26 16:50:44 by TheTerror        ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME = libftpp.a

OBJ_PATH = ./.obj/

INCLUDE = Libftpp.hpp

CC = 				c++
CPPFLAGS =			-std=c++98
CFLAGS=				$(CPPFLAGS) -Wall -Werror -Wextra -g3
AR = 				ar -rc
RM =				rm -rf

SRC = Libftpp.cpp Itoa.cpp StrTools.cpp FileParsingTools.cpp OutputTools.cpp Error.cpp

OBJ = $(SRC:%.cpp=$(OBJ_PATH)%.o)

all : $(NAME)

$(OBJ_PATH)%.o : %.cpp $(INCLUDE)
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJ) $(INCLUDE)
	$(AR) $@ $(OBJ)
	@ranlib $(NAME)

clean :
	@$(RM) $(OBJ)
	@$(RM) $(OBJ_PATH)
fclean : clean
	@$(RM) $(NAME)
re : fclean all