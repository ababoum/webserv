CXX			= @c++

CXXFLAGS	= -Wall -Werror -Wextra -g -std=c++98

NAME 		= webserv

SRCS		= srcs/main.cpp \
				srcs/utils.cpp \
				srcs/ConfigurationParser.cpp \
				srcs/GlobalConfiguration.cpp \
				srcs/Location.cpp \
				srcs/Server.cpp \
				srcs/ServerEngine.cpp \
				srcs/Request.cpp \
				srcs/Response.cpp \
				srcs/CGIEngine.cpp \
				srcs/autoindex.cpp

INCL		= includes/webserv.hpp \
				includes/ConfigurationParser.hpp \
				includes/GlobalConfiguration.hpp \
				includes/Location.hpp \
				includes/Server.hpp \
				includes/ServerEngine.hpp \
				includes/Request.hpp \
				includes/Response.hpp \
				includes/CGIEngine.hpp


OBJS		= $(SRCS:.cpp=.o)

all:		$(NAME)

$(NAME):	$(OBJS) $(INCL)
			$(CXX) $(CXXFLAGS) $(SRCS) -o $(NAME)
			@echo "\033[1;32m\e[1m $(NAME)\e[0m\033[32m executable is compiled and ready.\e[0m"

clean:		
			@rm -f $(OBJS)
			@echo "\033[1;36m .o files successfully deleted.\e[0m"

fclean: 	clean
			@rm -f $(NAME)
			@echo "\033[1;36m $(NAME) executable successfully deleted.\e[0m"

re: 		fclean ${NAME}

.PHONY: clean fclean all re