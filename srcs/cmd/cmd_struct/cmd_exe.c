/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/15 20:14:10 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include "lst.h"
#include "parse.h"
#include "files.h"
#include "utils.h"
#include "str.h"

extern int	g_status;

static t_lst	*ft_argv_expansion(t_lst *argv, t_lst *ev)
{
	t_lst	*tmp;
	t_lst	*ret;
	t_lst	*to_push;

	ret = NULL;
	tmp = argv;
	while (argv != NULL)
	{
		to_push = ft_expansion(argv->data, ev);
		lst_push(&ret, to_push);
		argv = argv->nxt;
	}
	ft_lst_free(tmp);
	return (ret);
}

static pid_t	ft_pu_exe(t_pu_cmd *_this, t_lst *ev, int fd[2], int context)
{
	char	**av;
	pid_t	last_pid;
	int		fd_tmp[2];
	int		std_fd[2];
	int		target_fd[2];

	fd_init(target_fd, fd[0], fd[1]);
	fd_init(std_fd, STDIN_FILENO, STDOUT_FILENO);
	last_pid = 0;
	if (ft_redirection(target_fd, fd, _this->redir, ev) != E_EXIT_SUCCESS)
		return (-1);
	_this->argv = ft_argv_expansion(_this->argv, ev);
	if (_this->argv != NULL)
	{
		av = mk_argv(_this->argv);
		ft_dup(fd_tmp, std_fd, E_DUP);
		ft_dup(target_fd, std_fd, E_DUP2);
		last_pid = ft_exe_cmd(av, ev, context);
		ft_dup(fd_tmp, std_fd, E_DUP2);
		ft_argv_free(av);
		ft_close(fd_tmp, fd);
	}
	ft_close(target_fd, fd);
	return (last_pid);
}

static pid_t	ft_su_exe(t_su_cmd *_this, t_lst *ev, int fd[2])
{
	int		target_fd[2];
	int		pipe[2];
	pid_t	last_pid;

	ft_signal(SIG_IGN, SIG_IGN, 0);
	fd_init(target_fd, fd[0], fd[1]);
	if (ft_redirection(target_fd, fd, _this->redir, ev) != E_EXIT_SUCCESS)
		return (-1);
	ft_pipe(pipe);
	last_pid = ft_fork();
	if (last_pid == 0)
	{
		ft_dup(target_fd, pipe, E_DUP2);
		last_pid = ft_exe(_this->target, ev, pipe, E_TRUE);
		ft_close(pipe, target_fd);
		ft_wait_pid(last_pid, E_FALSE);
		exit(g_status);
	}
	ft_close(target_fd, fd);
	ft_close(pipe, fd);
	return (last_pid);
}

static pid_t	ft_pipe_exe(t_cmd *_this, t_lst *ev, int fd[2])
{
	int		target_fd[2];
	int		pipe[2];
	pid_t	last_pid;

	ft_signal(SIG_IGN, SIG_IGN, 0);
	fd_init(target_fd, fd[0], fd[1]);
	while (_this != NULL)
	{
		if (_this->cmd->pb_cmd.nxt != NULL)
		{
			ft_pipe(pipe);
			fd_init(target_fd, target_fd[0], pipe[1]);
		}
		last_pid = ft_fork();
		if (last_pid == 0)
		{
			if (_this->cmd->pb_cmd.nxt != NULL)
				close(pipe[0]);
			pipe_exe(_this->cmd->pb_cmd.target, ev, target_fd, fd);
		}
		ft_close(target_fd, fd);
		fd_init(target_fd, pipe[0], fd[1]);
		_this = _this->cmd->pb_cmd.nxt;
	}
	return (last_pid);
}

pid_t	ft_exe(t_cmd *_this, t_lst *ev, int fd[2], int exe_context)
{
	pid_t	last_pid;

	if (_this->cmd_info == 0)
		last_pid = ft_pu_exe(&_this->cmd->pu_cmd, ev, fd, exe_context);
	else if (_this->cmd_info == E_T_SUB)
		last_pid = ft_su_exe(&_this->cmd->su_cmd, ev, fd);
	else if (_this->cmd_info == E_T_PIPE)
		last_pid = ft_pipe_exe(_this, ev, fd);
	else
	{
		last_pid = ft_exe(_this->cmd->bi_cmd.left, ev, fd, exe_context);
		ft_wait_pid(last_pid, E_TRUE);
		if ((_this->cmd_info == E_T_AND && g_status == 0) || \
			(_this->cmd_info == E_T_OR && g_status != 0))
			last_pid = ft_exe(_this->cmd->bi_cmd.right, ev, fd, exe_context);
	}
	return (last_pid);
}
