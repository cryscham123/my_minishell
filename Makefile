# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/07 06:51:22 by hyunghki          #+#    #+#              #
#    Updated: 2023/12/15 19:52:18 by hyunghki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= minishell
CFLAGS= -Wall -Werror -Wextra
DEBUG_FLAGS= -g3 -fsanitize=address
LIBS= -lreadline
INCLUDE= -Iincludes/cmd \
		 -Iincludes/parse \
		 -Iincludes/utils
SRC_DIR= srcs/
SRCS= $(SRC_DIR)main.c \
	  $(SRC_DIR)utils/lst/env_lst.c \
	  $(SRC_DIR)utils/lst/lst.c \
	  $(SRC_DIR)utils/lst/lst_free.c \
	  $(SRC_DIR)utils/lst/str_lst.c \
	  $(SRC_DIR)utils/str/str.c \
	  $(SRC_DIR)utils/str/str2.c \
	  $(SRC_DIR)utils/str/str3.c \
	  $(SRC_DIR)utils/heap_manage.c \
	  $(SRC_DIR)utils/print.c \
	  $(SRC_DIR)utils/files.c \
	  $(SRC_DIR)utils/heredoc.c \
	  $(SRC_DIR)utils/signal.c \
	  $(SRC_DIR)parse/parse.c \
	  $(SRC_DIR)parse/split/split.c \
	  $(SRC_DIR)parse/split/split_utils.c \
	  $(SRC_DIR)parse/split/parse_cmd.c \
	  $(SRC_DIR)parse/expansion/expansion.c \
	  $(SRC_DIR)parse/expansion/wild_card.c \
	  $(SRC_DIR)cmd/cmd_struct/mk_cmd.c \
	  $(SRC_DIR)cmd/cmd_struct/cmd_exe.c \
	  $(SRC_DIR)cmd/cmd_struct/cmd_utils.c \
	  $(SRC_DIR)cmd/exe/exe_cmd.c \
	  $(SRC_DIR)cmd/exe/exe_utils.c \
	  $(SRC_DIR)cmd/exe/process.c \
	  $(SRC_DIR)cmd/built_in/interface.c \
	  $(SRC_DIR)cmd/built_in/interface2.c \
	  $(SRC_DIR)cmd/built_in/utils.c \
	  $(SRC_DIR)cmd/built_in/utils2.c
OBJS= $(SRCS:.c=.o)

all: $(NAME)

.PHONY: re clean fclean

re:
	make fclean
	make all

clean:
	rm -f $(OBJS)

fclean:
	make clean
	rm -f $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE)
