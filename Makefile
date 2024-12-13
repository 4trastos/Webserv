# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/13 11:45:24 by davgalle          #+#    #+#              #
#    Updated: 2024/12/13 12:12:25 by davgalle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98

RM = rm -f

SRC = src/main.cpp \
	server/server.cpp \
	client/client.cpp \
	config/config.cpp

OBJTS = $(SRC:.cpp=.o)

$(NAME): $(OBJTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJTS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	$(RM) $(OBJTS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
