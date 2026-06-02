#include "utils.h"

static struct termios	*terminal_backup(void)
{
	static struct termios	backup;
	static t_bool			has_backup;

	if (has_backup == E_FALSE)
	{
		if (tcgetattr(STDOUT_FILENO, &backup) == 0)
			has_backup = E_TRUE;
	}
	if (has_backup == E_FALSE)
		return (NULL);
	return (&backup);
}

void	ft_terminal_restore(void)
{
	struct termios	*backup;
	char			*term_path;
	int				terminal_fd;

	backup = terminal_backup();
	term_path = ft_terminal_name(E_FALSE);
	if (backup == NULL || term_path == NULL)
		return ;
	terminal_fd = open(term_path, O_RDWR);
	if (terminal_fd < 0)
		return ;
	tcsetattr(terminal_fd, TCSANOW, backup);
	close(terminal_fd);
}

static void	set_terminal_c_lflag(int flag)
{
	char			*term_path;
	int				terminal_fd;
	struct termios	term;	

	term_path = ft_terminal_name(E_FALSE);
	if (term_path != NULL)
	{
		terminal_fd = open(term_path, O_RDWR);
		if (terminal_fd < 0)
			exit(ft_error(F_ERROR_DEVICE, NULL, E_EXIT_FAILURE));
		if (tcgetattr(terminal_fd, &term) < 0)
			exit(ft_error(F_ERROR_SYS, "tcgetattr", E_EXIT_FAILURE));
		if (flag == 1)
			term.c_lflag |= ECHOCTL;
		else
			term.c_lflag &= ~ECHOCTL;
		if (tcsetattr(terminal_fd, TCSANOW, &term) < 0)
			exit(ft_error(F_ERROR_SYS, "tcsetattr", E_EXIT_FAILURE));
		close(terminal_fd);
	}
}

char	*ft_terminal_name(t_bool is_set)
{
	static char	*terminal_path;

	if (isatty(STDOUT_FILENO) && is_set == E_TRUE)
	{
		terminal_path = ttyname(STDOUT_FILENO);
		if (terminal_path == NULL)
			exit(ft_error(F_ERROR_DEVICE, NULL, E_EXIT_FAILURE));
		terminal_backup();
	}
	return (terminal_path);
}

void	sigint_handler(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	ft_shell_set_status(130);
}

void	heredoc_signal_handler(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 2);
	exit(1);
}

void	ft_signal(void (*handler_1)(int), void (*handler_2)(int), int flag)
{
	set_terminal_c_lflag(flag);
	signal(SIGINT, handler_1);
	signal(SIGQUIT, handler_2);
}
