NAME			=	ircserv

CXX				=	c++

CFLAGS			=	-Wall -Wextra -Werror -std=c++98 -MMD

FILES			=	main

CLASSES			=	Server User

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

all				:	$(NAME)

$(NAME)			: $(OBJS_DIR) $(OBJ) Makefile
			$(CXX) -o $(NAME) $(OBJ)

$(OBJS_DIR)		:
			mkdir -p $(OBJS_DIR) #$(DEPS_DIR)

clean			:
			$(RM) $(OBJS_DIR) #$(DEPS_DIR)

fclean			:	clean
			$(RM) $(NAME)

re				:	fclean all

.PHONY			:	all clean fclean re
