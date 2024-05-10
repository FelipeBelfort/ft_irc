# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/11 20:26:23 by TheTerror         #+#    #+#              #
#    Updated: 2024/05/10 18:33:21 by jm               ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME			=	ircserv

LIBFT			=	$(LIBFT_PATH)libftpp.a
LIBFT_PATH		=	./libftpp/
SERVER_PATH		=	./server/
USER_PATH		=	./user/
CHANNEL_PATH	=	./channel/

CXX				=	c++

CFLAGS			=	-Wall -Wextra -Werror -std=c++98 -MMD -g3

SERVER_FILES	=	$(addprefix $(SERVER_PATH), Server ServerGettersSetters \
					ServerMessageTools ServerOtherTools)
USER_FILES		=	$(addprefix $(USER_PATH), User UserGettersSetters UserRegistration \
					UserJoinCommand UserPrivmsgCommand UserOtherCommands UserKickCommand \
					UserInviteCommand UserModeCommand UserTopicCommand UserOtherTools)
CHANNEL_FILES	=	$(addprefix $(CHANNEL_PATH), Channel ChannelGettersSetters \
					ChannelJoinCommand ChannelprivmsgCommand ChannelKickCommand \
					ChannelInviteCommand ChannelModeCommand ChannelTopicCommand \
					ChannelOtherTools ChannelMember)

FILES			=	main $(SERVER_FILES) $(USER_FILES) $(CHANNEL_FILES)

INCLUDE			=	Include
CLASSES			=	$(SERVER_PATH)Server $(USER_PATH)User $(CHANNEL_PATH)Channel

OBJS_DIR		=	objs/

# DEPS_DIR		=	deps/

HEADER			=	$(CLASSES:=.hpp) $(INCLUDE:=.h)

SRC				=	$(FILES:=.cpp)

OBJ				=	$(addprefix $(OBJS_DIR), $(SRC:.cpp=.o))

# DEPENDENCIES	=	$(addprefix $(DEPS_DIR), $(OBJ:.o=.d))
DEPENDENCIES	=	(OBJ:.o=.d)

RM				=	rm -fr

-include $(DEPENDENCIES)

$(OBJS_DIR)%.o	: %.cpp $(HEADER) $(LIBFT)
				$(CXX) $(CFLAGS) -c $< -o $@

all				:	make_libft $(NAME)

$(NAME)			: $(OBJS_DIR) $(OBJ) Makefile
			$(CXX) -o $(NAME) $(OBJ) $(LIBFT)

$(OBJS_DIR)		:
			@mkdir -p $(OBJS_DIR) #$(DEPS_DIR)
			@mkdir -p $(OBJS_DIR)$(SERVER_PATH)
			@mkdir -p $(OBJS_DIR)$(USER_PATH)
			@mkdir -p $(OBJS_DIR)$(CHANNEL_PATH)

make_libft		:
			make -C $(LIBFT_PATH)

clean			:
			make fclean -C $(LIBFT_PATH)
			$(RM) $(OBJS_DIR) #$(DEPS_DIR)

fclean			:	clean
			$(RM) $(NAME)

re				:	fclean all

.PHONY			:	all clean fclean re
