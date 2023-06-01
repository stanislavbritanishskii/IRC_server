# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbritani <sbritani@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/13 16:18:54 by sbritani          #+#    #+#              #
#    Updated: 2023/03/06 14:38:23 by sbritani         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++

SRCS = main.cpp

NAME =  IRC

FLAGS = #-Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

OBJ	= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAGS) -o $(NAME)

%.o: %.cpp
	$(CC) $(FLAGS) -c $^ -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
