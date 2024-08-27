/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 16:57:48 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/15 19:22:41 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

extern int	g_status;

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
	g_status = 1;
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
