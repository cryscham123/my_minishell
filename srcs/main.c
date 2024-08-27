/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/15 19:07:11 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "utils.h"
#include "parse.h"
#include "cmd.h"
#include "files.h"

int	g_status;

int	main(int argc, char **argv, char **env)
{
	t_cmd	*cmd;
	t_lst	*ev;
	int		origin_fd[2];
	pid_t	last_pid;

	(void)argv;
	if (argc != 1)
		return (ft_error(F_ERROR_ARG, NULL, E_EXIT_FAILURE));
	ft_terminal_name(E_TRUE);
	ev = mk_ev_lst(env);
	while (1)
	{
		origin_fd[0] = STDIN_FILENO;
		origin_fd[1] = STDOUT_FILENO;
		cmd = ft_parse(ev);
		if (cmd != NULL)
		{
			last_pid = ft_exe(cmd, ev, origin_fd, E_FALSE);
			ft_wait_pid(last_pid, E_TRUE);
		}
		cmd_free(cmd);
	}
	return (E_EXIT_SUCCESS);
}
