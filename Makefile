NAME= minishell
CFLAGS= -Wall -Werror -Wextra
DEBUG_FLAGS= -g3 -fsanitize=address
LIBS= -lreadline
INCLUDE= -Iinclude/cmd \
		 -Iinclude/parse \
		 -Iinclude/utils
SRC_DIR= src/
SRCS= $(SRC_DIR)main.c \
	  $(SRC_DIR)utils/lst/lst.c \
	  $(SRC_DIR)utils/lst/lst_free.c \
	  $(SRC_DIR)utils/lst/str_lst.c \
	  $(SRC_DIR)utils/str/str.c \
	  $(SRC_DIR)utils/str/str2.c \
	  $(SRC_DIR)utils/str/str3.c \
	  $(SRC_DIR)utils/heap_manage.c \
	  $(SRC_DIR)utils/env.c \
	  $(SRC_DIR)utils/shell.c \
	  $(SRC_DIR)utils/print.c \
	  $(SRC_DIR)utils/files.c \
	  $(SRC_DIR)utils/heredoc.c \
	  $(SRC_DIR)utils/signal.c \
	  $(SRC_DIR)parse/parse.c \
	  $(SRC_DIR)parse/quote.c \
	  $(SRC_DIR)parse/lexer.c \
	  $(SRC_DIR)parse/parser/parser.c \
	  $(SRC_DIR)parse/parser/parser_utils.c \
	  $(SRC_DIR)parse/parser/parser_cmd.c \
	  $(SRC_DIR)parse/expansion/expansion.c \
	  $(SRC_DIR)parse/expansion/wild_card.c \
	  $(SRC_DIR)cmd/structure/mk_cmd.c \
	  $(SRC_DIR)cmd/structure/cmd_exe.c \
	  $(SRC_DIR)cmd/structure/cmd_utils.c \
	  $(SRC_DIR)cmd/execution/exe_cmd.c \
	  $(SRC_DIR)cmd/execution/exe_utils.c \
	  $(SRC_DIR)cmd/execution/process.c \
	  $(SRC_DIR)cmd/builtin/interface.c \
	  $(SRC_DIR)cmd/builtin/interface2.c \
	  $(SRC_DIR)cmd/builtin/utils.c \
	  $(SRC_DIR)cmd/builtin/utils2.c
OBJS= $(SRCS:.c=.o)

all: $(NAME)

.PHONY: all re clean fclean debug asan test check-fds check-terminal memcheck verify

re: fclean all

debug: CFLAGS += -g3
debug: fclean $(NAME)

asan: CFLAGS += $(DEBUG_FLAGS)
asan: fclean $(NAME)

test: all
	./tests/run_regression.sh

check-fds: all
	./tests/check_fds.sh

check-terminal: all
	./tests/check_terminal.sh

memcheck: all
	./tests/check_valgrind.sh

verify: test check-fds check-terminal memcheck

clean:
	rm -f $(OBJS)

fclean:
	$(MAKE) clean
	rm -f $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE)
