/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/14 17:53:57 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include "files.h"
#include "utils.h"

extern int	g_status;

static int	cal_flag(int ret_val)
{
	if (ret_val == 0)
		return (ret_val);
	if (ret_val < 0)
		return (-ret_val);
	if (WIFEXITED(ret_val))
		return (WEXITSTATUS(ret_val));
	return (ret_val + 128);
}

pid_t	ft_fork(void)
{
	pid_t	ret;

	ret = fork();
	if (ret < 0)
		exit(ft_error(F_ERROR_SYS, "fork", E_EXIT_FAILURE));
	return (ret);
}

void	ft_wait_pid(pid_t target_pid, t_bool is_main)
{
	int		ret;
	int		status;
	pid_t	cmp_pid;

	if (target_pid <= 0)
		g_status = cal_flag(target_pid);
	ret = 0;
	while (1)
	{
		cmp_pid = waitpid(-1, &status, 0);
		if (cmp_pid == -1)
			break ;
		ret = cal_flag(status);
		if (cmp_pid == target_pid)
		{
			g_status = ret;
			if (ret == 131 && is_main == E_TRUE)
				ft_putstr_fd("Quit: 3\n", 1);
		}
	}
	if (ret == 130 && is_main == E_TRUE)
		ft_putstr_fd("\n", 1);
}
