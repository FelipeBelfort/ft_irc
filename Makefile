NAME			=	ircserv
SERVER_NAME		=	server
CLIENT_NAME		=	client
LIBFT			= $(LIBFTPP_PATH)libftpp.a

LIBFTPP_PATH	= ./libftpp/

CXX				=	c++

CFLAGS			=	-Wall -Wextra -Werror -std=c++98 -MMD

FILES			=	main

CLASSES			=	

OBJS_DIR		=	objs/

# DEPS_DIR		=	deps/

HEADER			=	$(CLASSES:=.hpp)

SRC				=	$(FILES:=.cpp) $(CLASSES:=.cpp)

OBJ				=	$(addprefix $(OBJS_DIR), $(SRC:.cpp=.o))

# DEPENDENCIES	=	$(addprefix $(DEPS_DIR), $(OBJ:.o=.d))
DEPENDENCIES	=	(OBJ:.o=.d)

RM				=	rm -fr

-include $(DEPENDENCIES)

$(OBJS_DIR)%.o	: %.cpp $(HEADER)
				$(CXX) $(CFLAGS) -c $< -o $@

all				: make_libft server client

$(NAME)			: $(OBJS_DIR) $(OBJ) Makefile
			$(CXX) -o $(NAME) $(OBJ)

server			:
			$(CXX) $(CFLAGS) -o $(SERVER_NAME) server.cpp $(LIBFT)
client			:
			$(CXX) $(CFLAGS) -o $(CLIENT_NAME) client.cpp $(LIBFT)


$(OBJS_DIR)		:
			mkdir -p $(OBJS_DIR) #$(DEPS_DIR)

C 				: all clean

make_libft		:
			make -C $(LIBFTPP_PATH)
clean			:
			make clean -C $(LIBFTPP_PATH)
			$(RM) $(OBJS_DIR) #$(DEPS_DIR)

fclean			:	clean
			make fclean -C $(LIBFTPP_PATH)
			$(RM) $(NAME) *.d $(SERVER_NAME) $(CLIENT_NAME)

re				:	fclean all

.PHONY			:	all clean fclean re server client
